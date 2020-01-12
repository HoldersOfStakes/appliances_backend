#ifndef __APPLIANCES_BACKEND_INTERFACE_BASE_H__
#define __APPLIANCES_BACKEND_INTERFACE_BASE_H__


// System
#include <atomic>
#include <thread>
#include <map>
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
    InterfaceBase() = default;

    virtual void registerAccessory(std::shared_ptr<Accessory> accessory) = 0;
    virtual void deregisterAccessory(std::string name) = 0;
  };
}


#endif /* __APPLIANCES_BACKEND_INTERFACE_BASE_H__ */
