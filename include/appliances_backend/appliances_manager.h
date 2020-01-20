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
  class AppliancesManager : public ManagerBase
  {
  public:
    AppliancesManager(Log log);

    template<class TApplianceType, class ... Args>
    void addAppliance(std::string key, Args ... args)
    {
      addManageableEntity<TApplianceType>(key, std::forward<Args>(args)...);
    }

    void setVariable(std::string appliance_key, std::list<std::string> variable_parts, nlohmann::json value);

    std::map<std::string, nlohmann::json> getChangedVariables();
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCES_MANAGER_H__ */
