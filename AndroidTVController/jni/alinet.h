#ifndef	_ALINET_H_
#define	_ALINET_H_

#include <unistd.h>   
#include <stdio.h>
#include <stdlib.h> 
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <jni.h>
#include <stdlib.h>
#include <linux/tcp.h>
#include <android/log.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
typedef struct _ALI_TIME
{
	unsigned int  mjd;
	unsigned short	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	weekday;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
}ALI_TIME;

typedef struct _ALI_HMS
{
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
}ALI_HMS;

typedef struct _ALI_LANG
{
	char	*event_name; 
	char	*short_detail;
	char	*ext_detail;
}ALI_LANG;

typedef struct _ALI_EPG_EVENT
{  
	ALI_HMS    start_time;               
	ALI_HMS    end_time;

	ALI_LANG    lang[3];
}ALI_EPG_EVENT;

typedef struct
{
	int startevent;
	int endevent;
	unsigned short	start_year;
	unsigned char	start_month;
	unsigned char	start_day;
	unsigned char	weekday;
} DayStartEnd;

typedef struct _ALI_EPG_DATA
{
	char			*chan_name;
	unsigned int	event_num;
	DayStartEnd		datainfo[8];
	int				langnum;
	char			*language[3];
	ALI_EPG_EVENT*	ALi_epg_data;  
}ALI_EPG_DATA;

typedef struct
{
	short broadcastport;
	char stbip[256][16];
	int stbnum;
	int curstbsock;
	ALI_EPG_DATA* m_pepg;
	char* pdata;
	char* plastepg;
	int pdata_size;
	pthread_mutex_t m_lock;
} AliPlayCons;

typedef enum
{
	ALI_MSG_DISCONNECT = 1,
	ALI_MSG_KEY,
	ALI_MSG_EPG,
	ALI_MSG_SYN,
}ALISTB_MSG_TYPE;

typedef struct
{
	short type;
	short key;
} ALI_MSG;

int tcp_connect(const char *rip, int rport, int timeout);
int tcp_connect(int s_addr, int rport, int timeout);
int tcp_accept(int socketid, int itimeout);
int tcp_recv(int socketid, char* recbuf, int size, int itimeout);
int tcp_send(int socketid, char* recbuf, int size, int itimeout);
int tcp_close(int socketid);
int udp_create(short port);
int sendbrocast(int socket_fd, short port);
int udp_recv(int socketid, int itimeout, char *);

#endif	// _ALINET_H_

