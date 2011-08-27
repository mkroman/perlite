#include "Command.hpp"

using namespace std;

namespace perlite {

Command* Command::parseLine(const std::string& line) {
	int    code = 0, index = 0;
	char*  sptr, *ptr, buffer[MIN_BUFFER_SIZE];
	char*  prefix = NULL, *name = NULL, *params[MAX_PARAMS_COUNT];
	Command* command = 0;

	ptr = sptr = buffer;
	
	memset(params, 0, sizeof(params));
	strcpy(buffer, line.c_str());

	// Parse prefix
	if (buffer[0] == ':') {
		while (*ptr && *ptr != ' ')
			ptr++;

		*ptr++ = '\0';

		prefix = buffer + 1; // Skip :
	}

	// Parse command/code
	if (isdigit(ptr[0]) && isdigit(ptr[1]) && isdigit(ptr[2])) {
		ptr[3] = '\0';
		code = atoi(ptr);
		ptr += 4;
	}
	else {
		for (sptr = ptr; *ptr && *ptr != ' '; ptr++)
			;

		*ptr++ = '\0';

		name = sptr;
	}

	// Parse parameters
	while (*ptr && index < MAX_PARAMS_COUNT) {
		if (*ptr == ':') { // last param
			params[index++] = ptr + 1;
			break;
		}
		else {
			for (sptr = ptr; *ptr && *ptr != ' '; ptr++)
				;
			params[index++] = sptr;

			if (*ptr)
				*ptr++ = '\0';
		}
	}

	if (name) {
		command = new Command(prefix, name, params, index);
		return command;
	}
	else {
		command = new Command(prefix, code, params, index);
		return command;
	}
}

Command::Command(char* prefix, int code, char* params[], size_t count) :
	m_code(code) {

	if (prefix)
		m_prefix = string(prefix);
	
	for (unsigned int i = 0; i < count; i++)
		m_params.push_back(string(params[i]));
}

Command::Command(char* prefix, char* name, char* params[], size_t count) :
	m_code(0), m_name(string(name)) {

	if (prefix)
		m_prefix = string(prefix);
	
	for (unsigned int i = 0; i < count; i++)
		m_params.push_back(string(params[i]));
}

} // namespace perlite