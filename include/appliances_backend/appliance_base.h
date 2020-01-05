#ifndef __APPLIANCES_BACKEND_APPLIANCE_BASE_H__
#define __APPLIANCES_BACKEND_APPLIANCE_BASE_H__


// System
#include <atomic>
#include <thread>
#include <map>
#include <deque>
#include <iostream>
#include <mutex>

// libproperty
#include <property/value.hpp>

// Private
#include <appliances_backend/manageable_base.h>


namespace appliances_backend
{
  class ApplianceBase : public ManageableBase
  {
  public:
    ApplianceBase() = default;
    virtual ~ApplianceBase() = default;

    void start() override;
    void stop() override;
    
    bool wasVariableChanged();
    std::pair<std::string, std::shared_ptr<property::RawData>> getChangedVariable();

  protected:
    std::atomic<bool> should_run_;
    virtual void run() = 0;

    template<typename TValueType>
    void setVariableState(std::string variable_path, TValueType value)
    {
      setRawVariableState(variable_path, std::make_shared<property::Value<TValueType>>(value));
    }

  private:
    std::thread worker_thread_;
    std::deque<std::string> changed_variables_;
    std::map<std::string, std::shared_ptr<property::RawData>> variable_states_;
    std::mutex variable_access_mutex_;

    void setRawVariableState(std::string variable_path, std::shared_ptr<property::RawData> content);
  };
}


#endif /* __APPLIANCES_BACKEND_APPLIANCE_BASE_H__ */
