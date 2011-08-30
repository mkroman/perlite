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
    else if (command->getName() == "PRIVMSG") {
      cmdPrivateMessage(m_network, command);
    }
    else {
      cmdUnhandled(m_network, command);
    }
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
                             command->getCParam(0), 
                             channel->getUserFlags(user),
                             binary.c_str());
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
          ;cit++) {
        (*cit)->delUserRef(*it);
      }

      network->delUser(*it);
    }
  }
}

void Client::cmdEndOfMOTD(Network* network, Command* command) {
  network->sendCommand("JOIN %s", "#uplink");
}

void Client::cmdNick(Network* network, Command* command) {
  User* user = 0;
  UserTable::iterator it;

  if ((user = network->getUserByNick(command->getNick())) != 0) {
    cout << user->getNick() << " changed nickname to " << command->getParam(0) << "." << endl;
    user->setNick(command->getParam(0));
  }
  else {
    cout << "Warning: Received NICK from an unknown user." << endl;
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
  StringTable nickList = splitNamesTable(command->getParam(3));

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

