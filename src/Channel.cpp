#include <string>
#include <vector>

#include "Channel.hpp"

namespace Perlite {

Channel::Channel(const std::string& name) : name_(name) {
	
}

size_t Channel::mergeUsers(std::vector<std::string>& names) {
	std::vector<std::string>::iterator it;
	size_t num = 0;
	User* user;

	for (it = names.begin(); it < names.end(); it++) {
		if (getUserByNick((std::string)*it) == NULL) {
			user = new User(*it, this);
			users_.push_back(user);
			num++;
		}
	}

	return num;
}

User* Channel::getUserByNick(const std::string& nick) {
	UserList::iterator it;

	for (it = users_.begin(); it < users_.end(); it++) {
		if (((User*)*it)->getNick() == nick)
			return *it;
	}

	return NULL;
}

}