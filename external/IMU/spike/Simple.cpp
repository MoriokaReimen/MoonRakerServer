#include "simple.h"

int CCONV CAttachHandler(CPhidgetHandle IFK, void *userptr)
{
	((PhidgetClass *)userptr)->AttachHandler(IFK);
	return 0;
}
int CCONV CDetachHandler(CPhidgetHandle IFK, void *userptr)
{
	((PhidgetClass *)userptr)->DetachHandler(IFK);
	return 0;
}

int CCONV CErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
	((PhidgetClass *)userptr)->ErrorHandler(IFK,ErrorCode,unknown);	
	return 0;
}

void PhidgetClass::start()
{
	CPhidgetInterfaceKit_create(&ifKit);
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, CAttachHandler, this);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, CDetachHandler, this);
	CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, CErrorHandler, this);
	CPhidget_open((CPhidgetHandle)ifKit, -1);
}

int PhidgetClass::AttachHandler(CPhidgetHandle IFK)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName(IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int PhidgetClass::DetachHandler(CPhidgetHandle IFK)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int PhidgetClass::ErrorHandler(CPhidgetHandle IFK, int ErrorCode, const char *unknown)
{
	printf("Error handled. %d - %s", ErrorCode, unknown);

	return 0;
}

int main(int argc, char* argv[])
{
	PhidgetClass pClass;
	pClass.start();

	while(1);

	return 0;
}
