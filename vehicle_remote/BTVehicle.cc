// Brad J. Yinger: May 10, 2009

#include "BTVehicle.h"
#include <iostream>

//--------------- Public Methods ---------------//

BTVehicle::BTVehicle(const char *pn, int mtrcnt) : vfd(-1), portname(pn), motorcount(mtrcnt), should_be_driving(false) {
	motorstate = new int[mtrcnt];
	motorspeed = new int[mtrcnt];
	for(int i = 0; i < mtrcnt; i++) {
		motorstate[i] = MOTOR_OFF;
		motorspeed[i] = 0;
	}
}

BTVehicle::~BTVehicle() {
	disconnect();
	
	delete[] motorstate;
	delete[] motorspeed;
}

int BTVehicle::motor_count() {
	return motorcount;
}

// Attempt to open serial device file @portname
// Returns bool signifying success or failure
bool BTVehicle::connect() {
	// Attempt to open the serial device file
	//vfd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);
	vfd = open(portname, O_RDWR | O_NOCTTY);
	if(vfd != -1) {
		fcntl(vfd, F_SETFL, 0);
		if(setup_vehicle()) {
			return true;
		}
	}
	disconnect();
	return false;
}

// Close the connection to the device
void BTVehicle::disconnect() {
	set_motorstates(MOTOR_OFF);
	stop_driving();
	close(vfd);
	vfd = -1;
}

bool BTVehicle::is_connected() {
	return vfd != -1;
}

// Kicks off a new thread with the &update_motors defined in subclass
bool BTVehicle::drive() {
	if(!should_be_driving) {
		should_be_driving = true;
		drive_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&BTVehicle::listen_for_updates, this)));
	}
	return (drive_thread != NULL);
}

// Requests thread to stop and waits for it to stop
void BTVehicle::stop_driving() {
	if(should_be_driving) {
		// set this before the listener updates for the last time
		should_be_driving = false;
		// turn motors off
		set_motorstates(MOTOR_OFF);
		drive_thread->join();
	}
}

// 0-based motor
void BTVehicle::set_motorspeed(int motor, int speed) {
	if(motor < motorcount) {
		boost::mutex::scoped_lock l(motorspeed_mutex);
		motorspeed[motor] = speed;
		set_should_update_and_notify();
	}
}

// 0-based motor
// we never really want one motor on the other off
void BTVehicle::set_motorstates(int state) {
	for(int i = 0; i < motorcount; i++) {
		boost::mutex::scoped_lock l(motorstate_mutex);
		motorstate[i] = state;
	}
	set_should_update_and_notify();
}

//--------------- Protected Methods ---------------//

// Initializes the connected bluetooth vehicle
// Sets: Baud Rate
// Returns true if the setup was a success or false otherwise
bool BTVehicle::setup_vehicle() {
	struct termios options;
	tcgetattr(vfd, &options); 					/* Get the current options for the device */
	// We'll use 115,200 baud
	cfsetispeed(&options, B115200);
   cfsetospeed(&options, B115200);
	
	options.c_cflag |= (CLOCAL | CREAD); 	/* Enable receiver and set local mode */
	//options.c_cflag &= ~PARENB;				/* No Parity */
	//options.c_cflag &= ~CSTOPB;
	//options.c_cflag &= ~CSIZE; 				/* Mask the character size bits */
	//options.c_cflag |= CS8;    				/* Select 8 data bits */
	
	// Apply the new settings
	tcsetattr(vfd, TCSANOW, &options);
	return true;
}

// Sends a message to the vehicle of size @size
// Returns weather or not the message was successfully sent
bool BTVehicle::send(char *msg, size_t size) {
	char len_header[] = { size, 0x00 };		/* Protocol begins with 2 byte message length header */
	boost::mutex::scoped_lock l(vfd_mutex);
	size_t s = write(vfd, len_header, 2);
	if(s >= 0) {
		s = write(vfd, msg, size);
		return (s >= 0);
	}
	return false;
}

// Gets a message from the vehicle of size @size
// Returns the number of bytes of the data received or -1 if error
int BTVehicle::receive(char *buff, size_t size) {
	//fcntl(vfd, F_SETFL, FNDELAY);
	// Read 2 bytes to get the length of the reply
	char len_header[] = { 0x00, 0x00 };
	boost::mutex::scoped_lock l(vfd_mutex);
	size_t num = read(vfd, len_header, 2);
	int size_to_get = len_header[0] + 256 * len_header[1];
	num = read(vfd, buff, size_to_get);
	//fcntl(vfd, F_SETFL, 0);
	return size_to_get;
}

// It's the thread function's responsibility to call this method
// After it's done updating the vehicle
void BTVehicle::done_updating() {
	should_update = false;
}

// thread function that continuously updates the motors
void BTVehicle::listen_for_updates() {
	// just continuously send the speed and turning commands
	while(true) {
		// this will pause the thread until we want to update the motor
		boost::mutex::scoped_lock lock(should_update_mutex);
		while (!should_update) {
			can_update.wait(lock);
		}
		do_update();
		// we've successfully updated
		done_updating();
		if(!should_be_driving) break;
	}
}

//--------------- Private Methods ---------------//

// everybody else can do their own locking on should_update
void BTVehicle::set_should_update_and_notify() {
	boost::mutex::scoped_lock l(should_update_mutex);
	should_update = true;
	can_update.notify_one();
}

