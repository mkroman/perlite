#ifndef __PERLITE_CLIENT_H_
#define __PERLITE_CLIENT_H_
#include <string>
#include <vector>
#include <string.h>

namespace Perlite {
class Network;
class Command;

class Client {
	public:
		Client(void);
		~Client(void);

	public:
		bool connect(const std::string& host, int port);
		void handleCommand(Command* command);
		void loop(void);
		std::vector<std::string> splitNamesTable(const std::string& table);

	private:
		Network* network_;
};
}

#endif
