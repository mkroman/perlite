#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#include "Network.hpp"
#include "Command.hpp"

using namespace std;

namespace Perlite {

Network::Network(void) {
}

Network::~Network(void) {
	// …
}

bool Network::connect(const string& host, int port) {
	host_ = host;
	port_ = port;

	struct hostent*    remote_host;
	struct sockaddr_in remote_addr;

	socket_ = ::socket(AF_INET, SOCK_STREAM, 0);

	memset(&remote_addr, 0, sizeof(struct sockaddr_in));

	if (socket_ == -1) {
		cerr << "::socket() failed" << endl;

		return false;
	}

	remote_host = ::gethostbyname(host.c_str());

	if (!remote_host) {
		cerr << "::gethostbyname() failed" << endl;

		return false;
	}

	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(port);

	memcpy(&remote_addr.sin_addr, remote_host->h_addr,
			remote_host->h_length);

	if (::connect(socket_, (struct sockaddr*)&remote_addr,
				sizeof(struct sockaddr)) == -1) {
		cerr << "::connect() failed" << endl;

		return false;
	}

	return true;
}

#define BUFFER_SIZE 512
size_t Network::readLine(std::string& buffer) {
	char   buf[BUFFER_SIZE], tmp;
	size_t size = 0, read = 0, i = 0;

	for (i = 0; i < sizeof(buf); i++) {
		read = ::recv(socket_, &tmp, sizeof(char), 0);

		if (tmp != '\n')
			buf[i] = tmp;
		else
			break;

		size += read;
	}

	buf[size] = '\0';
	buffer = std::string(buf);

	return size;
}

}
