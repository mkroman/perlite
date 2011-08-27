#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "User.hpp"
#include "Client.hpp"
#include "Channel.hpp"
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
		// The /NAMES list
		else if (command->getCode() == 353)
		{
			string    name;
			Channel*  channel;

			name = command->getParam(2);

			// Attempt to get the channel instance
			channel = network_->getChannelByName(name);

			// If it failed, create the channel
			if (!channel) {
				channel = new Channel(name);
				network_->addChannel(channel);
			}

			std::vector<std::string> users;
			users = splitNamesTable(command->getParam(3));

			channel->mergeUsers(users);
		}
	}
	else {
		if (command->getName() == "PING") {
			network_->sendCommand("PONG :%s", command->getParam(0).c_str());
		}
		else if (command->getName() == "PRIVMSG") {
			if (command->getParam(1) == "!stats") {
				char message[256];

				sprintf(message, "Channels: %d, Users in channel #1: %d", 
					(int)network_->getChannels().size(), (int)network_->getChannels()[0]->getUsers().size());

				network_->sendCommand("PRIVMSG %s :%s", command->getParam(0).c_str(), message);
			}
		}
	}
}

std::vector<std::string> Client::splitNamesTable(const std::string& table) {
	char* ptr;
	std::vector<std::string> names;

	ptr = strtok(const_cast<char*>(table.c_str()), " ");

	while (ptr != NULL) {
		names.push_back(std::string(ptr));

		ptr = strtok(NULL, " ");
	}

	return names;
}

}
