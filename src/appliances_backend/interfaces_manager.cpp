#include <appliances_backend/interfaces_manager.h>


namespace appliances_backend
{
  void InterfacesManager::setVariable(std::string variable_path, std::shared_ptr<property::RawData> value)
  {
    for(const std::pair<std::string, std::shared_ptr<ManageableBase>> managed_entity_pair : managed_entities_)
    {
      std::shared_ptr<InterfaceBase> interface = std::dynamic_pointer_cast<InterfaceBase>(managed_entity_pair.second);
      
      if(interface == nullptr)
      {
	throw std::runtime_error("Managed entity with key '" + managed_entity_pair.first + "' not of expected type.");
      }

      conditionallySetVariable(interface, variable_path, value);
    }
  }

  void InterfacesManager::registerAccessory(std::string name, std::shared_ptr<Accessory> accessory)
  {
    for(const std::pair<std::string, std::shared_ptr<ManageableBase>> managed_entity_pair : managed_entities_)
    {
      std::shared_ptr<InterfaceBase> interface = std::dynamic_pointer_cast<InterfaceBase>(managed_entity_pair.second);
      
      if(interface == nullptr)
      {
	throw std::runtime_error("Managed entity with key '" + managed_entity_pair.first + "' not of expected type.");
      }

      interface->registerAccessory(name, accessory);
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

  void InterfacesManager::conditionallySetVariable(std::shared_ptr<InterfaceBase> interface, std::string variable_path, std::shared_ptr<property::RawData> value)
  {
    // TODO(fairlight1337): Set variable in interfaces here.
  }
}
