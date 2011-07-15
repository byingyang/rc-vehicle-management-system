// Brad J. Yinger: May 10, 2009

#ifndef NXTVEHICLE_H
#define NXTVEHICLE_H

#include "BTVehicle.h"

class NXTVehicle: public BTVehicle {
	void setdrivestate(unsigned char, char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned long);
	void do_update();
public:
	NXTVehicle(const char *pn, int mtrcnt);
	int version(char *buff, size_t size);
	int name(char *buff, size_t size);
	int info(char *buff, size_t size);
};

#endif