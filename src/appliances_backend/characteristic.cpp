#include <appliances_backend/characteristic.h>


namespace appliances_backend
{
  void Characteristic::setProperty(std::string key, nlohmann::json content)
  {
    properties_[key] = content;
  }

  nlohmann::json Characteristic::getProperty(std::string key)
  {
    return properties_[key];
  }

  nlohmann::json Characteristic::getProperty(std::string key, nlohmann::json default_content)
  {
    if(properties_.find(key) != properties_.end())
    {
      return getProperty(key);
    }

    return default_content;
  }
}
