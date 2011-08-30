#include "User.hpp"
#include "Channel.hpp"

using namespace std;

namespace perlite {

void User::delChannelRef(Channel* channel) {
  ChannelTable::iterator it;

  for (it = m_channels.begin(); it < m_channels.end(); it++) {
    if (*it == channel) {
      m_channels.erase(it);
    }
  }
}

} // namespace perlite