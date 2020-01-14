#ifndef __APPLIANCES_BACKEND_HELIOS_KWL_MODBUS_CLIENT_H__
#define __APPLIANCES_BACKEND_HELIOS_KWL_MODBUS_CLIENT_H__


// System
#include <iostream>

// Private
#include <appliances_backend/utilities/modbus_client.h>


namespace appliances_backend
{
  class HeliosKwlModbusClient
  {
  public:
    HeliosKwlModbusClient(std::string host);
    virtual ~HeliosKwlModbusClient() = default;

    unsigned int readFanStage();
    void writeFanStage(unsigned int fan_stage);
    
    template<typename TValueType>
    void writeVariable(std::string variable, TValueType value, unsigned int register_count)
    {
      uint16_t value_to_write[register_count];

      variable += "=" + std::to_string(value); // Required syntax for setting the value.
      std::cout << "Set: " << variable << std::endl;

      for(unsigned int i = 0; i < register_count * 2 && i < variable.size(); ++i)
      {
	((uint8_t*)value_to_write)[i + (i % 2 ? -1 : 1)] = static_cast<uint8_t>(variable.at(i));
      }

      value_to_write[register_count - 1] = 0x00;

      printHex(value_to_write, register_count);

      modbus_client_.writeToRegister(1, register_count, value_to_write);
    }

    uint16_t* readVariable(std::string variable, unsigned int register_count);

  private:
    utilities::ModbusClient modbus_client_;

    void printHex(uint16_t* data, ssize_t length);
  };
}


#endif /* __APPLIANCES_BACKEND_HELIOS_KWL_MODBUS_CLIENT_H__ */
