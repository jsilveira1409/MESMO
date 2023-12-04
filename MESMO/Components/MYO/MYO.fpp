module Components {
    @ Component for F Prime FSW framework.
    passive component MYO {


        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################
        
        # @ Example telemetry counter
        telemetry Pose: string 

        # @ Example event
        event NewPose(pose: string) severity activity high id 0 format "New pose {}"

        @ Output Com port for sending data to the Hardware payload
        output port HwPktSend: Fw.Com

        @ Output Com port for sending data to the Software payload
        output port SwPktSend: Fw.Com

        @ deallocate
        output port deallocate : Fw.BufferSend

        @ allocate
        output port allocate : Fw.BufferGet

        @ input port 
        sync input port bufferSendIn : Fw.BufferSend

        @ file packet port
        output port FilePktSend: Fw.BufferSend

        @ pose output port
        output port move : Fw.BufferSend

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