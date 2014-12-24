#include "netconfig.h"

int UDPClientSetup( char *host, struct sockaddr_in *server )
{
	int sockfd, n;
	struct sockaddr_in servaddr;

	sockfd = socket( AF_INET, SOCK_DGRAM, 0);

	bzero( &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
   	servaddr.sin_addr.s_addr = inet_addr( host );
   	servaddr.sin_port = htons( PORT );
	
	*server = servaddr;
	return sockfd;
}

int UDPServerSetup( struct sockaddr_in *server )
{
	int sockfd, n;
	struct sockaddr_in servaddr;
  	
	sockfd = socket( AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr,sizeof(servaddr));
   	servaddr.sin_family = AF_INET;
   	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   	servaddr.sin_port = htons(PORT);

	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	*server = servaddr;
	return sockfd;
}
