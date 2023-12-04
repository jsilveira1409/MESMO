module Components {
    @ Perovskia Cells payload, which reads voltage and current values of the solar cells
    passive component ArduinoMKR1000 {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example telemetry counter
        telemetry IRSensor : U16
        telemetry Distance : F32
        

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # -------------------------------------------------
        # General ports
        # -------------------------------------------------

        sync input port Run: Svc.Sched

        output port deallocate : Fw.BufferSend

        output port allocate : Fw.BufferGet

        output port write: Drv.I2c
        
        output port read: Drv.I2c
        
        output port writeRead: Drv.I2cWriteRead


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