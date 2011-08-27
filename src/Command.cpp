#include "Command.hpp"

using namespace std;

namespace perlite {

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

signed int Command::strtoi(const string& source) {
	signed int i = 0;
	stringstream _stream(source);
	_stream >> i;

	return i;
}

string Command::slice(string& source, size_t start, size_t finish,
	size_t add) {
	string _slice;

	_slice = source.substr(start, finish);
	source.erase(0, finish + add);

	return _slice;
}

} // namespace perlite