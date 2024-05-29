#pragma once

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string>
using namespace std;
class SerialPort
{
public:
    SerialPort(const std::string& portName, int baudRate);
	SerialPort(const std::string& portName, int baudRate , int dummy);
	SerialPort(const std::string& fileVision);
    ~SerialPort();
	inline void setBlue(const std::string& port) 
	{
		portBlue = port;
	}
    bool Open();
    void Close();
    bool IsOpen() const;
    ssize_t Read(char* buffer, size_t size);
	ssize_t ReadBluetooth(char* buffer, size_t size);	
	ssize_t ReadVision(char* buffer, size_t size);
    ssize_t Write(const char* buffer, size_t size);
	bool OpenBluetooth();
	bool OpenVision();
	bool IsOpenBluetooth() const;
	bool IsOpenVision() const;
	void CloseBluetooth();
	void CloseVision();

private:
    std::string PortName;
	std::string portBlue;
	std::string fileName;
    int BaudRate;
	int nanoBaudRate;
    int FileDescriptor;
	int fdBlue;
    bool Opened;
	bool openedBlue;
	bool openedVision;
	int fdVision;
    struct termios Tty;
};
