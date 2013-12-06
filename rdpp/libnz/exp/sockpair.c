#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main() {
	int sv[2];
	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sv)) {
		fprintf(stderr, "*** error: %s", strerror(errno));
		return -1;
	}
	pid_t pid;
	if ((pid = fork()) == -1) {
		perror("fork");
		return -1;
	} else if (pid == 0) { // child process
		char rbuf[80];
		printf("ppid = %d\n", getppid());
		close(sv[0]);
		sv[0] = -1;
		char *fmt = "%F %T";
		printf("send request: %s\n", fmt);
		if (write(sv[1], fmt, strlen(fmt)) != strlen(fmt)) {
			perror("write");
			close(sv[1]);
			return -1;
		}
		if (shutdown(sv[1], SHUT_WR) == -1) {
			perror("shutdown");
			return -1;
		}
		int ret = read(sv[1], rbuf, sizeof rbuf);
		if (ret == -1) {
			perror("read");
			close(sv[1]);
			return -1;
		}
		rbuf[ret] = '\0';
		printf("server respond: %s\n", rbuf);
		close(sv[1]);
	} else { // parent process
		char buf[80];
		char wbuf[80];
		int status;
		printf("cpid = %d\n", pid);
		time_t t;
		close(sv[1]);
		sv[1] = -1;
		int sz = read(sv[0], buf, sizeof buf);
		if (sz == -1) {
			perror("read");
			close(sv[0]);
			return -1;
		}
		wbuf[sz] = '\0';
		time(&t);
		strftime(wbuf, sizeof wbuf,
				buf, localtime(&t));
		sz = write(sv[0], wbuf, strlen(wbuf));
		if (sz == -1) {
			perror("write");
			close(sv[0]);
			return -1;
		}
		close(sv[0]);
		waitpid(pid, &status, 0);
		printf("child process exit status: %d\n", status);
	}
	return 0;
}
