// ======================================================================
// \title  SubsystemDeframer.cpp
// \author jsilveira
// \brief  cpp file for SubsystemDeframer component implementation class
// ======================================================================

#include <cstring>

#include <Components/SubsystemDeframer/SubsystemDeframer.hpp>
#include <FpConfig.hpp>
#include "Fw/Com/ComPacket.hpp"
#include "Fw/Logger/Logger.hpp"
#include <Fw/FilePacket/FilePacket.hpp>
#include <FpConfig.hpp>

namespace Components {
    static Fw::FilePacket filePacket;

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  SubsystemDeframer ::
    SubsystemDeframer(
        const char *const compName
    ) : SubsystemDeframerComponentBase(compName),
        DeframingProtocolInterface()
  {
    (void) memset(m_pollBuffer, 0, sizeof m_pollBuffer);
    this->m_expectedSize = 0;
    this->m_receivedSize = 0;
    this->m_routeOption = FILE_PACKET;
    this->m_pktIndex = 0;

  }

  SubsystemDeframer ::
    ~SubsystemDeframer()
  {

  }

  void SubsystemDeframer ::init(const NATIVE_INT_TYPE instance) {
    SubsystemDeframerComponentBase::init(instance);
  }


  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void SubsystemDeframer ::
    cmdResponseIn_handler(
        const NATIVE_INT_TYPE portNum,
        FwOpcodeType opCode,
        U32 cmdSeq,
        const Fw::CmdResponse &response
    )
  {
    // TODO
  }

  void SubsystemDeframer ::framedIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &recvBuffer,
        const Drv::RecvStatus &recvStatus
    )
  {
    // Check whether there is data to process
    if (recvStatus.e == Drv::RecvStatus::RECV_OK) {
        // There is: process the data
        processBuffer(recvBuffer);
        //print recvBuffer size 
        Fw::Logger::logMsg("[INFO] RecvBuffer size: %d\n", recvBuffer.getSize());
    }
    // Deallocate the buffer
    framedDeallocate_out(0, recvBuffer);
  }

  void SubsystemDeframer ::schedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
   // TODO
  }

  // ----------------------------------------------------------------------
// Implementation of DeframingProtocolInterface
// ----------------------------------------------------------------------

Fw::Buffer SubsystemDeframer ::allocate(const U32 size)  {
    return bufferAllocate_out(0, size);
}

void SubsystemDeframer ::route(Fw::Buffer& packetBuffer) {
    
    Fw::SerializeStatus status = Fw::FW_SERIALIZE_OK;{
        Fw::SerializeBufferBase& serial = packetBuffer.getSerializeRepr();
        status = serial.setBuffLen(packetBuffer.getSize());
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
        //status = serial.deserialize(packetType);
    }
    // Whether to deallocate the packet buffer
    bool deallocate = true;
    // Process the packet
    if (status == Fw::FW_SERIALIZE_OK) {
        if (this->m_routeOption == FILE_PACKET){

            U8 *const packetData = packetBuffer.getData();
            const U32 packetSize = packetBuffer.getSize();
            // If the file uplink output port is connected,
            // send the file packet. Otherwise take no action.
            if (isConnected_bufferOut_OutputPort(0)) {
                // Send the packet buffer
                bufferOut_out(0, packetBuffer);
                // Transfer ownership of the buffer to the receiver
                deallocate = false;
            }
        }else if(this->m_routeOption == SW_PACKET){
            // If the software uplink output port is connected,
            // send the software packet. Otherwise take no action.
            //Fw::ComBuffer comBuffer(packetBuffer.getData(), packetBuffer.getSize());
            
            if (isConnected_SwPktSend_OutputPort(0)) {
                // Send the packet buffer
                //comOut_out(0, comBuffer, 0);
                SwPktSend_out(0, packetBuffer);
                // Transfer ownership of the buffer to the receiver
                deallocate = false;
            }
        }
    }else {
        Fw::Logger::logMsg(
            "[ERROR] Deserializing packet type failed with status %d\n",
            status
        );
    }

    if (deallocate) {
        // Deallocate the packet buffer
        bufferDeallocate_out(0, packetBuffer);
    }

}

// ----------------------------------------------------------------------
// Helper methods
// ----------------------------------------------------------------------

void SubsystemDeframer ::processBuffer(Fw::Buffer& buffer) {

    const U32 bufferSize = buffer.getSize();
    U8 *const bufferData = buffer.getData();
    
    
    // check whether the expected size of the incoming bytestream is 0, 
    // which means it's a new bytestream and we should check for the header
    printf("bufferSize: %d\n", bufferSize);
    printf("m_expectedSize: %d\n", this->m_expectedSize);

    if (this->m_expectedSize == 0){
        U32 start_header = 0;
        U32 route_header = 0;
        U32 size = 0;
        // check buffer size 
        if (bufferSize < 12){
            Fw::Logger::logMsg("[ERROR] Buffer size is less than 12 bytes\n");
            return;
        }
        start_header = (bufferData[0] << 24) | (bufferData[1] << 16) | (bufferData[2] << 8) | bufferData[3];
        size = (bufferData[0] << 24) | (bufferData[1] << 16) | (bufferData[2] << 8) | bufferData[3];

        // check whether the start header is correct
        if (start_header != 0xdeadbeef){
            Fw::Logger::logMsg("[ERROR] Start header is incorrect\n");
            return;
        }        
        this->m_expectedSize = size;        
        sendStartPacket();
        sendDataPacket(bufferData , bufferSize);
        
        this->m_receivedSize += (bufferSize );
        if (this->m_receivedSize == this->m_expectedSize){
            sendEndPacket();   
            // we'll also forward the data to the component, for telemetry logging
            Fw::Buffer data = this->allocate(size);
            this->bufferOut_out(0, data);
        }

    }else if(this->m_receivedSize < this->m_expectedSize) {
      // the next data bursts arrived with the data
      sendDataPacket(bufferData, bufferSize);
      this->m_receivedSize += bufferSize;

      if (this->m_receivedSize == this->m_expectedSize){
        sendEndPacket();
        this->m_expectedSize = 0;
        this->m_receivedSize = 0;
        this->m_routeOption = FILE_PACKET;

      }else if (this->m_receivedSize > this->m_expectedSize){
        Fw::Logger::logMsg("[ERROR] Received size is greater than expected size\n");
        this->m_expectedSize = 0;
        this->m_receivedSize = 0;
        this->m_routeOption = FILE_PACKET;
      }
    }
}

void SubsystemDeframer ::sendStartPacket(){
  Fw::FilePacket::StartPacket startPacket;
  const char* source = "source";
  const char* destination = "test";  
  startPacket.initialize(this->m_expectedSize, source, destination);
  filePacket.fromStartPacket(startPacket);
  Fw::Buffer startBuffer = this->allocate(filePacket.bufferSize());
  Fw::SerializeStatus status = filePacket.toBuffer(startBuffer);
  //TODO route the packet to fileUplink
  this->route(startBuffer);
  this->m_pktIndex ++;   
}

void SubsystemDeframer ::sendDataPacket(U8 *const bufferData, const U32 bufferSize){
  Fw::FilePacket::DataPacket dataPacket;
  dataPacket.initialize(this->m_pktIndex, this->m_receivedSize, bufferSize, bufferData);
  filePacket.fromDataPacket(dataPacket);
  Fw::Buffer dataBuffer = this->allocate(filePacket.bufferSize());
  Fw::SerializeStatus status = filePacket.toBuffer(dataBuffer);
  //TODO route the packet to fileUplink
  this->route(dataBuffer);
  this->m_pktIndex ++;
}

void SubsystemDeframer ::sendEndPacket(){
  Fw::FilePacket::EndPacket endPacket;
  U32 checksum = 0; // TODO calculate checksum
  endPacket.initialize(this->m_pktIndex, checksum);
  filePacket.fromEndPacket(endPacket);
  Fw::Buffer endBuffer = this->allocate(filePacket.bufferSize());
  Fw::SerializeStatus status = filePacket.toBuffer(endBuffer);
  //TODO route the packet to fileUplink
  this->route(endBuffer);
  this->m_pktIndex = 0;
  this->m_expectedSize = 0;
  this->m_receivedSize = 0;
  this->m_routeOption = FILE_PACKET;
}

} // end namespace Components
