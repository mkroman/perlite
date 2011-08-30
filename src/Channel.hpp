#ifndef __PERLITE_CHANNEL_H_
#define __PERLITE_CHANNEL_H_
#include <map>
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
  const time_t& getTopicTime() { return m_topicPosixTime; }
  const uint16_t getFlags() { return m_flags; }
  const std::string& getName() { return m_name; }
  const std::string& getTopic() { return m_topic; }

  const uint8_t getUserFlags(User* user);
  // </Getters>

  // <Setters>
  void addUserRef(User* user);
  void delUserRef(User* user);

  void addUserFlags(User* user, uint8_t flags);
  void delUserFlags(User* user, uint8_t flags);
  void setUserFlags(User* user, uint8_t flags);

  void setTopic(const std::string& topic) { m_topic = topic; }
  void setTopicTime(const time_t& nixtime) { m_topicPosixTime = nixtime; }
  // </Setters>

 private:
  time_t      m_topicPosixTime;
  uint16_t    m_flags;
  UserFlags   m_userFlags;
  UserTable   m_users;
  std::string m_name;
  std::string m_topic;
};

} // namespace perlite

#endif