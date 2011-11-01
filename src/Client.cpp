#include "User.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Network.hpp"
#include "ISupport.hpp"

using namespace std;

namespace perlite {

bool Client::connect(const std::string& host, int port) {
	if (m_network->connect(host, port)) {
		if (!m_network->sendCommand("NICK :%s", "mynick")) {
			cerr << "Could not send initial NICK command." << endl;
			return false;
		}

		if (!m_network->sendCommand("USER %s unknown unknown :%s", "myuser",
		                            "myrealname")) {
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
		cout << line << endl;
		command = Command::parseLine(line);
		processCommand(command);
		// After this, the command instance will be out of scope with no
		// reference to it. We can't have that.
		delete command;
	}
}

void Client::processCommand(Command* command) {
	if (command->getFlags() & kCmdLiteral) {
		if (command->getName() == "PING") {
			cmdPing(m_network, command);
		}
		else if (command->getName() == "JOIN") {
			cmdJoin(m_network, command);
		}
		else if (command->getName() == "PART") {
			cmdPart(m_network, command);
		}
		else if (command->getName() == "QUIT") {
			cmdQuit(m_network, command);
		}
		else if (command->getName() == "NICK") {
			cmdNick(m_network, command);
		}
		else if (command->getName() == "TOPIC") {
			cmdTopic(m_network, command);
		}
		else if (command->getName() == "KICK") {
			cmdKick(m_network, command);
		}
		else if (command->getName() == "PRIVMSG") {
			cmdPrivateMessage(m_network, command);
		}
		else {
			cmdUnhandled(m_network, command);
		}
	}
	else {
		if (command->getCode() == 005) {
			cmdISupportReply(m_network, command);
		}
		else if (command->getCode() == 353) {
			cmdNameReply(m_network, command);
		}
		else if (command->getCode() == 376 || command->getCode() == 422) {
			cmdEndOfMOTD(m_network, command);
		}
		else if (command->getCode() == 332) {
			cmdTopicReply(m_network, command);
		}
		// WARNING: non-standard!
		else if (command->getCode() == 333) {
			cmdTopicTimeReply(m_network, command);
		}
		else {
			cmdUnhandled(m_network, command);
		}
	}
}

void Client::cmdPing(Network* network, Command* command) {
	network->sendCommand("PONG :%s", command->getAsciiParam(0));
}

void Client::cmdISupportReply(Network* network, Command* command) {
	ParamTable parameters = command->getParameters();

	if (parameters.back() == "are supported by this server") {
		parameters.pop_back();
		parameters.erase(parameters.begin());
		network->getISupport().merge(parameters);
	}
}

void Client::cmdPrivateMessage(Network* network, Command* command) {
	User* user;
	Channel* channel;
	channel = network->getChannelByName(command->getParam(0));

	if (!channel) {
		cout << "Warning: Received message from an unknown channel." << endl;
	}

	if (command->getFlags() & kCmdUser) {
		user = network->getUserByNick(command->getNick());

		if (!user) {
			cout << "Warning: Received message from an unknown user." << endl;
		}
		else {
			if (command->getParam(1) == "!userflags") {
				string binary;
				binary = itobase2(channel->getUserFlags(user));
				network->sendCommand("PRIVMSG %s :Your flags are as follows: %#x (%s)",
				                     command->getAsciiParam(0),
				                     channel->getUserFlags(user),
				                     binary.c_str());
			}
			else if (command->getParam(1) == "!topic") {
				network->sendCommand("PRIVMSG %s :Topic is: %s", command->getAsciiParam(0),
				                     channel->getTopic().c_str());
			}
		}
	}
}

void Client::cmdJoin(Network* network, Command* command) {
	User* user;
	Channel* channel = network->getChannelByName(command->getParam(0));

	// This shouldn't happen unless the IRCd is bugged.
	if (!channel) {
		cout << "Warning: Received JOIN for an unknown channel." << endl;
	}
	else {
		user = network->getUserByNick(command->getNick());

		// This is a new user.
		if (!user && command->getFlags() & kCmdUser) {
			user = new User(command->getNick());
			user->setHost(command->getHost());
			user->setIdent(command->getIdent());
			network->addUser(user);
			channel->addUserRef(user);
			network->sendCommand("PRIVMSG #uplink :Welcome, %s! Your host is %s.",
			                     command->getNick().c_str(),
			                     command->getHost().c_str());
		}
		else {
			channel->addUserRef(user);
		}
	}
}

void Client::cmdPart(Network* network, Command* command) {
	User* user;
	Channel* channel = network->getChannelByName(command->getParam(0));

	// Should NOT happen, no matter what.
	if (!channel) {
		cout << "Warning: Received PART for an unknown channel." << endl;
	}
	else {
		user = network->getUserByNick(command->getNick());

		// Should NOT happen either, it's more likely, though.
		if (!user) {
			cout << "Warning: Received PART for an unknown user." << endl;
		}
		else {
			channel->delUserRef(user);

			if (user->getChannels().size() == 0)
				network->delUser(user);
		}
	}
}

void Client::cmdQuit(Network* network, Command* command) {
	UserTable::iterator it;
	ChannelTable::iterator cit;

	if (!(command->getFlags() & kCmdUser))
		return;

	for (it = network->getUsers().begin(); it < network->getUsers().end(); it++) {
		if ((*it)->getNick() == command->getNick()) {
			for (cit = (*it)->getChannels().begin(); cit < (*it)->getChannels().end()
			        ; cit++) {
				(*cit)->delUserRef(*it);
			}

			network->delUser(*it);
		}
	}
}

void Client::cmdKick(Network* network, Command* command) {
	string reason;
	Channel* channel;
	User* admin, *target;
	reason = command->getParam(2);

	if ((channel = network->getChannelByName(command->getParam(0)))) {
		if ((admin = network->getUserByNick(command->getNick()))) {
			if ((target = network->getUserByNick(command->getParam(1)))) {
				cout << admin->getNick() << " kicked " << target->getNick() << " from " << channel->getName() << endl;
				channel->delUserRef(target);

				if (target->getChannels().size() == 0)
					network->delUser(target);
			}
			else {
				cout << "Warning: Received KICK for an unknown target." << endl;
			}
		}
		else {
			cout << "Warning: Received KICK from an unknown user." << endl;
		}
	}
	else {
		cout << "Warning: Received KICK for an unknown channel." << endl;
	}
}

void Client::cmdEndOfMOTD(Network* network, Command* command) {
	network->sendCommand("JOIN %s", "#test");
}

void Client::cmdNick(Network* network, Command* command) {
	User* user = 0;
	UserTable::iterator it;

	if ((user = network->getUserByNick(command->getNick()))) {
		cout << user->getNick() << " changed nickname to " << command->getParam(0) << "." << endl;
		user->setNick(command->getParam(0));
	}
	else {
		cout << "Warning: Received NICK from an unknown user." << endl;
	}
}

void Client::cmdTopic(Network* network, Command* command) {
	User* user;
	Channel* channel;

	if ((channel = network->getChannelByName(command->getParam(0)))) {
		channel->setTopic(command->getParam(1));
		channel->setTopicTime(time(0));

		if ((user = network->getUserByNick(command->getNick()))) {
			// …
		}
		else {
			cout << "Warning: Received TOPIC from an unknown user." << endl;
		}
	}
	else {
		cout << "Warning: Received TOPIC from an unknown channel." << endl;
	}
}

void Client::cmdTopicReply(Network* network, Command* command) {
	Channel* channel;

	if ((channel = network->getChannelByName(command->getParam(1)))) {
		channel->setTopic(command->getParam(2));
	}
	else {
		channel = new Channel(command->getParam(1));
		channel->setTopic(command->getParam(2));
		network->addChannel(channel);
	}
}

void Client::cmdTopicTimeReply(Network* network, Command* command) {
	Channel* channel;

	if ((channel = network->getChannelByName(command->getParam(1)))) {
		channel->setTopicTime(static_cast<time_t>(atoi(command->getAsciiParam(3))));
	}
	else {
		cout << "Warning: Received RPL_TOPICTIME from an unknown channel." << endl;
	}
}

void Client::cmdNameReply(Network* network, Command* command) {
	User* user;
	Channel* channel = network->getChannelByName(command->getParam(2));

	// If the channel is new to us - create it.
	if (!channel) {
		channel = new Channel(command->getParam(2));
		// Save the reference to it.
		network->addChannel(channel);
	}

	// Split the nicknames and save them in a vector with strings.
	StringTable nickList = splitString(command->getParam(3));
	// Iterate through all the nicks and create a linked user instance.
	StringTable::iterator it;

	for (it = nickList.begin(); it < nickList.end(); it++) {
		// Check if the nickname is already known.
		user = network->getUserByNick(*it);

		if (!user) {
			// Create a new user.
			user = new User();
			// Add it.
			network->addUser(user);
			channel->addUserRef(user);

			// Parse the nickname for a leading character (~|!|@|+|&|%)
			switch ((*it)[0]) {
				case '@':
					channel->addUserFlags(user, kLevelOper);
					break;

				case '~':
					channel->addUserFlags(user, kLevelOwner);
					break;

				case '&':
					channel->addUserFlags(user, kLevelAdmin);
					break;

				case '%':
					channel->addUserFlags(user, kLevelHalfOper);
					break;

				case '+':
					channel->addUserFlags(user, kLevelVoice);
					break;

				default:
					channel->setUserFlags(user, 0);
			}

			// Skip the leading character.
			if (channel->getUserFlags(user) & 255) {
				user->setNick((*it).substr(1));
			}
			else {
				user->setNick(*it);
			}
		}
		else {
			channel->addUserRef(user);
		}
	}
}

void Client::cmdUnhandled(Network* network, Command* command) {
	if (command->getFlags() & kCmdLiteral) {
		// …
	}
	else {
		// …
	}
}

const StringTable splitString(const string& input) {
	size_t start = 0, end = 0;
	StringTable table;
	end = input.find(' ');

	while (end != string::npos) {
		table.push_back(input.substr(start, end - start));
		start = end + 1;
		end = input.find(' ', start);
	}

	return table;
}

string itobase2(int i) {
	int j = i;
	div_t result;
	string buffer;

	while (j > 0) {
		result = div(j, 2);
		buffer = static_cast<string>(result.rem == 0 ? "0" : "1").append(buffer);
		j = result.quot;
	}

	return buffer;
}

} // namespace perlite

