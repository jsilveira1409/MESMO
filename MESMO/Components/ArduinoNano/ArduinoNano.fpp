module Components {
    @ Component for F Prime FSW framework.
    active component ArduinoNano {

        enum Commands{
            LED_PATTERN_1 = 0x01
            LED_PATTERN_2 = 0x02
            LED_PATTERN_3 = 0x03
            LED_PATTERN_4 = 0x04
            LED_PATTERN_5 = 0x05
            LED_PATTERN_6 = 0x06
            LED_PATTERN_7 = 0x07
        }
        
  

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
        telemetry ledPattern : U8

        

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # -------------------------------------------------
        # General ports
        # -------------------------------------------------

        sync input port ready: Drv.ByteStreamReady

        sync input port $recv: Drv.ByteStreamRecv

        output port send: Drv.ByteStreamSend

        output port allocate: Fw.BufferGet

        output port deallocate: Fw.BufferSend
        
        

        

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