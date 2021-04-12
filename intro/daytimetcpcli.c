#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	//setting entire structure to 0
	bzero(&servaddr, sizeof(servaddr));

	//address family is AF_INET
	servaddr.sin_family = AF_INET;

	//daytime server port is 13
	//htons = host to network short, convert 13 to binary fmt
	servaddr.sin_port   = htons(13);	/* daytime server */

	//picking ip address
	//pton = presentation to numeric
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	//establishing tcp conn
	//(SA *) is a generic socket address structure
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	//the server's reply is 26-byte string
	//when read() returns val > 0, conn is closed
	//using HTTP 1.0v technique with '\r\b' chars
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}

	if (n < 0)
		err_sys("read error");

	exit(0);
}
