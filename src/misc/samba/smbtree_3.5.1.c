/* 
   Unix SMB/CIFS implementation.
   Network neighbourhood browser.
   
   Copyright (C) Tim Potter      2000
   Copyright (C) Jelmer Vernooij 2003
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "includes.h"
#include "../librpc/gen_ndr/cli_srvsvc.h"
#include <semaphore.h>
#include <libsmbclient.h>
#include <unistd.h>
#include <pthread.h>
#include "event_proxy.h"
#include "samba.h"
#include "ILibLinkedList.h"

static int use_bcast;

/*==================== add code ====================*/
#define TEMP_SMBCLIENT_FILE "/tmp/smbclient.conf"
#define MAX_NAME_LEN (256*2)
#define MAX_FILE_READ_BUFFER_SIZE   (PATH_MAX *2)
#define NUM_ROWS_PER_MALLOC 128
#ifndef CONF_SAMBA_CONF_PATH
#define CONF_SAMBA_CONF_PATH            "/tmp/smb.conf"
#endif

#define ENABLE_SMBTREE_DEBUG_MODE
#define ENABLE_PTHREAD
#define ENABLE_LOCK

#define SMBTREE_MOUNT_RSIZE		(64512)
//#define ENABLE_SMBTREE_PASSWORD_DISPLAY

static void *serverlist = NULL, *sharefolderlist = NULL;
static int is_getserver_done = 1; /*this value is used to prevent user calls refresh_server command when previous refresh_server command isn't done*/
static ptr_event_object_handler smb_event_ep_obj;
static int smb_event_ep_fdr=-1;
static char smb_username[MAX_NAME_LEN], smb_passwd[MAX_NAME_LEN];
#ifdef ENABLE_LOCK
static sem_t SMBTREE_LOCK;
#endif

struct smbserverinfo_t{
       char* name;
       char* ip;
       char* user;
       char *passwd;
       int cmd_id;
};

static void SMBTREE_free_smbserverinfo(struct smbserverinfo_t *pobj);
static void SMBTREE_send_event_to_OSD(smb_cmd_t cmd, smb_cmd_status_t status, int id, char *data);
static void SMBTREE_send_list_to_OSD(void *list);
static void nmblookup_ip(char* computerName, char** pstr);
static void smb_umount(char *dir_path);
static void smb_replace_special_char(char *s1, char *s2);
static int smb_mount(char *name, char *passwd, char *pcname, char *ip, char *dir);
void *access_dir(void *value);
static void get_static_auth_data_fn(const char * pServer,
		const char * pShare,
		char * pWorkgroup,
		int maxLenWorkgroup,
		char * pUsername,
		int maxLenUsername,
		char * pPassword,
		int maxLenPassword);
void *smb_getdir(void *value);
int get_serverlists_by_nmblookup(int id);
void *get_serverlists(void *value);
static unsigned char smb_event_monitor_init_callback(void *readfd,void *writefd);
static int smb_event_monitor_init();
static int smb_event_ep_send_event(void *event);
static int smb_event_handler (void *cmd, int cmd_len);
static int smb_receive_cmd(int eventfd);
static int  smb_event_ep_get_event_fd();
static void smb_proc(void);

/*==================== end of add code ====================*/


/* How low can we go? */

enum tree_level {LEV_WORKGROUP, LEV_SERVER, LEV_SHARE};
static enum tree_level level = LEV_SHARE;

/* Holds a list of workgroups or servers */

struct smb_name_list {
        struct smb_name_list *prev, *next;
        char *name, *comment;
        uint32 server_type;
};

static struct smb_name_list *workgroups, *servers, *shares;

int strHTTPUnEscape(char* data)
{
    char hex[3];
    char *stp=NULL;
    int src_x=0;
    int dst_x=0;
    long hex_val = 0;

    int length = (int)strlen(data);
    hex[2]=0;

    if (data == NULL)
        return 0;
    while(src_x<length)
    {
        if(strncmp(data+src_x,"%",1)==0)
        {
            //
            // Since we encountered a '%' we know this is an escaped character
            //

            // we have to make sure no mistake
            if ((src_x+2) < length) {
                stp = NULL;  // clear endptr 
                hex[0] = data[src_x+1];
                hex[1] = data[src_x+2];
		hex_val = strtol(hex,&stp,16);
                if (*stp == 0) {  // No invalid characters
                    data[dst_x] = (char)hex_val;
                    dst_x += 1;
                    src_x += 3;
                    continue;
                }
            }
        }

        if(src_x!=dst_x)
        {
            //
            // This doesn't need to be unescaped. If we didn't unescape anything previously
            // there is no need to copy the string either
            //
            data[dst_x] = data[src_x];
            src_x += 1;
            dst_x += 1;
        }
        else
        {
            //
            // This doesn't need to be unescaped, however we need to copy the string
            //
            src_x += 1;
            dst_x += 1;
        }
    }
    data[dst_x] = '\0';
    return(dst_x);
}


static void free_name_list(struct smb_name_list *list)
{
        while(list)
                DLIST_REMOVE(list, list);
}

static void add_name(const char *machine_name, uint32 server_type,
                     const char *comment, void *state)
{
        struct smb_name_list **name_list = (struct smb_name_list **)state;
        struct smb_name_list *new_name;

        new_name = SMB_MALLOC_P(struct smb_name_list);

        if (!new_name)
                return;

        ZERO_STRUCTP(new_name);

	new_name->name = SMB_STRDUP(machine_name);
	new_name->comment = SMB_STRDUP(comment);
        new_name->server_type = server_type;

	if (!new_name->name || !new_name->comment) {
		SAFE_FREE(new_name->name);
		SAFE_FREE(new_name->comment);
		SAFE_FREE(new_name);
		return;
	}

        DLIST_ADD(*name_list, new_name);
}

/****************************************************************************
  display tree of smb workgroups, servers and shares
****************************************************************************/
static bool get_workgroups(struct user_auth_info *user_info)
{
        struct cli_state *cli;
        struct sockaddr_storage server_ss;
	TALLOC_CTX *ctx = talloc_tos();
	char *master_workgroup = NULL;

        /* Try to connect to a #1d name of our current workgroup.  If that
           doesn't work broadcast for a master browser and then jump off
           that workgroup. */

	master_workgroup = talloc_strdup(ctx, lp_workgroup());
	if (!master_workgroup) {
		return false;
	}

        if (!use_bcast && !find_master_ip(lp_workgroup(), &server_ss)) {
                DEBUG(4, ("Unable to find master browser for workgroup %s, falling back to broadcast\n", 
			  master_workgroup));
				use_bcast = True;
		} else if(!use_bcast) {
			char addr[INET6_ADDRSTRLEN];
			print_sockaddr(addr, sizeof(addr), &server_ss);
			if (!(cli = get_ipc_connect(addr, &server_ss, user_info)))
				return False;
		}

		if (!(cli = get_ipc_connect_master_ip_bcast(talloc_tos(),
							user_info,
							&master_workgroup))) {
			DEBUG(4, ("Unable to find master browser by "
				  "broadcast\n"));
			return False;
        }

        if (!cli_NetServerEnum(cli, master_workgroup,
                               SV_TYPE_DOMAIN_ENUM, add_name, &workgroups))
                return False;

        return True;
}

/* Retrieve the list of servers for a given workgroup */

static bool get_servers(char *workgroup, struct user_auth_info *user_info)
{
        struct cli_state *cli;
        struct sockaddr_storage server_ss;
	char addr[INET6_ADDRSTRLEN];

        /* Open an IPC$ connection to the master browser for the workgroup */

        if (!find_master_ip(workgroup, &server_ss)) {
                DEBUG(4, ("Cannot find master browser for workgroup %s\n",
                          workgroup));
                return False;
        }

	print_sockaddr(addr, sizeof(addr), &server_ss);
        if (!(cli = get_ipc_connect(addr, &server_ss, user_info)))
                return False;

        if (!cli_NetServerEnum(cli, workgroup, SV_TYPE_ALL, add_name,
                               &servers))
                return False;

        return True;
}

static bool get_rpc_shares(struct cli_state *cli,
			   void (*fn)(const char *, uint32, const char *, void *),
			   void *state)
{
	NTSTATUS status;
	struct rpc_pipe_client *pipe_hnd = NULL;
	TALLOC_CTX *mem_ctx;
	WERROR werr;
	struct srvsvc_NetShareInfoCtr info_ctr;
	struct srvsvc_NetShareCtr1 ctr1;
	int i;
	uint32_t resume_handle = 0;
	uint32_t total_entries = 0;

	mem_ctx = talloc_new(NULL);
	if (mem_ctx == NULL) {
		DEBUG(0, ("talloc_new failed\n"));
		return False;
	}

	status = cli_rpc_pipe_open_noauth(cli, &ndr_table_srvsvc.syntax_id,
					  &pipe_hnd);

	if (!NT_STATUS_IS_OK(status)) {
		DEBUG(10, ("Could not connect to srvsvc pipe: %s\n",
			   nt_errstr(status)));
		TALLOC_FREE(mem_ctx);
		return False;
	}

	ZERO_STRUCT(info_ctr);
	ZERO_STRUCT(ctr1);

	info_ctr.level = 1;
	info_ctr.ctr.ctr1 = &ctr1;

	status = rpccli_srvsvc_NetShareEnumAll(pipe_hnd, mem_ctx,
					       pipe_hnd->desthost,
					       &info_ctr,
					       0xffffffff,
					       &total_entries,
					       &resume_handle,
					       &werr);

	if (!NT_STATUS_IS_OK(status) || !W_ERROR_IS_OK(werr)) {
		TALLOC_FREE(mem_ctx);
		TALLOC_FREE(pipe_hnd);
		return False;
	}

	for (i=0; i<total_entries; i++) {
		struct srvsvc_NetShareInfo1 info = info_ctr.ctr.ctr1->array[i];
		fn(info.name, info.type, info.comment, state);
	}

	TALLOC_FREE(mem_ctx);
	TALLOC_FREE(pipe_hnd);
	return True;
}


static bool get_shares(char *server_name, struct user_auth_info *user_info)
{
        struct cli_state *cli;

        if (!(cli = get_ipc_connect(server_name, NULL, user_info)))
                return False;

	if (get_rpc_shares(cli, add_name, &shares))
		return True;

        if (!cli_RNetShareEnum(cli, add_name, &shares))
                return False;

        return True;
}

static bool print_tree(struct user_auth_info *user_info)
{
        struct smb_name_list *wg, *sv, *sh;

        /* List workgroups */

        if (!get_workgroups(user_info))
                return False;

        for (wg = workgroups; wg; wg = wg->next) {

                printf("%s\n", wg->name);

                /* List servers */

                free_name_list(servers);
                servers = NULL;

                if (level == LEV_WORKGROUP || 
                    !get_servers(wg->name, user_info))
                        continue;

                for (sv = servers; sv; sv = sv->next) {

                        printf("\t\\\\%-15s\t\t%s\n", 
			       sv->name, sv->comment);

                        /* List shares */

                        free_name_list(shares);
                        shares = NULL;

                        if (level == LEV_SERVER ||
                            !get_shares(sv->name, user_info))
                                continue;

                        for (sh = shares; sh; sh = sh->next) {
                                printf("\t\t\\\\%s\\%-15s\t%s\n", 
				       sv->name, sh->name, sh->comment);
                        }
                }
        }

        return True;
}

/****************************************************************************
  main program
****************************************************************************/
 int main(int argc,char *argv[])
{
	TALLOC_CTX *frame = talloc_stackframe();
	struct user_auth_info *auth_info;
	struct poptOption long_options[] = {
		POPT_AUTOHELP
		{ "broadcast", 'b', POPT_ARG_VAL, &use_bcast, True, "Use broadcast instead of using the master browser" },
		{ "domains", 'D', POPT_ARG_VAL, &level, LEV_WORKGROUP, "List only domains (workgroups) of tree" },
		{ "servers", 'S', POPT_ARG_VAL, &level, LEV_SERVER, "List domains(workgroups) and servers of tree" },
		POPT_COMMON_SAMBA
		POPT_COMMON_CREDENTIALS
		POPT_TABLEEND
	};
	poptContext pc;

	/* Initialise samba stuff */
	load_case_tables();

	setlinebuf(stdout);

	dbf = x_stderr;

	setup_logging(argv[0],True);

	auth_info = user_auth_info_init(frame);
	if (auth_info == NULL) {
		exit(1);
	}
	popt_common_set_auth_info(auth_info);

	pc = poptGetContext("smbtree", argc, (const char **)argv, long_options,
						POPT_CONTEXT_KEEP_FIRST);
	while(poptGetNextOpt(pc) != -1);
	poptFreeContext(pc);

	lp_load(get_dyn_CONFIGFILE(),True,False,False,True);
	load_interfaces();

	/* Parse command line args */

	if (get_cmdline_auth_info_use_machine_account(auth_info) &&
	    !set_cmdline_auth_info_machine_account_creds(auth_info)) {
		TALLOC_FREE(frame);
		return 1;
	}

#ifdef ENABLE_LOCK
	sem_init(&SMBTREE_LOCK, 0, 1);
#endif
	serverlist = XLinkedList_Create();
       sharefolderlist = XLinkedList_Create();
       smb_event_monitor_init();
       smb_proc();


       set_cmdline_auth_info_getpass(auth_info);

	/* Now do our stuff */

#if 0
        if (!print_tree(auth_info)) {
		TALLOC_FREE(frame);
                return 1;
	}
#endif
	TALLOC_FREE(frame);
	return 0;
}

static void SMBTREE_free_smbserverinfo(struct smbserverinfo_t *pobj)
{
       if(pobj) {
               if(pobj->user) free(pobj->user);
               if(pobj->passwd) free(pobj->passwd);
               if(pobj->name) free(pobj->name);
               if(pobj->ip) free(pobj->ip);
               free(pobj);
       }
}

static void SMBTREE_send_event_to_OSD(smb_cmd_t cmd, smb_cmd_status_t status, int id, char *data)
{
	struct smbEventProxyCmd event;

	memset(&event, 0, sizeof(event));
	event.cmd = cmd;
	event.status = status;
	event.id = id;
	if(data) {
		event.datalen = strlen(data) + 1;
		strcpy((char *)event.data, data);
	} else {
		event.datalen = 0;
	}
	smb_event_ep_send_event((void *)&event);
}

static void SMBTREE_send_list_to_OSD(void *list)
{
	void *node = NULL;

	XLinkedList_Lock(list);
	node = XLinkedList_GetNode_Head(list);
	while (node) {
		struct smbEventProxyCmd *pevent = NULL;

		pevent = XLinkedList_GetDataFromNode(node);
		if(pevent != NULL) {
			smb_event_ep_send_event((void *)pevent);
			free(pevent);
			pevent = NULL;
		}
		XLinkedList_Remove(node);
		node = XLinkedList_GetNode_Head(list);
	}
	XLinkedList_UnLock(list);
}

static void nmblookup_ip(char* computerName, char** pstr)
{
	char param[256] = {0};
	FILE *pfile = NULL;
	char stream[MAX_FILE_READ_BUFFER_SIZE];
	char pcIpAddress[MAX_NAME_LEN];
	bool isGetIp = false;

	snprintf(param, sizeof(param), "nmblookup -s %s %s | grep %s", CONF_SAMBA_CONF_PATH, computerName, computerName);
	pfile = popen(param, "r");
	if (NULL == pfile)
		return;

	memset(pcIpAddress, 0, MAX_NAME_LEN);
	while (fgets(stream, MAX_FILE_READ_BUFFER_SIZE, pfile))
	{
		char *c;
		char temp[MAX_NAME_LEN];

		sscanf(stream, "%s %s", pcIpAddress, temp);

		c = strchr(pcIpAddress, '.');
		if(NULL == c)
			continue;
		c++;

		c = strchr(pcIpAddress, '.');
		if(NULL == c)
			continue;
		c++;

		c = strchr(pcIpAddress, '.');
		if(NULL == c)
			continue;

		// already get computer ip address: pcIpAddress
		snprintf(param, sizeof(param), "%s%s", param, pcIpAddress);
		isGetIp = true;
		break;
	}
	pclose(pfile);
	if(isGetIp && strlen(pcIpAddress) > 0) {
		int pstr_len = strlen(pcIpAddress) + 1;
		*pstr = malloc(pstr_len);
		snprintf(*pstr,pstr_len,"%s",pcIpAddress);
	} 
	return;
}

static void smb_umount(char *dir_path)
{
        DIR *dir_stream = NULL;
        struct dirent *item = NULL;
	char cmdline[MAX_NAME_LEN] = {0};
	int ret;

        dir_stream = opendir(dir_path);
        if (!dir_stream) {
                printf("%s, %d, Open dir stream is NULL, dir_path=%s \n", __FUNCTION__, __LINE__, dir_path);
                return;
        }
        while(NULL != (item = readdir(dir_stream))) {
                if(item->d_type == DT_DIR && item->d_name[0] != '.')  {
			snprintf(cmdline, sizeof(cmdline), "umount \"%s/%s\"", dir_path, item->d_name);	
			ret = system(cmdline);
			if(WIFEXITED(ret) && WEXITSTATUS(ret) == 0) {
				memset(cmdline, 0, sizeof(cmdline));
				snprintf(cmdline, sizeof(cmdline), "rmdir \"%s/%s\"", dir_path, item->d_name);
				ret = system(cmdline);
			}
                }
        }
        closedir(dir_stream);
}

static void smb_replace_special_char(char *s1, char *s2)
{
	int i = 0, j = 0;
	
	while(s1[i] != '\0') {
		if(s1[i] == '$') {
			s2[j++] = '\\';
		}
		s2[j++] = s1[i];
		i++;
	}
}

static int smb_mount(char *name, char *passwd, char *pcname, char *ip, char *dir)
{
	char filename[MAX_NAME_LEN] = {0};
	char cmdline[MAX_NAME_LEN] = {0};
	struct stat fs;
	int ret;

	snprintf(filename, sizeof(filename), "\"%s/%s\"", CONF_SAMBA_MOUNT_POINT, pcname);	
	if(access(filename, 0) != 0) { // pc root dir don't exist.
		snprintf(cmdline, sizeof(cmdline), "mkdir -p \"%s/%s\"", CONF_SAMBA_MOUNT_POINT, pcname);	
		ret = system(cmdline);
		if(ret == -1) {
			return ret;
		}
	} 


	// need to mount
	memset(filename, 0, sizeof(filename));
	snprintf(filename, sizeof(filename), "\"%s/%s/%s\"", CONF_SAMBA_MOUNT_POINT, pcname, dir);	
	if(access(filename, 0) != 0) { // shared dir don't exist.
		memset(cmdline, 0, sizeof(cmdline));
		snprintf(cmdline, sizeof(cmdline), "mkdir -p \"%s/%s/%s\"", CONF_SAMBA_MOUNT_POINT, pcname, dir);	
		ret = system(cmdline);
		if(ret == -1) {
			printf("[%s%d] %s failed\n", __FUNCTION__, __LINE__, cmdline);
			return ret;
		}
		memset(cmdline, 0, sizeof(cmdline));
		snprintf(cmdline, sizeof(cmdline), "chmod 0 \"%s/%s/%s\"", CONF_SAMBA_MOUNT_POINT, pcname, dir);	
		ret = system(cmdline);
		if(ret == -1) {
			printf("[%s%d] %s failed\n", __FUNCTION__, __LINE__, cmdline);
			return ret;
		}
	} else {
		ret = stat(filename, &fs);		
		if(ret == 0) {
			if(((S_IRUSR & fs.st_mode) || (S_IWUSR & fs.st_mode) || (S_IXUSR & fs.st_mode) ||
						(S_IRGRP & fs.st_mode) || (S_IWGRP & fs.st_mode) || (S_IXGRP & fs.st_mode) ||
						(S_IROTH & fs.st_mode) || (S_IWOTH & fs.st_mode) || (S_IXOTH & fs.st_mode) ) != 0) { 
				return ret;
			}	
		}
	}
	memset(cmdline, 0, sizeof(cmdline));
#ifdef CONF_SAMBA_CIFS_MOUNT_READ_ONLY
	snprintf(cmdline, sizeof(cmdline), "mount -t cifs \"//%s/%s\" \"%s/%s/%s\" -o ro,username=\"%s\",password=\"%s\",iocharset=utf8,direct,nobrl,rsize=%d", ip,dir, CONF_SAMBA_MOUNT_POINT, pcname, dir, name, passwd, SMBTREE_MOUNT_RSIZE);
#else
	snprintf(cmdline, sizeof(cmdline), "mount -w -t cifs \"//%s/%s\" \"%s/%s/%s\" -o username=\"%s\",password=\"%s\",iocharset=utf8,direct,nobrl,rsize=%d", ip,dir, CONF_SAMBA_MOUNT_POINT, pcname, dir, name, passwd, SMBTREE_MOUNT_RSIZE);
#endif
	ret = system(cmdline);
	if(ret != 0) {
		printf("[%s:%d] %s failed!!!!!\n", __FUNCTION__, __LINE__, cmdline);
		memset(cmdline, 0, sizeof(cmdline));
		snprintf(cmdline, sizeof(cmdline), "rm \"%s/%s/%s\" -rf", CONF_SAMBA_MOUNT_POINT, pcname, dir);	
		system(cmdline);
	}
	return ret;

}

/*
 * FUNCTION	:	static BOOL access_dir(char* name, char* user, char *passwd)
 * DESCRIPTION	:	check user can access directory or not
 * INPUT	:	char* name - server name
 * 			char* user - username
 * 			char *passwd - password
 * 			int id -
 * OUTPUT	:	N/A
 * RETURN	:	TRUE - can access
 * 			FALSE - can't access 
 */
void *access_dir(void *value)
{
	int err, dh1;
	char url[MAX_NAME_LEN] = {0};
	struct smbserverinfo_t *psmbserverinfo = NULL;

	if(value == NULL)
		return NULL;

	psmbserverinfo = (struct smbserverinfo_t *)value;

#ifdef ENABLE_SMBTREE_PASSWORD_DISPLAY
	printf("%s[%d], name = [%s], ip = [%s], user = [%s], passwd = [%s], cmd_id = [%d]\n", __FILE__, __LINE__, psmbserverinfo->name, psmbserverinfo->ip, psmbserverinfo->user, psmbserverinfo->passwd, psmbserverinfo->cmd_id);
#endif
	memset(smb_username, 0, sizeof(smb_username));
	memset(smb_passwd, 0, sizeof(smb_passwd));
	if(psmbserverinfo->user != NULL) {
		strncpy(smb_username, psmbserverinfo->user, strlen(psmbserverinfo->user));
		smb_username[strlen(psmbserverinfo->user)] = '\0';
	}
	if(psmbserverinfo->passwd != NULL) {
		strncpy(smb_passwd, psmbserverinfo->passwd, strlen(psmbserverinfo->passwd));
		smb_passwd[strlen(psmbserverinfo->passwd)] = '\0';
	}

	snprintf(url, sizeof(url), "smb://%s:%s@%s", smb_username, smb_passwd, psmbserverinfo->name);
	err = smbc_init(get_static_auth_data_fn,  0); /* Initialize things */
	if (err < 0) {
		printf("%s[%d], Initializing the smbclient library ...: %s\n", __FILE__, __LINE__, strerror(errno));
		if(errno == EACCES) {
			printf("\033[1;33;41m%s[%d], errno = EACCES (%s)\033[0m\n", __FILE__, __LINE__, strerror(errno));
			SMBTREE_send_event_to_OSD(SMB_CMD_ACCESS_RESPONSE, SMB_ACCESS_LOGIN_FAILED, psmbserverinfo->cmd_id, NULL);
		} else {
			printf("\033[1;33;41m%s[%d], SMB_ACCESS_FAILED\033[0m\n", __FILE__, __LINE__);
			SMBTREE_send_event_to_OSD(SMB_CMD_ACCESS_RESPONSE, SMB_ACCESS_FAILED, psmbserverinfo->cmd_id, NULL);
		}
		SMBTREE_free_smbserverinfo(psmbserverinfo);
		return NULL;
	}
	if ((dh1 = smbc_opendir(url))<1) {
#ifdef ENABLE_SMBTREE_PASSWORD_DISPLAY
		printf("%s[%d], Could not open directory: %s: %s\n", __FILE__, __LINE__, url, strerror(errno));
#else //ENABLE_SMBTREE_PASSWORD_DISPLAY
		printf("%s[%d], Could not open directory: %s\n", __FILE__, __LINE__, strerror(errno));
#endif //ENABLE_SMBTREE_PASSWORD_DISPLAY
		if(errno == EACCES) {
			printf("\033[1;33;41m%s[%d], errno = EACCES (%s)\033[0m\n", __FILE__, __LINE__, strerror(errno));
			SMBTREE_send_event_to_OSD(SMB_CMD_ACCESS_RESPONSE, SMB_ACCESS_LOGIN_FAILED, psmbserverinfo->cmd_id, NULL);
		} else {
			printf("\033[1;33;41m%s[%d], SMB_ACCESS_FAILED\033[0m\n", __FILE__, __LINE__);
			SMBTREE_send_event_to_OSD(SMB_CMD_ACCESS_RESPONSE, SMB_ACCESS_FAILED, psmbserverinfo->cmd_id, NULL);
		}
		SMBTREE_free_smbserverinfo(psmbserverinfo);
		return NULL;
	}

	smbc_closedir(dh1);

	SMBTREE_send_event_to_OSD(SMB_CMD_ACCESS_RESPONSE, SMB_ACCESS_OK, psmbserverinfo->cmd_id, NULL);
	SMBTREE_free_smbserverinfo(psmbserverinfo);
	return NULL;
}

static void get_static_auth_data_fn(const char * pServer,
		const char * pShare,
		char * pWorkgroup,
		int maxLenWorkgroup,
		char * pUsername,
		int maxLenUsername,
		char * pPassword,
		int maxLenPassword)
{
	strncpy(pUsername, smb_username, maxLenUsername);
	strncpy(pPassword, smb_passwd, maxLenPassword);
}

void *smb_getdir(void *value)
{
	int err, dh1, dsize, dirc, i, count;
	char dirbuf[2048] = {0};
	struct smbc_dirent *dirp;
	char url[MAX_NAME_LEN] = {0};
	char buf[MAX_NAME_LEN] = {0};
	char tmpdirname[MAX_NAME_LEN] = {0};
	char dir_path[MAX_NAME_LEN] = {0};
	int ret, mount_ok_num = 0, mount_fail_num = 0;
	char *p = NULL;
	struct smbserverinfo_t *psmbserverinfo = NULL;
	struct smbEventProxyCmd *pevent = NULL;

	if(value == NULL)
		return NULL;

	psmbserverinfo = (struct smbserverinfo_t *)value;

	//printf("%s[%d], name = [%s], ip = [%s], user = [%s], passwd = [%s], cmd_id = [%d]\n", __FILE__, __LINE__, psmbserverinfo->name, psmbserverinfo->ip, psmbserverinfo->user, psmbserverinfo->passwd, psmbserverinfo->cmd_id);

	memset(smb_username, 0, sizeof(smb_username));
	memset(smb_passwd, 0, sizeof(smb_passwd));
	if(psmbserverinfo->user != NULL) {
		strncpy(smb_username, psmbserverinfo->user, strlen(psmbserverinfo->user));
		smb_username[strlen(psmbserverinfo->user)] = '\0';
	}
	if(psmbserverinfo->passwd != NULL) {
		strncpy(smb_passwd, psmbserverinfo->passwd, strlen(psmbserverinfo->passwd));
		smb_passwd[strlen(psmbserverinfo->passwd)] = '\0';
	}

	snprintf(url, sizeof(url), "smb://%s:%s@%s", smb_username, smb_passwd, psmbserverinfo->name);
	err = smbc_init(get_static_auth_data_fn,  0); /* Initialize things */
	if (err < 0) {
		printf("%s[%d], Initializing the smbclient library ...: %s\n", __FILE__, __LINE__, strerror(errno));
		SMBTREE_send_event_to_OSD(SMB_CMD_BROWSE_RESPONSE, SMB_MOUNT_FAILED, psmbserverinfo->cmd_id, NULL);
		SMBTREE_free_smbserverinfo(psmbserverinfo);
		return NULL;
	}
	if ((dh1 = smbc_opendir(url))<1) {
		printf("%s[%d], Could not open directory: %s: %s\n", __FILE__, __LINE__, url, strerror(errno));
		SMBTREE_send_event_to_OSD(SMB_CMD_BROWSE_RESPONSE, SMB_MOUNT_FAILED, psmbserverinfo->cmd_id, NULL);
		SMBTREE_free_smbserverinfo(psmbserverinfo);
		return NULL;
	}

	snprintf(dir_path, sizeof(dir_path), "%s/%s", CONF_SAMBA_MOUNT_POINT, psmbserverinfo->name);
	smb_umount(dir_path);

	/* Now, list those directories, but in funny ways ... */
	/* keep call smbc_getdents() until there is no share folder to fix Device only can browse less than 8 root folder.*/
	while(0 != (dirc = smbc_getdents(dh1, (struct smbc_dirent *)dirbuf, sizeof(dirbuf)))) {
		if(dirc < 0) {
			fprintf(stderr, "Problems getting directory entries: %s\n", strerror(errno));
			smbc_closedir(dh1);
			SMBTREE_send_event_to_OSD(SMB_CMD_BROWSE_RESPONSE, SMB_MOUNT_FAILED, psmbserverinfo->cmd_id, NULL);
			SMBTREE_free_smbserverinfo(psmbserverinfo);
			return NULL;	
		}
		/* Now, process the list of names ... */
		fprintf(stdout, "Directory listing, size = %u\n", dirc);
		
		dirp = (struct smbc_dirent *)dirbuf;
		count = 0;
		mount_ok_num = 0;
		mount_fail_num = 0;

		while (dirc > 0) {
			dsize = dirp->dirlen;
			switch (dirp->smbc_type) {
				case SMBC_DIR:
				case SMBC_FILE:
				case SMBC_FILE_SHARE:
					snprintf(buf, sizeof(buf), "%s\\%s\\%s", psmbserverinfo->name, psmbserverinfo->ip, dirp->name);
					fprintf(stdout, "Shared Dir Type: %u, Name: %s\n", dirp->smbc_type, dirp->name); 
					/*if share-folder-name is end of $, system doesn't mount it*/
					if((p=strrchr(buf, '$')) != NULL && strlen(p) == 1) {
						printf("%s[%d], ##### %s not match ####\n", __FILE__, __LINE__, buf);
						break;
					}
					memset(tmpdirname, '\0', sizeof(tmpdirname));
					smb_replace_special_char(dirp->name, tmpdirname);
					ret = smb_mount(psmbserverinfo->user, psmbserverinfo->passwd, psmbserverinfo->name, psmbserverinfo->ip, tmpdirname);
					if (ret == 0) {
#ifdef ENABLE_PTHREAD
						pevent = malloc(sizeof(struct smbEventProxyCmd));
						pevent->cmd = SMB_CMD_BROWSE_RESPONSE;
						pevent->status = SMB_MOUNTING;
						pevent->id = psmbserverinfo->cmd_id;
						pevent->datalen = strlen(buf) + 1;
						strcpy((char *)pevent->data, buf);
						XLinkedList_Lock(sharefolderlist);
							XLinkedList_AddTail(sharefolderlist, (void *)pevent);
						XLinkedList_UnLock(sharefolderlist);
#else
						SMBTREE_send_event_to_OSD(SMB_CMD_BROWSE_RESPONSE, SMB_MOUNTING, psmbserverinfo->cmd_id, buf);
#endif
						mount_ok_num++;
					} else {
						mount_fail_num++;
					}
					break;
				default:
					break;
			}
			dirp = (struct smbc_dirent *) ((char *) dirp + dsize);
			dirc -= dsize;
		}
		memset(dirbuf, 0, sizeof(dirbuf));
	}
	smbc_closedir(dh1);
	if(mount_fail_num > 0 && mount_ok_num == 0) {
		SMBTREE_send_event_to_OSD(SMB_CMD_BROWSE_RESPONSE, SMB_MOUNT_FAILED, psmbserverinfo->cmd_id, NULL);
	} else {
		SMBTREE_send_event_to_OSD(SMB_CMD_BROWSE_RESPONSE, SMB_MOUNT_OK, psmbserverinfo->cmd_id, NULL);
	}
	SMBTREE_free_smbserverinfo(psmbserverinfo);
	return NULL;
}

int get_serverlists_by_nmblookup(int id)
{
	char param[256] = {0}, param1[256] = {0};
	FILE *pfile = NULL, *pfile1 = NULL;
	char stream[MAX_FILE_READ_BUFFER_SIZE], stream1[MAX_FILE_READ_BUFFER_SIZE];
	char pcIpAddress[MAX_NAME_LEN], pcName[MAX_NAME_LEN];
	char aliases[MAX_NAME_LEN], type[MAX_NAME_LEN], length[MAX_NAME_LEN], addresses[MAX_NAME_LEN];
	char buf[MAX_NAME_LEN] = {0};
	struct smbEventProxyCmd *pevent = NULL;
	int samba_list_cnt = 0;
	char cmd[128] = {0};
	char samba_workgroup[16] = {0};

#ifdef CONF_SAMBA_SUPPORT_ONLY_BROWSE_WORKGROUP_SERVERS
	securesoho_samba_workgroup_get(samba_workgroup);
#endif //CONF_SAMBA_SUPPORT_ONLY_BROWSE_WORKGROUP_SERVERS

	snprintf(cmd, sizeof(cmd), "nmblookup  -s %s \\*", CONF_SAMBA_CONF_PATH);
	strcpy(param, cmd);
	pfile = popen(param, "r");
	if (NULL == pfile) {
		printf("popen failed\n");
		return -1;
	}

	memset(pcIpAddress, 0, MAX_NAME_LEN);
	memset(stream, 0, MAX_NAME_LEN);
	while (fgets(stream, MAX_FILE_READ_BUFFER_SIZE, pfile)) {
		char *c;
		char temp[MAX_NAME_LEN];

		sscanf(stream, "%s %s", pcIpAddress, temp);

		c = strchr(pcIpAddress, '.');
		if(NULL == c)
			continue;
		c++;

		c = strchr(pcIpAddress, '.');
		if(NULL == c)
			continue;
		c++;

		c = strchr(pcIpAddress, '.');
		if(NULL == c)
			continue;

#ifdef CONF_SAMBA_SUPPORT_ONLY_BROWSE_WORKGROUP_SERVERS
		snprintf(cmd, sizeof(cmd), "nmblookup -s %s -A %s|grep \"%s\"", CONF_SAMBA_CONF_PATH, pcIpAddress, samba_workgroup);
		strcpy(param1, cmd);
		pfile1 = popen(param1, "r");
		if (NULL == pfile1) {
			printf("popen failed (%s)\n", samba_workgroup);
			continue;
		}
		memset(stream, 0, MAX_FILE_READ_BUFFER_SIZE);
		if(fgets(stream, MAX_FILE_READ_BUFFER_SIZE, pfile1)) {
			pclose(pfile1);
		} else {
			pclose(pfile1);
			continue;
		}
#endif //CONF_SAMBA_SUPPORT_ONLY_BROWSE_WORKGROUP_SERVERS
		snprintf(cmd, sizeof(cmd), "nmblookup -s %s -A %s", CONF_SAMBA_CONF_PATH, pcIpAddress);
		strcpy(param1, cmd);
		pfile1 = popen(param1, "r");
		if (NULL == pfile1) {
			printf("popen failed\n");
			continue;
		}
		memset(pcName, 0, MAX_NAME_LEN);
		memset(stream1, 0, MAX_FILE_READ_BUFFER_SIZE);
		while (fgets(stream1, MAX_FILE_READ_BUFFER_SIZE, pfile1)) {
			if(strstr(stream1, "<00>") != NULL) {
				sscanf(stream1, "%s %s %s %s %s", pcName, aliases, type, length, addresses);
				sprintf(buf,"%s\\%s\\0", pcName, pcIpAddress); 
#ifdef ENABLE_SMBTREE_DEBUG_MODE
				printf("%s[%d], samber_sever:%s\n", __FUNCTION__, __LINE__, buf);
#endif //ENABLE_SMBTREE_DEBUG_MODE
#ifdef ENABLE_PTHREAD
				pevent = malloc(sizeof(struct smbEventProxyCmd));
				pevent->cmd = SMB_CMD_REFRESH_SERVER_RESPONSE;
				pevent->status = SMB_REFRESHING;
				pevent->id = id;
				pevent->datalen = strlen(buf) + 1;
				strcpy((char *)pevent->data, buf);
				XLinkedList_Lock(serverlist);
					XLinkedList_AddTail(serverlist, (void *)pevent);
				XLinkedList_UnLock(serverlist);
#else
				SMBTREE_send_event_to_OSD(SMB_CMD_REFRESH_SERVER_RESPONSE, SMB_REFRESHING, id, buf);
#endif
				samba_list_cnt++;
				break;
			}
			memset(stream1, 0, MAX_FILE_READ_BUFFER_SIZE);
		}
		pclose(pfile1);
		memset(stream, 0, MAX_FILE_READ_BUFFER_SIZE);
		memset(pcIpAddress, 0, MAX_NAME_LEN);
	}
	pclose(pfile);

	return (samba_list_cnt>0)?1:0;
}

void *get_serverlists(void *value)
{
	struct smb_name_list *wg, *sv;
	//struct smbEventProxyCmd *pcmd = (struct smbEventProxyCmd *)cmd;
	char buf[MAX_NAME_LEN] = {0};
	int get_serverlists_by_nmblookup_state = 0; /*0:fail; 1:ok*/
	int id = (int )value;
	struct smbEventProxyCmd *pevent = NULL;
	char samba_workgroup[16] = {0};
	struct user_auth_info *user_info = popt_common_get_auth_info();
	//securesoho_samba_workgroup_get(samba_workgroup);
	get_serverlists_by_nmblookup_state = get_serverlists_by_nmblookup(id);

	/* List workgroups */
	if(workgroups) {
		free_name_list(workgroups);
		workgroups = NULL;
	}
	if (!get_workgroups(user_info)) {
		is_getserver_done = 1;
		if(get_serverlists_by_nmblookup_state) {
			SMBTREE_send_event_to_OSD(SMB_CMD_REFRESH_SERVER_RESPONSE, SMB_REFRESHED_OK, id, NULL);
			printf("%s[%d], It's finish to search samba server(s). id:[%d]\n", __FILE__, __LINE__, id);
			return NULL;
		} else {
			SMBTREE_send_event_to_OSD(SMB_CMD_REFRESH_SERVER_RESPONSE, SMB_REFRESHED_FAILED, id, NULL);
			printf("%s[%d], It's finish to search samba server(s).\n", __FILE__, __LINE__);
			return NULL;
		}	
	}

	for (wg = workgroups; wg; wg = wg->next) {
#ifdef ENABLE_SMBTREE_DEBUG_MODE
		printf("%s[%d], WorkGroup : [%s]\n", __FILE__, __LINE__, wg->name);
#endif //ENABLE_SMBTREE_DEBUG_MODE
		/* List servers */

#ifdef CONF_SAMBA_SUPPORT_ONLY_BROWSE_WORKGROUP_SERVERS
		printf("samba_workgroup:[%s], wg->name:[%s]\n", samba_workgroup, wg->name);
		if(strcasecmp(samba_workgroup, wg->name) != 0) {
			printf("%s[%d], not match\n", __FILE__, __LINE__);
			continue;
		} else
			printf("%s[%d], match\n", __FILE__, __LINE__);
#endif //CONF_SAMBA_SUPPORT_ONLY_BROWSE_WORKGROUP_SERVERS
		if(servers) {
			free_name_list(servers);
			servers = NULL;
		}

		if (level == LEV_WORKGROUP || !get_servers(wg->name, user_info))
			continue;

		for (sv = servers; sv; sv = sv->next) {

			char* ip = NULL;
			nmblookup_ip(sv->name, &ip);
			if(ip != NULL) {
				snprintf(buf, sizeof(buf), "%s\\%s\\0", sv->name,ip); 
#ifdef ENABLE_SMBTREE_DEBUG_MODE
				printf("\t\\\\%-15s\t\t%s buf:%s\n", sv->name, sv->comment, buf);
#endif //ENABLE_SMBTREE_DEBUG_MODE
#ifdef ENABLE_PTHREAD
				pevent = malloc(sizeof(struct smbEventProxyCmd));
				pevent->cmd = SMB_CMD_REFRESH_SERVER_RESPONSE;
				pevent->status = SMB_REFRESHING;
				pevent->id = id;
				pevent->datalen = strlen(buf) + 1;
				strcpy((char *)pevent->data, buf);
				XLinkedList_Lock(serverlist);
					XLinkedList_AddTail(serverlist, (void *)pevent);
				XLinkedList_UnLock(serverlist);
#else
				SMBTREE_send_event_to_OSD(SMB_CMD_REFRESH_SERVER_RESPONSE, SMB_REFRESHING, id, buf);
#endif
				free(ip);
				ip = NULL;
			}
		}
	}

	printf("%s[%d], It's finish to search samba server(s). id:[%d]\n", __FILE__, __LINE__, id);
	SMBTREE_send_event_to_OSD(SMB_CMD_REFRESH_SERVER_RESPONSE, SMB_REFRESHED_OK, id, NULL);
	is_getserver_done = 1;

	if(workgroups) {
		free_name_list(workgroups);
		workgroups = NULL;
	}

	if(servers) {
		free_name_list(servers);
		servers = NULL;
	}

	return NULL;
}

static unsigned char smb_event_monitor_init_callback(void *readfd,void *writefd)
{
	smb_event_ep_fdr =  *(int *)readfd;	
	return 0;
}

static int smb_event_monitor_init()
{
	smb_event_ep_obj = create_event_proxy(EVENT_PROXY_FIFO,
			SMB_P2SFIFO,
			SMB_S2PFIFO,
			smb_event_monitor_init_callback);
	if (smb_event_ep_obj == NULL){
		printf("%s:%d create smb_event_ep_obj failed\n", __FILE__, __LINE__);
		return -1;
	}
	return 0;
}

static int smb_event_ep_send_event(void *event)
{
	struct smbEventProxyCmd *pcmd = (struct smbEventProxyCmd *)event;
	if (smb_event_ep_obj == NULL ) {
		fprintf(stderr,"create device_object failed\n");
		return -1;
	}
#ifdef ENABLE_LOCK
	sem_wait(&SMBTREE_LOCK);
#endif
	event_proxy_send_event(smb_event_ep_obj, event,sizeof(struct smbEventProxyCmd));
#ifdef ENABLE_LOCK
	sem_post(&SMBTREE_LOCK);
#endif
	return -1;
}

static int smb_event_handler (void *cmd, int cmd_len)
{
	int quit = -1;
	struct smbEventProxyCmd *pcmd = (struct smbEventProxyCmd *)cmd;
	char* ip = NULL;
	char server_name[MAX_NAME_LEN] = {0};
	char user[MAX_NAME_LEN] = {0};
	char passwd[MAX_NAME_LEN] = {0};
	pthread_t get_serverlist_pthread, browse_server_pthread;
	struct smbserverinfo_t *pSmbServerInfo = NULL;

	switch (pcmd->cmd) 
	{
		case SMB_CMD_REFRESH_SERVER:
#ifdef ENABLE_PTHREAD
			if(is_getserver_done) {
				printf("%s[%d], do get_serverlists\n", __FILE__, __LINE__);
				is_getserver_done = 0;
				pthread_create(&get_serverlist_pthread, NULL, get_serverlists, (void*)pcmd->id);
				pthread_detach(get_serverlist_pthread);
			}	
#else
			get_serverlists(pcmd);
#endif
			break;
		case SMB_CMD_ACCESS:
			sscanf((char *)pcmd->data, "%s %s %s", server_name, user, passwd);
			strHTTPUnEscape(user);
			strHTTPUnEscape(passwd);

			pSmbServerInfo = (struct smbserverinfo_t *)malloc(sizeof(struct smbserverinfo_t));
			if(pSmbServerInfo == NULL) {
				SMBTREE_send_event_to_OSD(SMB_CMD_ACCESS_RESPONSE, SMB_ACCESS_FAILED, pcmd->id, NULL);
				break;
			}
			pSmbServerInfo->name = strdup(server_name);
			pSmbServerInfo->ip = NULL;
			pSmbServerInfo->user = strdup(user);
			pSmbServerInfo->passwd = strdup(passwd);
			pSmbServerInfo->cmd_id = pcmd->id;

			access_dir(pSmbServerInfo);
			break;
		case SMB_CMD_BROWSE:
			sscanf((char *)pcmd->data, "%s %s %s", server_name, user, passwd);
			nmblookup_ip(server_name, &ip);
			strHTTPUnEscape(user);
			strHTTPUnEscape(passwd);

			pSmbServerInfo = (struct smbserverinfo_t *)malloc(sizeof(struct smbserverinfo_t));
			if(pSmbServerInfo == NULL) {
				SMBTREE_send_event_to_OSD(SMB_CMD_BROWSE_RESPONSE, SMB_MOUNT_FAILED, pcmd->id, NULL);
				break;
			}
			pSmbServerInfo->name = strdup(server_name);
			pSmbServerInfo->ip = strdup(ip);
			pSmbServerInfo->user = strdup(user);
			pSmbServerInfo->passwd = strdup(passwd);
			pSmbServerInfo->cmd_id = pcmd->id;
			free(ip);

#ifdef ENABLE_PTHREAD
			pthread_create(&browse_server_pthread, NULL, smb_getdir, (void*)pSmbServerInfo);
			pthread_detach(browse_server_pthread);
#else
			smb_getdir(pSmbServerInfo);
#endif
			break;
		case SMB_CMD_UMOUNT:
			printf("cmd umount\n");
			break;
		case SMB_CMD_QUIT:
			quit = 0;
		default:
			printf ("Error cmd %d\n ", pcmd->cmd);
			break;
	}
	return quit;
}

static int smb_receive_cmd(int eventfd)
{
	fd_set rfds;
	int retval, ret;
	struct timeval tv;

	while(1) {
		FD_ZERO(&rfds);
		FD_SET(eventfd, &rfds);
		tv.tv_sec = 1; tv.tv_usec = 0; 

		retval = select(eventfd+1, &rfds, NULL, NULL, &tv);
		if( -1 == retval ) {
			if(EINTR == errno) {
				printf("error in select()....\n");
				continue;
			}
		} else if (retval) {
			//get return value
			if (FD_ISSET(eventfd, &rfds)) {
				void *cmd=NULL;
				int cmd_len;
				event_proxy_get_event(smb_event_ep_obj,(void *)&cmd, &cmd_len);
				if (!cmd){
					fprintf(stderr, "%s:%d received unknown command, please check it!\n", __FUNCTION__, __LINE__);
					return  false;
				}
				ret = smb_event_handler(cmd, cmd_len);
				free(cmd);
				cmd = NULL;
				if(ret == 0) {
					destory_event_proxy(smb_event_ep_obj);
					XLinkedList_Destroy(serverlist);
					XLinkedList_Destroy(sharefolderlist);
					serverlist = NULL;
					sharefolderlist = NULL;
#ifdef ENABLE_LOCK
					sem_destroy(&SMBTREE_LOCK);
#endif
					return true;
				}
			}
		} else {
			SMBTREE_send_list_to_OSD(serverlist);
			SMBTREE_send_list_to_OSD(sharefolderlist);
			continue;
		}
	}

}

static int  smb_event_ep_get_event_fd()
{
	return smb_event_ep_fdr;
}

static void smb_proc(void)
{
	fd_set readset;
	int eventfd;

	FD_ZERO(&readset);
	eventfd = smb_event_ep_get_event_fd();
	if( eventfd < 0 ){
		return;
	}
	smb_receive_cmd(eventfd);
}
