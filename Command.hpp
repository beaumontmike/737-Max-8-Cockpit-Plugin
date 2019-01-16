//
//  Command.hpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-13.
//

#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include <string>
#include "XPLMUtilities.h"

namespace Cockpit {
    class Command {
    public:
        Command();
        Command(std::string, std::string);
        
        bool parse(std::string);
        
    private:
        XPLMCommandRef  m_xp_command;
        std::string     m_serial_command;
        std::string     m_xp_command_string;
    };
} // namespace Cockpit

#endif /* Command_hpp */
