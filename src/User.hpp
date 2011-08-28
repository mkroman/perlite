#ifndef __PERLITE_USER_H_
#define __PERLITE_USER_H_
#include <string>
#include <stdint.h>

namespace perlite {

class Channel;

class User {
 public:
  User(const std::string& nick, Channel* channel);

 public:
  // <Getters>
  // Get the bitmask flags for the user.
  // Flags:
  //
  // CMD_LITERAL - Set when it's not a numeric command (353, e.g.)
  // CMD_USER    - Set when the prefix is in the form of a users hostmask.
  const uint16_t getFlags() { return m_flags; }

  const std::string& getNick() { return m_nick; }
  const std::string& getHost() { return m_host; }
  const std::string& getIdent() { return m_ident; }
  // </Getters>

  // <Setters>
  void setNick(const std::string& nick) { m_nick = nick; }
  void setHost(const std::string& host) { m_host = host; }
  void setIdent(const std::string& ident) { m_ident = ident; }
  // </Setters>

 private:
  uint8_t     m_flags;
  Channel*    m_channel;

  std::string m_nick;
  std::string m_host;
  std::string m_ident;
};

} // namespace perlite

#endif