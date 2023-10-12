module Components {
    @ Perovskia Cells payload, which reads voltage and current values of the solar cells
    active component ArduinoMega {
        enum Commands{
            TOGGLE_LED1 = 0x01
            TOGGLE_LED2 = 0x02
            TOGGLE_LED3  = 0x03
            SEND_TELEMETRY = 0x04
            STOP_TELEMETRY = 0x05
        }
        
        @ Command to send a string to the Payload
        async command SendString(
                                    $text: string size 40 @< String to send
                                ) \
        opcode 0

        @ Command to send a defined command to the Payload
        async command SendCommand (
                                    payloadcommand : Commands
                                ) \
        opcode 1

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example telemetry counter
        telemetry ldrVal : U16
        telemetry pirState : bool
        telemetry LED1State : bool
        telemetry LED2State : bool
        telemetry LED3State : bool

        

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # -------------------------------------------------
        # General ports
        # -------------------------------------------------

        @ Input Scheduler port 
        async input port Run: Svc.Sched

        @ Output Com port for sending data to the Hardware payload
        output port HwPktSend: Fw.Com

        @ Output Com port for sending data to the Software payload
        output port SwPktSend: Fw.Com

        @ Input Byte port for receiving data from the payload
        async input port PktRecv: Drv.ByteStreamRecv

        @ deallocate
        output port deallocate : Fw.BufferSend

        @ allocate
        output port allocate : Fw.BufferGet

        @ input port 
        async input port bufferSendIn : Fw.BufferSend

        @ file packet port
        output port FilePktSend: Fw.BufferSend


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