#ifndef __APPLIANCES_BACKEND_INTERFACES_MQTT_H__
#define __APPLIANCES_BACKEND_INTERFACES_MQTT_H__


// System
#include <string>
#include <unistd.h>

// libmosquitto
#include <mosquitto.h>

// Private
#include <appliances_backend/interface_base.h>


namespace appliances_backend
{
  namespace interfaces
  {
    class Mqtt : public InterfaceBase
    {
    public:
      Mqtt(std::string host, unsigned int port);

    private:
    };
  }
}


#endif /* __APPLIANCES_BACKEND_INTERFACES_MQTT_H__ */
