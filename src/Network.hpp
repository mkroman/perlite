#ifndef __PERLITE_NETWORK_H_
#define __PERLITE_NETWORK_H_
#include <string>

namespace Perlite {
class Network {
	public:
		Network();
		~Network();

	public:
		bool   connect(const std::string& host, int port);

		size_t readLine(std::string& buffer);
		size_t sendCommand(const std::string& format, ...);
		size_t sendData(const char* data, size_t size);

	private:
		std::string  host_;
		unsigned int port_;
		signed int   socket_;
};
}

#endif
