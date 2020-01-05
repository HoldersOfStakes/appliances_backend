#ifndef __APPLIANCES_BACKEND_MANAGER_BASE_H__
#define __APPLIANCES_BACKEND_MANAGER_BASE_H__


// System
#include <atomic>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>
#include <mutex>
#include <iostream>

// Private
#include <appliances_backend/manageable_base.h>


namespace appliances_backend
{
  class ManagerBase
  {
  public:
    ManagerBase() = default;
    virtual ~ManagerBase() = default;

    void start();
    void stop();

  protected:
    std::atomic<bool> is_started_;
    std::map<std::string, std::shared_ptr<ManageableBase>> managed_entities_;
    std::mutex managed_entities_access_;

    template<class TManageableType, class ... Args>
    void addManageableEntity(std::string key, Args ... args)
    {
      if(managed_entities_.find(key) != managed_entities_.end())
      {
	throw std::runtime_error("Key '" + key + "' already present.");
      }

      managed_entities_[key] = std::make_shared<TManageableType>(std::forward<Args>(args)...);
    }
  };
}


#endif /* __APPLIANCES_BACKEND_MANAGER_BASE_H__ */
