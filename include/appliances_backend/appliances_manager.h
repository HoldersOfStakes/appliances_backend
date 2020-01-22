#ifndef __APPLIANCES_BACKEND_APPLIANCES_MANAGER_H__
#define __APPLIANCES_BACKEND_APPLIANCES_MANAGER_H__


// System
#include <atomic>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>
#include <list>
#include <mutex>

// Private
#include <appliances_backend/appliance_base.h>
#include <appliances_backend/manager_base.h>


namespace appliances_backend
{
  class AppliancesManager : public ManagerBase<ApplianceBase>
  {
  public:
    AppliancesManager(Log log);

    void addAppliance(std::string type_key, std::string entity_key, nlohmann::json parameters)
    {
      instantiateRegisteredType(type_key, entity_key, parameters);
    }

    void setVariable(std::string appliance_key, std::list<std::string> variable_parts, nlohmann::json value);

    std::map<std::string, nlohmann::json> getChangedVariables();
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCES_MANAGER_H__ */
