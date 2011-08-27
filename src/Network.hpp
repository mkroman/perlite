#ifndef __PERLITE_NETWORK_H_
#define __PERLITE_NETWORK_H_
#include <vector>
#include <string>

namespace Perlite {
class Channel;
typedef std::vector<Channel*> ChannelList;

class Network {
	public:
		Network();
		~Network();

	public:
		bool     connect(const std::string& host, int port);

		size_t   readLine(std::string& destination);

		size_t   sendData(const char* data, size_t size);
		size_t   sendCommand(const std::string& format, ...);

		size_t   addChannel(Channel* channel);
		Channel* getChannelByName(const std::string& name);
		ChannelList& getChannels() {
			return channels_;
		}

	private:
		std::string  host_;
		unsigned int port_;

		signed int   socket_;

		ChannelList  channels_;
};
}

#endif
