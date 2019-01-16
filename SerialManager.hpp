//
//  SerialManager.hpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-07.
//

#ifndef SerialManager_hpp
#define SerialManager_hpp

#include "SerialPort.hpp"
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>

namespace Cockpit {
    class SerialManager {
    public:
        SerialPort *mcp_port; // MCP Serial Port Connection
        // TODO: Add more ports for different hardware items.
        
        SerialManager();
        void updateDevices();
        std::vector<SerialPort> *getDevices();
        
    private:
        std::vector<SerialPort> m_serial_devices;
    };
    
} // Namespace Cockpit.

#endif /* SerialManager_hpp */
