#include "BTVehicleServer.h"
#include "NXTVehicle.h"
#include "BTVehicle.h"
#include <string>
#include <iostream>
#include <vector>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using namespace std;

int main(int argc, char **argv) {
	int motorCount;
	string portName, name;
	
	if(argc < 3) {
		cout << "Usage: server <address> <port> [stdin] configfile";
		return 1;
	}
	
	BTVehicleServer server(argv[1], argv[2], 5);
	int i = 0;
	while(cin >> name) {
		cin >> motorCount;
		cin >> portName;
		if(name == "nxt") {
			boost::shared_ptr<BTVehicle> nxt(new NXTVehicle(portName.c_str(), motorCount));
			if(!server.AddVehicle(nxt)) {
				cout << "Error adding nxt: " << portName << endl;
				return 1;
			}
		}
		i++;
	}
	
	try {
		cout << "Starting server with " << i << " vehicles on " << argv[1] << ":" << argv[2] << "..." << endl;
		server.run();
	}
	catch(exception &e) {
		cout << cerr << e.what() << endl;
	}
	return 0;
}