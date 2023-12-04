// ======================================================================
// \title  MYO.hpp
// \author jsilveira
// \brief  hpp file for MYO component implementation class
// ======================================================================

#ifndef MYO_HPP
#define MYO_HPP

#include "Components/MYO/MYOComponentAc.hpp"

namespace Components {

  class MYO :
    public MYOComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object MYO
      //!
      MYO(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object MYO
      //!
      ~MYO();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for bufferSendIn
      //!
      void bufferSendIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler implementation for cmdResp
      //!
      void cmdResp_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          FwOpcodeType opCode, /*!< 
      Command Op Code
      */
          U32 cmdSeq, /*!< 
      Command Sequence
      */
          const Fw::CmdResponse &response /*!< 
      The command response argument
      */
      );



    };

} // end namespace Components

#endif
