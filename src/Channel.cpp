#include "Channel.hpp"

using namespace std;

namespace perlite {

Channel::Channel(const string& name) : m_name(name) {
	// …
}

void Channel::merge(StringTable& names) {
	User* user;
	StringTable::iterator it;

	// Iterate over every nickname that is in the channel.
	for (it = names.begin(); it < names.end(); it++) {
		// If it doesn't exist, create it.
		if (getUserByNick(*it) == 0) {
			user = new User(*it, this);

			m_users.push_back(user);
		}
	}
}

User* Channel::getUserByNick(const string& nick) {
	UserTable::iterator it;

	// Iterate over every user that is in the channel.
	for (it = m_users.begin(); it < m_users.end(); it++) {
		// Return a pointer reference to the instance if the nicks match.
		if (((User*)*it)->getNick() == nick)
			return *it;
	}

	return 0;
}

} // namespace perlite