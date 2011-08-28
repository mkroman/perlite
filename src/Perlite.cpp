#include <stdlib.h>
#include <iostream>
#include <string>

#include "Client.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  perlite::Client* client = new perlite::Client();

  if (client->connect("uplink.io", 6667)) {
    client->runLoop();
  }
  else {
    cerr << "Connection failed." << endl;
  }

  delete client;

  return 0;
}