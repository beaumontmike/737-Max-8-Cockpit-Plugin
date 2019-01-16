//
//  SerialManager.cpp
//  737_Cockpit_Plugin
//
//  Created by Michael Beaumont on 2019-01-07.
//

#include "SerialManager.hpp"

namespace Cockpit {

    SerialManager::SerialManager() {
        this->updateDevices();
    }

    std::vector<SerialPort>* SerialManager::getDevices() {
        return &this->m_serial_devices;
    }

    void SerialManager::updateDevices() {
        // Get /dev directory pointer
        DIR *dp;
        struct dirent *dirp;
        
        if((dp = opendir("/dev")) == NULL) {
            printf("Error: %d", errno);
        }
        
        // Add all files to a temp vector and close the pointer.
        std::vector<std::string> all_files;
        while((dirp = readdir(dp)) != NULL) {
            all_files.push_back(std::string(dirp->d_name));
        }
        closedir(dp);
        
        // Loop files and find 'cu.usbmodem' devices; add them to serial devices list vector.
        for(int inx = 0; inx <= all_files.size() - 1; inx++) {
            if(all_files[inx].find("cu.usbmodem") != std::string::npos || all_files[inx].find("cu.usbserial") != std::string::npos) {
                m_serial_devices.push_back(SerialPort("/dev/" + all_files[inx]));
            }
        }
        
        printf("MCP: Serial Devices Returned: %d \n",  (int)this->m_serial_devices.size());
        
        // Loop through device list and query for the device name; store it in the SerialPort's deviceName property.
        for (int inx = 0; inx < this->m_serial_devices.size(); inx++) {
            this->m_serial_devices[inx].openPort();
            printf("MCP: Getting Device Name... \n");
            
            while(!this->m_serial_devices[inx].writeable()) {
                usleep(1000);
            }
            this->m_serial_devices[inx].writeData("QUERY_DEVICE_NAME");
            
//            int wait_for_data_loops = 0;
//            while(!this->m_serial_devices[inx].hasData() && wait_for_data_loops <= 5) {
//                usleep(1000);
//                ++wait_for_data_loops;
//            }
            
            usleep(1000);
            if(this->m_serial_devices[inx].hasData()) {
                std::string dev_name = this->m_serial_devices[inx].readData();
                this->m_serial_devices[inx].setName(dev_name);
                printf("MCP: Device Name Returned: %s \n", dev_name.c_str());
            }
            
            this->m_serial_devices[inx].closePort();
            mcp_port = &m_serial_devices[inx];
    
        }
    }
} // namespace Cockpit
