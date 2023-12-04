module Components {
    @ Component for F Prime FSW framework.
    active component ArduinoMega {
        
        enum MoveCommands{
            NONE = 0x00
            SERVO_SHOULDER_SET = 0x01
            SERVO_ELBOW_SET = 0x02
            SERVO_FOREARM_SET = 0x03
            SERVO_WRIST_SET = 0x04
            SERVO_SHOULDER_SWEEP = 0x05
            SERVO_ELBOW_SWEEP = 0x06
            SERVO_FOREARM_SWEEP = 0x07
            SERVO_WRIST_SWEEP = 0x08
        }

        enum ConfigCommands{
            SERVO_SHOULDER_SPEED = 0x09
            SERVO_ELBOW_SPEED = 0x0A
            SERVO_FOREARM_SPEED = 0x0B
            SERVO_WRIST_SPEED = 0x0C
            
        }
        
        @ Command to send a string to the Payload
        async command SendString(
                                    $text: string size 40 @< String to send
                                ) \
        opcode 0

        @ Command to send a defined command to the Payload
        async command MoveServo (
                                    cmd : MoveCommands, 
                                    angle : U8
                                ) \
        opcode 1

        @ Command to send a defined command to the Payload
        async command ConfigureServoSpeed (
                                    cmd : ConfigCommands, 
                                    speed : U8        
                                ) \
        opcode 2


        async input port Run: Svc.Sched

        output port deallocate : Fw.BufferSend

        output port allocate : Fw.BufferGet

        async input port bufferSendIn: Fw.BufferSend

        output port PktSend : Fw.Com

        sync input port move : Fw.BufferSend

        # @ Example telemetry counter
        telemetry shoulderAngle: U8
        # @ Example telemetry counter
        telemetry elbowAngle: U8
        # @ Example telemetry counter
        telemetry forearmAngle: U8
        # @ Example telemetry counter
        telemetry wristAngle : U8
        

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