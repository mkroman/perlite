#ifndef __PERLITE_NETWORK_H_
#define __PERLITE_NETWORK_H_
#include <vector>
#include <string>

#include "Perlite.hpp"
#include "ISupport.hpp"

namespace perlite {

class Channel;
class ISupport;

class Network {
public:
	Network() {};
	~Network();

public:
	bool connect(const std::string& host, int port);
	void disconnect();
	size_t sendData(const char* data, size_t size);
	size_t sendCommand(const std::string& format, ...);

	// <Getters>
	User* getUserByNick(const std::string& nick);
	size_t readLine(std::string& destination);
	Channel* getChannelByName(const std::string& name);
	ISupport& getISupport() { return m_isupport; }
	UserTable& getUsers() { return m_users; }
	ChannelTable& getChannels() { return m_channels; }
	// </Getters>

	// <Setters>
	void addUser(User* user) { m_users.push_back(user); }
	void addChannel(Channel* channel) { m_channels.push_back(channel); }

	void delUser(User* user);
	void delChannel(Channel* channel);
	// </Setters>

private:
	ISupport     m_isupport;
	UserTable    m_users;
	std::string  m_host;
	unsigned int m_port;
	signed int   m_socket;
	ChannelTable m_channels;
};

} // namespace perlite

#endif
