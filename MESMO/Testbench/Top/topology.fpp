module Testbench {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
      rateGroup2
      rateGroup3
    }

  topology Testbench {

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

    # custom components shared components
    instance subsystemsFileUplinkBufferManager
    instance subsystemsStaticMemory
    instance subsystemsFileUplink

    # custom components
    instance mkr1000
    instance mkr1000_comm

    instance camera
    instance camera_deframer
    instance camera_framer
    instance camera_comm

    instance mega
    instance mega_comm
    instance mega_deframer
    instance mega_framer

    instance gps
    instance gps_comm

    instance nano
    instance nano_comm

    instance myo
    instance myo_comm
    instance myo_deframer
    instance myo_framer

    instance activeGpio
    instance active_gpio_comm
    instance passiveGpio
    instance passive_gpio_comm

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

    connections Testbench {
      activeGpio.gpioWrite -> active_gpio_comm.gpioWrite
      passiveGpio.gpioWrite -> passive_gpio_comm.gpioWrite

    }


    connections SubsystemsSharedRessources {
      subsystemsFileUplink.bufferSendOut -> subsystemsFileUplinkBufferManager.bufferSendIn
    }

    connections MKR1000 {
      rateGroup1.RateGroupMemberOut[4] -> mkr1000.Run
      mkr1000.read ->mkr1000_comm.read
      mkr1000.write ->mkr1000_comm.write
      mkr1000.writeRead ->mkr1000_comm.writeRead
    }

    connections Camera {
      camera.HwPktSend -> camera_framer.comIn
      
      camera_framer.framedAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      camera_framer.framedOut -> camera_comm.send

      camera_comm.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      camera_comm.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      camera_comm.$recv -> camera_deframer.framedIn
      
      camera_deframer.framedDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      camera_deframer.bufferAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      camera_deframer.bufferDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      camera_deframer.FilePktSend -> subsystemsFileUplink.bufferSendIn
      camera_deframer.bufferOut -> camera.bufferSendIn 
    }

    connections Mega {
      rateGroup1.RateGroupMemberOut[3] -> mega.Run
      
      mega.PktSend -> mega_framer.comIn
      
      mega_framer.framedAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      mega_framer.framedOut -> mega_comm.send

      mega_comm.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn    
      mega_comm.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      mega_comm.$recv -> mega_deframer.framedIn
      
      mega_deframer.framedDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      mega_deframer.bufferAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      mega_deframer.bufferDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      mega_deframer.FilePktSend -> subsystemsFileUplink.bufferSendIn
      mega_deframer.bufferOut -> mega.bufferSendIn 

    }

     connections GPS {
      gps.send -> gps_comm.send 
      gps.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      gps.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      gps_comm.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      gps_comm.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      gps_comm.$recv -> gps.$recv
      gps_comm.ready -> gps.ready
     }

    connections Nano {
      nano.send -> nano_comm.send 
      nano.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      nano.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      nano_comm.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      nano_comm.$recv -> nano.$recv
      nano_comm.ready -> nano.ready
    }


    connections Myo {
      myo.HwPktSend -> myo_framer.comIn
      
      myo_framer.framedAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      myo_framer.framedOut -> myo_comm.send

      myo_comm.deallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      myo_comm.allocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      myo_comm.$recv -> myo_deframer.framedIn
      
      myo_deframer.framedDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      myo_deframer.bufferAllocate -> subsystemsFileUplinkBufferManager.bufferGetCallee
      myo_deframer.bufferDeallocate -> subsystemsFileUplinkBufferManager.bufferSendIn
      myo_deframer.FilePktSend -> subsystemsFileUplink.bufferSendIn
      myo_deframer.bufferOut -> myo.bufferSendIn 

      myo.move -> mega.move
    }

  }

}
