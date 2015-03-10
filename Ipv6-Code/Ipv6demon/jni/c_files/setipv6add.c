#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <netdb.h>
#include <linux/in6.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT    6666
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512





int setipv6(char const   *eth,  char const  *ipv6add, int prefix)
{
		char *server = "127.0.0.1";
		int ret;
		int flag = 0;
	  //设置一个socket地址结构client_addr,代表客户机internet地址, 端口
	    struct sockaddr_in client_addr;
	    bzero(&client_addr,sizeof(client_addr)); //把一段内存区的内容全部设置为0
	    client_addr.sin_family = AF_INET;    //internet协议族
	    client_addr.sin_addr.s_addr = htons(INADDR_ANY);//INADDR_ANY表示自动获取本机地址
	    client_addr.sin_port = htons(0);    //0表示让系统自动分配一个空闲端口
	    //创建用于internet的流协议(TCP)socket,用client_socket代表客户机socket
	    int client_socket = socket(AF_INET,SOCK_STREAM,0);
	    if( client_socket < 0)
	    {
	        printf("Create Socket Failed!\n");
	        exit(1);
	    }
	    //把客户机的socket和客户机的socket地址结构联系起来
	    if( bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
	    {
	        printf("Client Bind Port Failed!\n");
	        exit(1);
	    }

	    //设置一个socket地址结构server_addr,代表服务器的internet地址, 端口
	    struct sockaddr_in server_addr;
	    bzero(&server_addr,sizeof(server_addr));
	    server_addr.sin_family = AF_INET;
	    if(inet_aton(server,&server_addr.sin_addr) == 0) //服务器的IP地址来自程序的参数
	    {
	        printf("Server IP Address Error!\n");
	        exit(1);
	    }
	    server_addr.sin_port = htons(SERVER_PORT);
	    socklen_t server_addr_length = sizeof(server_addr);
	    //向服务器发起连接,连接成功后client_socket代表了客户机和服务器的一个socket连接
	    if(connect(client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0)
	    {
	        printf("Can Not Connect To %s!\n",server);
	        exit(1);
	    }

	    char file_name[FILE_NAME_MAX_SIZE+1];
	    bzero(file_name, FILE_NAME_MAX_SIZE+1);
	    //printf("Please Input File Name On Server:\t");
	    //scanf("%s", file_name);
	    strcpy(file_name, ipv6add);

	    char buffer[BUFFER_SIZE];
	    bzero(buffer,BUFFER_SIZE);
	    strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name));
	    //向服务器发送buffer中的数据
	    send(client_socket,buffer,BUFFER_SIZE,0);

	    //从服务器接收数据到buffer中
	    bzero(buffer,BUFFER_SIZE);
	    int length = 0;
	    while( length = recv(client_socket,buffer,BUFFER_SIZE,0))
	    {
	    	flag = 1;
	    	switch(buffer[0]) {

		    	case 'R':
		    		ret =-2;
		    		break;
		    	case 'i':
		    		ret = -1;
		    		break;
		    	case 's': if(buffer[1] == 'o') ret = -2;
		    			  else if (buffer[1] == 'e') ret = 0;
		    		break;
		    	default:
		    		ret = -2;
		    		break;
		    }
		//printf("%d\n", strlen(buffer));
		//ret = buffer;
		//printf("%s\n",ret);
	    bzero(buffer,BUFFER_SIZE);
	    }
	    //关闭socket
	    if(flag == 0) ret = -2;
	    close(client_socket);
	    return ret;
}
