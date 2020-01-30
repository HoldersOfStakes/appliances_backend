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


namespace appliances_backend
{
  namespace appliances
  {
    class IkeaTradfri : public ApplianceBase
    {
    public:
      IkeaTradfri(std::string host);

      void run() override;

      void setVariable(std::list<std::string> variable_parts, nlohmann::json value) override;

    private:
    };
  }
}


#endif /* __APPLIANCES_BACKEND_APPLIANCES_IKEA_TRADFRI_IKEA_TRADFRI_H__ */
