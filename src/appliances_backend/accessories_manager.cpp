#include <appliances_backend/accessories_manager.h>


namespace appliances_backend
{
  void AccessoriesManager::addAccessory(std::string name, AccessoryType accessory_type)
  {
    if(accessories_.find(name) != accessories_.end())
    {
      throw std::runtime_error("Accessory name already present: '" + name + "'");
    }

    accessories_[name] = std::make_shared<Accessory>(accessory_type);
  }

  std::vector<std::string> AccessoriesManager::getAccessoryNames()
  {
    std::vector<std::string> accessory_names;

    for(const std::pair<std::string, std::shared_ptr<Accessory>> accessory_pair : accessories_)
    {
      accessory_names.emplace_back(accessory_pair.first);
    }

    return accessory_names;
  }

  std::shared_ptr<Accessory> AccessoriesManager::getAccessory(std::string name)
  {
    if(accessories_.find(name) == accessories_.end())
    {
      throw std::runtime_error("Couldn't find accessory by the name '" + name + "'.");
    }

    return accessories_[name];
  }
}
