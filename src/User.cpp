#include <string>

#include "User.hpp"
#include "Channel.hpp"

namespace Perlite {

User::User(const std::string& nick, Channel* channel) : 
	nick_(nick), channel_(channel) {
	// …
}

}