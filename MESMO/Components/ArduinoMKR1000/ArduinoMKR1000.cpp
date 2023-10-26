// ======================================================================
// \title  ArduinoMKR1000.cpp
// \author jsilveira
// \brief  cpp file for ArduinoMKR1000 component implementation class
// ======================================================================


#include <Components/ArduinoMKR1000/ArduinoMKR1000.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ArduinoMKR1000 ::
    ArduinoMKR1000(
        const char *const compName
    ) : ArduinoMKR1000ComponentBase(compName)
  {

  }

  ArduinoMKR1000 ::
    ~ArduinoMKR1000()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ArduinoMKR1000 ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  void ArduinoMKR1000 ::
    bufferSendIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    // here we transform the buffer data into the sensor telemetry
    const U8* data = fwBuffer.getData();
    //print data
    this->tlmWrite_LED1State((bool)data[0]);
    this->tlmWrite_LED2State((bool)data[1]);
    this->tlmWrite_LED3State((bool)data[2]);
    this->tlmWrite_pirState((bool)data[3]);
    this->tlmWrite_ldrVal((U16)(data[4] << 8 | data[5]));
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void ArduinoMKR1000 ::
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

    this->PktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void ArduinoMKR1000 ::
    SendCommand_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoMKR1000_Commands payloadcommand
    ){
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

