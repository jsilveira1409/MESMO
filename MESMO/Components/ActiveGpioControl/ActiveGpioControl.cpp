// ======================================================================
// \title  ActiveGpioControl.cpp
// \author jsilveira
// \brief  cpp file for ActiveGpioControl component implementation class
// ======================================================================


#include <Components/ActiveGpioControl/ActiveGpioControl.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ActiveGpioControl ::
    ActiveGpioControl(
        const char *const compName
    ) : ActiveGpioControlComponentBase(compName)
  {

  }

  ActiveGpioControl ::
    ~ActiveGpioControl()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ActiveGpioControl ::
    gpioRead_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Logic &state
    )
  {
    // TODO
  }

  void ActiveGpioControl ::
    run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void ActiveGpioControl ::
    SetGpio_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Fw::Logic state
    )
  {
    // TODO
    this->gpioWrite_out(0,state);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Components
