#ifndef SIMPLE_H
#define SIMPLE_H

#include <stdio.h>
#include <phidget21.h>

class PhidgetClass
{
public:
	void start();
	int AttachHandler(CPhidgetHandle IFK);
	int DetachHandler(CPhidgetHandle IFK);
	int ErrorHandler(CPhidgetHandle IFK, int ErrorCode, const char *unknown);
private:
	CPhidgetInterfaceKitHandle ifKit;
};

#endif //SIMPLE_H
