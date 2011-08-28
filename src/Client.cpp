#include "User.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Network.hpp"

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
    command = Command::parseLine(line);

    processCommand(command);

    cout << line << endl;

    // After this, the command instance will be out of scope with no
    // reference to it. We can't have that.
    delete command;
  }
}

void Client::processCommand(Command* command) {
  if (command->getFlags() & CMD_LITERAL) {
    if (command->getName() == "PING")
      cmdPing(m_network, command);
    else if (command->getName() == "JOIN")
      cmdJoin(m_network, command);
    else if (command->getName() == "PART")
      cmdPart(m_network, command);
    else
      cmdUnhandled(m_network, command);
  }
  else {
    if (command->getCode() == 353)
      cmdNameReply(m_network, command);
    else if (command->getCode() == 376 || command->getCode() == 422)
      cmdEndOfMOTD(m_network, command);
    else
      cmdUnhandled(m_network, command);
  }
}

void Client::cmdPing(Network* network, Command* command) {
  network->sendCommand("PONG :%s", command->getCParam(0));
}

void Client::cmdJoin(Network* network, Command* command) {
  User* user;
  Channel* channel = network->getChannelByName(command->getParam(0));

  // This shouldn't happen unless the IRCd is bugged.
  if (!channel) {
    cout << "Warning: Received JOIN for an unknown channel." << endl;
  }
  else {
    user = channel->getUserByNick(command->getNick());

    // This is a new user.
    if (!user && command->getFlags() & CMD_USER) {
      user = new User(command->getNick(), channel);
      user->setHost(command->getHost());
      user->setIdent(command->getIdent());

      channel->addUser(user);

      network->sendCommand("PRIVMSG #test :Welcome, %s! Your host is %s.",
                           command->getNick().c_str(), command->getHost().c_str());
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
    user = channel->getUserByNick(command->getNick());

    // Should NOT happen either, it's more likely, though.
    if (!user) {
      cout << "Warning: Received PART for an unknown user." << endl;
    }
    else {
      channel->release(user);
    }
  }
}

void Client::cmdEndOfMOTD(Network* network, Command* command) {
  network->sendCommand("JOIN %s", "#test");
}

void Client::cmdNameReply(Network* network, Command* command) {
  Channel* channel = network->getChannelByName(command->getParam(2));

  // If the channel is new to us - create it.
  if (!channel) {
    channel = new Channel(command->getParam(2));
    // Save the reference to it.
    network->addChannel(channel);
  }

  // Split the nicknames and save them in a vector with strings.
  StringTable nickList = splitNamesTable(command->getParam(3));

  // Iterate through all the nicks and create a linked user instance.
  StringTable::iterator it;

  _vector_each(it, nickList) {
    // Check if the nickname is already known.
    if (!channel->getUserByNick(*it)) {
      // Create and add it.
      channel->addUser(new User(*it, channel));
     }
  }
}

void Client::cmdUnhandled(Network* network, Command* command) {
  if (command->getFlags() & CMD_LITERAL) {
    // …
  }
  else {
    // …
  }
}

const StringTable Client::splitNamesTable(const string& names) {
  size_t start = 0, end = 0;
  StringTable table;

  end = names.find(' ');

  while (end != string::npos) {
    table.push_back(names.substr(start, end - start));
    start = end + 1;

    end = names.find(' ', start);
  }

  return table;
}

} // namespace perlite
