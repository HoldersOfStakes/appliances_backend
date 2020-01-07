#include <appliances_backend/interfaces/homebridge/homebridge_mqtt.h>


namespace appliances_backend
{
  namespace interfaces
  {
    HomebridgeMqtt::HomebridgeMqtt(std::string host, unsigned short port)
      : mqtt_client_{ host, port }
    {
    }

    void HomebridgeMqtt::registerAccessory(std::shared_ptr<Accessory> accessory)
    {
      std::cout << "Register accessory '" << accessory->getName() << "'" << std::endl;

      publishString("homebridge/to/add", accessoryToJsonString(accessory));

      if(!accessory->getCanBeSwitchedOff())
      {
	publishString("homebridge/to/set", "{\"name\": \"" + accessory->getName() + "\", \"service_name\": \"" + accessory->getLabel() + "\", \"characteristic\": \"On\", \"value\": true}");
      }
    }

    void HomebridgeMqtt::deregisterAccessory(std::string name)
    {
      std::cout << "Deregister accessory '" << name << "'" << std::endl;

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

    std::string HomebridgeMqtt::accessoryToJsonString(std::shared_ptr<Accessory> accessory)
    {
      std::shared_ptr<property::Map> data = std::make_shared<property::Map>();
      data->set("name", std::make_shared<property::Value<std::string>>(accessory->getName()));

      switch(accessory->getType())
      {
      case AccessoryType::Fan:
	data->set("service", std::make_shared<property::Value<std::string>>("Fan"));

	std::shared_ptr<property::Map> rotation_speed = std::make_shared<property::Map>();
	rotation_speed->set("minValue", std::make_shared<property::Value<double>>(accessory->getMinValue()));
	rotation_speed->set("maxValue", std::make_shared<property::Value<double>>(accessory->getMaxValue()));

	data->set("RotationSpeed", rotation_speed);
      }

      data->set("service_name", std::make_shared<property::Value<std::string>>(accessory->getLabel()));

      std::stringstream sts;
      sts << std::dynamic_pointer_cast<property::RawData>(data);

      return sts.str();
    }

    void HomebridgeMqtt::publishString(std::string topic, std::string payload)
    {
      mqtt_client_.publish(topic, const_cast<char*>(payload.c_str()), payload.size());
    }
  }
}
