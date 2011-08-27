#ifndef __PERLITE_USER_H_
#define __PERLITE_USER_H_
#include <string>

namespace Perlite {
class Channel;

class User {
public:
	User(const std::string& nick, Channel* channel);

public:
	std::string& getNick() {
		return nick_;
	}

private:
	std::string nick_;
	Channel*    channel_;
};
}

#endif