#include <appliances_backend/appliances_manager.h>


namespace appliances_backend
{
  std::map<std::string, nlohmann::json> AppliancesManager::getChangedVariables()
  {
    std::lock_guard<std::mutex> lock(managed_entities_access_);
    std::map<std::string, nlohmann::json> changed_variables;

    for(const std::pair<std::string, std::shared_ptr<ManageableBase>>& manageable_entity_pair : managed_entities_)
    {
      std::shared_ptr<ApplianceBase> appliance = std::dynamic_pointer_cast<ApplianceBase>(manageable_entity_pair.second);

      if(appliance == nullptr)
      {
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
