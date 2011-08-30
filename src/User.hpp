#ifndef __PERLITE_USER_H_
#define __PERLITE_USER_H_
#include <string>
#include <stdint.h>

#include "Perlite.hpp"

namespace perlite {

class Channel;

class User {
 public:
  User() {};
  User(const std::string& nick) : m_nick(nick) {};

 public:
  // <Getters>
  ChannelTable& getChannels() { return m_channels; }

  const std::string& getNick() { return m_nick; }
  const std::string& getHost() { return m_host; }
  const std::string& getIdent() { return m_ident; }
  // </Getters>

  // <Setters>
  void setNick(const std::string& nick) { m_nick = nick; }
  void setHost(const std::string& host) { m_host = host; }
  void setIdent(const std::string& ident) { m_ident = ident; }

  void delChannelRef(Channel* channel);
  void addChannelRef(Channel* channel) { m_channels.push_back(channel); }
  // </Setters>

 private:
  std::string  m_nick;
  std::string  m_host;
  std::string  m_ident;
  ChannelTable m_channels;
};

} // namespace perlite

#endif