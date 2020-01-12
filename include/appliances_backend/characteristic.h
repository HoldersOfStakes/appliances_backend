#ifndef __APPLIANCES_BACKEND_CHARACTERISTIC_H__
#define __APPLIANCES_BACKEND_CHARACTERISTIC_H__


// System
#include <memory>
#include <string>
#include <map>

// nlohmann
#include <nlohmann/json.hpp>


namespace appliances_backend
{
  class Characteristic
  {
  public:
    enum class Type
    {
      RotationSpeed = 0,
      AlwaysOn
    };
    
    Characteristic() = default;

    void setProperty(std::string key, nlohmann::json property);
    nlohmann::json getProperty(std::string key);
    nlohmann::json getProperty(std::string key, nlohmann::json default_content);

  private:
    std::map<std::string, nlohmann::json> properties_;
  };
}


#endif /* __APPLIANCES_BACKEND_CHARACTERISTIC_H__ */
