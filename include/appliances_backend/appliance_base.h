#ifndef __APPLIANCES_BACKEND_APPLIANCE_BASE_H__
#define __APPLIANCES_BACKEND_APPLIANCE_BASE_H__


// System
#include <atomic>
#include <thread>
#include <map>
#include <deque>
#include <iostream>
#include <list>
#include <string>
#include <mutex>

// nlohmann
#include <nlohmann/json.hpp>

// Private
#include <appliances_backend/variable_managing_base.h>


namespace appliances_backend
{
  class ApplianceBase : public VariableManagingBase
  {
  public:
    ApplianceBase(Log log);

    virtual void setVariable(std::list<std::string> variable_parts, nlohmann::json value) = 0;
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCE_BASE_H__ */
