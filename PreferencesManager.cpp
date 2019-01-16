//
//  PreferencesManager.cpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-13.
//

#include "PreferencesManager.hpp"

namespace Cockpit {

    // Static var forward declares.
    XPWidgetID              PreferencesManager::m_window;
    int                     PreferencesManager::m_menu_container_index;
    XPLMMenuID              PreferencesManager::m_menu_id;
    SerialManager           *PreferencesManager::m_serial_manager;

    void PreferencesManager::init(SerialManager *t_serial_manager) {
        m_serial_manager = t_serial_manager;
        XPLMEnableFeature("XPLM_USE_NATIVE_WIDGET_WINDOWS", 1);
        PreferencesManager::setupMenu();
    }

    void PreferencesManager::deinit() {
        XPLMDestroyMenu(PreferencesManager::m_menu_id);
        XPDestroyWidget(m_window, 1);
        PreferencesManager::m_window = NULL;
    }
    
    // Not called in init until fully implemented.
    void PreferencesManager::loadFromFile() {
        char system_path[512];
        XPLMGetSystemPath(system_path);
        const char *sep = XPLMGetDirectorySeparator();
        char prefs_filename[512];
        sprintf(prefs_filename, "%s%sResources%splugins%s737_Cockpit_Plugin%sprefs.txt", system_path, sep, sep, sep, sep);
        
        int prefs_file = open(prefs_filename, O_RDWR);
        
        fd_set set;
        struct timeval timeout;
        
        __DARWIN_FD_ZERO(&set);
        __DARWIN_FD_SET(prefs_file, &set);
        
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        
        while(select(__DARWIN_FD_SETSIZE, &set, NULL, NULL, &timeout) == 1) {
            char line_buffer[512];
            size_t bytes_read = read(prefs_file, &line_buffer, 512);
            
        }
        
        
    }

    void PreferencesManager::setupMenu() {
        PreferencesManager::m_menu_container_index = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "737 Cockpit", 0, 0);
        PreferencesManager::m_menu_id = XPLMCreateMenu("737 Cockpit", XPLMFindPluginsMenu(),
                                                       PreferencesManager::m_menu_container_index,
                                                       PreferencesManager::menuHandler, NULL);
        XPLMAppendMenuItem(PreferencesManager::m_menu_id, "Preferences", (void *)"MNU_OPT_PREFS", 1);
        XPLMAppendMenuSeparator(PreferencesManager::m_menu_id);
        XPLMAppendMenuItem(PreferencesManager::m_menu_id, "Reset Panels", (void *)"MNU_OPT_RESET", 1);
        
    }

    void PreferencesManager::menuHandler(void *t_inMenuRef, void *t_inItemRef) {
        if(!strcmp((const char *)t_inItemRef, "MNU_OPT_PREFS")) {
            createWindow();
        } else if (!strcmp((const char *)t_inItemRef, "MNU_OPT_RESET")) {
            ; // TODO: Implement Reset
        }
    }

    void PreferencesManager::createWindow() {
        int width, height;
        XPLMGetScreenSize(&width, &height);
        
        PreferencesManager::m_window = XPCreateWidget(100, height - 100, width - 100, 100, 0, "737 Cockpit - Preferences", 1, NULL, xpWidgetClass_MainWindow);
        XPSetWidgetProperty(m_window, xpProperty_MainWindowHasCloseBoxes, 1);
        XPAddWidgetCallback(m_window, PreferencesManager::widgetMessage);
        XPShowWidget(m_window);
        
        int left, top, right, bottom;
        XPGetWidgetGeometry(m_window, &left, &top, &right, &bottom);
        
        
        auto inc(0), offset(300);
        
        for(auto device : *m_serial_manager->getDevices()) {
            auto offset_computed = inc * offset;
            XPWidgetID button = XPCreateWidget(left + offset_computed + 10, top - 50, left + offset_computed + offset, top - 10, 0, device.getName().c_str(), 0, m_window, xpWidgetClass_Button);
            XPSetWidgetProperty(button, xpProperty_ButtonType, xpRadioButton);
            XPSetWidgetProperty(button, xpProperty_ButtonBehavior, xpButtonBehaviorRadioButton);
            XPShowWidget(button);
            ++inc;
        }
        
        XPLMWindowID underlying_window = XPGetWidgetUnderlyingWindow(PreferencesManager::m_window);
        XPLMSetWindowPositioningMode(underlying_window, xplm_WindowPositionFree, -1);
        XPLMSetWindowGravity(underlying_window, 0, 1, 1, 1);
    }

    int PreferencesManager::widgetMessage(XPWidgetMessage  inMessage, XPWidgetID  inWidget, long  inParam1, long  inParam2)
    {
        if (inMessage == xpMessage_CloseButtonPushed)
        {
            XPHideWidget(m_window);
            return 1;
        }
        
        return 0;
    }
    
} // namespace Cockpit
