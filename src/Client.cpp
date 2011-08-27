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
	if (network_->connect(host, port)) {
		size_t result;

		result = network_->sendCommand("NICK :%s\r\n", "mynick");
		result = network_->sendCommand("USER %s unknown unknown :%s\r\n", "myuser", "myrealname");
	}
}

void Client::loop(void) {
	Command* command;
	std::string buffer;

	while (network_->readLine(buffer)) {
		command = Command::parse(buffer);

		if (!command->isNumeric()) {
			cout << "name: " << command->getName() << " prefix: " << command->getPrefix() << endl;
		}
		else {
			cout << "code: " << command->getCode() << " prefix: " << command->getPrefix() << endl;
		}

		cout << buffer << endl;
	}
}

}
