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
    ready_handler(
        const NATIVE_INT_TYPE portNum
    )
    {
      // TODO
    }

    void ArduinoNano ::
      recv_handler(
          const NATIVE_INT_TYPE portNum,
          Fw::Buffer &recvBuffer,
          const Drv::RecvStatus &recvStatus
      )
    {
      printf("Received: %x\n", recvBuffer.getData()[0]);
      this->deallocate_out(portNum, recvBuffer);
    }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  

  void ArduinoNano ::
    SendCommand_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoNano_Commands payloadcommand
    )
  {
    const U32 size = 2;
    printf("Command: %x\n", payloadcommand.e);
    U8 data[size] = {0x00, payloadcommand.e};
    Fw::Buffer buff(data, size);
  
    this->send_out(0, buff);
    this->tlmWrite_ldrVal(payloadcommand.e);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);  

  }

} // end namespace Components
