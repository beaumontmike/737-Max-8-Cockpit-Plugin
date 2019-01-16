//
//  DataRef.cpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-13.
//

#include "DataRef.hpp"

namespace Cockpit {
    IntegerDataRef::IntegerDataRef() {
        return;
    }

    IntegerDataRef::IntegerDataRef(std::string t_ref_name, SerialPort *t_port, std::string t_cmd_id) {
        this->p_xp_dataref = XPLMFindDataRef(t_ref_name.c_str());
        this->m_serial_port = t_port;
        this->m_command_identifier = t_cmd_id;
    }

    void IntegerDataRef::updatePanel(bool t_override_change_check) {
        // Check for null reference and return if null
        if(this->p_xp_dataref == NULL) {
            m_current_status = 0;
            return;
        }
        // Get current data
        this->m_current_status = XPLMGetDatai(this->p_xp_dataref);
        
        // If same as last update return from function;
        if(this->m_current_status == this->m_last_status && !t_override_change_check) return;
        
        // If different
        this->m_current_status == 1 ? // ternary operator.
            this->m_serial_port->writeData(this->m_command_identifier + "_ON") :
            this->m_serial_port->writeData(this->m_command_identifier + "_OFF");
        this->m_last_status = this->m_current_status;
    }
} //namespace Cockpit
