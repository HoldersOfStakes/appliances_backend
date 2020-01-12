#ifndef __APPLIANCES_BACKEND_VARIABLE_MANAGING_BASE_H__
#define __APPLIANCES_BACKEND_VARIABLE_MANAGING_BASE_H__


// System
#include <atomic>
#include <thread>
#include <map>
#include <deque>
#include <iostream>
#include <mutex>

// nlohmann
#include <nlohmann/json.hpp>

// Private
#include <appliances_backend/manageable_base.h>


namespace appliances_backend
{
  class VariableManagingBase : public ManageableBase
  {
  public:
    VariableManagingBase() = default;

    bool wasVariableChanged();
    std::pair<std::string, nlohmann::json> getChangedVariable();

  protected:
    void setVariableState(std::string variable_path, nlohmann::json value)
    {
      setRawVariableState(variable_path, value);
    }

  private:
    std::deque<std::string> changed_variables_;
    std::map<std::string, nlohmann::json> variable_states_;
    std::mutex variable_access_mutex_;

    void setRawVariableState(std::string variable_path, nlohmann::json content);
  };
}


#endif /* __APPLIANCES_BACKEND_VARIABLE_MANAGING_BASE_H__ */
