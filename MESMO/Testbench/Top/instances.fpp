module Testbench {

  # ----------------------------------------------------------------------
  # Defaults
  # ----------------------------------------------------------------------

  module Default {
    constant QUEUE_SIZE = 10
    constant STACK_SIZE = 64 * 1024
  }

  # ----------------------------------------------------------------------
  # Active component instances
  # ----------------------------------------------------------------------

  instance blockDrv: Drv.BlockDriver base id 0x0100 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 140

  instance rateGroup1: Svc.ActiveRateGroup base id 0x0200 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 120

  instance rateGroup2: Svc.ActiveRateGroup base id 0x0300 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 119

  instance rateGroup3: Svc.ActiveRateGroup base id 0x0400 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 118

  instance cmdDisp: Svc.CommandDispatcher base id 0x0500 \
    queue size 20 \
    stack size Default.STACK_SIZE \
    priority 101

  instance cmdSeq: Svc.CmdSequencer base id 0x0600 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 100

  instance comQueue: Svc.ComQueue base id 0x0700 \
      queue size Default.QUEUE_SIZE \
      stack size Default.STACK_SIZE \
      priority 100 \

  instance fileDownlink: Svc.FileDownlink base id 0x0800 \
    queue size 30 \
    stack size Default.STACK_SIZE \
    priority 100

  instance fileManager: Svc.FileManager base id 0x0900 \
    queue size 30 \
    stack size Default.STACK_SIZE \
    priority 100

  instance fileUplink: Svc.FileUplink base id 0x0A00 \
    queue size 30 \
    stack size Default.STACK_SIZE \
    priority 100

  instance eventLogger: Svc.ActiveLogger base id 0x0B00 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 98

  # comment in Svc.TlmChan or Svc.TlmPacketizer
  # depending on which form of telemetry downlink
  # you wish to use

  instance tlmSend: Svc.TlmChan base id 0x0C00 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 97

  #instance tlmSend: Svc.TlmPacketizer base id 0x0C00 \
  #    queue size Default.QUEUE_SIZE \
  #    stack size Default.STACK_SIZE \
  #    priority 97

  instance prmDb: Svc.PrmDb base id 0x0D00 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 96

    instance mkr1000: Components.ArduinoMKR1000 base id 0x0E00

  instance camera: Components.Camera base id 0x0E10 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 94
  
  instance mega : Components.ArduinoMega base id 0x0E20 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 93

  instance gps : Components.Gps base id 0x0E30 

  instance nano : Components.ArduinoNano base id 0x0E40 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 92

  instance myo : Components.MYO base id 0x0E50 
  instance mkr1000_comm: Drv.LinuxI2cDriver base id 0x1000

  instance camera_framer : Components.SubsystemFramer base id 0x1010
  instance camera_deframer : Components.SubsystemDeframer base id 0x1011
  instance camera_comm: Drv.ByteStreamDriverModel base id 0x1012 type "Drv::TcpClient" at "../../Drv/TcpClient/TcpClient.hpp"

  instance mega_framer : Components.SubsystemFramer base id 0x1020
  instance mega_deframer : Components.SubsystemDeframer base id 0x1021
  instance mega_comm: Drv.LinuxUartDriver base id 0x1022

  instance gps_comm: Drv.LinuxUartDriver base id 0x1030

  #instance nano_framer : Components.SubsystemFramer base id 0x1050
  #instance nano_deframer : Components.SubsystemDeframer base id 0x1060
  instance nano_comm: Drv.LinuxUartDriver base id 0x1040

  instance myo_framer : Components.SubsystemFramer base id 0x1070
  instance myo_deframer : Components.SubsystemDeframer base id 0x1080
  instance myo_comm : Drv.ByteStreamDriverModel base id 0x1090 type "Drv::TcpClient" at "../../Drv/TcpClient/TcpClient.hpp"
  
  ## subsystems Shares Ressources
    instance subsystemsFileUplink: Svc.FileUplink base id 0x1300 \
      queue size 100 \
      stack size Default.STACK_SIZE \
      priority 100

  instance activeGpio : Components.ActiveGpioControl base id 0x1100 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 140

  instance subsystemsFileUplinkBufferManager: Svc.BufferManager base id 0x1400
  
  instance subsystemsStaticMemory: Svc.StaticMemory base id 0x1500


  # ----------------------------------------------------------------------
  # Queued component instances
  # ----------------------------------------------------------------------

  instance $health: Svc.Health base id 0x2000 \
    queue size 25

  # ----------------------------------------------------------------------
  # Passive component instances
  # ----------------------------------------------------------------------

  @ Communications driver. May be swapped with other com drivers like UART or TCP
  instance comDriver: Drv.LinuxUartDriver base id 0x4000

  instance framer: Svc.Framer base id 0x4100

  instance fatalAdapter: Svc.AssertFatalAdapter base id 0x4200

  instance fatalHandler: Svc.FatalHandler base id 0x4300

  instance bufferManager: Svc.BufferManager base id 0x4400

  instance linuxTime: Svc.Time base id 0x4500 \
    type "Svc::LinuxTime" \
    at "../../Svc/LinuxTime/LinuxTime.hpp"

  instance rateGroupDriver: Svc.RateGroupDriver base id 0x4600

  instance textLogger: Svc.PassiveTextLogger base id 0x4800

  instance deframer: Svc.Deframer base id 0x4900

  instance systemResources: Svc.SystemResources base id 0x4A00

  instance comStub: Svc.ComStub base id 0x4B00

  instance active_gpio_comm : Drv.LinuxGpioDriver base id 0x4C00

  instance passiveGpio : Components.PassiveGpioControl base id 0x1120

  instance passive_gpio_comm : Drv.LinuxGpioDriver base id 0x1130

}
