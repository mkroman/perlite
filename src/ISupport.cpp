#include <iostream>

#include "ISupport.hpp"

using namespace std;

namespace perlite {

ISupport::ISupport() : m_params() {
	// …
}

const string& ISupport::getValue(const string& name) {
	return m_params.find(name)->second;
}

bool ISupport::hasParameter(const string& name) {
	//map<string, string>::iterator it;
	map<string, string>::iterator it;

	for (it = m_params.begin(); it != m_params.end(); it++) {
		if (it->first == name)
			return true;
	}

	return false;
}

void ISupport::merge(const ParamTable& parameters) {
	ParamTable params = parameters;
	ParamTable::iterator it;
	size_t pos;

	for (it = params.begin(); it < params.end(); it++) {
		pos = (*it).find_first_of("=");
		string key, value;

		if (pos != string::npos) {
			key   = it->substr(0, pos);
			value = it->substr(++pos, string::npos);
			
			cout << key << " = " << value << endl << endl;
			m_params.insert(pair<string, string>(key, value));
		}
		else {
			m_params.insert(pair<string, string>(*it, ""));
			cout << *it << endl << endl;
		}
	}
}

}
