// ======================================================================
// \title  Camera.cpp
// \author jsilveira
// \brief  cpp file for Camera component implementation class
// ======================================================================


#include <Components/Camera/Camera.hpp>
#include <FpConfig.hpp>
#include <Fw/Types/String.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  Camera ::
    Camera(
        const char *const compName
    ) : CameraComponentBase(compName)
  {

  }

  Camera ::
    ~Camera()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Camera ::
    PktRecv_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &recvBuffer,
        const Drv::RecvStatus &recvStatus
    )
  {
    // TODO
  }

  void Camera ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  void Camera ::
    bufferSendIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    // check if first chars are "center"
    U8* data = (U8*)fwBuffer.getData();
    U32 size = fwBuffer.getSize();
    Fw::String str((char*)data);
    if (size > 6) {
      if (str == "center") {
        U32 x = (U32) data[6] << 24 | data[7] << 16 | data[8] << 8 | data[9];
        U32 y = (U32) data[10] << 24 | data[11] << 16 | data[12] << 8 | data[13];
        printf("x: %d, y: %d\n", x, y);
      }
    }
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void Camera ::
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

  void Camera ::
    SendCommand_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::Camera_Commands payloadcommand
    )
  {
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
