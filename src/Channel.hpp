#ifndef __PERLITE_CHANNEL_H_
#define __PERLITE_CHANNEL_H_
#include <vector>
#include <string>
#include <stdint.h>

#include "Perlite.hpp"
#include "User.hpp"

namespace perlite {

class Channel {
public:
	Channel(const std::string& name) : m_name(name) {};
	~Channel();

public:
	// Iterates through a StringTable and create an instance for each
	// unique string represented as a nickname in this channel.
	void merge(StringTable& nickList);

	// <Getters>
	User* getUserByNick(const std::string& nick);
	UserTable& getUsers() { return m_users; }
	const uint16_t getFlags() { return m_flags; }
	const std::string& getName() { return m_name; }
	// </Getters>

	// <Setters>
	void addUser(User* user) { m_users.push_back(user); }
	// </Setters>

private:
	uint16_t    m_flags;
	UserTable   m_users;
	std::string m_name;
};

} // namespace perlite

#endif