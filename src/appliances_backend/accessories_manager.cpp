#include <appliances_backend/accessories_manager.h>


namespace appliances_backend
{
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
