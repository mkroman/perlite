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
			prefix = slicestr(buffer, 1, index);
	}

	// Parse command
	if (isdigit(buffer[0]) && isdigit(buffer[1]) && isdigit(buffer[2])) {
		code = strtoi(slicestr(buffer, 0, 3));
	}
	else {
		index = buffer.find(' ', 0);

		if (index != string::npos)
			name = slicestr(buffer, 0, index);
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
				params.push_back(slicestr(buffer, 0, index, 0));
			}
			else {
				params.push_back(slicestr(buffer, 0, index, 1));
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

} // namespace perlite