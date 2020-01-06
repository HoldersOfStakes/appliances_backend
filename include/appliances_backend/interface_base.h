#ifndef __APPLIANCES_BACKEND_INTERFACE_BASE_H__
#define __APPLIANCES_BACKEND_INTERFACE_BASE_H__


// Private
#include <appliances_backend/manageable_base.h>


namespace appliances_backend
{
  class InterfaceBase : public ManageableBase
  {
  public:
    InterfaceBase() = default;
    virtual ~InterfaceBase() = default;
  };
}


#endif /* __APPLIANCES_BACKEND_INTERFACE_BASE_H__ */
