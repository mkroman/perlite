#ifndef __PERLITE_COMMAND_H_
#define __PERLITE_COMMAND_H_
#include <vector>
#include <string>

#define MIN_BUFFER_SIZE 512
#define MAX_PARAMS_LENGTH 10

typedef std::vector<std::string> ParamList;

namespace Perlite {
	class Command {
	public:
		static Command* parse(const std::string& line);

	// Getters
	public:
		ParamList& getParameters() {
			return params_;
		}

		std::string& getName() {
			return name_;
		}

		std::string& getPrefix() {
			return prefix_;
		}

	protected:
		Command(char* prefix, char* command, char* params[], size_t params_count);
		~Command(void);

	private:
		std::string name_;
		std::string prefix_;
		ParamList   params_;
	};
}

#endif