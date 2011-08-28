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

using namespace std;

namespace perlite {

// Destroy every channel joined on this network and close the socket connection.
Network::~Network() {
  ChannelTable::iterator it;

  // Iterate over every channel instance and wipe it from memory.
  for (it = m_channels.begin(); it < m_channels.end(); it++)
    delete *it;

  if (m_socket)
    ::close(m_socket);
}

// Establish a basic blocking IPv4 TCP connection.
// Returns true on success, otherwise false.
bool Network::connect(const string& host, int port) {
  m_host = host;
  m_port = port;

  struct hostent*    remote_host;
  struct sockaddr_in remote_addr;

  m_socket = ::socket(AF_INET, SOCK_STREAM, 0);

  memset(&remote_addr, 0, sizeof(struct sockaddr_in));

  if (m_socket == -1) {
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

  memcpy(&remote_addr.sin_addr, remote_host->h_addr, remote_host->h_length);

  if (::connect(m_socket, (struct sockaddr*)&remote_addr,
      sizeof(struct sockaddr)) == -1) {
    cerr << "::connect() failed" << endl;

    return false;
  }

  return true;
}

size_t Network::readLine(string& destination) {
  char   buffer[kMinBufferSize], tmp;
  size_t size = 0, read = 0, i = 0;

  for (i = 0; i < sizeof(buffer); i++) {
    read = ::recv(m_socket, &tmp, sizeof(char), 0);

    if (tmp == '\r')
      continue;
    else if (tmp == '\n')
      break;

    buffer[i] = tmp;

    size += read;
  }

  buffer[size] = '\0';
  destination = string(buffer);

  return size;
}

size_t Network::sendCommand(const string& format, ...) {
  va_list vargs;
  char buffer[kMinBufferSize];
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

size_t Network::sendData(const char* data, size_t length) {
  int size = 0;

  size = ::send(m_socket, data, length, 0);

  if (size == -1)
    return -1;
  else
    return size;
}

Channel* Network::getChannelByName(const string& name) {
  ChannelTable::iterator it;

  for (it = m_channels.begin(); it < m_channels.end(); it++) {
    if (((Channel*)*it)->getName() == name)
      return *it;
  }

  return 0;
}

size_t Network::addChannel(Channel* channel) {
  m_channels.push_back(channel);
  
  return m_channels.size();
}

} // namespace perlite
