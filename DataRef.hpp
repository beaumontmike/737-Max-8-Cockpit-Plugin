//
//  DataRef.hpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-13.
//

#ifndef DataRef_hpp
#define DataRef_hpp

#include <stdio.h>
#include <string>
#include "XPLMDataAccess.h"
#include "SerialPort.hpp"

namespace Cockpit {
    class DataRef {
    protected:
        XPLMDataRef     p_xp_dataref;
    }; // class DataRef

    class IntegerDataRef : DataRef {
    public:
        IntegerDataRef();
        IntegerDataRef(std::string t_ref_name, SerialPort *t_port, std::string t_cmd_ident);
        
        void updatePanel(bool t_override_change_check);
        
    private:
        SerialPort      *m_serial_port;
        int             m_last_status = 0;
        int             m_current_status = 0;
        std::string     m_command_identifier;
    }; // struct IntegerDataRef
} // namespace Cockpit

#endif /* DataRef_hpp */
