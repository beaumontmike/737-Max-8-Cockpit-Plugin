//
//  MCP Manager.hpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-10.
//

#ifndef MCP_Manager_hpp
#define MCP_Manager_hpp

#include "XPLMMenus.h"
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "SerialPort.hpp"
#include "SerialManager.hpp"
#include "DataRef.hpp"
#include "Command.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

namespace Cockpit {
    class MCPManager {
    public:
        MCPManager();
        MCPManager(SerialPort *port);
        
        void    serialBegin();
        float   deferredInit();
        float   writeFlightLoop(bool t_override_change_check = false);
        float   readFlightLoop();
        
    private:
        SerialPort                      *m_serial_port;
        std::vector<IntegerDataRef>     m_xp_int_datarefs;
        std::vector<Command>            m_xp_commands;

    };
} // Namespace Cockpit.

#endif /* MCP_Manager_hpp */
