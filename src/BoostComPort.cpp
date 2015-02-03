/*
Copyright 2010 by Fabian Schwartau
E-Mail: fabian@opencode.eu

This file is part of BoostComPort.

BoostComPort is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

HM2PC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BoostComPort.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "BoostComPort.hpp"
#include <boost/bind.hpp>

BoostComPort::BoostComPort():
currentContent(0),
serialPort(io_service)
{
	lastError.clear();
	buffer=new char[BUFFER_SIZE];
	eventBuffer=new char[EVENTBUFFER_SIZE];
}

BoostComPort::~BoostComPort()
{
	serialPort.close();
	delete[] buffer;
	delete[] eventBuffer;
}

/*
 * Opens a new com port connection.
 * No flow control setting supported so far. You just set the port name
 * and the baud rate. The port name is something like COMx on Windows
 * or /dev/ttySx on Linux.
 * Returns: 0 if the port is opened
 */
int BoostComPort::open(std::string port, int baud)
{
	if(serialPort.is_open())
		close();
	serialPort.open(port);
	boost::asio::serial_port_base::baud_rate baudRate(baud);
	serialPort.set_option(baudRate);
	serialPort.async_read_some(boost::asio::buffer(eventBuffer, EVENTBUFFER_SIZE), boost::bind(&BoostComPort::onPortRead, this, _1, _2));
	char* tmp=new char[100]; // 100Bytes of temporary buffer to clean the system buffers
	int read;
	do  // I think here is missing a timeout or something like that
	{
		read=this->read(tmp, 100, false);
	} while(read>0);
	delete[] tmp;
	return 0;
}

/*
 * Get the last error.
 * Returns: Reference to the last boost error
 */
boost::system::error_code& BoostComPort::getLastError()
{
	return lastError;
}

/*
 * Close the port.
 * Returns: Always 0 at the moment, -1 if an error occurred (may be
 *          implemented in later versions)
 */
int BoostComPort::close()
{
	serialPort.cancel();  // make shure all pending operations are stopped
	serialPort.close();
	io_service.reset();  // don't know why this is needed but without it's not working
	currentContent=0;
	return 0;
}

bool BoostComPort::isOpended()
{
	return serialPort.is_open();
}

int BoostComPort::write(char* data, unsigned int length)
{
	size_t written=boost::asio::write(serialPort, boost::asio::buffer(data, length), boost::asio::transfer_all(), ec);
	if(written!=length)
		return -1;
	return 0;
}

/*
 * Read a certain amount of data
 * The function supports blocking and non-blocking mode.
 * In non-blocking mode the requested amount of data will be read
 * except it is not available, then the available amount will be read.
 * In blocking mode the method will wait for all requested data. If a
 * timeout is given in the non-blocking mode the method will return
 * all read data after the timeout.
 * The read method also polls the com port buffer, so make shure the
 * method is called frequently enough to get all the data from the
 * system buffers without loosing something.
 */
int BoostComPort::read(char* data, int length, bool blocking, int timeout)
{
	boost::timer time;
	do
	{
		executed=false;
		io_service.poll(); // read new data
		if(executed)
			serialPort.async_read_some(boost::asio::buffer(eventBuffer, EVENTBUFFER_SIZE), boost::bind(&BoostComPort::onPortRead, this, _1, _2));
		if(timeout>0)
		{
			if((int)(time.elapsed()*1000)>timeout || !serialPort.is_open())
				return -1;
		}
	} while(blocking && currentContent<length);
	int toCopy;
	if(length<currentContent)
		toCopy=length;
	else
		toCopy=currentContent;
	memcpy(data, buffer, toCopy);
	memcpy(buffer, buffer+toCopy, currentContent-toCopy);
	currentContent-=toCopy;
	return toCopy;
}

/*
 * Internal asynchronous method for read events.
 * It is "called" by the io_service.poll() call in the
 * read method.
 */
void BoostComPort::onPortRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(error==0)
	{
		cout<<"Read some bytes: "<<bytes_transferred<<endl;
		memcpy(buffer+currentContent, eventBuffer, bytes_transferred);
		currentContent+=bytes_transferred;
	}
	else if(error!=boost::asio::error::operation_aborted)  // not cancelled
	{
		cerr<<"BoostComPort::onPortRead(): Unable to read from com port with error: "<<endl;
		cerr<<"BoostComPort::onPortRead():   Error message: "<<error.message()<<endl;
		cerr<<"BoostComPort::onPortRead():   Error code: "<<error<<endl;
		lastError=ec;
	}
	executed=true;
}
