#include <appliances_backend/utilities/modbus_client.h>


namespace appliances_backend
{
  namespace utilities
  {
    ModbusClient::ModbusClient(std::string host, unsigned short port, unsigned int slave_id)
      : modbus_connection_{ modbus_new_tcp(host.c_str(), port) }
    {
      response_timeout_.tv_sec = 3;
      response_timeout_.tv_usec = 0;

      modbus_set_response_timeout(modbus_connection_, &response_timeout_);
      modbus_set_slave(modbus_connection_, slave_id);

      if(modbus_connect(modbus_connection_) == -1)
      {
	modbus_free(modbus_connection_);

	throw std::runtime_error("Error (" + std::to_string(errno) + ") during initialization: " + std::string(modbus_strerror(errno)));
      }
    }

    ModbusClient::~ModbusClient()
    {
      modbus_close(modbus_connection_);
      modbus_free(modbus_connection_);
    }

    void ModbusClient::writeToRegister(unsigned int address, unsigned int count, uint16_t* value)
    {
      if(modbus_write_registers(modbus_connection_, address, count, value) != count)
      {
	throw std::runtime_error("Error (" + std::to_string(errno) + ") while writing to register: " + std::string(modbus_strerror(errno)));
      }
    }

    uint16_t* ModbusClient::readFromRegister(unsigned int address, unsigned int count)
    {
      uint16_t* value = new uint16_t[count];

      if(modbus_read_registers(modbus_connection_, address, count, value) != count)
      {
	delete[] value;
	throw std::runtime_error("Error (" + std::to_string(errno) + ") while reading from register: " + std::string(modbus_strerror(errno)));
      }

      return value;
    }
  }
}
