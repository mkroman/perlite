#ifndef __PERLITE_CHANNEL_H_
#define __PERLITE_CHANNEL_H_
#include <vector>
#include <string>

#include "User.hpp"

namespace Perlite {
typedef std::vector<User*> UserList;

class Channel {
public:
	Channel(const std::string& name);

public:
	std::string& getName() {
		return name_;
	}

	UserList& getUsers() {
		return users_;
	}

	User*  getUserByNick(const std::string& nick);
	size_t mergeUsers(std::vector<std::string>& names);

private:
	std::string name_;
	UserList    users_;
};

}

#endif