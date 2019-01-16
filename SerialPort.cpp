//
//  AsyncSerialPort.cpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-06.
//

#include "SerialPort.hpp"

namespace Cockpit {
    
    SerialPort::SerialPort(std::string t_device_id) {
        this->m_device_id = t_device_id;
    }

    SerialPort::SerialPort() {
        return;
    }
    
    void SerialPort::setName(std::string t_name) {
        this->m_device_name = t_name;
    }
    
    std::string SerialPort::getName() {
        return this->m_device_name;
    }

    void SerialPort::openPort() {
        this->m_handle = open(this->m_device_id.c_str(), O_RDWR | O_NOCTTY | O_EXCL);
        
        struct termios tty;
        
        /* Error Handling */
        if ( tcgetattr ( this->m_handle, &tty ) != 0 ) {
            printf("MCP: Error %d from tcgetattr \n", errno);
        }
        
        /* Set Baud Rate */
        cfsetospeed (&tty, SERIAL_BAUD);
        cfsetispeed (&tty, SERIAL_BAUD);
        
        /* Setting other Port Stuff */
        tty.c_cflag     &=  ~PARENB;            // Make 8n1
        tty.c_cflag     &=  ~CSTOPB;
        tty.c_cflag     &=  ~CSIZE;
        tty.c_cflag     |=  CS8;
        tty.c_cflag     &=  ~CRTSCTS;
        tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
        
        tty.c_lflag     |= ICANON;
        
        //tty.c_oflag     |= OPOST;
        
        /* Flush Port, then applies attributes */
        tcflush( this->m_handle, TCIFLUSH );
        if ( tcsetattr ( this->m_handle, TCSANOW, &tty ) != 0) {
            printf("MCP: Error %d from tcgetattr \n", errno);
        }
        printf("MCP: Connected to '%s' (%s). \n", this->m_device_name.c_str(), this->m_device_id.c_str());
        usleep(1000000);
    }

    void SerialPort::closePort() {
        this->m_handle = -1;
        close(this->m_handle);
        printf("MCP: Disconnected from '%s' (%s).\n", this->m_device_name.c_str(), this->m_device_id.c_str());
    }

    std::string SerialPort::readData() {
        if(!this->isOpen()) {
            printf("MCP: Error: Port '%s' is not open \n", this->m_device_name.c_str());
            return "";
        }
        
        int n = 0;
        char buf[255];
        std::string result;
        
        n = (int)read(this->m_handle, &buf, sizeof(buf) - 1);
        buf[n] = 0;
        result = std::string(buf);
        
        if (n < 0) {
            printf("MCP: Error reading: %d \n", errno);
        }
        else if (n == 0) {
            printf("MCP: Empty Message \n");
        }
        else {
            result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
            result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
            printf("MCP: Command Recieved: '%s' \n", result.c_str());
            return result;
        }
        return "Retry";
    }

    void SerialPort::writeData(std::string t_message) {
        t_message = t_message + "\n";
        char buffer[t_message.length()];
        
        strcpy(buffer, t_message.c_str());
        int nBytes = (int)sizeof(buffer);
        
        int n_written = (int)write(this->m_handle, buffer, nBytes);
        tcflush( this->m_handle, TCIFLUSH );
        
        if(n_written <= 0) {
            printf("MCP: Error writing to serial port: %s - %s \n", this->m_device_id.c_str(), strerror(errno));
        }
        
        printf("MCP: Command Transmitted: %s \n", buffer);
    }

    bool SerialPort::isOpen() {
        return this->m_handle > 0;
    }

    bool SerialPort::hasData() {
        if(this->m_handle <= 0) return false;
        
        fd_set set;
        struct timeval timeout;
        
        __DARWIN_FD_ZERO(&set);
        __DARWIN_FD_SET(this->m_handle, &set);
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        return select(__DARWIN_FD_SETSIZE, &set, NULL, NULL, &timeout) == 1;
    }

    bool SerialPort::writeable() {
        if(this->m_handle <= 0) return false;
        
        fd_set set;
        struct timeval timeout;
        
        __DARWIN_FD_ZERO(&set);
        __DARWIN_FD_SET(this->m_handle, &set);
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        
        return select(__DARWIN_FD_SETSIZE, NULL, &set, NULL, &timeout) == 1;
    }
} // namespace Cockpit

