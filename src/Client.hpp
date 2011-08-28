#ifndef __PERLITE_CLIENT_H_
#define __PERLITE_CLIENT_H_
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
  void cmdNameReply(Network* network, Command* command);
  void cmdUnhandled(Network* network, Command* command);
  void cmdEndOfMOTD(Network* network, Command* command);
  void cmdPrivateMessage(Network* network, Command* command);

  Network* m_network;
};

} // namespace perlite

#endif
