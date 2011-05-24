/*
 * dhcpcd - DHCP client daemon -
 * Copyright (C) 1996 - 1997 Yoichi Hariguchi <yoichi@fore.com>
 * Copyright (C) January, 1998 Sergei Viznyuk <sv@phystech.com>
 * 
 * dhcpcd is an RFC2131 and RFC1541 compliant DHCP client daemon.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include "pathnames.h"
#include "client.h"
#include "cache.h"

extern char		*ProgramName;
extern char		*IfNameExt;
extern char		*ConfigDir;
extern int		DebugFlag;
extern jmp_buf		env;
extern void		*(*currState)();
/*****************************************************************************/
void killPid(sig)
int sig;
{
  FILE *fp;
  pid_t pid;
  char pidfile[64];
  snprintf(pidfile,sizeof(pidfile),PID_FILE_PATH,ConfigDir,IfNameExt);
  fp=fopen(pidfile,"r");
  if ( fp == NULL ) goto ntrn;
  fscanf(fp,"%u",&pid);
  fclose(fp);
  if ( kill(pid,sig) )
    {
      unlink(pidfile);
ntrn: if ( sig == SIGALRM ) return;
      fprintf(stderr,"****  %s: not running\n",ProgramName);
      exit(1);
    }
  exit(0);
}
/*****************************************************************************/
void writePidFile(pid_t pid)
{
  FILE *fp;
  char pidfile[64];
#if 1
  if (strcmp(IfNameExt, "usbeth0") == 0)
	  snprintf(pidfile,sizeof(pidfile),PID_FILE_PATH,ConfigDir,IfNameExt);
  else
	  snprintf(pidfile,sizeof(pidfile),PID_FILE_PATH,ConfigDir,"lan");
#else
  snprintf(pidfile, sizeof(pidfile), PID_FILE_PATH_EXT, ConfigDir);
#endif
  fp=fopen(pidfile,"w");
  if ( fp == NULL )
    {
      syslog(LOG_ERR,"writePidFile: fopen: %m\n");
      exit(1);
    }
  fprintf(fp,"%u\n",pid);
  fclose (fp);
}
/*****************************************************************************/
void deletePidFile()
{
  char pidfile[64];
#if 1
  if (strcmp(IfNameExt, "usbeth0") == 0)
	  snprintf(pidfile,sizeof(pidfile),PID_FILE_PATH,ConfigDir,IfNameExt);
  else
	  snprintf(pidfile,sizeof(pidfile),PID_FILE_PATH,ConfigDir,"lan");
#else
  snprintf(pidfile, sizeof(pidfile), PID_FILE_PATH_EXT, ConfigDir);
#endif
  unlink(pidfile);
}
/*****************************************************************************/
void sigHandler(sig)
int sig;
{
	if( sig == SIGCHLD ){
		waitpid(-1,NULL,WNOHANG);
		return;
	}
	if ( sig == SIGALRM ){
		if ( currState == &dhcpBound ){
			syslog(LOG_ERR,"timed out T1\n");
			siglongjmp(env,1); /* this timeout is T1 */
		}else if ( currState == &dhcpRenew ){
			syslog(LOG_ERR,"timed out T2\n");
			siglongjmp(env,2); /* this timeout is T2 */
		}else if ( currState == &dhcpRebind ){
			syslog(LOG_ERR,"timed out dhcpIpLeaseTime\n");
#ifdef __EM86XX__
			execute_on_change("down");
  			if (strcmp(IfNameExt, "usbeth0") == 0)
				touch("/tmp/dhcpcd_lease_fail_yota");
			else
				touch("/tmp/dhcpcd_lease_fail");
#else
  			if (strcmp(IfNameExt, "usbeth0") == 0)
				touch("/tmp/dhcpcd_timeout_yota");
			else
				touch("/tmp/dhcpcd_timeout");
#endif
			siglongjmp(env,3);  /* this timeout is dhcpIpLeaseTime */
		}else if ( currState == &dhcpReboot ){
			syslog(LOG_ERR,"fail to acquire the same IP address\n");
			siglongjmp(env,4);  /* failed to acquire the same IP address */
#ifdef __EM86XX__
		}else if ( currState == &dhcpInit){
			syslog(LOG_ERR,"timed out waiting for a valid DHCP server response\n");
			execute_on_change("down");
			siglongjmp(env,5);  /* failed to wait for server response */
#endif
		}else {
			syslog(LOG_ERR,"timed out for unknown reasons\n");
		}
	}else{
		if ( sig == SIGHUP ) {
			dhcpRelease();
			/* allow time for final packets to be transmitted before shutting down     */
			/* otherwise 2.0 drops unsent packets. fixme: find a better way than sleep */
			sleep(1);
		}
		syslog(LOG_ERR,"terminating on signal %d\n",sig);
	}

	dhcpStop();
	deletePidFile();
	exit(sig);
}
/*****************************************************************************/
void signalSetup()
{
  int i;
  struct sigaction action;
  sigaction(SIGHUP,NULL,&action);
  action.sa_handler= &sigHandler;
  action.sa_flags = 0;
  for (i=1;i<16;i++) sigaction(i,&action,NULL);
  sigaction(SIGCHLD,&action,NULL);
}
