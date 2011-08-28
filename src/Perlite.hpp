#ifndef __PERLITE_H_
#define __PERLITE_H_
#include <vector>
#include <string>

#define MAX_PARAM_COUNT 10  // Maximum number of parameters parsed in a line.
#define MIN_BUFFER_SIZE 512 // Minimum buffer size. RFC 1459 defines max as 512
                            // where the last 2 bytes is CRLF.

#define _vector_each(i,v) \
	for ((i) = (v).begin(); (i) < (v).end(); (i)++)

namespace perlite {

class User;
class Channel;

typedef std::string              Parameter;
typedef std::vector<Parameter>   ParamTable;

typedef std::vector<User*>       UserTable;
typedef std::vector<Channel*>    ChannelTable;
typedef	std::vector<std::string> StringTable;

typedef enum { CMD_LITERAL = 0x80, CMD_USER = 0x40 } CommandTypes;

typedef enum { PERM_OWNER = 0x80, PERM_ADMIN = 0x40, PERM_OPER = 0x20,
			   PERM_HALFOPER = 0x10, PERM_VOICE = 0x04 } UserPermissions;

} // namespace perlite

#endif