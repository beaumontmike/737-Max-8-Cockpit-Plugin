//
//  Command.cpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-13.
//

#include "Command.hpp"

namespace Cockpit {
    Command::Command() {
        return;
    }

    Command::Command(std::string t_xp_command_string, std::string t_serial_cmd) {
        this->m_xp_command = XPLMFindCommand(t_xp_command_string.c_str());
        this->m_serial_command = t_serial_cmd;
        this->m_xp_command_string = t_xp_command_string;
    }

    bool Command::parse(std::string t_cmd) {
        if(this->m_xp_command == NULL) {
            this->m_xp_command = XPLMFindCommand(this->m_xp_command_string.c_str());
        }
        
        if(this->m_serial_command == t_cmd) {
            XPLMCommandOnce(this->m_xp_command);
            return true;
        }
        return false;
    }
} //namespace Cockpit
