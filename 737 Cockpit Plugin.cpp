
#include "XPLMMenus.h"
#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
#include "XPLMUtilities.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"

#include "SerialPort.hpp"
#include "SerialManager.hpp"
#include "MCPManager.hpp"
#include "PreferencesManager.hpp"

#include <errno.h>
#include <vector>
#include <string>
#include <memory>

#if IBM
	#include <windows.h>
#endif
#if LIN
	#include <GL/gl.h>
#elif __GNUC__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif

namespace Cockpit {
    std::unique_ptr<SerialManager> g_serial_manager;
    std::unique_ptr<MCPManager> g_mcp_manager;

    float deferred_init(float, float, int, void*);
    float write_flight_loop(float, float, int, void*);
    float read_flight_Loop(float, float, int, void*);

    PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc) {
        strcpy(outName, "737 Cockpit Plugin");
        strcpy(outSig, "mtb.Cockpit.737");
        strcpy(outDesc, "An interface for a 737 Hardware Cockpit.");
        
        PreferencesManager::init(g_serial_manager.get());
        
        // Initialize SerialManager and return if no devices available.
        g_serial_manager = std::unique_ptr<SerialManager>(new SerialManager());
        if(g_serial_manager->getDevices()->size() <= 0) return 1;
        
        g_mcp_manager = std::unique_ptr<MCPManager>(new MCPManager(g_serial_manager->mcp_port));
        g_mcp_manager->serialBegin();
        
        
        
        XPLMRegisterFlightLoopCallback(deferred_init, 1, 0);
        XPLMRegisterFlightLoopCallback(write_flight_loop, 1, 0);
        XPLMRegisterFlightLoopCallback(read_flight_Loop, -10, 0);
        
        return 1;
    }

    PLUGIN_API void	XPluginStop(void) {
        XPLMUnregisterFlightLoopCallback(write_flight_loop, 0);
        XPLMUnregisterFlightLoopCallback(read_flight_Loop, 0);
        XPLMUnregisterFlightLoopCallback(deferred_init, 0);
    }

    PLUGIN_API void XPluginDisable(void) { }
    PLUGIN_API int  XPluginEnable(void)  { return 1; }
    PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }

    float deferred_init(float                inElapsedSinceLastCall,
                           float                inElapsedTimeSinceLastFlightLoop,
                           int                  inCounter,
                           void *               inRefcon) {
        return g_mcp_manager->deferredInit();
    }

    float write_flight_loop(float                inElapsedSinceLastCall,
                           float                inElapsedTimeSinceLastFlightLoop,
                           int                  inCounter,
                           void *               inRefcon) {
        return g_mcp_manager->writeFlightLoop();
    }

    float read_flight_Loop(float                inElapsedSinceLastCall,
                          float                inElapsedTimeSinceLastFlightLoop,
                          int                  inCounter,
                          void *               inRefcon) {
        return g_mcp_manager->readFlightLoop();
    }
} // namespace Cockpit


