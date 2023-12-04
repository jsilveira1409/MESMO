// ======================================================================
// \title  MYO.cpp
// \author jsilveira
// \brief  cpp file for MYO component implementation class
// ======================================================================


#include <Components/MYO/MYO.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  MYO ::
    MYO(
        const char *const compName
    ) : MYOComponentBase(compName)
  {

  }

  MYO ::
    ~MYO()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------
 void MYO ::
    cmdResp_handler(
        const NATIVE_INT_TYPE portNum,
        FwOpcodeType opCode,
        U32 cmdSeq,
        const Fw::CmdResponse &response
    )
  {
    // TODO
  }

  void MYO::bufferSendIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer &fwBuffer)
  {
    
    const U32 size = fwBuffer.getSize();
    U8 data[size + 1]; // +1 for the null-terminating character

    for (U32 i = 0; i < size; i++){
      data[i] = fwBuffer.getData()[i];
    }
    data[size] = '\0'; // null-terminating character
    this->log_ACTIVITY_HI_NewPose((char *)data);
    this->tlmWrite_Pose((char *)data);

    Fw::Buffer buffer;

    if (strcmp((char *)data, "FINGERS_SPREAD") == 0)
    {
      buffer.setData((U8 *)"UP");
      buffer.setSize(2);
    }
    else if (strcmp((char *)data, "FIST") == 0)
    {
      buffer.setData((U8 *)"DOWN");
      buffer.setSize(4);
    }
    else if (strcmp((char *)data, "WAVE_IN") == 0)
    {
      buffer.setData((U8 *)"LEFT");
      buffer.setSize(4);
    }
    else if (strcmp((char *)data, "WAVE_OUT") == 0)
    {
      buffer.setData((U8 *)"RIGHT");
      buffer.setSize(5);
    }else{
      buffer.setData((U8 *)"NONE");
      buffer.setSize(4);
    }
    this->move_out(0, buffer);
  }


} // end namespace Components
