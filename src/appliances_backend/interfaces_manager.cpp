#include <appliances_backend/interfaces_manager.h>


namespace appliances_backend
{
  void InterfacesManager::setVariable(std::list<std::string> path_parts, nlohmann::json value)
  {
    if(!path_parts.empty())
    {
      for(const std::pair<std::string, std::shared_ptr<ManageableBase>> managed_entity_pair : managed_entities_)
      {
	std::shared_ptr<InterfaceBase> interface = std::dynamic_pointer_cast<InterfaceBase>(managed_entity_pair.second);
      
	if(interface == nullptr)
	{
	  throw std::runtime_error("Managed entity with key '" + managed_entity_pair.first + "' not of expected type.");
	}

	if(path_parts.front() == managed_entity_pair.first)
	{
	  path_parts.pop_front();
	  //std::cout << "Set interface variable: " << variable_path << " = " << value << std::endl;

	  interface->setVariable(path_parts, value);
	}
      }
    }
  }

  void InterfacesManager::registerAccessory(std::shared_ptr<Accessory> accessory)
  {
    for(const std::pair<std::string, std::shared_ptr<ManageableBase>> managed_entity_pair : managed_entities_)
    {
      std::shared_ptr<InterfaceBase> interface = std::dynamic_pointer_cast<InterfaceBase>(managed_entity_pair.second);
      
      if(interface == nullptr)
      {
	throw std::runtime_error("Managed entity with key '" + managed_entity_pair.first + "' not of expected type.");
      }

      interface->registerAccessory(accessory);
    }
  }

  void InterfacesManager::deregisterAccessory(std::string name)
  {
    for(const std::pair<std::string, std::shared_ptr<ManageableBase>> managed_entity_pair : managed_entities_)
    {
      std::shared_ptr<InterfaceBase> interface = std::dynamic_pointer_cast<InterfaceBase>(managed_entity_pair.second);
      
      if(interface == nullptr)
      {
	throw std::runtime_error("Managed entity with key '" + managed_entity_pair.first + "' not of expected type.");
      }

      interface->deregisterAccessory(name);
    }
  }

  std::map<std::string, nlohmann::json> InterfacesManager::getChangedVariables()
  {
    std::lock_guard<std::mutex> lock(managed_entities_access_);
    std::map<std::string, nlohmann::json> changed_variables;

    for(const std::pair<std::string, std::shared_ptr<ManageableBase>>& manageable_entity_pair : managed_entities_)
    {
      std::shared_ptr<InterfaceBase> interface = std::dynamic_pointer_cast<InterfaceBase>(manageable_entity_pair.second);

      if(interface == nullptr)
      {
	throw std::runtime_error("Managed entity with key '" + manageable_entity_pair.first + "' not of expected type.");
      }

      while(interface->wasVariableChanged())
      {
	std::pair<std::string, nlohmann::json> changed_variable = interface->getChangedVariable();
	changed_variables[manageable_entity_pair.first + "." + changed_variable.first] = changed_variable.second;
      }
    }

    return changed_variables;
  }
}
