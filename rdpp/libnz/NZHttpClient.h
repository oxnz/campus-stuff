#pragma once

#include "NZExcept.h"

namespace NZ {
	class NZHttpClient {
	public:
		class NZSockExcept : public NZExcept {
		public:
			NZSockExcept(const char* msg) : NZExcept(msg) {}
		};
		class NZHttpExcept : public NZExcept {
		public:
			NZHttpExcept(const char* msg) : NZExcept(msg) {}
		};
	public:
		NZHttpClient(const char* addr, int port = 80);
		~NZHttpClient();
		int request(const char* path, int timeout = 60,
			const char* agent = "Mozilla/4.0");
	private:
		struct __core* m_pcore;
	};
}
