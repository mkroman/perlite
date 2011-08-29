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
  // <Getters>
  UserTable& getUsers() { return m_users; }
  const uint16_t getFlags() { return m_flags; }
  const std::string& getName() { return m_name; }
  // </Getters>

  // <Setters>
  void addUserRef(User* user);
  void delUserRef(User* user);
  // </Setters>

 private:
  uint16_t    m_flags;
  UserTable   m_users;
  std::string m_name;
};

} // namespace perlite

#endif