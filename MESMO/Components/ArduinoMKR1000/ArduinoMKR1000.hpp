// ======================================================================
// \title  ArduinoMKR1000.hpp
// \author jsilveira
// \brief  hpp file for ArduinoMKR1000 component implementation class
// ======================================================================

#ifndef ArduinoMKR1000_HPP
#define ArduinoMKR1000_HPP

#include "Components/ArduinoMKR1000/ArduinoMKR1000ComponentAc.hpp"

namespace Components {

  // Component Implementation Class
  class ArduinoMKR1000 :
    public ArduinoMKR1000ComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ArduinoMKR1000
      //!
      ArduinoMKR1000(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object ArduinoMKR1000
      //!
      ~ArduinoMKR1000();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for PktRecv
      //!
      void PktRecv_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &recvBuffer, 
          const Drv::RecvStatus &recvStatus 
      );

      //! Handler implementation for Run
      //!
      void Run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );

      //! Handler implementation for bufferSendIn
      //!
      void bufferSendIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for SendString command handler
      //! Command to send a string to the Payload
      void SendString_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          const Fw::CmdStringArg& text /*!< 
          String to send
          */
      );

      //! Implementation for SendCommand command handler
      //! Command to send a defined command to the Payload
      void SendCommand_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Components::ArduinoMKR1000_Commands payloadcommand 
      );


    };

} // end namespace Components

#endif
