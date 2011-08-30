#ifndef __PERLITE_CLIENT_H_
#define __PERLITE_CLIENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>

#include "Perlite.hpp"
#include "Network.hpp"

namespace perlite {

// <Forward declaration>
class Command;
// </Forward declaration>

class Client {
 public:
  Client() : m_network(new Network()) {};
  ~Client() { delete m_network; }

 public:
  bool connect(const std::string& host, int port);
  void runLoop();
  
  void processCommand(Command* command);

 protected:
  const StringTable splitNamesTable(const std::string& names);

  void cmdPing(Network* network, Command* command);
  void cmdJoin(Network* network, Command* command);
  void cmdPart(Network* network, Command* command);
  void cmdQuit(Network* network, Command* command);
  void cmdNick(Network* network, Command* command);
  void cmdTopic(Network* network, Command* command);
  void cmdNameReply(Network* network, Command* command);
  void cmdUnhandled(Network* network, Command* command);
  void cmdEndOfMOTD(Network* network, Command* command);
  void cmdTopicReply(Network* network, Command* command);
  void cmdPrivateMessage(Network* network, Command* command);

  Network* m_network;
};

std::string itobase2(int i);

} // namespace perlite

#endif
