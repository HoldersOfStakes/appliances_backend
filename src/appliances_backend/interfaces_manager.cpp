#include <appliances_backend/interfaces_manager.h>


namespace appliances_backend
{
  void setVariable(std::string variable_path, std::shared_ptr<property::RawData> value)
  {
    for(const std::pair<std::string, std::shared_ptr<ManageableBase>> managed_entity_pair : managed_entities_)
    {
      std::shared_ptr<InterfaceBase> interface = std::dynamic_pointer_cast<InterfaceBase>(manageable_entity_pair.second);
      
      if(interface == nullptr)
      {
	throw std::runtime_error("Managed entity with key '" + manageable_entity_pair.first + "' not of expected type.");
      }

      conditionallySetVariable(interface, variable_path, value);
    }
  }
}
