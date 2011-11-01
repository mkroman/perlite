#ifndef __PERLITE_ISUPPORT_H_
#define __PERLITE_ISUPPORT_H_
#include <vector>

#include "Perlite.hpp"

namespace perlite {

class ISupport {
public:
	ISupport();

	void merge(const ParamTable& parameters);
	bool hasParameter(const std::string& name);
	const std::string& getValue(const std::string& name);

private:
	std::map<std::string, std::string> m_params;
};

} // namespace perlite

#endif
