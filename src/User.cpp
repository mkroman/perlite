#include "User.hpp"
#include "Channel.hpp"

namespace perlite {

User::User(const std::string& nick, Channel* channel) : 
	m_nick(nick), m_channel(channel) {
	// …
}

} // namespace perlite