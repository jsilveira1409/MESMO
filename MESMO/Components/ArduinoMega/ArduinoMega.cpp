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
    PktRecv_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &recvBuffer,
        const Drv::RecvStatus &recvStatus
    )
  {
    // TODO
  }

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
    // TODO
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
    Fw::ComBuffer arg;
    U8* textPtr = (U8*)text.toChar();
    U32 size = text.getCapacity();
    arg = Fw::ComBuffer(textPtr, size);
    //U32 size = arg.getBuffLength();

    this->HwPktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void ArduinoMega ::
    SendCommand_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoMega_Commands payloadcommand
    ){
    const U32 size = sizeof(payloadcommand.e);
    U8 data[size];
    data[0] = (U8) (payloadcommand.e >> 24);
    data[1] = (U8) (payloadcommand.e >> 16);
    data[2] = (U8) (payloadcommand.e >> 8);
    data[3] = (U8) payloadcommand.e;

    
    Fw::ComBuffer arg(data, size);
    
    this->HwPktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Components
