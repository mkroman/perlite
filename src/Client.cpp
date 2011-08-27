#include <iostream>

#include "Client.hpp"
#include "Command.hpp"
#include "Network.hpp"
#include "Constants.hpp"

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

		result = network_->sendCommand("NICK :%s", "mynick");

		if (result == (size_t)-1) {
			cerr << "Could not send initial NICK command." << endl;

			return false;
		}

		result = network_->sendCommand("USER %s unknown unknown :%s", "myuser", "myrealname");

		if (result == (size_t)-1) {
			cerr << "Could not send initial USER command." << endl;

			return false;
		}
	}

	return true;
}

void Client::loop(void) {
	Command* command;
	std::string buffer;

	while (network_->readLine(buffer)) {
		command = Command::parse(buffer);

		if (command)
			handleCommand(command);

		cout << buffer << endl;
	}
}

void Client::handleCommand(Command* command) {
	if (command->isNumeric()) {
		if (command->getCode() == 422 || command->getCode() == 376) {
			network_->sendCommand("JOIN %s", "#uplink");
		}
	}
	else {
		if (command->getName() == "PING") {
			network_->sendCommand("PONG :%s", command->getParam(0).c_str());
		}
		else if (command->getName() == "PRIVMSG") {
			if (command->getParam(1) == "!test")
				network_->sendCommand("PRIVMSG %s :slik et beskidt askebæger", command->getParam(0).c_str());
		}
	}
}

}
