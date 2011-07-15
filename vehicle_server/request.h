#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/// A request received from a client.
struct request {
  	std::string command;
  	int vehicle_index;
  	int motor_index;
	int speed;
};

#endif