#ifndef __APPLIANCES_BACKEND_APPLIANCES_HELIOS_KWL_HELIOS_KWL_H__
#define __APPLIANCES_BACKEND_APPLIANCES_HELIOS_KWL_HELIOS_KWL_H__


// System
#include <string>
#include <unistd.h>

// Private
#include <appliances_backend/appliance_base.h>
#include <appliances_backend/appliances/helios_kwl/helios_kwl_modbus_client.h>


namespace appliances_backend
{
  namespace appliances
  {
    class HeliosKwl : public ApplianceBase
    {
    public:
      HeliosKwl(std::string host);

      unsigned int getFanStage();
      void setFanStage(unsigned int fan_stage);

      void run() override;

    private:
      HeliosKwlModbusClient helios_kwl_modbus_client_;

      unsigned int fan_stage_;
    };
  }
}


#endif /* __APPLIANCES_BACKEND_APPLIANCES_HELIOS_KWL_HELIOS_KWL_H__ */
