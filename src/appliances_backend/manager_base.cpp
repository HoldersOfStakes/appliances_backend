#include <appliances_backend/manager_base.h>


namespace appliances_backend
{
  void ManagerBase::start()
  {
    std::lock_guard<std::mutex> lock(managed_entities_access_);
    is_started_ = true;

    for(const std::pair<std::string, std::shared_ptr<ManageableBase>>& managed_entity_pair : managed_entities_)
    {
      std::cout << "Starting '" << managed_entity_pair.first << "'" << std::endl;
      managed_entity_pair.second->start();
      std::cout << "Started '" << managed_entity_pair.first << "'" << std::endl;
    }
  }

  void ManagerBase::stop()
  {
    std::lock_guard<std::mutex> lock(managed_entities_access_);
    is_started_ = false;

    for(const std::pair<std::string, std::shared_ptr<ManageableBase>>& managed_entity_pair : managed_entities_)
    {
      std::cout << "Stopping '" << managed_entity_pair.first << "'" << std::endl;
      managed_entity_pair.second->stop();
      std::cout << "Stopped '" << managed_entity_pair.first << "'" << std::endl;
    }
  }
}
