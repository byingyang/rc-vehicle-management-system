// Brad J. Yinger: May 10, 2009

#include "NXTVehicle.h"

// We could do all the sensor stuff if we wanted to
// But for now, I'm only worried about the motors

NXTVehicle::NXTVehicle(const char *pn, int mtrcnt) : BTVehicle(pn, mtrcnt) {
	
}

// Tries to put the version of the NXT in @buff of size @size
int NXTVehicle::version(char *buff, size_t size) {
	// Protocol is 1,0x88
	char msg[] = { 0x01, 0x88 };
	bool b = send(msg, 2);
	int len = -1;
	if(b) {
		len = receive(buff, size);
	}
	return len;
}

// Returns the name of the NXT in @buff
int NXTVehicle::name(char *buff, size_t size) {
	char msg[256];
	int len = info(msg, 256);
	int i;
	for(i = 0; i < 15 && i < size - 1; i++) {
		buff[i] = msg[i + 3];
	}
	buff[i] = 0;
}

// Returns some info about the NXT including name and id in @buff
int NXTVehicle::info(char *buff, size_t size) {
	// Protocol is 1,0x9B
	char msg[] = { 0x01, 0x9B };
	bool b = send(msg, 2);
	int len = -1;
	if(b) {
		len = receive(buff, size);
	}
	return len;
}

// Kudos to (V 1.01 (c) 12.2006 by Daniel Berger) for figuring this one out
void NXTVehicle::setdrivestate(unsigned char motor, char speed, unsigned char modebyte, unsigned char regulationmode, unsigned char turnratio, unsigned char runstate, unsigned long tacholimit) {
	int res;
	char buff[12];
	
	buff[0] = 0x80; //no response please
	buff[1] = 0x04; //SETOUTPUTSTATE
	buff[2] = motor; //port number 0..2
	buff[3] = (unsigned char)speed;
	buff[4] = modebyte;
	buff[5] = regulationmode;
	buff[6] = turnratio;
	buff[7] = runstate;
	buff[8] = 0; //(unsigned char)(tacholimit & 0xff); //unsigned long tacholimit -not sure if the byte order is correct
	buff[9] = 0; //(unsigned char)((tacholimit >> 8) & 0xff);
	buff[10] = 0; //(unsigned char)((tacholimit >> 16) & 0xff);
	buff[11] = 0; //(unsigned char)(tacholimit >> 24);
	res = send(buff, 12);
}

void NXTVehicle::do_update() {
	// update the motors
	for (int i = 0; i < motorcount; i++) {
		if (motorstate[i] == MOTOR_ON) {
			setdrivestate(i, motorspeed[i], 5, 0x01, 0x00, 0x20, 0);
		}
		else {
			setdrivestate(i, motorspeed[i], 0, 0x00, 0x00, 0x00, 0);
		}
	}
}

