#include <appliances_backend/interfaces/homebridge/homebridge_mqtt.h>


namespace appliances_backend
{
  namespace interfaces
  {
    HomebridgeMqtt::HomebridgeMqtt(std::string host, unsigned short port, Log log)
      : InterfaceBase{ log }
      , mqtt_client_{ host, port }
    {
    }

    void HomebridgeMqtt::registerAccessory(std::shared_ptr<Accessory> accessory)
    {
      log() << Log::Severity::Debug << "Register accessory '" << accessory->getKey() << "'" << std::endl;

      publishString("homebridge/to/add", accessoryServiceToJsonString(accessory, accessory->getPrimaryService()));

      for(std::map<std::string, std::shared_ptr<Service>>::iterator service_iterator = accessory->servicesBegin();
	  service_iterator != accessory->servicesEnd(); ++service_iterator)
      {
	if(service_iterator->first != accessory->getPrimaryServiceKey())
	{
	  publishString("homebridge/to/add/service", accessoryServiceToJsonString(accessory, service_iterator->second));
	}

	if(service_iterator->second->hasCharacteristic(Characteristic::Type::On) &&
	   service_iterator->second->getCharacteristic(Characteristic::Type::On)->getProperty("always_on", false) == true)
	{
	  publishString("homebridge/to/set", "{\"name\": \"" + accessory->getKey() + "\", \"service_name\": \"" + service_iterator->second->getKey() + "\", \"characteristic\": \"On\", \"value\": true}");
	}
      }
    }

    void HomebridgeMqtt::deregisterAccessory(std::string name)
    {
      log() << Log::Severity::Debug << "Deregister accessory '" << name << "'" << std::endl;

      publishString("homebridge/to/remove", "{\"name\": \"" + name + "\"}");
    }

    void HomebridgeMqtt::setVariable(std::shared_ptr<Accessory> accessory, std::shared_ptr<Service> service, std::shared_ptr<Characteristic> characteristic, nlohmann::json value)
    {
      nlohmann::json data;
      data["name"] = accessory->getKey();
      data["service_name"] = service->getKey();
      data["characteristic"] = characteristic->getTypeString();
      data["value"] = value;

      setVariableState(data["name"].get<std::string>() + "." + data["service_name"].get<std::string>() + "." + data["characteristic"].get<std::string>(), value);

      publishString("homebridge/to/set", data.dump());
    }

    void HomebridgeMqtt::initialize()
    {
      while(true)
      {
	try
	{
	  log() << Log::Severity::Debug << "Connecting to MQTT" << std::endl;
	  mqtt_client_.connect();
	  break;
	}
	catch(const std::runtime_error& error)
	{
	  std::cout << error.what() << std::endl;
	}
      }
    }

    void HomebridgeMqtt::run()
    {
      using namespace std::chrono_literals;

      std::string homebridge_control_in_topic = "homebridge/from/set";
      mqtt_client_.subscribe(homebridge_control_in_topic);

      while(should_run_)
      {
	if(mqtt_client_.wasDataReceivedOnTopic(homebridge_control_in_topic))
	{
	  std::pair<char*, ssize_t> data = mqtt_client_.getNextReceivedDataOnTopic(homebridge_control_in_topic);
	  std::string message = std::string(data.first, data.second);

	  std::cout << "Message: " << message << std::endl;

	  nlohmann::json parsed = nlohmann::json::parse(message);
	  std::string variable_name = parsed["name"].get<std::string>() + "." + parsed["service_name"].get<std::string>() + "." + parsed["characteristic"].get<std::string>();

	  setVariableState(variable_name, parsed["value"]);

	  delete[] data.first;
	}

	waitFor(50ms);
      }

      // TODO(fairlight1337): Unsubscribe here.
    }

    std::string HomebridgeMqtt::accessoryToJsonString(std::shared_ptr<Accessory> accessory)
    {
      nlohmann::json data;
      data["name"] = accessory->getKey();

      return data.dump();
    }

    std::string HomebridgeMqtt::accessoryServiceToJsonString(std::shared_ptr<Accessory> accessory, std::shared_ptr<Service> service)
    {
      nlohmann::json data;
      data["name"] = accessory->getKey();

      std::string service_type;

      switch(service->getType())
      {
      case Service::Type::Fan:
	data["service"] = "Fan";

	if(service->hasCharacteristic(Characteristic::Type::RotationSpeed))
	{
	  data["RotationSpeed"]["minValue"] = service->getCharacteristic(Characteristic::Type::RotationSpeed)->getProperty("min_value");
	  data["RotationSpeed"]["maxValue"] = service->getCharacteristic(Characteristic::Type::RotationSpeed)->getProperty("max_value");
	}
	break;

      case Service::Type::TemperatureSensor:
	data["service"] = "TemperatureSensor";
	break;

      default:
       	data["service"] = "Undefined";
	break;
      }

      data["service_name"] = service->getKey();

      return data.dump();
    }

    void HomebridgeMqtt::publishString(std::string topic, std::string payload)
    {
      std::cout << topic << ": " << payload << std::endl;
      mqtt_client_.publish(topic, const_cast<char*>(payload.c_str()), payload.size());
    }
  }
}
