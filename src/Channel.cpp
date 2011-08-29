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