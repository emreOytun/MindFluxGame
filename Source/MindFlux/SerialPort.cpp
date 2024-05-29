#include "SerialPort.h"
#include <iostream>

SerialPort::SerialPort(const std::string& portName, int baudRate)
    : PortName(portName), BaudRate(baudRate), FileDescriptor(-1), Opened(false)
{
}

SerialPort::SerialPort(const std::string& portName, int baudRate , int dummy)
    : portBlue(portName), nanoBaudRate(baudRate), fdBlue(-1), openedBlue(false)
{

}

SerialPort::SerialPort(const std::string& fileVision)
    : fileName(fileVision), openedVision(false)
{

}


SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::Open()
{
    FileDescriptor = open(PortName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (FileDescriptor == -1)
    {
        std::cerr << "Failed to open port: " << strerror(errno) << std::endl;
        return false;
    }

    memset(&Tty, 0, sizeof(Tty));
    if (tcgetattr(FileDescriptor, &Tty) != 0)
    {
        std::cerr << "Error from tcgetattr: " << strerror(errno) << std::endl;
        Close();
        return false;
    }

Tty.c_cflag |= (CLOCAL | CREAD); // Enable the receiver and set local mode
Tty.c_cflag &= ~CSIZE; // Mask the character size bits
Tty.c_cflag |= CS8; // Select 8 data bits
Tty.c_cflag &= ~PARENB; // No parity
Tty.c_cflag &= ~CSTOPB; // 1 stop bit
Tty.c_cflag &= ~CRTSCTS; // No hardware flow control

Tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input mode
Tty.c_iflag &= ~(IXON | IXOFF | IXANY); // No software flow control
Tty.c_iflag &= ~(ICRNL | INLCR); // Disable special handling of CR and NL

Tty.c_oflag &= ~OPOST; // Raw output mode

Tty.c_cc[VMIN] = 1; // Minimum number of characters for non-canonical read
Tty.c_cc[VTIME] = 10; // Timeout in deciseconds for non-canonical read


    if (tcsetattr(FileDescriptor, TCSANOW, &Tty) != 0)
    {
        std::cerr << "Error from tcsetattr: " << strerror(errno) << std::endl;
        Close();
        return false;
    }

    Opened = true;
    return true;
}

void SerialPort::Close()
{
    if (Opened)
    {
        close(FileDescriptor);
        Opened = false;
    }
}

bool SerialPort::IsOpen() const
{
    return Opened;
}

ssize_t SerialPort::Read(char* buffer, size_t size)
{
    if (!Opened) return -1;
    return read(FileDescriptor, buffer, size);
}

ssize_t SerialPort::ReadBluetooth(char* buffer, size_t size)
{
    if (!openedBlue) return -1;
    return read(fdBlue, buffer, size);
}

ssize_t SerialPort::ReadVision(char* buffer, size_t size)
{
    if (!openedVision) return -1;
    return read(fdVision, buffer, size);
}


ssize_t SerialPort::Write(const char* buffer, size_t size)
{
    if (!Opened) return -1;
    return write(FileDescriptor, buffer, size);
}

bool SerialPort::OpenBluetooth()
{
    fdBlue = open(portBlue.c_str(), O_RDWR | O_NOCTTY | O_NDELAY , 0777);
	if (fdBlue == -1)
    {
        // Use UE_LOG to log the error message
        UE_LOG(LogTemp, Error, TEXT("fdBLUE"));
        return false;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fdBlue, &tty) != 0)
    {
        std::cerr << "Error from tcgetattr for Bluetooth: " << strerror(errno) << std::endl;
        UE_LOG(LogTemp, Error, TEXT("SECOND"));
		close(fdBlue);
        return false;
    }

    tty.c_cflag |= (CLOCAL | CREAD);  // Enable the receiver and set local mode
    tty.c_cflag &= ~CSIZE;            // Mask the character size bits
    tty.c_cflag |= CS8;               // Select 8 data bits
    tty.c_cflag &= ~PARENB;           // No parity
    tty.c_cflag &= ~CSTOPB;           // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;          // No hardware flow control

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input mode
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);        // No software flow control
    tty.c_iflag &= ~(ICRNL | INLCR);               // Disable special handling of CR and NL
    tty.c_oflag &= ~OPOST;                         // Raw output mode

    tty.c_cc[VMIN] = 1;    // Minimum number of characters for non-canonical read
    tty.c_cc[VTIME] = 10;  // Timeout in deciseconds for non-canonical read

    if (tcsetattr(fdBlue, TCSANOW, &tty) != 0)
    {
        std::cerr << "Error from tcsetattr for Bluetooth: " << strerror(errno) << std::endl;
        close(fdBlue);
        return false;
    }

    openedBlue = true;
    return true;
}

bool SerialPort::OpenVision() {
    fdVision = open(fileName.c_str(), O_RDONLY);
    if (fdVision == -1) {
        UE_LOG(LogTemp, Error, TEXT("fdVision"));
        return false;
    }

    openedVision = true;
    return true;
}

bool SerialPort::IsOpenBluetooth() const
{
    return openedBlue;
}

bool SerialPort::IsOpenVision() const
{
    return openedVision;
}

void SerialPort::CloseBluetooth()
{
    if (openedBlue)
    {
        close(fdBlue);
        openedBlue = false;
    }
}

void SerialPort::CloseVision() {
    if (fdVision != -1) {
        close(fdVision);
        fdVision = -1;
        openedVision = false;
    }
}
