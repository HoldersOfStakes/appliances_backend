#include <appliances_backend/appliances/helios_kwl/helios_kwl_modbus_client.h>


namespace appliances_backend
{
  HeliosKwlModbusClient::HeliosKwlModbusClient(std::string host)
    : modbus_client_{ host, 502, 180 }
  {
  }

  unsigned int HeliosKwlModbusClient::readFanStage()
  {
    uint16_t* fan_stage_data = readVariable("v00102", 5);

    char txt[10];
    for(unsigned int i = 0; i < 10; ++i)
    {
      txt[i] = ((uint8_t*)fan_stage_data)[i + (i % 2 ? -1 : 1)];
    }

    delete[] fan_stage_data;

    std::string fan_stage_string(txt, 10);

    return std::stoi(fan_stage_string.substr(7));
  }

  void HeliosKwlModbusClient::writeFanStage(unsigned int fan_stage)
  {
    int retry_count = 5;

    while(retry_count > 0)
    {
      try
      {
	writeVariable("v00102", fan_stage, 5);
	break;
      }
      catch(const std::runtime_error& err)
      {
	std::cerr << "Error: " << err.what() << std::endl;
	usleep(100000);

	retry_count--;
      }
    }

    if(retry_count == 0)
    {
      std::cerr << "Giving up" << std::endl;
    }
  }

  double HeliosKwlModbusClient::readTemperatureSupplyAir()
  {
    return readTemperature("v00105");
  }

  double HeliosKwlModbusClient::readTemperatureExtractAir()
  {
    return readTemperature("v00107");
  }

  double HeliosKwlModbusClient::readTemperature(std::string variable)
  {
    uint16_t* temperature_data = readVariable(variable, 8);
    char raw_temperature[9];

    for(unsigned int i = 7; i < 16; ++i)
    {
      raw_temperature[i - 7] = ((uint8_t*)temperature_data)[i + (i % 2 ? -1 : 1)];
    }

    delete[] temperature_data;

    double temperature;
    sscanf(raw_temperature, "%lf", &temperature);

    return temperature;
  }

  uint16_t* HeliosKwlModbusClient::readVariable(std::string variable, unsigned int count)
  {
    // Encode variable name to read as uint16_t.
    uint16_t variable_as_uint16[4];
    for(unsigned int i = 0; i < 6; ++i)
    {
      ((uint8_t*)variable_as_uint16)[i + (i % 2 ? -1 : 1)] = static_cast<uint8_t>(variable.at(i));
    }

    variable_as_uint16[3] = 0;

    int retries = 5;

    while(retries > 0)
    {
      try
      {
	// Select variable to read.
	modbus_client_.writeToRegister(1, 4, variable_as_uint16);

	// Retrieve result.
	return modbus_client_.readFromRegister(1, count);
      }
      catch(const std::runtime_error& ex)
      {
	if(retries == 0)
	{
	  std::cerr << "Failed to read variable, giving up." << std::endl;
	  throw;
	}

	retries--;
	std::cerr << "Failed to read variable, retries left: " << retries << std::endl;

	usleep(100000);
      }
    }

    throw std::runtime_error("Failed to read variable.");
  }

  void HeliosKwlModbusClient::printHex(uint16_t* data, ssize_t length)
  {
    if(data == nullptr)
      {
	std::cout << "<null>" << std::endl;
      }
    else
      {
	for(unsigned int i = 0; i < length; ++i)
	  {
	    std::cout << "0x" << std::hex << data[i] << " ";
	  }

	std::cout << std::endl;
      }
  }
}
