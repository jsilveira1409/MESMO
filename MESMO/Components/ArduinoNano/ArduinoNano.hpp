// ======================================================================
// \title  ArduinoNano.hpp
// \author jsilveira
// \brief  hpp file for ArduinoNano component implementation class
// ======================================================================

#ifndef ArduinoNano_HPP
#define ArduinoNano_HPP

#include "Components/ArduinoNano/ArduinoNanoComponentAc.hpp"

namespace Components {

  class ArduinoNano :
    public ArduinoNanoComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ArduinoNano
      //!
      ArduinoNano(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object ArduinoNano
      //!
      ~ArduinoNano();

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

      //! Implementation for SendCommand command handler
      //! Command to send a defined command to the Payload
      void SendCommand_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Components::ArduinoNano_Commands payloadcommand 
      );


    };

} // end namespace Components

#endif
