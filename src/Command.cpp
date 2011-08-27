#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdio.h>

#include "Command.hpp"

namespace Perlite {

Command* Command::parse(const std::string& line) {
	int    code = 0, index = 0;
	char*  sptr, *ptr, buffer[MIN_BUFFER_SIZE];
	char*  prefix, *command, *params[MAX_PARAMS_LENGTH];

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

		command = sptr;
	}

	// Parse parameters
	while (*ptr && index < MAX_PARAMS_LENGTH) {
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

	Command* instance = NULL;

	if (command)
		instance = new Command(prefix, command, params, index);

	return instance;
}

Command::Command(char* prefix, char* command, char* params[], size_t params_count) :
	name_(std::string(command)),
	prefix_(std::string(prefix)) {
	
	for (unsigned int i = 0; i < params_count; i++)
		params_.push_back(std::string(params[i]));
}

Command::~Command(void) {
	
}

}