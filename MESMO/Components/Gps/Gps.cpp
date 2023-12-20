// ======================================================================
// \title  Gps.cpp
// \author jsilveira
// \brief  cpp file for Gps component implementation class
// ======================================================================


#include <Components/Gps/Gps.hpp>
#include <FpConfig.hpp>
#include "Fw/Logger/Logger.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  Gps ::Gps(const char *const compName) : GpsComponentBase(compName){
      // Initialize the lock status to false
      m_locked = false;
      m_ready = false;
      memset(this->m_uartBuffers, 0, sizeof(this->m_uartBuffers));
  }


  Gps ::
    ~Gps()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Gps ::
    ready_handler(
        const NATIVE_INT_TYPE portNum
    )
  {
    this->m_ready = true;
    printf("Gps::ready_handler\n");
  }

  void Gps ::recv_handler(const NATIVE_INT_TYPE portNum,Fw::Buffer &recvBuffer,const Drv::RecvStatus &recvStatus){
      int status = 0;
      U32 buffsize = recvBuffer.getSize();
      char* ptr = reinterpret_cast<char*>(recvBuffer.getData());
      GpsPacket packet;

      if (recvStatus != Drv::RecvStatus::RECV_OK) {
          Fw::Logger::logMsg("[WARNING] Received buffer with bad packet: %d\n", recvStatus);
          this->deallocate_out(0, recvBuffer);
          return;
      }
      // append to recv buffer
      if (this->m_recvSize < GPS_DATA_LENGTH){
        // copy data to buffer
        memcpy(&this->m_uartBuffers[this->m_recvSize], ptr, buffsize);
        this->m_recvSize += buffsize;
        this->m_uartBuffers[this->m_recvSize] = '\0';
      } else {
        
        float time, latitude, longitude, altitude;
        char ns, ew;
        int quality, satellites;
        float hdop, geoidheight;
        ptr = this->m_uartBuffers;

        while ((ptr = strstr(ptr, "$GPGGA")) != NULL) {
          
          int parsed = sscanf(ptr, "$GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f,M,%f,M",
                              &time, &latitude, &ns, &longitude, &ew, &quality, &satellites, &hdop, &altitude, &geoidheight);
          if (parsed == 10) {
              //printf("Time: %f, Latitude: %f%c, Longitude: %f%c, Quality: %d, Satellites: %d, HDOP: %f, Altitude: %f, GeoidHeight: %f\n",
              //      time, latitude, ns, longitude, ew, quality, satellites, hdop, altitude, geoidheight);
              break;
          }
          ptr++;  // Move to the next character to avoid infinite loop
        
        }
        //printf("Time: %f, Latitude: %f%c, Longitude: %f%c, Quality: %d, Satellites: %d, HDOP: %f, Altitude: %f, GeoidHeight: %f\n\n",
        //            time, latitude, ns, longitude, ew, quality, satellites, hdop, altitude, geoidheight);
        this->m_recvSize = 0;
        
        // set the packet data
        packet.utcTime = time;
        packet.latitude = latitude;
        packet.northSouth = ns;
        packet.longitude = longitude;
        packet.eastWest = ew;
        packet.gpsQuality = quality;
        packet.numSatellites = satellites;
        packet.hdop = hdop;
        packet.altitude = altitude;        
        packet.geoidalSeparation  = geoidheight;
        
        float lat_deg = (int)(packet.latitude / 100.0f);  // Extracting degrees from ddmm.mm format
        float lat_min = packet.latitude - (lat_deg * 100.0f);  // Extracting minutes from ddmm.mm format
        float lat = lat_deg + lat_min / 60.0f;  // Converting to decimal format
        lat = lat * ((packet.northSouth == 'N') ? 1 : -1);  // Applying North/South orientation

        float lon_deg = (int)(packet.longitude / 100.0f);  // Extracting degrees from dddmm.mm format
        float lon_min = packet.longitude - (lon_deg * 100.0f);  // Extracting minutes from dddmm.mm format
        float lon = lon_deg + lon_min / 60.0f;  // Converting to decimal format
        lon = lon * ((packet.eastWest == 'E') ? 1 : -1);  // Applying East/West orientation

        this->tlmWrite_Latitude(lat);
        this->tlmWrite_Longitude(lon);
        this->tlmWrite_Altitude(packet.altitude);
        this->tlmWrite_GpsCount(packet.numSatellites);

        if (packet.gpsQuality == 0 && m_locked) {
            m_locked = false;
            this->log_ACTIVITY_HI_LockLost();
        } else if (packet.gpsQuality >= 1 && !m_locked) {
            m_locked = true;
            this->log_ACTIVITY_HI_LockAcquired();
        }

      }
      this->deallocate_out(0, recvBuffer);
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

} // end namespace Components
