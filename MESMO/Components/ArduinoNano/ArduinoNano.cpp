// ======================================================================
// \title  ArduinoNano.cpp
// \author jsilveira
// \brief  cpp file for ArduinoNano component implementation class
// ======================================================================


#include <Components/ArduinoNano/ArduinoNano.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ArduinoNano ::
    ArduinoNano(
        const char *const compName
    ) : ArduinoNanoComponentBase(compName)
  {

  }

  ArduinoNano ::
    ~ArduinoNano()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ArduinoNano ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  void ArduinoNano ::
    bufferSendIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    const U8* data = fwBuffer.getData();

  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void ArduinoNano ::
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

    this->PktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void ArduinoNano ::
    SendCommand_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoNano_Commands payloadcommand
    )
  {
    const U32 size = sizeof(payloadcommand.e);
    U8 data[size];
    data[0] = (U8) (payloadcommand.e >> 24);
    data[1] = (U8) (payloadcommand.e >> 16);
    data[2] = (U8) (payloadcommand.e >> 8);
    data[3] = (U8) payloadcommand.e;

    
    Fw::ComBuffer arg(data, size);
    
    this->PktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Components
