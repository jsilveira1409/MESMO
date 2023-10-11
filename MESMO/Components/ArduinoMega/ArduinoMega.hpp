// ======================================================================
// \title  ArduinoMega.hpp
// \author jsilveira
// \brief  hpp file for ArduinoMega component implementation class
// ======================================================================

#ifndef ArduinoMega_HPP
#define ArduinoMega_HPP

#include "Components/ArduinoMega/ArduinoMegaComponentAc.hpp"

namespace Components {

  class ArduinoMega :
    public ArduinoMegaComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ArduinoMega
      //!
      ArduinoMega(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object ArduinoMega
      //!
      ~ArduinoMega();

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
          Components::ArduinoMega_Commands payloadcommand 
      );


    };

} // end namespace Components

#endif
