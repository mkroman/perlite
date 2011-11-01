#include "Channel.hpp"

using namespace std;

namespace perlite {

// Destroy every user in this channel.
Channel::~Channel() {
	UserTable::iterator it;

	// Iterate over every user in the channel and wipe them from memory.
	for (it = m_users.begin(); it < m_users.end(); it++)
		delete *it;
}

const uint8_t Channel::getUserFlags(User* user) {
	UserTable::iterator it;

	// Check if the user even is in this channel.
	for (it = m_users.begin(); it < m_users.end(); it++) {
		if (*it == user) {
			// Note: map::operator[] creates the key without a value if it doesn't
			// already exist, that's why we perform this check.
			return m_userFlags[user];
		}
	}

	return -1;
}

void Channel::addUserFlags(User* user, uint8_t flags) {
	m_userFlags[user] |= flags;
}

void Channel::delUserFlags(User* user, uint8_t flags) {
	m_userFlags[user] &= ~flags;
}

void Channel::setUserFlags(User* user, uint8_t flags) {
	m_userFlags[user] = flags;
}

void Channel::addUserRef(User* user) {
	m_users.push_back(user);
	user->addChannelRef(this);
}

// Remove a pointer to a user instance;
void Channel::delUserRef(User* user) {
	UserTable::iterator it;

	for (it = m_users.begin(); it < m_users.end(); it++) {
		if (*it == user) {
			m_users.erase(it);
			(*it)->delChannelRef(this);
		}
	}
}

} // namespace perlite