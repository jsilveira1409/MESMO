module RPi4 {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
      rateGroup2
      rateGroup3
    }

  topology RPi4 {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance $health
    instance blockDrv
    instance tlmSend
    instance cmdDisp
    instance cmdSeq
    instance comDriver
    instance comQueue
    instance comStub
    instance deframer
    instance eventLogger
    instance fatalAdapter
    instance fatalHandler
    instance fileDownlink
    instance fileManager
    instance fileUplink
    instance bufferManager
    instance framer
    instance linuxTime
    instance prmDb
    instance rateGroup1
    instance rateGroup2
    instance rateGroup3
    instance rateGroupDriver
    instance textLogger
    instance systemResources

    # custom components
    instance arduinoMega
    instance arduino_deframer
    instance arduino_framer
    instance arduino_comm

    # custom components shared components
    instance subsystemsFileUplinkBufferManager
    instance subsystemsStaticMemory
    instance subsystemsFileUplink

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    param connections instance prmDb

    telemetry connections instance tlmSend

    text event connections instance textLogger

    time connections instance linuxTime

    health connections instance $health

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections Downlink {

      eventLogger.PktSend -> comQueue.comQueueIn[0]
      tlmSend.PktSend -> comQueue.comQueueIn[1]
      fileDownlink.bufferSendOut -> comQueue.buffQueueIn[0]

      comQueue.comQueueSend -> framer.comIn
      comQueue.buffQueueSend -> framer.bufferIn

      framer.framedAllocate -> bufferManager.bufferGetCallee
      framer.framedOut -> comStub.comDataIn
      framer.bufferDeallocate -> fileDownlink.bufferReturn

      comDriver.deallocate -> bufferManager.bufferSendIn
      comDriver.ready -> comStub.drvConnected

      comStub.comStatus -> framer.comStatusIn
      framer.comStatusOut -> comQueue.comStatusIn
      comStub.drvDataOut -> comDriver.send

    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections RateGroups {
      # Block driver
      blockDrv.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      rateGroup1.RateGroupMemberOut[0] -> tlmSend.Run
      rateGroup1.RateGroupMemberOut[1] -> fileDownlink.Run
      rateGroup1.RateGroupMemberOut[2] -> systemResources.run

      # Rate group 2
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2.CycleIn
      rateGroup2.RateGroupMemberOut[0] -> cmdSeq.schedIn

      # Rate group 3
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup3] -> rateGroup3.CycleIn
      rateGroup3.RateGroupMemberOut[0] -> $health.Run
      rateGroup3.RateGroupMemberOut[1] -> blockDrv.Sched
      rateGroup3.RateGroupMemberOut[2] -> bufferManager.schedIn
    }

    connections Sequencer {
      cmdSeq.comCmdOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> cmdSeq.cmdResponseIn
    }

    connections Uplink {

      comDriver.allocate -> bufferManager.bufferGetCallee
      comDriver.$recv -> comStub.drvDataIn
      comStub.comDataOut -> deframer.framedIn

      deframer.framedDeallocate -> bufferManager.bufferSendIn
      deframer.comOut -> cmdDisp.seqCmdBuff

      cmdDisp.seqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> bufferManager.bufferGetCallee
      deframer.bufferOut -> fileUplink.bufferSendIn
      deframer.bufferDeallocate -> bufferManager.bufferSendIn
      fileUplink.bufferSendOut -> bufferManager.bufferSendIn
    }

    connections SubsystemsSharedRessources {
      
      arduinoMega.FilePktSend -> subsystemsFileUplink.bufferSendIn
      arduinoMega.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      arduinoMega.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn

      subsystemsFileUplink.bufferSendOut -> subsystemsFileUplinkBufferManager.bufferSendIn
    }

    connections RPi4 {
    # downlink  
      arduinoMega.HwPktSend -> arduino_framer.comIn
      arduino_framer.framedAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      arduino_framer.framedOut -> arduino_comm.send
      arduino_comm.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      
    # uplink
      arduino_comm.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      arduino_comm.$recv -> arduino_deframer.framedIn
      
      arduino_deframer.framedDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      arduino_deframer.bufferAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      arduino_deframer.bufferDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      arduino_deframer.bufferOut -> subsystemsFileUplink.bufferSendIn
    }

  }

}
