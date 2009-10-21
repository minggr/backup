#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>

#define BUFSIZE 1024
char buf[BUFSIZE];

void serve(int s)
{
	int n;

	n = read(s, buf, BUFSIZE);

	if (n == -1)
		perror("read");
	else
		printf("%s\n", buf);	
}

int main(int argc, char *argv[])
{
	int s;
	struct sockaddr_in myaddr;
	int optval;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	/*
	 * Socket option SO_REUSEADDR: Allow bind(), even when old 
	 * protocol instances are still using the address.
	 */
	optval = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
		perror("setsockopt");
		exit(1);
	}
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(atoi(argv[1]));
	myaddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr))) {
		perror("bind");
		exit(1);
	}
	if (listen(s, SOMAXCONN)) {
		perror("listen");
		exit(1);
	}

	while (1) {
		int new_s;
		struct sockaddr_in claddr;
		int claddrlen;
		claddrlen = sizeof(claddr);
		if ((new_s =
		     accept(s, (struct sockaddr *)&claddr, &claddrlen)) < 0) {
			perror("accept");
			continue;
		}
		if (fork()) {	/* Parent process */
			close(new_s);	/* New socket is used by child process only. */
		} else {	/* Child process */
			close(s);	/* Old socket is used by parent process only. */

			serve(s);

			exit(0);
		}
	}
}
