// ======================================================================
// \title  ArduinoMega.cpp
// \author jsilveira
// \brief  cpp file for ArduinoMega component implementation class
// ======================================================================


#include <Components/ArduinoMega/ArduinoMega.hpp>
#include <FpConfig.hpp>

namespace Components {
  
  
  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ArduinoMega ::
    ArduinoMega(
        const char *const compName
    ) : ArduinoMegaComponentBase(compName)
  {
    this->shoulder_angle = 70;
    this->elbow_angle = 70;
    this->forearm_angle = 70;
    this->wrist_angle = 70;
    this->move_cmd = Move_t::NONE;
  }

  ArduinoMega ::
    ~ArduinoMega()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

    void ArduinoMega ::Run_handler(const NATIVE_INT_TYPE portNum,NATIVE_UINT_TYPE context){
      //check if the mega_comm is open 
      if (this->comm_open){
        
        U8 mouvement[2] = {0, 0};

        switch (this->move_cmd){
          case Move_t::DOWN:{
            printf("DOWN\n");
            mouvement[0] = Components::ArduinoMega_MoveCommands::SERVO_WRIST_SET;
            if (this->wrist_angle < 160) this->wrist_angle += 1;
            else this->wrist_angle = 160;
            mouvement [1] = this->wrist_angle;
            Fw::ComBuffer arg(mouvement, sizeof(mouvement));
            this->PktSend_out(0, arg, 0); 
            break;
          }
          case Move_t::LEFT:{
            printf("LEFT\n");
            mouvement[0] = Components::ArduinoMega_MoveCommands::SERVO_FOREARM_SET;
            if (this->forearm_angle < 160) this->forearm_angle += 1;
            else this->forearm_angle = 180;
            mouvement [1] = this->forearm_angle;
            Fw::ComBuffer arg(mouvement, sizeof(mouvement));
            this->PktSend_out(0, arg, 0); 
            break;
          }
          case Move_t::RIGHT:{
            printf("RIGHT\n");
            mouvement[0] = Components::ArduinoMega_MoveCommands::SERVO_FOREARM_SET;
            if (this->forearm_angle > 0) this->forearm_angle -= 1;
            else this->forearm_angle = 0;
            mouvement [1] = this->forearm_angle;
            Fw::ComBuffer arg(mouvement, sizeof(mouvement));
            this->PktSend_out(0, arg, 0); 
            break;
          }
          case Move_t::UP:{
            printf("UP\n");
            mouvement[0] = Components::ArduinoMega_MoveCommands::SERVO_WRIST_SET;
            if (this->wrist_angle > 0) this->wrist_angle -= 1;
            else this->wrist_angle = 0;
            mouvement [1] = this->wrist_angle;
            Fw::ComBuffer arg(mouvement, sizeof(mouvement));
            this->PktSend_out(0, arg, 0); 
            break;
          }
          case Move_t::NONE:
            mouvement[0] = 0;
            mouvement[1] = 0;
            break;
          default:
            break;
        }
      }
  }

  void ArduinoMega ::bufferSendIn_handler(const NATIVE_INT_TYPE portNum,Fw::Buffer &fwBuffer){
    // print out the data in ascii
    U8* data = (U8*)fwBuffer.getData();
  }

  void ArduinoMega::set_comm(){
    this->comm_open = true;
  }

  void ArduinoMega::move_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer &fwBuffer){
    char* data = (char*)fwBuffer.getData();
    if (data == NULL ) {
      return;
    }
    
    if (strcmp(data, "UP") == 0) {
        this->move_cmd = Move_t::UP;
    } else if (strcmp(data, "LEFT") == 0) {
        this->move_cmd = Move_t::LEFT;
    } else if (strcmp(data, "RIGHT") == 0) {
        this->move_cmd = Move_t::RIGHT;
    }else if(strcmp(data, "DOWN") == 0){
        this->move_cmd = Move_t::DOWN;
    }else{
        this->move_cmd = Move_t::NONE;
    }
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void ArduinoMega ::
    SendString_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        const Fw::CmdStringArg& text
    )
  {
    // TODO
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void ArduinoMega ::
    MoveServo_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoMega_MoveCommands cmd,
        U8 angle
    )
  {
    const U32 size = 2;
    U8 data[2] = {cmd.e, angle};
    Fw::ComBuffer arg(data, size);

    switch (cmd){
      case Components::ArduinoMega_MoveCommands::SERVO_WRIST_SET:
        this->wrist_angle = angle;
        this->tlmWrite_wristAngle(angle);
        break;
      case Components::ArduinoMega_MoveCommands::SERVO_FOREARM_SET:
        this->forearm_angle = angle;
        this->tlmWrite_forearmAngle(angle);
        break;
      case Components::ArduinoMega_MoveCommands::SERVO_ELBOW_SET:
        this->elbow_angle = angle;
        this->tlmWrite_elbowAngle(angle);
        break;
      case Components::ArduinoMega_MoveCommands::SERVO_SHOULDER_SET:
        this->shoulder_angle = angle;
        this->tlmWrite_shoulderAngle(angle);
        break;    
      default:
        break;
    }
    
    this->PktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

  void ArduinoMega ::
    ConfigureServoSpeed_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq,
        Components::ArduinoMega_ConfigCommands cmd,
        U8 speed
    )
  {

    const U32 size = 2;
    U8 data[2] = {cmd.e, speed};
    Fw::ComBuffer arg(data, size);
    
    this->PktSend_out(0, arg, 0);
    this->cmdResponse_out(opCode,cmdSeq,Fw::CmdResponse::OK);
  }

} // end namespace Components
