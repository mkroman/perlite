#ifndef __PERLITE_COMMAND_H_
#define __PERLITE_COMMAND_H_
#include <vector>
#include <string>

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

		std::string getParam(ParamList::size_type index) {
			return params_.at(index);
		}

		std::string& getName() {
			return name_;
		}

		std::string& getPrefix() {
			return prefix_;
		}

		signed int getCode() {
			return code_;
		}

		bool isNumeric() {
			return numeric_;
		}

	protected:
		Command(char* prefix, int code, char* params[], size_t count);
		Command(char* prefix, char* command, char* params[], size_t count);
		~Command(void);

	private:
		signed int  code_;
		std::string name_;
		std::string prefix_;
		ParamList   params_;
		bool        numeric_;
	};
}

#endif