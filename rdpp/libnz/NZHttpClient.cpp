#include "NZHttpClient.h"
#include "NZLogger.h"

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

using namespace std;
namespace NZ {
	struct __core {
		int sockfd;
		uint8_t buf[2048];
	};
	NZHttpClient::NZHttpClient(const char* addr, int port)
	: m_pcore(new struct __core) {
		m_pcore->sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if (m_pcore->sockfd < 0) throw NZSockExcept(strerror(errno));
		struct sockaddr_in server_addr;
		server_addr.sin_family = PF_INET;
		inet_aton(addr, &server_addr.sin_addr);
		//server_addr.sin_addr.s_addr = inet_aton(addr);
		server_addr.sin_port = htons(port);
		if (connect(m_pcore->sockfd, (struct sockaddr*)&server_addr,
					sizeof(server_addr)) == -1)
			throw NZHttpExcept(strerror(errno));
	}
	int NZHttpClient::request(const char* path, int timeout,
			const char* agent) {
		string req("GET http://127.0.0.1/ HTTP/1.1\r\n");
		req.append("Host: 127.0.0.1\r\nAccept: */*\r\n").append("Progma:no-cache\r\n").append(agent).append("\r\n\r\n");
		cout << req << endl;
		int ret = send(m_pcore->sockfd, req.c_str(), req.size(), 0);
		if (ret == -1)
			throw NZHttpExcept(strerror(errno));
		ret = recv(m_pcore->sockfd, m_pcore->buf, sizeof(m_pcore->buf), 0);
		if (ret == -1)
			throw NZHttpExcept(strerror(errno));
		NZLog(NZLogger::LogLevel::DEBUG, "ret = %d, value = %s\n", m_pcore->buf);
		return ret;
	}
	NZHttpClient::~NZHttpClient() {
		delete m_pcore;
	}
}
