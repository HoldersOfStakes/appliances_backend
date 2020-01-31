#ifndef __APPLIANCES_BACKEND_INTERFACE_BASE_H__
#define __APPLIANCES_BACKEND_INTERFACE_BASE_H__


// System
#include <atomic>
#include <thread>
#include <map>
#include <list>
#include <deque>
#include <iostream>
#include <mutex>

// Private
#include <appliances_backend/variable_managing_base.h>
#include <appliances_backend/accessory.h>


namespace appliances_backend
{
  class InterfaceBase : public VariableManagingBase
  {
  public:
    InterfaceBase(Log log);

    virtual void registerAccessory(std::shared_ptr<Accessory> accessory) = 0;
    virtual void deregisterAccessory(std::string name) = 0;

    virtual void setVariable(std::shared_ptr<Accessory> accessory, std::shared_ptr<Service> service, std::shared_ptr<Characteristic> characteristic, nlohmann::json value) = 0;
  };
}


#endif /* __APPLIANCES_BACKEND_INTERFACE_BASE_H__ */
