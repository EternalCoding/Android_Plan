#include <string.h>
#include "alinet.h"
#include <errno.h>

#define	SOCKET_TAG		"SOCKET"

int tcp_connect(const char *rip, int rport, int timeout)
{ 
	int res; 
	struct sockaddr_in addr; 
	long arg; 
	fd_set myset; 
	struct timeval tv; 
	int valopt; 
	socklen_t lon; 
	int soc;

	// Create socket 
	soc = socket(AF_INET, SOCK_STREAM, 0); 
	if (soc < 0)
	{
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error creating socket (%d %s)/n", errno, strerror(errno)); 
		return 0; 
	}

	addr.sin_family = AF_INET; 
	addr.sin_port = htons(rport); 
	addr.sin_addr.s_addr = inet_addr(rip); 

	// Set non-blocking 
	if( (arg = fcntl(soc, F_GETFL, NULL)) < 0)
	{ 
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error fcntl(..., F_GETFL) (%s)/n", strerror(errno)); 
		close(soc);
		return 0; 
	} 
	arg |= O_NONBLOCK; 
	if( fcntl(soc, F_SETFL, arg) < 0)
	{ 
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error fcntl(..., F_SETFL) (%s)/n", strerror(errno)); 
		close(soc);
		return 0; 
	} 

	arg = 1;
	if(setsockopt( soc, IPPROTO_TCP, TCP_NODELAY, (char *)&arg, sizeof(arg)) < 0) 
	{ 
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error fcntl(..., TCP_NODELAY) (%s)/n", strerror(errno)); 
		close(soc);
		return 0; 
	} 
	
	
	// Trying to connect with timeout 
	res = connect(soc, (struct sockaddr *)&addr, sizeof(addr)); 
	if(res < 0)
	{ 
		if (errno == EINPROGRESS)
		{
			__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "EINPROGRESS in connect() - selecting/n"); 
			FD_ZERO(&myset); 
			tv.tv_sec = timeout/1000;
			tv.tv_usec = (timeout%1000) * 1000;
			FD_SET(soc, &myset); 
			res = select(soc+1, NULL, &myset, NULL, &tv); 
			if (res < 0 && errno != EINTR)
			{
				__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error connecting %d - %s/n", errno, strerror(errno)); 
				close(soc);
				return 0; 
			} 
			else if (res > 0)
			{
				// Socket selected for write 
				lon = sizeof(int); 
				if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0)
				{
					__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error in getsockopt() %d - %s/n", errno, strerror(errno)); 
					close(soc);
					return 0; 
				} 
				// Check the value returned... 
				if (valopt)
				{
					__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error in delayed connection() %d - %s/n", valopt, strerror(valopt) ); 
					close(soc);
					return 0; 
				} 
				return soc;
			} 
			else
			{
				__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Timeout in select() - Cancelling!/n"); 
				return 0; 
			} 
		} 
		else
		{
			__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error connecting %d - %s/n", errno, strerror(errno)); 
			return 0; 
		} 
	} 
	return 0;
}

int tcp_connect(int s_addr, int rport, int timeout)
{ 
	int res; 
	struct sockaddr_in addr; 
	long arg; 
	fd_set myset; 
	struct timeval tv; 
	int valopt; 
	socklen_t lon; 
	int soc;

	// Create socket 
	soc = socket(AF_INET, SOCK_STREAM, 0); 
	if (soc < 0)
	{
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error creating socket (%d %s)/n", errno, strerror(errno)); 
		return 0; 
	}

	addr.sin_family = AF_INET; 
	addr.sin_port = htons(rport); 
	addr.sin_addr.s_addr = s_addr; 

	// Set non-blocking 
	if( (arg = fcntl(soc, F_GETFL, NULL)) < 0)
	{ 
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error fcntl(..., F_GETFL) (%s)/n", strerror(errno)); 
		close(soc);
		return 0; 
	} 
	arg |= O_NONBLOCK; 
	if( fcntl(soc, F_SETFL, arg) < 0)
	{ 
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error fcntl(..., F_SETFL) (%s)/n", strerror(errno)); 
		close(soc);
		return 0; 
	} 

	// Trying to connect with timeout 
	res = connect(soc, (struct sockaddr *)&addr, sizeof(addr)); 
	if(res < 0)
	{ 
		if (errno == EINPROGRESS)
		{
			__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "EINPROGRESS in connect() - selecting/n"); 
			FD_ZERO(&myset); 
			tv.tv_sec = timeout/1000;
			tv.tv_usec = (timeout%1000) * 1000;
			FD_SET(soc, &myset); 
			res = select(soc+1, NULL, &myset, NULL, &tv); 
			if (res < 0 && errno != EINTR)
			{
				__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error connecting %d - %s/n", errno, strerror(errno)); 
				close(soc);
				return 0; 
			} 
			else if (res > 0)
			{
				// Socket selected for write 
				lon = sizeof(int); 
				if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0)
				{
					__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error in getsockopt() %d - %s/n", errno, strerror(errno)); 
					close(soc);
					return 0; 
				} 
				// Check the value returned... 
				if (valopt)
				{
					__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error in delayed connection() %d - %s/n", valopt, strerror(valopt) ); 
					close(soc);
					return 0; 
				} 
				return soc;
			} 
			else
			{
				__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Timeout in select() - Cancelling!/n"); 
				return 0; 
			} 
		} 
		else
		{
			__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "Error connecting %d - %s/n", errno, strerror(errno)); 
			return 0; 
		} 
	} 
	return 0;
}

int tcp_accept(int socketid, int itimeout)
{
	fd_set fdread;
	int rc;
	int retlen;
	struct timeval timeout;

    FD_ZERO(&fdread);
    timeout.tv_sec = itimeout/1000;
    timeout.tv_usec = (itimeout%1000) * 1000;
	FD_SET(socketid,&fdread); //Ìí¼ÓÃèÊö·û 
	
    rc = select(socketid + 1, &fdread, NULL, NULL, &timeout);
	__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "accept select ret %d", rc);
	if(rc < 0)
		return -1;
	if(rc == 0)
		return 0;
	return accept(socketid, NULL, NULL);
}

int tcp_recv(int socketid, char* recbuf, int size, int itimeout)
{
	fd_set fdread;
	int rc;
	int retlen;
	struct timeval timeout;
	int len = 0;

	while(size != 0)
	{
	    FD_ZERO(&fdread);
		timeout.tv_sec = itimeout/1000;
		timeout.tv_usec = (itimeout%1000) * 1000;
		FD_SET(socketid,&fdread); //Ìí¼ÓÃèÊö·û 

		rc = select(socketid + 1, &fdread, NULL, NULL, &timeout);
//		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "recv select ret %d", rc);
		if(rc <= 0)
		{
			__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "socket recv error");
			return -1;
		}

		retlen = recv(socketid, recbuf + len, size, 0);
//		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "recv ret %d", retlen);
		if(retlen == 0)
		{
			if(len != 0)
				return len;
			else
				return -1;
		}

		size -= retlen;
		len += retlen;
	}

	return len;
}

int tcp_send(int socketid, char* recbuf, int size, int itimeout)
{
	fd_set fdwrite;
	int rc;
	int retlen;
	struct timeval timeout;
	int len = 0;

	while(size != 0)
	{
	    FD_ZERO(&fdwrite);
		timeout.tv_sec = itimeout/1000;
		timeout.tv_usec = (itimeout%1000) * 1000;
		FD_SET(socketid,&fdwrite); //Ìí¼ÓÃèÊö·û 
		rc = select(socketid+1, NULL, &fdwrite, NULL, &timeout);
		if(rc <= 0)
		{
			__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "socket sent error");
			return -1;
		}

		retlen = send(socketid, recbuf, size, 0);
		if(retlen == 0)
			return -1;
		else
			return retlen;
	}
}

int tcp_close(int socketid)
{
	close(socketid);
}

int udp_create(short port)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	int b = 1;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "socket create error");
		return -1;
	}

	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &b, sizeof(b)) == -1)
	{
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "socket SO_REUSEADDR error");
		close(sockfd);
		return -1;
	}

	if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&b,sizeof(b)) == -1)
	{
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "socket SO_BROADCAST error");
		close(sockfd);
		return -1;
	}

	/* init servaddr */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //#define INADDR_ANY	((unsigned long int) 0x00000000)
	servaddr.sin_port = htons(port);

	/* bind address and port to socket */
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "bind error");
		close(sockfd);
		return -1;
	}
	return sockfd;
}

int sendbrocast(int socket_fd, short port)
{
	struct sockaddr_in my_addr;
	char buf[128];

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr=inet_addr("255.255.255.255");
	bzero(&(my_addr.sin_zero),8);

	strcpy(buf,"Hello,I'm on line!");

	if(sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr *)&my_addr,sizeof(my_addr)) != strlen(buf))
		return -1;
	if(sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr *)&my_addr,sizeof(my_addr)) != strlen(buf))
		return -1;
	if(sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr *)&my_addr,sizeof(my_addr)) != strlen(buf))
		return -1;
	return 0;
}


int udp_recv(int socketid, int itimeout, char* retaddr)
{
	fd_set fdread;
	char recbuf[80];
	int rc;
	struct timeval timeout;
	int len = 0;
	struct sockaddr_in user_addr;
	socklen_t usize;

	if(1)
	{
		FD_ZERO(&fdread);
		timeout.tv_sec = itimeout/1000;
		timeout.tv_usec = (itimeout%1000) * 1000;
		FD_SET(socketid,&fdread); //Ìí¼ÓÃèÊö·û 
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "recv select wait %d", itimeout);

		rc = select(socketid + 1, &fdread, NULL, NULL, &timeout);
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "recv select ret %d", rc);
		if(rc < 0)
			return -1;
		if(rc == 0)
			return -1;

		usize=sizeof(user_addr);
		len = recvfrom(socketid, recbuf, 80, 0, (struct sockaddr *)&user_addr,&usize);
		__android_log_print(ANDROID_LOG_INFO, SOCKET_TAG, "recv ret %d", len);
		if(len <= 0)
		{
			return -1;
		}
		__android_log_print(ANDROID_LOG_ERROR, SOCKET_TAG, "%s:%d\n", inet_ntoa(user_addr.sin_addr),user_addr.sin_port);
		sprintf(retaddr, "%s", inet_ntoa(user_addr.sin_addr));

	}

	return 0;
}

