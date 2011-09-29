#include "ISupport.hpp"

namespace perlite {

ISupport::ISupport() {
  // …
}

string& ISupport::setValue(const string& key, const string& value) {
  m_params[key] = value;

  return value;
}

string& ISupport::getValue(const string& key) {
  return m_params[key];
}

}