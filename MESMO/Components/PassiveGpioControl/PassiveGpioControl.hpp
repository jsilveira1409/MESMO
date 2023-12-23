// ======================================================================
// \title  PassiveGpioControl.hpp
// \author jsilveira
// \brief  hpp file for PassiveGpioControl component implementation class
// ======================================================================

#ifndef PassiveGpioControl_HPP
#define PassiveGpioControl_HPP

#include "Components/PassiveGpioControl/PassiveGpioControlComponentAc.hpp"

namespace Components {

  class PassiveGpioControl :
    public PassiveGpioControlComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object PassiveGpioControl
      //!
      PassiveGpioControl(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object PassiveGpioControl
      //!
      ~PassiveGpioControl();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for gpioRead
      //!
      void gpioRead_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Logic &state 
      );

      //! Handler implementation for run
      //!
      void run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for SetGpio command handler
      //! 
      void SetGpio_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq, /*!< The command sequence number*/
          Fw::Logic state 
      );


    };

} // end namespace Components

#endif
