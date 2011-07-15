#include "SessionController.h"
#include <iostream>

using namespace std;

SessionController::SessionController() {
	
}

void SessionController::StopAllSessions() {
	for(std::vector<boost::shared_ptr<BTVehicle> >::iterator iter = vehicles.begin(); iter != vehicles.end(); iter++) {
		(*iter)->disconnect();
	}
}

boost::shared_ptr<BTVehicle> SessionController::GetVehicle(int index) {
	if(index > 0 && index <= vehicles.size())
		return vehicles[index - 1];
}

bool SessionController::AddVehicle(boost::shared_ptr<BTVehicle> v) {
	if(v->connect()) {
		vehicles.push_back(v);
		return true;
	}
	return false;
}

int SessionController::NumVehicles() {
	return vehicles.size();
}