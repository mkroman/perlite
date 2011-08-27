#ifndef __PERLITE_USER_H_
#define __PERLITE_USER_H_
#include <string>

namespace perlite {

class Channel;

class User {
public:
	User(const std::string& nick, Channel* channel);

public:
	const std::string& getNick() { return m_nick; }

private:
	std::string m_nick;
	Channel*    m_channel;
};

} // namespace perlite

#endif