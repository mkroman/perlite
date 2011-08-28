#ifndef __PERLITE_COMMAND_H_
#define __PERLITE_COMMAND_H_
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "Perlite.hpp"

namespace perlite {

class Command {
public:
	static Command* parseLine(const std::string& line);

public:
	// <Getters>
	bool isNumeric() { return (m_code != 0); }
	const uint8_t getFlags() { return m_flags; }
	signed int getCode() { return m_code; }
	const char* getCParam(size_t index) { return m_params.at(index).c_str(); }
	const Parameter& getParam(size_t index) { return m_params.at(index); }
	const ParamTable& getParameters() { return m_params; }
	const std::string& getNick() { return m_nick; }
	const std::string& getHost() { return m_host; }
	const std::string& getName() { return m_name; }
	const std::string& getIdent() { return m_ident; }
	const std::string& getPrefix() { return m_prefix; }
	// </Getters>

	// <Setters>
	void setMask(std::string nick, std::string ident, std::string host);
	// </Setters>

	// <Helpers>
	// Converts a std::string instance to a signed integer.
	static signed int  strtoi(const std::string& str);

	// Returns the substring of source with start index of start, to finish.
	// The source is then cut from 0 to the finish index + the add parameter.
	static std::string slice(std::string& source, size_t start, size_t finish,
		                                                        size_t add = 1);
	// </Helpers>

protected:
	Command(signed int code, std::string prefix, ParamTable params);
	Command(std::string name, std::string prefix, ParamTable params);

	// Cut the prefix up and save nickname, username and hostname to their 
	// designated member variable.
	void parsePrefix();

private:
	uint8_t       m_flags;
	signed int    m_code;
	ParamTable    m_params;
	std::string   m_name;
	std::string   m_prefix;
	std::string   m_nick;
	std::string   m_host;
	std::string   m_ident;
};

} // namespace perlite

#endif
