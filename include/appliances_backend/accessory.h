#ifndef __APPLIANCES_BACKEND_ACCESSORY_H__
#define __APPLIANCES_BACKEND_ACCESSORY_H__


// System
#include <string>
#include <list>
#include <memory>

// Private
#include <appliances_backend/service.h>


namespace appliances_backend
{
  class Accessory
  {
  public:
    Accessory(std::string key);
    virtual ~Accessory() = default;

    std::string getKey() const;

    std::shared_ptr<Service> addService(std::string key, Service::Type type);

    std::map<std::string, std::shared_ptr<Service>>::iterator servicesBegin();
    std::map<std::string, std::shared_ptr<Service>>::iterator servicesEnd();

    std::shared_ptr<Service> getService(std::string key);
    std::shared_ptr<Service> getPrimaryService();

    void setPrimaryServiceKey(std::string key);
    std::string getPrimaryServiceKey();

  private:
    std::string key_;
    std::string primary_service_key_;

    std::map<std::string, std::shared_ptr<Service>> services_;
  };
}


#endif /* __APPLIANCES_BACKEND_ACCESSORY_H__ */
