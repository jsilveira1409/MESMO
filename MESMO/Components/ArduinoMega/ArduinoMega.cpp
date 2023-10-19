// ======================================================================
// \title  ArduinoMega.cpp
// \author jsilveira
// \brief  cpp file for ArduinoMega component implementation class
// ======================================================================


#include <Components/ArduinoMega/ArduinoMega.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ArduinoMega ::
    ArduinoMega(
        const char *const compName
    ) : ArduinoMegaComponentBase(compName)
  {

  }

  ArduinoMega ::
    ~ArduinoMega()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

    void ArduinoMega ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  void ArduinoMega ::
    bufferSendIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    // print out the data in ascii
    U8* data = (U8*)fwBuffer.getData();
    printf("Received: %c\n", data[0]);
    for (U32 i = 0; i < fwBuffer.getSize(); i++) {
      printf("%c", data[i]);
    }
    printf("\n");
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void ArduinoMega ::
    SendString_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& text
    )
  {
    // TODO
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void ArduinoMega ::
    MoveServo_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoMega_MoveCommands cmd,
        U8 angle
    )
  {
    const U32 size =2;
    U8 data[2] = {cmd.e, angle};
    Fw::ComBuffer arg(data, size);
    
    this->PktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void ArduinoMega ::
    ConfigureServoSpeed_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoMega_ConfigCommands cmd,
        U8 speed
    )
  {

    const U32 size = 2;
    U8 data[2] = {cmd.e, speed};
    Fw::ComBuffer arg(data, size);
    
    this->PktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Components
