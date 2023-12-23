// ======================================================================
// \title  ActiveGpioControl.hpp
// \author jsilveira
// \brief  hpp file for ActiveGpioControl component implementation class
// ======================================================================

#ifndef ActiveGpioControl_HPP
#define ActiveGpioControl_HPP

#include "Components/ActiveGpioControl/ActiveGpioControlComponentAc.hpp"

namespace Components {

  class ActiveGpioControl :
    public ActiveGpioControlComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ActiveGpioControl
      //!
      ActiveGpioControl(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object ActiveGpioControl
      //!
      ~ActiveGpioControl();

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
