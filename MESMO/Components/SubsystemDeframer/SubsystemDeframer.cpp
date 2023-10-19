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

  SubsystemDeframer :: SubsystemDeframer(const char *const compName) : 
    SubsystemDeframerComponentBase(compName),DeframingProtocolInterface(){
        this->m_expectedSize = 0;
        this->m_receivedSize = 0;
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
    ){
    if (recvStatus.e == Drv::RecvStatus::RECV_OK) {
        processBuffer(recvBuffer);
        //Fw::Logger::logMsg("[INFO] RecvBuffer size: %d\n", recvBuffer.getSize());
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
            U8 *const packetData = packetBuffer.getData();
            const U32 packetSize = packetBuffer.getSize();

            if (isConnected_FilePktSend_OutputPort(0)) {
                // Send the packet buffer
                FilePktSend_out(0, packetBuffer);
                // Transfer ownership of the buffer to the receiver
                deallocate = false;
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
    
    if (this->m_expectedSize == 0){
        U32 start_header = 0;
        U32 size = 0;
        // check buffer size 
        if (bufferSize < 8){
            //Fw::Logger::logMsg("[ERROR] Buffer size is less than 8 bytes\n");
            return;
        }

        start_header = (bufferData[0] << 24) | (bufferData[1] << 16) | (bufferData[2] << 8) | bufferData[3];
        size = (bufferData[4] << 24) | (bufferData[5] << 16) | (bufferData[6] << 8) | bufferData[7];
        
        // check whether the start header is correct
        if (start_header == 0xdeadbeef){
             
          this->m_expectedSize = size;                  // total size of the incoming data, which can come in multiple packets
          sendStartPacket();
          sendDataPacket(bufferData + 8 , bufferSize - 8); 

          // means it is a small telemetry packet
          if (this->m_expectedSize == this->m_receivedSize){
              // we directly forward it to the component so that it can publish
              // the values into the telemetry channels
              Fw::Buffer data = this->allocate(size);
              if (data.getData() == NULL) {
                  Fw::Logger::logMsg("[ERROR] Failed to allocate buffer\n");
                  return;
              }
              data.setData(bufferData + 8);
              this->bufferOut_out(0, data);
              
              sendEndPacket();
                      
          }
        }

    }else if(this->m_receivedSize <= this->m_expectedSize) {
      // the next data bursts arrived with the data
      sendDataPacket(bufferData, bufferSize);

      if (this->m_receivedSize == this->m_expectedSize){
        sendEndPacket();

      }else if (this->m_receivedSize > this->m_expectedSize){
        Fw::Logger::logMsg("[ERROR] Received size is greater than expected size\n");
        this->m_expectedSize = 0;
        this->m_receivedSize = 0;
      }
    }
}

void SubsystemDeframer ::sendStartPacket(){
  printf("send start packet\n");
  Fw::FilePacket::StartPacket startPacket;
  char source[10];  // Ensure this buffer is large enough
  sprintf(source, "source%d", this->m_fileIndex);
  char destination[10];  // Ensure this buffer is large enough
  sprintf(destination, "file%d", this->m_fileIndex);
  startPacket.initialize(this->m_expectedSize, source, destination);
  filePacket.fromStartPacket(startPacket);
  Fw::Buffer startBuffer = this->allocate(filePacket.bufferSize());
  if (startBuffer.getData() == NULL) {
    Fw::Logger::logMsg("[ERROR] Failed to allocate buffer\n");
    return;
  }
  Fw::SerializeStatus status = filePacket.toBuffer(startBuffer);
  //TODO route the packet to fileUplink
  this->route(startBuffer);
  this->m_pktIndex ++;
  this->m_fileIndex ++;
}

void SubsystemDeframer ::sendDataPacket(U8 *const bufferData, const U32 bufferSize){
  printf("send data packet nb %d\n", this->m_pktIndex);
  Fw::FilePacket::DataPacket dataPacket;
  dataPacket.initialize(this->m_pktIndex, this->m_receivedSize, bufferSize, bufferData);
  filePacket.fromDataPacket(dataPacket);
  Fw::Buffer dataBuffer = this->allocate(filePacket.bufferSize());
  if (dataBuffer.getData() == NULL) {
    Fw::Logger::logMsg("[ERROR] Failed to allocate buffer\n");
    return;
  }
  Fw::SerializeStatus status = filePacket.toBuffer(dataBuffer);
  //TODO route the packet to fileUplink
  this->route(dataBuffer);
  this->m_receivedSize += bufferSize;
  this->m_pktIndex ++;
}

void SubsystemDeframer ::sendEndPacket(){
    printf("send end packet\n");
  Fw::FilePacket::EndPacket endPacket;
  U32 checksum = 0; // TODO calculate checksum
  endPacket.initialize(this->m_pktIndex, checksum);
  filePacket.fromEndPacket(endPacket);
  Fw::Buffer endBuffer = this->allocate(filePacket.bufferSize());
  if (endBuffer.getData() == NULL) {
    Fw::Logger::logMsg("[ERROR] Failed to allocate buffer\n");
    return;
  }
  Fw::SerializeStatus status = filePacket.toBuffer(endBuffer);
  //TODO route the packet to fileUplink
  this->route(endBuffer);
  this->m_pktIndex = 0;
  this->m_expectedSize = 0;
  this->m_receivedSize = 0;
}

} // end namespace Components