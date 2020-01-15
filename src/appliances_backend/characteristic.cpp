#include <appliances_backend/characteristic.h>


namespace appliances_backend
{
  Characteristic::Characteristic(Type type)
    : type_{ type }
  {
  }

  Characteristic::Type Characteristic::getType()
  {
    return type_;
  }

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

  Characteristic::Type Characteristic::parseTypeString(std::string type_string)
  {
    Type type;

    if(type_string == "RotationSpeed")
    {
      type = Type::RotationSpeed;
    }
    else if(type_string == "On")
    {
      type = Type::On;
    }
    else if(type_string == "CurrentTemperature")
    {
      type = Type::CurrentTemperature;
    }
    else
    {
      type = Type::Undefined;
    }

    return type;
  }

  std::string Characteristic::getTypeString()
  {
    switch(type_)
    {
    case Type::RotationSpeed:
      return "RotationSpeed";
      break;

    case Type::On:
      return "On";
      break;

    case Type::CurrentTemperature:
      return "CurrentTemperature";
      break;

    case Type::Undefined:
    default:
      return "Undefined";
      break;
    }
  }
}
