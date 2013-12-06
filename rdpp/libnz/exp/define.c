#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int define(const char* word, int color, int verbose, int timeout) {
	struct addrinfo hints, *res, *res0;
	int error;
	int s;
	const char *cause = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	error = getaddrinfo("dict.youdao.com", "http", &hints, &res0);
	if (error) {
		errx(1, "%s", gai_strerror(error));
		/*NOTREACHED*/
	}
	s = -1;
	for (res = res0; res; res = res->ai_next) {
		s = socket(res->ai_family, res->ai_socktype,
				res->ai_protocol);
		if (s < 0) {
			cause = "socket";
			continue;
		}

		if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
			cause = "connect";
			close(s);
			s = -1;
			continue;
		}

		break;  /* okay we got one */
	}
	if (s < 0) {
		err(1, "%s", cause);
		/*NOTREACHED*/
	}
	freeaddrinfo(res0);
	return 0;
}

int main(int argc, char* argv[]) {
	define("hello", 1, 1, 1);
	return 0;
}
