#ifndef __PERLITE_CLIENT_H_
#define __PERLITE_CLIENT_H_
#include <string>

namespace Perlite {
class Network;

class Client {
	public:
		Client(void);
		~Client(void);

	public:
		bool connect(const std::string& host, int port);
		void loop(void);

	private:
		Network* network_;
};
}

#endif
