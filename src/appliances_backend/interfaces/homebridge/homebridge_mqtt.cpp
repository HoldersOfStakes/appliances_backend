#include <appliances_backend/interfaces/homebridge/homebridge_mqtt.h>


namespace appliances_backend
{
  namespace interfaces
  {
    HomebridgeMqtt::HomebridgeMqtt(std::string host, unsigned short port)
      : mqtt_client_{ host, port }
    {
    }

    void HomebridgeMqtt::registerAccessory(std::string name, std::shared_ptr<Accessory> accessory)
    {
      std::cout << "Register " << name << std::endl;

      publishString("homebridge/to/add", "{\"name\": \"" + name + "\", \"service\": \"Fan\", \"RotationSpeed\": \"level\", \"minValue\": 0, \"maxValue\": 4}");
    }

    void HomebridgeMqtt::deregisterAccessory(std::string name)
    {
      std::cout << "Deregister " << name << std::endl;

      publishString("homebridge/to/remove", "{\"name\": \"" + name + "\"}");
    }

    void HomebridgeMqtt::run()
    {
      mqtt_client_.subscribe("topic_name");

      while(should_run_)
      {
	if(mqtt_client_.wasDataReceivedOnTopic("topic_name"))
	{
	  std::pair<char*, ssize_t> data = mqtt_client_.getNextReceivedDataOnTopic("topic_name");

	  std::cout << "Message: " << std::string(data.first, data.second) << std::endl;

	  delete[] data.first;
	}

	usleep(50000);
      }

      // TODO(fairlight1337): Unsubscribe here.
    }

    void HomebridgeMqtt::publishString(std::string topic, std::string payload)
    {
      mqtt_client_.publish(topic, const_cast<char*>(payload.c_str()), payload.size());
    }
  }
}
