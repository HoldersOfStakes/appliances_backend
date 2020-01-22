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
#include <appliances_backend/logging_base.h>
#include <appliances_backend/factory.h>


namespace appliances_backend
{
  template<class TManagedType>
  class ManagerBase : public LoggingBase
  {
  public:
    ManagerBase(Log log)
      : LoggingBase{ log }
    {
    }

    ~ManagerBase() override
    {
      stop();
    }

    void start()
    {
      if(!is_started_)
      {
	std::lock_guard<std::mutex> lock(managed_entities_access_);
	is_started_ = true;

	for(const std::pair<std::string, std::shared_ptr<ManageableBase>>& managed_entity_pair : managed_entities_)
	{
	  startManagedEntity(managed_entity_pair.first);
	}
      }
    }

    void stop()
    {
      if(is_started_)
      {
	std::lock_guard<std::mutex> lock(managed_entities_access_);
	is_started_ = false;

	for(const std::pair<std::string, std::shared_ptr<ManageableBase>>& managed_entity_pair : managed_entities_)
	{
	  stopManagedEntity(managed_entity_pair.first);
	}
      }
    }

    void registerType(std::string type_key, std::function<std::shared_ptr<TManagedType>(nlohmann::json)> generator)
    {
      factory_.registerType(type_key, generator);
    }

    std::shared_ptr<TManagedType> instantiateRegisteredType(std::string type_key, std::string entity_key, nlohmann::json parameters)
    {
      if(!validateInputParametersForRegisteredType(type_key, parameters))
      {
	throw std::runtime_error("Failed to validate input parameters for entity '" + entity_key + "' (type '" + type_key + "').");
      }

      std::shared_ptr<TManagedType> entity = factory_.instantiate(type_key, parameters);
      addManageableEntity(entity_key, entity);

      return entity;
    }

  protected:
    std::atomic<bool> is_started_;
    std::map<std::string, std::shared_ptr<ManageableBase>> managed_entities_;
    std::mutex managed_entities_access_;
    Factory<TManagedType> factory_;

    template<class TManageableType>
    void addManageableEntity(std::string key, std::shared_ptr<TManageableType> entity)
    {
      {
	std::lock_guard<std::mutex> lock(managed_entities_access_);

	if(managed_entities_.find(key) != managed_entities_.end())
	{
	  throw std::runtime_error("Key '" + key + "' already present.");
	}

	managed_entities_[key] = entity;
      }

      if(is_started_)
      {
	startManagedEntity(key);
      }
    }

    void startManagedEntity(std::string key)
    {
      if(managed_entities_.find(key) == managed_entities_.end())
      {
	log() << Log::Severity::Error << "Cannot find managed entity to start with key '" << key << "'." << std::endl;
	throw std::runtime_error("Cannot find managed entity to start with key '" + key + "'.");
      }

      log() << Log::Severity::Info << "Starting '" << key << "'" << std::endl;
      managed_entities_[key]->start();
      log() << Log::Severity::Info << "Started '" << key << "'" << std::endl;
    }

    void stopManagedEntity(std::string key)
    {
      if(managed_entities_.find(key) == managed_entities_.end())
      {
	log() << Log::Severity::Error << "Cannot find managed entity to stop with key '" << key << "'." << std::endl;
	throw std::runtime_error("Cannot find managed entity to stop with key '" + key + "'.");
      }

      log() << Log::Severity::Info << "Stopping '" << key << "'" << std::endl;
      managed_entities_[key]->stop();
      log() << Log::Severity::Info << "Stopped '" << key << "'" << std::endl;
    }

    bool validateInputParametersForRegisteredType(std::string type_key, nlohmann::json parameters)
    {
      // TODO(fairlight1337): Implement json schema storage and validate the parameters here.

      return true;
    }
  };
}


#endif /* __APPLIANCES_BACKEND_MANAGER_BASE_H__ */
