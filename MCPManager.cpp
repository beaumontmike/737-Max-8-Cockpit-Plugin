//
//  MCP Manager.cpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-10.
//

#include "MCPManager.hpp"

namespace Cockpit {
    
    MCPManager::MCPManager() {
        return;
    }

    MCPManager::MCPManager(SerialPort *t_port) {
        this->m_serial_port = t_port;
    }

    void MCPManager::serialBegin() {
        this->m_serial_port->openPort();
    }

    float MCPManager::deferredInit() {
        // Init Commands
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/flight_director_toggle",       "AP_FD_CP_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/flight_director_fo_toggle",    "AP_FD_FO_PRESS"));
        
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/autothrottle_arm_toggle",      "AP_ATH_ARM_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/speed_press",                  "AP_ATH_SPD_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/n1_press",                     "AP_ATH_N1_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/lvl_chg_press",                "AP_ATH_FLC_PRESS"));
        
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/hdg_sel_press",                "AP_HDG_SEL_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/lnav_press",                   "AP_HDG_LNAV_PRESS"));

        this->m_xp_commands.push_back(Command("sim/autopilot/altitude_hold",                         "AP_ALT_HLD_PRESS"));
        this->m_xp_commands.push_back(Command("sim/autopilot/alt_vs",                                "AP_ALT_VSPD_PRESS"));
        
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/cmd_a_press",                  "AP_CMD_A_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/cmd_b_press",                  "AP_CMD_B_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/cws_a_press",                  "AP_CWS_A_PRESS"));
        this->m_xp_commands.push_back(Command("laminar/B738/autopilot/cws_b_press",                  "AP_CWS_B_PRESS"));
        
        // Init Integer Datarefs
        this->m_xp_int_datarefs.push_back(IntegerDataRef("laminar/B738/autopilot/cmd_a_status", this->m_serial_port, "AP_CMD_A"));
        this->m_xp_int_datarefs.push_back(IntegerDataRef("laminar/B738/autopilot/cmd_b_status", this->m_serial_port,"AP_CMD_B"));
        this->m_xp_int_datarefs.push_back(IntegerDataRef("laminar/B738/autopilot/hdg_sel_status", this->m_serial_port,"AP_HDG_SEL"));
        
        this->writeFlightLoop(true);
        return 0;
    }

    float MCPManager::readFlightLoop() {
        if(this->m_serial_port->hasData()) {
            // Reading data from serial device.
            std::string data(this->m_serial_port->readData());
            // Loop through commands and look for a match, break loop if found.
            for(int inx = 0; inx < this->m_xp_commands.size(); ++inx) {
                if(this->m_xp_commands[inx].parse(data)) break;
            }
        }
        return -1; // Do every flightLoop call.
    };

    float MCPManager::writeFlightLoop(bool t_override_change_check) {
        if(this->m_serial_port->writeable()) {
            // Loop through datarefs, which will update the panel if necessary.
            for(int inx = 0; inx < this->m_xp_int_datarefs.size(); ++inx) {
                this->m_xp_int_datarefs[inx].updatePanel(t_override_change_check);
            }
            
        }
        return -1;
    }
    
} // namespace Cockpit
