#include <appliances_backend/appliances/helios_kwl/helios_kwl.h>


namespace appliances_backend
{
  namespace appliances
  {
    HeliosKwl::HeliosKwl(std::string host, Log log)
      : ApplianceBase{ log }
      , host_{ host }
    {
    }

    unsigned int HeliosKwl::getFanStage()
    {
      std::lock_guard<std::mutex> lock(modbus_access_);
      return helios_kwl_modbus_client_->readFanStage();
    }

    void HeliosKwl::setFanStage(unsigned int fan_stage)
    {
      std::lock_guard<std::mutex> lock(modbus_access_);
      helios_kwl_modbus_client_->writeFanStage(fan_stage);
    }

    double HeliosKwl::getTemperatureSupplyAir()
    {
      std::lock_guard<std::mutex> lock(modbus_access_);
      return helios_kwl_modbus_client_->readTemperatureSupplyAir();
    }

    double HeliosKwl::getTemperatureExtractAir()
    {
      std::lock_guard<std::mutex> lock(modbus_access_);
      return helios_kwl_modbus_client_->readTemperatureExtractAir();
    }

    void HeliosKwl::run()
    {
      helios_kwl_modbus_client_ = std::make_unique<HeliosKwlModbusClient>(host_);

      while(should_run_)
      {
	setVariableState("fan_stage", getFanStage());
	usleep(100000);
	setVariableState("temperature_supply_air", getTemperatureSupplyAir());
	usleep(100000);
	setVariableState("temperature_extract_air", getTemperatureExtractAir());
	usleep(100000);
      }

      helios_kwl_modbus_client_ = nullptr;
    }

    void HeliosKwl::setVariable(std::list<std::string> variable_parts, nlohmann::json value)
    {
      if(variable_parts.size() == 1 &&
	 variable_parts.front() == "fan_stage")
      {
	log() << Log::Severity::Debug << "Set fan stage to " << value.get<unsigned int>() << std::endl;

	setFanStage(value.get<unsigned int>());
      }
    }
  }
}
