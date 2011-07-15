#ifndef SESSIONCONTROLLER_H
#define SESSIONCONTROLLER_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "BTVehicle.h"
#include "request.h"

class SessionController {
public:
	SessionController();
	void StopAllSessions();
	// Add a vehicle
	bool AddVehicle(boost::shared_ptr<BTVehicle>);
	// Get a vehicle (1-based index)
	boost::shared_ptr<BTVehicle> GetVehicle(int);
	// Get the number of vehicles
	int NumVehicles();
private:
	// Vector of vehicles
	std::vector<boost::shared_ptr<BTVehicle> > vehicles;
};
#endif
