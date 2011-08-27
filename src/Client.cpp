#include "User.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Network.hpp"

using namespace std;

namespace perlite {

Client::Client(void) : m_network(new Network()) {
	// …
}

Client::~Client(void) {
	delete m_network;
}

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

void Client::runLoop(void) {
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

			StringTable nicknames;
			nicknames = splitNamesTable(command->getParam(3));

			channel->merge(nicknames);
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

// FIXME: Utilize C++ strings.
const StringTable Client::splitNamesTable(const string& names) {
	char* ptr;
	StringTable stringTable;

	ptr = strtok(const_cast<char*>(names.c_str()), " ");

	while (ptr != NULL) {
		stringTable.push_back(string(ptr));

		ptr = strtok(NULL, " ");
	}

	return stringTable;
}

} // namespace perlite
