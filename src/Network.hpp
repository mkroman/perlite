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

	private:
		std::string  host_;
		unsigned int port_;
		signed int   socket_;
};
}

#endif
