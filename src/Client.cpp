#include <iostream>

#include "Client.hpp"
#include "Command.hpp"
#include "Network.hpp"

using namespace std;

namespace Perlite {

Client::Client(void) {
	network_ = new Network();
}

Client::~Client(void) {
	delete network_;
}

bool Client::connect(const std::string& host, int port) {
	return network_->connect(host, port);
}

void Client::loop(void) {
	Command* command;
	std::string buffer;

	while (network_->readLine(buffer)) {
		command = Command::parse(buffer);

		if (command) {
			cout << "name: " << command->getName() << " prefix: " << command->getPrefix() << endl;	
		}
	}
}

}
