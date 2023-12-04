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
    enum Move_t {
      LEFT = 1,
      RIGHT,
      UP,
      DOWN,
      NONE = 0xff
    };

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

      void set_comm();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

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

      //! Handler implementation for move
      //!
      void move_handler(
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

      //! Implementation for MoveServo command handler
      //! Command to send a defined command to the Payload
      void MoveServo_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Components::ArduinoMega_MoveCommands cmd, 
          U8 angle 
      );

      //! Implementation for ConfigureServoSpeed command handler
      //! Command to send a defined command to the Payload
      void ConfigureServoSpeed_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Components::ArduinoMega_ConfigCommands cmd, 
          U8 speed 
      );

      

      U8 shoulder_angle;
      U8 elbow_angle;
      U8 forearm_angle;
      U8 wrist_angle;
      Move_t move_cmd = Move_t::NONE;    
      bool comm_open = false;   
    };

} // end namespace Components

#endif
