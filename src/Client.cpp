#include "User.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Network.hpp"

using namespace std;

namespace perlite {

bool Client::connect(const std::string& host, int port) {
	if (m_network->connect(host, port)) {
		int size;

		size = m_network->sendCommand("NICK :%s", "mynick");

		if (size == -1) {
			cerr << "Could not send initial NICK command." << endl;

			return false;
		}

		size = m_network->sendCommand("USER %s unknown unknown :%s",
			"myuser", "myrealname");

		if (size == -1) {
			cerr << "Could not send initial USER command." << endl;

			return false;
		}
	}

	return true;
}

void Client::runLoop() {
	Command* command;
	std::string line;

	while (m_network->readLine(line)) {
		command = Command::parseLine(line);

		processCommand(command);

		cout << line << endl;

		// After this, the command instance will be out of scope with no
		// reference to it. We can't have that.
		delete command;
	}
}

void Client::processCommand(Command* command) {
	if (command->isNumeric()) {
		if (command->getCode() == 422 || command->getCode() == 376) {
			m_network->sendCommand("JOIN %s", "#uplink");
		}
		// The /NAMES list
		else if (command->getCode() == 353) {
			string   name;
			Channel* channel;

			name = command->getParam(2);

			// Attempt to get the channel instance
			channel = m_network->getChannelByName(name);

			// If it failed, create the channel
			if (!channel) {
				channel = new Channel(name);
				m_network->addChannel(channel);
			}

			StringTable users;
			users = splitNamesTable(command->getParam(3));

			channel->merge(users);
		}
	}
	else {
		if (command->getName() == "PING") {
			m_network->sendCommand("PONG :%s", command->getParam(0).c_str());
		}
		else if (command->getName() == "PRIVMSG") {
			// …
		}
	}
}

const StringTable Client::splitNamesTable(const string& names) {
	size_t start = 0, end = 0;
	StringTable table;

	end = names.find(' ');

	while (end != string::npos) {
		table.push_back(names.substr(start, end - start + 1));
		start = end + 1;

		end = names.find(' ', start);
	}

	return table;
}

} // namespace perlite
