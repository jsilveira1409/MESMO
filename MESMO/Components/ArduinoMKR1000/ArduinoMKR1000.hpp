// ======================================================================
// \title  ArduinoMKR1000.hpp
// \author jsilveira
// \brief  hpp file for ArduinoMKR1000 component implementation class
// ======================================================================

#ifndef ArduinoMKR1000_HPP
#define ArduinoMKR1000_HPP

#include "Components/ArduinoMKR1000/ArduinoMKR1000ComponentAc.hpp"

namespace Components {

  class ArduinoMKR1000 :
    public ArduinoMKR1000ComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ArduinoMKR1000
      //!
      ArduinoMKR1000(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object ArduinoMKR1000
      //!
      ~ArduinoMKR1000();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for Run
      //!
      void Run_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );

    PRIVATE:

     


    };

} // end namespace Components

#endif
