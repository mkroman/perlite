#include "Command.hpp"

using namespace std;

namespace perlite {

Command::Command(signed int code, string prefix, ParamTable params) : 
                 m_code(code), m_params(params), m_prefix(prefix) {
    // Parse the prefix if it's a hostmask.
    if (!m_prefix.empty() && m_prefix.find_first_of("!@") != string::npos)
    	parsePrefix();
    	
	m_flags &= ~CMD_LITERAL;
}

Command::Command(string name, string prefix, ParamTable params) :
                 m_code(0), m_params(params), m_name(name), m_prefix(prefix) {
    // Parse the prefix if it's a hostmask.
    if (!m_prefix.empty() && m_prefix.find_first_of("!@") != string::npos)
    	parsePrefix();

	m_flags |= CMD_LITERAL;
}

Command* Command::parseLine(const string& line) {
	Command* command;
	vector<string> params;
	size_t index = 0, code = 0;
	string prefix, name, buffer;

	buffer = line;

	// Parse prefix
	if (line[0] == ':') {
		index = buffer.find(' ', 0);

		if (index != string::npos)
			prefix = slice(buffer, 1, index);
	}

	// Parse command
	if (isdigit(buffer[0]) && isdigit(buffer[1]) && isdigit(buffer[2])) {
		code = strtoi(slice(buffer, 0, 3));
	}
	else {
		index = buffer.find(' ', 0);

		if (index != string::npos)
			name = slice(buffer, 0, index);
	}

	// Parse parameters
	while (params.size() < MAX_PARAM_COUNT && !buffer.empty()) {
		if (buffer[0] == ':') {
			params.push_back(buffer.substr(1));
			break;
		}
		else {
			index = buffer.find(' ', 0);

			if (index == string::npos) {
				params.push_back(slice(buffer, 0, index, 0));
			}
			else {
				params.push_back(slice(buffer, 0, index, 1));
			}
		}
	}

	if (!name.empty()) {
		command = new Command(name, prefix, params);
	}
	else {
		command = new Command(code, prefix, params);
	}

	return command;
}

void Command::parsePrefix() {
	size_t begin, end;
	string nick, ident, host;

	// Parse the nickname, username and hostname if it's in the prefix.
	if ((end = m_prefix.find_first_of('!')) != string::npos) {
		// Save the nickname.
		m_nick = m_prefix.substr(0, end);

		// Save the ident/username.
		begin = end + 1;
		end = m_prefix.find_first_of('@');
		m_ident = m_prefix.substr(begin, end);

		// Save the hostname.
		m_host = m_prefix.substr(end + 1);
	}
}

signed int Command::strtoi(const string& source) {
	signed int i = 0;
	stringstream stream(source);
	
	stream >> i;

	return i;
}

string Command::slice(string& source, size_t start, size_t finish,
	size_t add) {
	string slice;

	slice = source.substr(start, finish);
	source.erase(0, finish + add);

	return slice;
}

} // namespace perlite