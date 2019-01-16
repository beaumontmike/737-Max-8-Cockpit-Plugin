//
//  AsyncSerialPort.hpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-06.
//

#ifndef SerialPort_hpp
#define SerialPort_hpp

#include "XPLMUtilities.h"
#include <algorithm>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string>

const speed_t SERIAL_BAUD(B115200);

namespace Cockpit {
    class SerialPort {
    public:
        SerialPort();
        SerialPort(std::string t_device_id);
        
        void            setName(std::string t_name);
        std::string     getName();
        void            openPort();
        void            closePort();
        void            writeData(std::string t_data);
        std::string     readData();
        bool            hasData();
        bool            writeable();
        bool            isOpen();
        
    private:
        int             m_handle;
        std::string     m_device_name;
        std::string     m_device_id;
    };
} // namespace Cockpit

#endif /* AsyncSerialPort_hpp */
