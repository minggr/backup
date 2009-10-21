#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFSIZE 1024
char buf[BUFSIZE] = "Hello World!";

/* Main program: 
 * * -   Process arguments. 
 * * -   Open socket and establish connection to server. 
 * * -   Read text line by line and send it over this connection. 
 * * -   Close connection at end of entry (Ctrl-D). 
 * */
int main(int argc, char *argv[])
{
	int s;
	struct sockaddr_in addr;
	char *p;
	int n;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <address> <port>\n", argv[0]);
		exit(1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	if (connect(s, (struct sockaddr *)&addr, sizeof(addr))) {
		perror("connect");
		exit(1);
	}

	while (1) {
		printf("%s\n", buf);
		n = write(s, buf, strlen(buf));
		if (n == -1) {
			perror("write");
			break;
		}

		printf("%d\n", n);
	}
	close(s);
	exit(0);
}
