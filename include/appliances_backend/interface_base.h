#ifndef __APPLIANCES_BACKEND_INTERFACE_BASE_H__
#define __APPLIANCES_BACKEND_INTERFACE_BASE_H__


// Private
#include <appliances_backend/manageable_base.h>
#include <appliances_backend/accessory.h>


namespace appliances_backend
{
  class InterfaceBase : public ManageableBase
  {
  public:
    InterfaceBase() = default;
    virtual ~InterfaceBase() = default;

    virtual void registerAccessory(std::string name, std::shared_ptr<Accessory> accessory) = 0;
    virtual void deregisterAccessory(std::string name) = 0;
  };
}


#endif /* __APPLIANCES_BACKEND_INTERFACE_BASE_H__ */
