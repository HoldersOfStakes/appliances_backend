#include <appliances_backend/appliances_manager.h>


namespace appliances_backend
{
  AppliancesManager::AppliancesManager(Log log)
    : ManagerBase{ log }
  {
  }

  void AppliancesManager::setVariable(std::string appliance_key, std::list<std::string> variable_parts, nlohmann::json value)
  {
    if(managed_entities_.find(appliance_key) != managed_entities_.end())
    {
      std::shared_ptr<ApplianceBase> appliance = std::dynamic_pointer_cast<ApplianceBase>(managed_entities_[appliance_key]);
      
      if(appliance == nullptr)
      {
	log() << Log::Severity::Error << "Managed entity with key '" << appliance_key << "' not of expected type." << std::endl;
	throw std::runtime_error("Managed entity with key '" + appliance_key + "' not of expected type.");
      }

      appliance->setVariable(variable_parts, value);
    }
  }

  std::map<std::string, nlohmann::json> AppliancesManager::getChangedVariables()
  {
    std::lock_guard<std::mutex> lock(managed_entities_access_);
    std::map<std::string, nlohmann::json> changed_variables;

    for(const std::pair<std::string, std::shared_ptr<ManageableBase>>& manageable_entity_pair : managed_entities_)
    {
      std::shared_ptr<ApplianceBase> appliance = std::dynamic_pointer_cast<ApplianceBase>(manageable_entity_pair.second);

      if(appliance == nullptr)
      {
	log() << Log::Severity::Error << "Managed entity with key '" << manageable_entity_pair.first << "' not of expected type." << std::endl;
	throw std::runtime_error("Managed entity with key '" + manageable_entity_pair.first + "' not of expected type.");
      }

      while(appliance->wasVariableChanged())
      {
	std::pair<std::string, nlohmann::json> changed_variable = appliance->getChangedVariable();
	changed_variables[manageable_entity_pair.first + "." + changed_variable.first] = changed_variable.second;
      }
    }

    return changed_variables;
  }
}
