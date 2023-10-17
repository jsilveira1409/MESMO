// ======================================================================
// \title  SubsystemDeframer.hpp
// \author jsilveira
// \brief  hpp file for SubsystemDeframer component implementation class
// ======================================================================

#ifndef SubsystemDeframer_HPP
#define SubsystemDeframer_HPP

#include "Components/SubsystemDeframer/SubsystemDeframerComponentAc.hpp"
#include <Svc/FramingProtocol/DeframingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocolInterface.hpp>
#include <Utils/Types/CircularBuffer.hpp>
#include <config/DeframerCfg.hpp>

namespace Components {

  class SubsystemDeframer :
    public SubsystemDeframerComponentBase,
    public Svc::DeframingProtocolInterface

  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object SubsystemDeframer
      //!
      SubsystemDeframer(
          const char *const compName /*!< The component name*/
      );

      //! Initialize Deframer instance
      void init(
          const NATIVE_INT_TYPE instance = 0 //!< The instance number
      );

      //! Destroy object SubsystemDeframer
      //!
      ~SubsystemDeframer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

     //! Handler implementation for cmdResponseIn
      //!
      void cmdResponseIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          FwOpcodeType opCode, /*!< Command Op Code */
          U32 cmdSeq, /*!< Command Sequence */
          const Fw::CmdResponse &response /*!< The command response argument*/
      );

      //! Handler implementation for framedIn
      //!
      void framedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &recvBuffer, 
          const Drv::RecvStatus &recvStatus 
      );

      //! Handler implementation for schedIn
      //!
      void schedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< The call order
      */
      );

      // ----------------------------------------------------------------------
      // Implementation of DeframingProtocolInterface
      // ----------------------------------------------------------------------
    
      //! The implementation of DeframingProtocolInterface::route
      //! Send a data packet
      void route(
          Fw::Buffer& packetBuffer //!< The packet buffer
      );

      //! The implementation of DeframingProtocolInterface::allocate
      //! Allocate a packet buffer
      //! \return The packet buffer
      Fw::Buffer allocate(
          const U32 size //!< The number of bytes to request
      );

      //Svc::DeframingProtocol::DeframingStatus routeToFiles(Types::CircularBuffer& ring, U32 size);
      
      //Svc::DeframingProtocol::DeframingStatus routeToSwPayload(Types::CircularBuffer& ring, U32 size);


      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Copy data from an incoming frame buffer into the internal
      //! circular buffer
      void processBuffer(
          Fw::Buffer& buffer //!< The frame buffer
      );

      void sendStartPacket();
      void sendDataPacket(U8 *const bufferData, const U32 bufferSize);
      void sendEndPacket();


      // ----------------------------------------------------------------------
      // Member variables
      // ----------------------------------------------------------------------

      //! The Expected size of the incoming bytestream, which can come in multiple packets, in bytes
    U32 m_expectedSize;
    // the size of the incoming bytestream, in bytes, that we already received and processed
    U32 m_receivedSize;

      //! The DeframingProtocol implementation
      Svc::DeframingProtocol* m_protocol;

      //! The circular buffer
      //Types::CircularBuffer m_inRing;
      U8 m_buffer[4096] = {0};

      U32 m_pktIndex = 0;

      U32 m_fileIndex = 0;

    };

} // end namespace Components

#endif