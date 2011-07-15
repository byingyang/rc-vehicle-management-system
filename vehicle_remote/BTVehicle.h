// Brad J. Yinger: May 10, 2009

#ifndef BTVEHICLE_H
#define BTVEHICLE_H
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <boost/bind.hpp>
#include <boost/thread.hpp> /* BOOST threading */
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/noncopyable.hpp>

enum { MOTOR_OFF, MOTOR_ON };

class BTVehicle : private boost::noncopyable {
	int vfd; /* File descriptor for bluetooth device */
	boost::shared_ptr<boost::thread> drive_thread;
	boost::mutex vfd_mutex;
	void set_should_update_and_notify();
	const char *portname;
protected:
	boost::mutex motorspeed_mutex, motorstate_mutex, should_update_mutex;
	boost::condition can_update;
	volatile int motorcount;
	volatile bool should_update;
	volatile int *motorspeed;
	volatile int *motorstate;
	volatile bool should_be_driving;
	bool send(char *msg, size_t size);
	int receive(char *buff, size_t size);
	void done_updating();
	void listen_for_updates();
	virtual bool setup_vehicle();
	virtual void do_update() { }
public:
	BTVehicle(const char *pn, int mtrcnt);
	~BTVehicle();
	bool connect();
	void disconnect();
	bool is_connected();
	bool drive();
	void stop_driving();
	int motor_count();
	virtual void set_motorspeed(int motor, int speed);
	virtual void set_motorstates(int state);
	virtual int version(char *buff, size_t size) { }
	virtual int name(char *buff, size_t size) { }
};
#endif