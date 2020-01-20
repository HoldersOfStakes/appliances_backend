#include <appliances_backend/manager_base.h>


namespace appliances_backend
{
  ManagerBase::ManagerBase(Log log)
    : LoggingBase{ log }
  {
  }

  ManagerBase::~ManagerBase()
  {
    stop();
  }

  void ManagerBase::start()
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

  void ManagerBase::stop()
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

  void ManagerBase::startManagedEntity(std::string key)
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

  void ManagerBase::stopManagedEntity(std::string key)
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
}
