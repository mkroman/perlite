#ifndef __PERLITE_H_
#define __PERLITE_H_
#include <vector>
#include <string>

#define MAX_PARAMS_COUNT 10  // Maximum number of parameters parsed in a line.
#define MIN_BUFFER_SIZE  512 // Minimum buffer size. RFC 1459 defines max as 512
							 // where the last 2 bytes is CRLF.
namespace perlite {

class User;
class Channel;

typedef std::string              Parameter;
typedef std::vector<Parameter>   ParamTable;

typedef std::vector<User*>       UserTable;
typedef std::vector<Channel*>    ChannelTable;
typedef	std::vector<std::string> StringTable;

} // namespace perlite

#endif