#ifndef __APPLIANCES_BACKEND_INTERFACES_HOMEBRIDGE_MQTT_H__
#define __APPLIANCES_BACKEND_INTERFACES_HOMEBRIDGE_MQTT_H__


// System
#include <memory>
#include <string>
#include <sstream>
#include <unistd.h>

// nlohmann
#include <nlohmann/json.hpp>

// libmosquitto
#include <mosquitto.h>

// Private
#include <appliances_backend/interface_base.h>
#include <appliances_backend/utilities/mqtt_client.h>


namespace appliances_backend
{
  namespace interfaces
  {
    class HomebridgeMqtt : public InterfaceBase
    {
    public:
      HomebridgeMqtt(std::string host, unsigned short port);

      void registerAccessory(std::shared_ptr<Accessory> accessory) override;
      void deregisterAccessory(std::string name) override;

    protected:
      void run() override;

    private:
      static std::string accessoryToJsonString(std::shared_ptr<Accessory> accessory);
      static std::string accessoryServiceToJsonString(std::shared_ptr<Accessory> accessory, std::shared_ptr<Service> service);

      utilities::MqttClient mqtt_client_;

      void publishString(std::string topic, std::string payload);
    };
  }
}


#endif /* __APPLIANCES_BACKEND_INTERFACES_HOMEBRIDGE_MQTT_H__ */
