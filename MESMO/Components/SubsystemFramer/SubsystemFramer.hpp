// ======================================================================
// \title  SubsystemFramer.hpp
// \author jsilveira
// \brief  hpp file for SubsystemFramer component implementation class
// ======================================================================

#ifndef SubsystemFramer_HPP
#define SubsystemFramer_HPP

#include "Components/SubsystemFramer/SubsystemFramerComponentAc.hpp"
#include "Svc/FramingProtocol/FramingProtocol.hpp"
#include "Svc/FramingProtocol/FramingProtocolInterface.hpp"

namespace Components {

  class SubsystemFramer :public SubsystemFramerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object SubsystemFramer
      //!
      SubsystemFramer(
          const char *const compName /*!< The component name*/
      );

        //! Initialize object Framer
        //!
        void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
        );


      //! Destroy object SubsystemFramer
      //!
      ~SubsystemFramer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for bufferIn
      //!
      void bufferIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler implementation for comIn
      //!
      void comIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::ComBuffer &data, /*!< 
      Buffer containing packet data
      */
          U32 context /*!< 
      Call context value; meaning chosen by user
      */
      );

      //! Handler implementation for comStatusIn
      //!
      void comStatusIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Success &condition /*!< 
      Condition success/failure
      */
      );

           //! \brief Allocation callback used to request memory for the framer
    //!
    //! Method used by the FramingProtocol to allocate memory for the framed buffer. Framing
    //! typically adds tokens on the beginning and end of the raw data so it must allocate new space
    //! to place those and a copy of the data in.
    //!
    //! \param size: size of allocation
    //! \return Fw::Buffer containing allocation to write into
    Fw::Buffer allocate(const U32 size);

    //! Send implementation
    //!
    void send(Fw::Buffer& outgoing  //!< The buffer to send
    );

    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! \brief helper function to handle framing of the raw data
    //!
    void handle_framing(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type);

    void frame(const U8 *const data, const U32 size, Fw::ComPacket::ComPacketType packet_type);

    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The FramingProtocol implementation
    //PayloadFrameProtocol* m_protocol;

    //! Flag determining if at least one frame was sent during framing
    bool m_frame_sent;




    };

} // end namespace Components

#endif
