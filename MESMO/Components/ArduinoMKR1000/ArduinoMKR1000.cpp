// ======================================================================
// \title  ArduinoMKR1000.cpp
// \author jsilveira
// \brief  cpp file for ArduinoMKR1000 component implementation class
// ======================================================================


#include <Components/ArduinoMKR1000/ArduinoMKR1000.hpp>
#include <FpConfig.hpp>
#include <cmath>


namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ArduinoMKR1000 ::
    ArduinoMKR1000(
        const char *const compName
    ) : ArduinoMKR1000ComponentBase(compName)
  {

  }

  ArduinoMKR1000 ::
    ~ArduinoMKR1000()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ArduinoMKR1000 ::
    Run_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    U8 data[2] ={0};
    Fw::Buffer buffer(data, 2);
    
    this->read_out(0, 0x08, buffer);
    
    U16 value = (data[0] << 8) | data[1];
    F32 voltage = value * 0.0048828125;
    F32 distance = 65 * pow(voltage, -1.10);

    this->tlmWrite_Distance(distance);
    this->tlmWrite_IRSensor(value);
  }

  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------


} // end namespace Components

