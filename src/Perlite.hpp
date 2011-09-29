#ifndef __PERLITE_H_
#define __PERLITE_H_
#include <stdint.h>
#include <map>
#include <vector>
#include <string>

namespace perlite {

const int kMaxParamCount = 32;  // Maximum number of parameters parsed in a line.
const int kMinBufferSize = 512; // Minimum buffer size. RFC 1459 defines max as 512
                                // where the last 2 bytes is CRLF.

class User;
class Channel;

typedef std::string              Parameter;
typedef std::vector<Parameter>   ParamTable;

typedef std::vector<User*>       UserTable;
typedef std::vector<Channel*>    ChannelTable;
typedef std::vector<std::string> StringTable;

typedef std::map<User*, uint8_t> UserFlags;

// kCmdUser is set when the command prefix is in the format of a users hostmask.
// kCmdLiteral is set when the command is not in numeric form (353, e.g.)
enum CommandTypes {
  kCmdUser = 0x40,
  kCmdLiteral = 0x80
};

// (032) kLevelOper - Mode +o
// (004) kLevelVoice - Mode +v
// (128) kLevelOwner - Mode +q
// (064) kLevelAdmin - Mode +a
// (016) kLevelHalfOper - Mode +h
enum UserPermissions {
  kLevelOper = 0x20,
  kLevelVoice = 0x04,
  kLevelOwner = 0x80,
  kLevelAdmin = 0x40,
  kLevelHalfOper = 0x10
};


} // namespace perlite

#endif