#include <appliances_backend/service.h>


namespace appliances_backend
{
  Service::Service(std::string key, std::string label, Type type)
    : key_{ key }
    , label_{ label }
    , type_{ type }
  {
  }

  std::string Service::getKey() const
  {
    return key_;
  }

  std::string Service::getLabel() const
  {
    return label_;
  }

  Service::Type Service::getType() const
  {
    return type_;
  }
  
  std::shared_ptr<Characteristic> Service::addCharacteristic(Characteristic::Type type)
  {
    std::shared_ptr<Characteristic> characteristic = std::make_shared<Characteristic>();
    characteristics_[type] = characteristic;

    return characteristic;
  }

  std::shared_ptr<Characteristic> Service::getCharacteristic(Characteristic::Type type)
  {
    return characteristics_[type];
  }

  void Service::removeCharacteristic(Characteristic::Type type)
  {
    characteristics_.erase(type);
  }

  bool Service::hasCharacteristic(Characteristic::Type type)
  {
    return getCharacteristic(type) != nullptr;
  }
}
