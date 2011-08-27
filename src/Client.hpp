#ifndef __PERLITE_CLIENT_H_
#define __PERLITE_CLIENT_H_
#include <string>
#include <vector>
#include <iostream>
#include <string.h>

#include "Perlite.hpp"

namespace perlite {

// <Forward declaration>
class Network;
class Command;
// </Forward declaration>

class Client {
	public:
		Client(void);
		~Client(void);

	public:
		bool connect(const std::string& host, int port);
		void runLoop(void);
		
		void processCommand(Command* command);

	protected:
		const StringTable splitNamesTable(const std::string& names);

		Network* m_network;
};

} // namespace perlite

#endif
