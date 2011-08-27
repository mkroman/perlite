#ifndef __PERLITE_COMMAND_H_
#define __PERLITE_COMMAND_H_
#include <vector>
#include <string>
#include <sstream>
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

	// <Helpers>
	// Converts a std::string instance to a signed integer.
	static signed int  strtoi(const std::string& str);

	// Returns the substring of source with start index of start, to finish.
	// The source is then cut from 0 to the finish index + the add parameter.
	static std::string slice(std::string& source, size_t start, size_t finish,
		size_t add = 1);
	// </Helpers>

protected:
	Command(signed int code, std::string prefix, ParamTable params) :
		m_code(code), m_prefix(prefix), m_params(params) {};

	Command(std::string name, std::string prefix, ParamTable params) :
		m_code(0), m_name(name), m_prefix(prefix), m_params(params) {};

private:
	signed int  m_code;
	std::string m_name;
	std::string m_prefix;
	ParamTable  m_params;
};

} // namespace perlite

#endif
