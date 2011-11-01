#include <iostream>

#include "Perlite.hpp"
#include "ISupport.hpp"

using namespace std;

namespace perlite {

ISupport::ISupport() {
	// …
}

void ISupport::merge(const ParamTable& parameters) {
	ParamTable test = parameters;
	ParamTable::iterator it;

	for (it = test.begin(); it < test.end(); it++) {
		cout << *it << endl << endl;
	}
}

}
