
#ifndef _RTL871X_WLAN_MLME_H_
#define _RTL871X_WLAN_MLME_H_

#define SURVEY_TO	(40)
#define REAUTH_TO		(1000)
#define REASSOC_TO		(1000)
#define DISCONNECT_TO	(10000)

#define REAUTH_LIMIT	6
#define REASSOC_LIMIT	6


enum Synchronization_Sta_State {
        STATE_Sta_Min			= 0,
        STATE_Sta_No_Bss		= 1,
        STATE_Sta_Bss			= 2,
        STATE_Sta_Ibss_Active	= 3,
        STATE_Sta_Ibss_Idle		= 4,
        STATE_Sta_Auth_Success	= 5,
        STATE_Sta_Roaming_Scan	= 6,
};

void report_join_res(_adapter *padapter, int res);

unsigned int issue_assocreq(_adapter *padapter);
void issue_auth(_adapter *padapter, struct sta_info *pstat, unsigned short status);
void issue_probereq(_adapter *padapter);

void start_clnt_assoc(_adapter *padapter);
void start_clnt_auth(_adapter* padapter);
void start_clnt_join(_adapter* padapter);

void report_BSSID_info(_adapter *padapter, u8 *pframe, uint len);
void site_survey(_adapter *padapter);


void survey_timer_hdl (_adapter *padapter);
void reauth_timer_hdl(_adapter *padapter);
void reassoc_timer_hdl(_adapter *padapter);

#endif
