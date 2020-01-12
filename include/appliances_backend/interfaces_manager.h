#ifndef __APPLIANCES_BACKEND_INTERFACES_MANAGER_H__
#define __APPLIANCES_BACKEND_INTERFACES_MANAGER_H__


// System
#include <atomic>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>
#include <mutex>

// nlohmann
#include <nlohmann/json.hpp>

// Private
#include <appliances_backend/interface_base.h>
#include <appliances_backend/manager_base.h>


namespace appliances_backend
{
  class InterfacesManager : public ManagerBase
  {
  public:
    InterfacesManager() = default;

    template<class TApplianceType, class ... Args>
    void addInterface(std::string key, Args ... args)
    {
      addManageableEntity<TApplianceType>(key, std::forward<Args>(args)...);
    }

    void setVariable(std::string variable_path, nlohmann::json value);

    void registerAccessory(std::shared_ptr<Accessory> accessory);
    void deregisterAccessory(std::string name);

    std::map<std::string, nlohmann::json> getChangedVariables();
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCES_MANAGER_H__ */
