module Components {
    @ Component for F Prime FSW framework.
    passive component Gps {

        sync input port ready: Drv.ByteStreamReady

        sync input port $recv: Drv.ByteStreamRecv

        output port send: Drv.ByteStreamSend

        output port allocate: Fw.BufferGet

        output port deallocate: Fw.BufferSend
        
        @ A command to force an EVR reporting lock status.
        sync command ReportLockStatus() opcode 0

        event LockAcquired() severity activity high id 0 format "GPS Lock acquired"

        event LockLost() severity activity high id 1 format "GPS Lock lost"

        telemetry Latitude: F32 

        telemetry Longitude: F32

        telemetry Altitude: F32

        telemetry GpsCount: U32


        
        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32


        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}