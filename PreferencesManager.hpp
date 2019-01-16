//
//  PreferencesManager.hpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-13.
//

#ifndef PreferencesManager_hpp
#define PreferencesManager_hpp

#include <string>
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMMenus.h"
#include "XPLMPlugin.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"
#include "SerialManager.hpp"

namespace Cockpit {

    class PreferencesManager {
    public:
        static void                             init(SerialManager *t_serial_manager);
        static void                             deinit();
    private:
        // Variables
        static XPWidgetID                       m_window;
        static int                              m_menu_container_index;
        static XPLMMenuID                       m_menu_id;
        static SerialManager                    *m_serial_manager;
        
        // Window Functions
        static int                              widgetMessage(XPWidgetMessage  inMessage, XPWidgetID  inWidget, long  inParam1, long  inParam2);
        
        // Menu Functions
        static void                             menuHandler(void * t_inMenuRef, void * t_inItemRef);
        
        // Init functions
        static void                             createWindow();
        static void                             setupMenu();
        static void                             loadFromFile();
    };
    
} // namespace Cockpit

#endif /* PreferencesManager_hpp */

