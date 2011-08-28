#ifndef __PERLITE_USER_H_
#define __PERLITE_USER_H_
#include <string>
#include <stdint.h>

namespace perlite {

class Channel;

class User {
public:
	User(const std::string& nick, Channel* channel);

public:
	const uint16_t getFlags() { return m_flags; }
	const std::string& getNick() { return m_nick; }

private:
	uint8_t     m_flags;
	Channel*    m_channel;
	std::string m_nick;
};

} // namespace perlite

#endif