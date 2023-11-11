// ======================================================================
// \title  SubsystemFramer.cpp
// \author jsilveira
// \brief  cpp file for SubsystemFramer component implementation class
// ======================================================================


#include <Components/SubsystemFramer/SubsystemFramer.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  SubsystemFramer ::SubsystemFramer(const char *const compName) : SubsystemFramerComponentBase(compName){}

    void SubsystemFramer ::init(const NATIVE_INT_TYPE instance) {
        SubsystemFramerComponentBase::init(instance);
    }   

  SubsystemFramer ::~SubsystemFramer(){

  }

   void SubsystemFramer ::handle_framing(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type) {
        
        this->m_frame_sent = false;  // Clear the flag to detect if frame was sent
        this->frame(data, size, packet_type);
        // If no frame was sent, Framer has the obligation to report success
        if (this->isConnected_comStatusOut_OutputPort(0) && (!this->m_frame_sent)) {
            Fw::Success status = Fw::Success::SUCCESS;
            this->comStatusOut_out(0, status);
        }
    }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

     void SubsystemFramer ::bufferIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer &fwBuffer){
        this->handle_framing(fwBuffer.getData(), fwBuffer.getSize(), Fw::ComPacket::FW_PACKET_FILE);
        // Deallocate the buffer after it was processed by the framing protocol
        //this->bufferDeallocate_out(0, fwBuffer);    
    }

    void SubsystemFramer ::comIn_handler(const NATIVE_INT_TYPE portNum,Fw::ComBuffer &data,U32 context){
        this->handle_framing(data.getBuffAddr(), data.getBuffLength(), Fw::ComPacket::FW_PACKET_UNKNOWN);
    }

    void SubsystemFramer ::comStatusIn_handler(const NATIVE_INT_TYPE portNum,Fw::Success &condition){
        if (this->isConnected_comStatusOut_OutputPort(portNum)) {   
            this->comStatusOut_out(portNum, condition);
        }
    }

    void SubsystemFramer::frame(const U8 *const data, const U32 size, Fw::ComPacket::ComPacketType packet_type) {
        // TODO check the + 8, i though it was not needed but if else the data is not correctly copied
        Fw::Buffer my_framed_data = this->allocate(size + 8);
        my_framed_data.getSerializeRepr().serialize(0xdeadbeef); // Some start word
        my_framed_data.getSerializeRepr().serialize(size);       // Write size
        my_framed_data.getSerializeRepr().serialize(data, size, true); // Data copied to buffer no length included
        printf("\n");
        for (U32 i = 0; i < (8 + size); i++) {
            printf("%x ", my_framed_data.getData()[i]);
        }
        printf("\n");

        this->send(my_framed_data);
    }

    // ----------------------------------------------------------------------
    // Framing protocol implementations
    // ----------------------------------------------------------------------

    void SubsystemFramer ::send(Fw::Buffer& outgoing) {
        FW_ASSERT(!this->m_frame_sent); // Prevent multiple sends per-packet
        const Drv::SendStatus sendStatus = this->framedOut_out(0, outgoing);
        if (sendStatus.e != Drv::SendStatus::SEND_OK) {
            // Note: if there is a data sending problem, an EVR likely wouldn't
            // make it down. Log the issue in hopes that
            // someone will see it.
            //TODO check this
            //Fw::Logger::logMsg("[ERROR] Failed to send framed data: %d\n", sendStatus.e);
            
        }
        this->m_frame_sent = true;  // A frame was sent
    }

    Fw::Buffer SubsystemFramer ::allocate(const U32 size) {
        return this->framedAllocate_out(0, size);
    }




} // end namespace Components
