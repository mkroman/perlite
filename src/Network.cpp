#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>

#include "Network.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Constants.hpp"

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

size_t Network::readLine(std::string& destination) {
	char   buffer[MIN_BUFFER_SIZE], tmp;
	size_t size = 0, read = 0, i = 0;

	for (i = 0; i < sizeof(buffer); i++) {
		read = ::recv(socket_, &tmp, sizeof(char), 0);

		if (tmp == '\r')
			continue;
		else if (tmp == '\n')
			break;

		buffer[i] = tmp;

		size += read;
	}

	buffer[size] = '\0';
	destination = std::string(buffer);

	return size;
}

size_t Network::sendCommand(const std::string& format, ...) {
	va_list vargs;
	char buffer[MIN_BUFFER_SIZE];
	size_t result = 0, length = 0;

	va_start(vargs, format);

	if (vsprintf(buffer, format.c_str(), vargs) < 0)
		return -1;

	va_end(vargs);

	length = strlen(buffer);

	if (!strstr(buffer, "\r\n")) {
		strcpy(&buffer[length], "\r\n");
		length += 2;
	}

	result = sendData(buffer, length);

	return result;
}

size_t Network::sendData(const char* data, size_t size) {
	size_t result = 0;

	result = ::send(socket_, data, size, 0);

	if (result == (size_t)-1)
		return (size_t)-1;
	else
		return result;
}

Channel* Network::getChannelByName(const std::string& name) {
	ChannelList::iterator it;

	for (it = channels_.begin(); it < channels_.end(); it++) {
		if (((Channel*)*it)->getName() == name)
			return *it;
	}

	return NULL;
}

size_t Network::addChannel(Channel* channel) {
	channels_.push_back(channel);
	
	return channels_.size();
}

}
