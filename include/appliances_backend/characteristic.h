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
      Undefined = 0,
      RotationSpeed,
      On,
      CurrentTemperature
    };
    
    Characteristic(Type type);

    Type getType();

    void setProperty(std::string key, nlohmann::json property);
    nlohmann::json getProperty(std::string key);
    nlohmann::json getProperty(std::string key, nlohmann::json default_content);

    static Type parseTypeString(std::string type_string);
    std::string getTypeString();

  private:
    Type type_;
    std::map<std::string, nlohmann::json> properties_;
  };
}


#endif /* __APPLIANCES_BACKEND_CHARACTERISTIC_H__ */
