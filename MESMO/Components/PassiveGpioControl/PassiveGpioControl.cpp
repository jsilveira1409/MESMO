// ======================================================================
// \title  PassiveGpioControl.cpp
// \author jsilveira
// \brief  cpp file for PassiveGpioControl component implementation class
// ======================================================================


#include <Components/PassiveGpioControl/PassiveGpioControl.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  PassiveGpioControl ::
    PassiveGpioControl(
        const char *const compName
    ) : PassiveGpioControlComponentBase(compName)
  {

  }

  PassiveGpioControl ::
    ~PassiveGpioControl()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void PassiveGpioControl ::
    gpioRead_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Logic &state
    )
  {
    
  }

  void PassiveGpioControl ::
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

  void PassiveGpioControl ::
    SetGpio_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Fw::Logic state
    )
  {
    this->gpioWrite_out(0, state);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Components
