#include "User.hpp"
#include "Channel.hpp"

using namespace std;

namespace perlite {

User::User(const std::string& nick, Channel* channel) : m_channel(channel) {
  // Change permission flags according to the leading character in the
  // nickname, if any is present.
  switch (nick[0]) {
    case '~':
      m_flags |= PERM_OWNER;
      break;
    case '@':
      m_flags |= PERM_OPER;
      break;
    case '%':
      m_flags |= PERM_HALFOPER;
      break;
    case '+':
      m_flags |= PERM_VOICE;
      break;
    default:
      m_flags = 0;
  }

  // If no flags were set, don't skip the leading character.
  if (m_flags & 255)
    m_nick = nick.substr(1);
  else
    m_nick = nick;
}

} // namespace perlite