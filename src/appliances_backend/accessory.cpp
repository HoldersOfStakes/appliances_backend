#include <appliances_backend/accessory.h>


namespace appliances_backend
{
  Accessory::Accessory(std::string key)
    : key_{ key }
    , primary_service_key_{ "" }
  {
  }

  std::string Accessory::getKey() const
  {
    return key_;
  }

  std::shared_ptr<Service> Accessory::addService(std::string key, std::string label, Service::Type type)
  {
    std::shared_ptr<Service> service = std::make_shared<Service>(key, label, type);
    services_[key] = service;

    return service;
  }

  std::map<std::string, std::shared_ptr<Service>>::iterator Accessory::servicesBegin()
  {
    return services_.begin();
  }

  std::map<std::string, std::shared_ptr<Service>>::iterator Accessory::servicesEnd()
  {
    return services_.end();
  }

  std::shared_ptr<Service> Accessory::getService(std::string key)
  {
    return services_[key];
  }

  std::shared_ptr<Service> Accessory::getPrimaryService()
  {
    return services_[primary_service_key_];
  }

  void Accessory::setPrimaryServiceKey(std::string key)
  {
    primary_service_key_ = key;
  }

  std::string Accessory::getPrimaryServiceKey()
  {
    return primary_service_key_;
  }
}
