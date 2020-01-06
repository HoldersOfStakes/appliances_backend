#ifndef __APPLIANCES_BACKEND_UTILITIES_MODBUS_CLIENT_H__
#define __APPLIANCES_BACKEND_UTILITIES_MODBUS_CLIENT_H__


// System
#include <string>
#include <stdexcept>

// libmodbus
#include <modbus/modbus.h>


namespace appliances_backend
{
  namespace utilities
  {
    class ModbusClient
    {
    public:
      ModbusClient(std::string host, unsigned short port, unsigned int slave_id);
      virtual ~ModbusClient();

      void writeToRegister(unsigned int address, unsigned int count, uint16_t* value);
      uint16_t* readFromRegister(unsigned int address, unsigned int count);

    private:
      modbus_t* modbus_connection_;
    };
  }
}


#endif /* __APPLIANCES_BACKEND_UTILITIES_MODBUS_CLIENT_H__ */
