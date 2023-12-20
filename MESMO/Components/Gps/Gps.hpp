// ======================================================================
// \title  Gps.hpp
// \author jsilveira
// \brief  hpp file for Gps component implementation class
// ======================================================================

#ifndef Gps_HPP
#define Gps_HPP

#include "Components/Gps/GpsComponentAc.hpp"

#define GPS_DATA_LENGTH 1024    //bytes


namespace Components {

  class Gps :
    public GpsComponentBase
  {

    /**
       * GpsPacket:
       *   A structure containing the information in the GPS location packet
       * received via the NMEA GPS receiver.
       */
      struct GpsPacket {
          float utcTime;           // 1) Time (UTC)
          float latitude;          // 2) Latitude (in ddmm.mm format)
          char northSouth;         // 3) N or S (North or South)
          float longitude;         // 4) Longitude (in dddmm.mm format)
          char eastWest;           // 5) E or W (East or West)
          unsigned int gpsQuality; // 6) GPS Quality Indicator
          unsigned int numSatellites; // 7) Number of satellites in view
          float hdop;              // 8) Horizontal Dilution of Precision
          float altitude;          // 9) Antenna Altitude above/below mean-sea-level (geoid)
          char altitudeUnits;      // 10) Units of antenna altitude
          float geoidalSeparation; // 11) Geoidal separation
          char geoidalUnits;       // 12) Units of geoidal separation
          float dgpsDataAge;       // 13) Age of differential GPS data
          unsigned int dgpsStationId; // 14) Differential reference station ID
      };



    public:


      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Gps
      //!
      Gps(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object Gps
      //!
      ~Gps();


    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for ready
      //!
      void ready_handler(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //! Handler implementation for recv
      //!
      void recv_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &recvBuffer, 
          const Drv::RecvStatus &recvStatus 
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------


      //!< Has the device acquired GPS lock?
      bool m_locked;
      //!< Is the device ready ? 
      bool m_ready;
      //!< Create member variables to store buffers and the data array that those buffers use for storage
      char m_uartBuffers[GPS_DATA_LENGTH];
      U16 m_recvSize = 0;

    };

} // end namespace Components

#endif
