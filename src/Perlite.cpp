#include <stdlib.h>
#include <iostream>
#include <string>

#include "Client.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	Perlite::Client* client = new Perlite::Client();

	if (client->connect("uplink.io", 6667)) {
		client->loop();
	}
	else {
		cerr << "Connection failed." << endl;
	}

	delete client;

	return 0;
}