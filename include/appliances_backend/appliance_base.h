#ifndef __APPLIANCES_BACKEND_APPLIANCE_BASE_H__
#define __APPLIANCES_BACKEND_APPLIANCE_BASE_H__


// System
#include <atomic>
#include <thread>
#include <map>
#include <deque>
#include <iostream>
#include <mutex>

// Private
#include <appliances_backend/variable_managing_base.h>


namespace appliances_backend
{
  class ApplianceBase : public VariableManagingBase
  {
  public:
    ApplianceBase() = default;
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCE_BASE_H__ */
