// remote.cc - a keyboard input remote control for a bluetooth connected Lego NXT vehicle
// Brad J. Yinger: May 10, 2009

#include <iostream>
#include "NXTVehicle.h"

using namespace std;

int main(int nArgs, char **argv) {
	char *portname = argv[1];
	// vehicle with 2 motors
	NXTVehicle *nxt = new NXTVehicle(portname, 3);
	if(!nxt->connect()) {
		cout << "error connecting to device file " << portname << endl;
		delete nxt;
		return 1;
	}
	cout << "Successfully connected." << endl << endl;
	char buf[256];
	int len, i;
	len = nxt->name(buf, sizeof(buf));
	// Print out the name of the vehicle we connected to
	cout << "Name: " << buf << endl;
	len = nxt->version(buf, sizeof(buf));
	// Print out its version
	cout << "Version (AVR, Firmware, BC4): ";
	for(i = 4; i < len; i++) {
		printf("%d ", buf[i]);
	}
	cout << endl;
	
	// start driving
	cout << "starting the engine..." << endl;
	nxt->drive();
	
	// start the motors
	cout << "putting it in drive..." << endl;
	nxt->set_motorstates(MOTOR_ON);
	
	
	//if (i_motorval[i]<-100){i_motorval[i]=-100; NXT_motorval[i]=-100;}
	//if (i_motorval[i]>100){i_motorval[i]=100; NXT_motorval[i]=100;}
	
	char c;
	while(cin >> c) {
		if(c == 'x') {
			cout << "turning off engine..." << endl;
			// Turn off the motors
			nxt->set_motorstates(MOTOR_OFF);
			nxt->stop_driving();
			break;
		}
		else if(c == 'w') {
			cout << "moving..." << endl;
			nxt->set_motorspeed(0, -50);
			nxt->set_motorspeed(2, 50);
		}
		else if(c == 's') {
			cout << "stopping..." << endl;
			nxt->set_motorspeed(0, 0);
			nxt->set_motorspeed(2, 0);
		}
	}
	
	// Don't forget to disconnect even though we don't really have to
	nxt->disconnect();
	delete nxt;
	return 0;
}