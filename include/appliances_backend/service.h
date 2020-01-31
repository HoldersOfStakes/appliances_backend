#ifndef __APPLIANCES_BACKEND_SERVICE_H__
#define __APPLIANCES_BACKEND_SERVICE_H__


// System
#include <string>
#include <map>

// Private
#include <appliances_backend/characteristic.h>


namespace appliances_backend
{
  class Service
  {
  public:
    enum class Type
    {
      Undefined = 0,
      Fan,
      TemperatureSensor,
      Light
    };

    Service(std::string key, Type type);
    virtual ~Service() = default;

    std::string getKey() const;
    Type getType() const;

    std::shared_ptr<Characteristic> addCharacteristic(Characteristic::Type type);
    std::shared_ptr<Characteristic> getCharacteristic(Characteristic::Type type);
    void removeCharacteristic(Characteristic::Type type);
    bool hasCharacteristic(Characteristic::Type type);

    static Type parseTypeString(std::string type_string);

  private:
    std::string key_;
    Type type_;

    std::map<Characteristic::Type, std::shared_ptr<Characteristic>> characteristics_;
  };
}


#endif /* __APPLIANCES_BACKEND_SERVICE_H__ */
