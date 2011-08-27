#ifndef __PERLITE_COMMAND_H_
#define __PERLITE_COMMAND_H_
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "Perlite.hpp"

namespace perlite {

class Command {
public:
	static Command* parseLine(const std::string& line);

public:
	// <Getters>
	bool isNumeric() { return (m_code != 0); }
	signed int getCode() { return m_code; }
	const std::string& getName() { return m_name; }
	const std::string& getPrefix() { return m_prefix; }
	const ParamTable& getParameters() { return m_params; }
	const Parameter& getParam(size_t index) { return m_params.at(index); }
	// </Getters>

protected:
	Command(char* prefix, int code, char* params[], size_t count);
	Command(char* prefix, char* name, char* params[], size_t count);

private:
	signed int  m_code;
	std::string m_name;
	std::string m_prefix;
	ParamTable  m_params;
};

} // namespace perlite

#endif
