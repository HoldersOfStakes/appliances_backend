#ifndef __APPLIANCES_BACKEND_APPLIANCES_IKEA_TRADFRI_IKEA_TRADFRI_H__
#define __APPLIANCES_BACKEND_APPLIANCES_IKEA_TRADFRI_IKEA_TRADFRI_H__


// System
#include <string>
#include <list>
#include <iostream>
#include <mutex>
#include <unistd.h>

// nlohmann
#include <nlohmann/json.hpp>

// Private
#include <appliances_backend/appliance_base.h>
#include <appliances_backend/appliances/ikea_tradfri/ikea_tradfri_coap_client.h>


namespace appliances_backend
{
  namespace appliances
  {
    class IkeaTradfri : public ApplianceBase
    {
    public:
      IkeaTradfri(std::string host, Log log);

      void run() override;

      void setVariable(std::list<std::string> variable_parts, nlohmann::json value) override;

    private:
    };
  }
}


#endif /* __APPLIANCES_BACKEND_APPLIANCES_IKEA_TRADFRI_IKEA_TRADFRI_H__ */
