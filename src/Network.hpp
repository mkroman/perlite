#ifndef __PERLITE_NETWORK_H_
#define __PERLITE_NETWORK_H_
#include <vector>
#include <string>

#include "Perlite.hpp"

namespace perlite {

class Channel;

class Network {
	public:
		Network();
		~Network();

	public:
		bool connect(const std::string& host, int port);
		size_t sendData(const char* data, size_t size);
		size_t sendCommand(const std::string& format, ...);

		// <Getters>
		size_t readLine(std::string& destination);
		Channel* getChannelByName(const std::string& name);
		const ChannelTable& getChannels() { return m_channels; }
		// </Getters>

		// <Setters>
		size_t addChannel(Channel* channel);
		// </Setters>

	private:
		std::string  m_host;
		unsigned int m_port;
		signed int   m_socket;
		ChannelTable m_channels;
};

} // namespace perlite

#endif
