#ifndef __APPLIANCES_BACKEND_INTERFACES_MANAGER_H__
#define __APPLIANCES_BACKEND_INTERFACES_MANAGER_H__


// System
#include <atomic>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>
#include <map>
#include <mutex>

// nlohmann
#include <nlohmann/json.hpp>

// Private
#include <appliances_backend/interface_base.h>
#include <appliances_backend/manager_base.h>
#include <appliances_backend/accessory.h>
#include <appliances_backend/service.h>
#include <appliances_backend/characteristic.h>


namespace appliances_backend
{
  class InterfacesManager : public ManagerBase<InterfaceBase>
  {
  public:
    InterfacesManager(Log log);

    void addInterface(std::string type_key, std::string entity_key, nlohmann::json parameters)
    {
      instantiateRegisteredType(type_key, entity_key, parameters);
    }

    void setVariable(std::string interface_key, std::shared_ptr<Accessory> accessory, std::shared_ptr<Service> service, std::shared_ptr<Characteristic> characteristic, nlohmann::json value);

    void registerAccessory(std::shared_ptr<Accessory> accessory);
    void deregisterAccessory(std::string name);

    std::map<std::string, nlohmann::json> getChangedVariables();
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCES_MANAGER_H__ */
