#ifndef __APPLIANCES_BACKEND_INTERFACES_HOMEBRIDGE_MQTT_H__
#define __APPLIANCES_BACKEND_INTERFACES_HOMEBRIDGE_MQTT_H__


// System
#include <memory>
#include <string>
#include <sstream>
#include <unistd.h>

// libmosquitto
#include <mosquitto.h>

// libproperty
#include <property/value.hpp>
#include <property/map.h>

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

      utilities::MqttClient mqtt_client_;

      void publishString(std::string topic, std::string payload);
    };
  }
}


#endif /* __APPLIANCES_BACKEND_INTERFACES_HOMEBRIDGE_MQTT_H__ */
