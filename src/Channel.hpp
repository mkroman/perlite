#ifndef __PERLITE_CHANNEL_H_
#define __PERLITE_CHANNEL_H_
#include <vector>
#include <string>

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
	void merge(StringTable& names);

	// <Getters>
	const std::string& getName() { return m_name; }
	const UserTable& getUsers() { return m_users; }
	User* getUserByNick(const std::string& nick);
	// </Getters>

	// <Setters>
	// …
	// </Setters>

private:
	std::string m_name;
	UserTable   m_users;
};

} // namespace perlite

#endif