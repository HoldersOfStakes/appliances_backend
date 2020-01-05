#include <appliances_backend/appliances/helios_kwl.h>


namespace appliances_backend
{
  namespace appliances
  {
    HeliosKwl::HeliosKwl(std::string host)
      : helios_kwl_modbus_client_{ host }
      , fan_stage_{ 0 }
    {
    }

    unsigned int HeliosKwl::getFanStage()
    {
      return helios_kwl_modbus_client_.readFanStage();
    }

    void HeliosKwl::setFanStage(unsigned int fan_stage)
    {
      helios_kwl_modbus_client_.writeFanStage(fan_stage);
    }

    void HeliosKwl::run()
    {
      while(should_run_)
      {
	setVariableState("fan_stage", getFanStage());

	usleep(100000);
      }
    }
  }
}
