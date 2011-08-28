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

// Merge the current user table with a string containing nicks split by spaces.
// The nicks are prefixed with their flags (+/@/%/~), no host- or username is
// available at this point.
void Channel::merge(StringTable& nickList) {
  StringTable::iterator it;

  // Iterate over every nickname that is in the channel.
  for (it = nickList.begin(); it < nickList.end(); it++) {
    // If it doesn't exist, create it.
    if (!getUserByNick(*it))
      m_users.push_back(new User(*it, this));
  }
}

// Remove a pointer to a user instance and release the user instance from memory.
void Channel::release(User* user) {
  UserTable::iterator it;

  for (it = m_users.begin(); it < m_users.end(); it++) {
    if (*it == user) {
      m_users.erase(it);

      delete user;
    }
  }
}

// Search for a user with a specific nickname.
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