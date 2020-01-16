#include <appliances_backend/backend.h>


namespace appliances_backend
{
  Backend::Backend(std::string config_file_path)
    : config_file_path_{ config_file_path }
  {
  }

  void Backend::initialize()
  {
    loadConfiguration(config_file_path_);

    interfaces_manager_.start();
    appliances_manager_.start();
  }

  void Backend::deinitialize()
  {
    for(const std::string accessory_name : accessories_manager_.getAccessoryNames())
    {
      interfaces_manager_.deregisterAccessory(accessory_name);
    }

    appliances_manager_.stop();
    interfaces_manager_.stop();
  }

  void Backend::run()
  {
    should_run_ = true;

    for(const std::string accessory_name : accessories_manager_.getAccessoryNames())
    {
      interfaces_manager_.registerAccessory(accessories_manager_.getAccessory(accessory_name));
    }

    while(should_run_)
    {
      std::map<std::string, nlohmann::json> changed_variables = appliances_manager_.getChangedVariables();

      for(const std::pair<std::string, nlohmann::json>& changed_variable_pair : changed_variables)
      {
	std::cout << "From appliance: " << changed_variable_pair.first << " = " << changed_variable_pair.second << std::endl;

	for(const std::string& mapped_entity : mapper_.getMappedEntities(changed_variable_pair.first))
	{
	  std::list<std::string> path_parts = split(mapped_entity, '.');

	  if(path_parts.size() == 4)
	  {
	    std::string interface_key = path_parts.front();
	    path_parts.pop_front();
	    std::string accessory_key = path_parts.front();
	    path_parts.pop_front();
	    std::string service_key = path_parts.front();
	    path_parts.pop_front();
	    std::string characteristic_key = path_parts.front();

	    std::shared_ptr<Accessory> accessory = accessories_manager_.getAccessory(accessory_key);

	    if(accessory != nullptr)
	    {
	      std::shared_ptr<Service> service = accessory->getService(service_key);

	      if(service != nullptr)
	      {
		Characteristic::Type characteristic_type = Characteristic::parseTypeString(characteristic_key);

		if(service->hasCharacteristic(characteristic_type))
		{
		  std::shared_ptr<Characteristic> characteristic = service->getCharacteristic(characteristic_type);
	      
		  if(characteristic != nullptr)
		  {
		    interfaces_manager_.setVariable(interface_key, accessory, service, characteristic, changed_variable_pair.second);
		  }
		}
	      }
	    }
	  }
	}
      }

      changed_variables = interfaces_manager_.getChangedVariables();

      for(const std::pair<std::string, nlohmann::json>& changed_variable_pair : changed_variables)
      {
	std::cout << "From interface: " << changed_variable_pair.first << " = " << changed_variable_pair.second << std::endl;

	bool was_handled = false;
	{
	  std::list<std::string> path_parts = split(changed_variable_pair.first, '.');

	  if(path_parts.size() == 4)
	  {
	    std::string interface_key = path_parts.front();
	    path_parts.pop_front();
	    std::string accessory_key = path_parts.front();
	    path_parts.pop_front();
	    std::string service_key = path_parts.front();
	    path_parts.pop_front();
	    std::string characteristic_key = path_parts.front();

	    std::shared_ptr<Accessory> accessory = accessories_manager_.getAccessory(accessory_key);

	    if(accessory != nullptr)
	    {
	      std::shared_ptr<Service> service = accessory->getService(service_key);

	      if(service != nullptr)
	      {
		Characteristic::Type characteristic_type = Characteristic::parseTypeString(characteristic_key);

		if(service->hasCharacteristic(characteristic_type))
		{
		  std::shared_ptr<Characteristic> characteristic = service->getCharacteristic(characteristic_type);
	      
		  if(characteristic != nullptr)
		  {
		    if(characteristic_type == Characteristic::Type::On &&
		       characteristic->getProperty("always_on", false) == true)
		    {
		      interfaces_manager_.setVariable(interface_key, accessory, service, characteristic, true);
		      was_handled = true;
		    }
		  }
		}
	      }
	    }
	  }
	}

	if(!was_handled)
	{
	  for(const std::string& mapped_entity : mapper_.getMappedEntities(changed_variable_pair.first))
	  {
	    std::list<std::string> path_parts = split(mapped_entity, '.');

	    if(path_parts.size() > 0)
	    {
	      std::string appliance_key = path_parts.front();
	      path_parts.pop_front();
	    
	      appliances_manager_.setVariable(appliance_key, path_parts, changed_variable_pair.second);
	    }
	  }
	}
      }

      usleep(50000);
    }
  }

  void Backend::stop()
  {
    should_run_ = false;
  }

  std::list<std::string> Backend::split(const std::string& str, char delimeter)
  {
    std::list<std::string> result;

    std::size_t current, previous = 0;
    current = str.find(delimeter);

    while(current != std::string::npos)
    {
      result.push_back(str.substr(previous, current - previous));
      previous = current + 1;
      current = str.find(delimeter, previous);
    }
      
    result.emplace_back(str.substr(previous, current - previous));

    return result;
  }

  void Backend::loadConfiguration(std::string config_file_path)
  {
    interfaces_manager_.addInterface<interfaces::HomebridgeMqtt>("homebridge", "192.168.100.2", 1883);
    appliances_manager_.addAppliance<appliances::HeliosKwl>("helios", "192.168.100.14");

    std::shared_ptr<Accessory> ventilation = accessories_manager_.addAccessory("Lueftung");
    std::shared_ptr<Service> fan = ventilation->addService("Luefter", Service::Type::Fan);
    std::shared_ptr<Characteristic> rotation_speed = fan->addCharacteristic(Characteristic::Type::RotationSpeed);
    rotation_speed->setProperty("min_value", 1);
    rotation_speed->setProperty("max_value", 4);
    std::shared_ptr<Characteristic> on = fan->addCharacteristic(Characteristic::Type::On);
    on->setProperty("always_on", true);
    std::shared_ptr<Service> supply_air_temperature = ventilation->addService("Zuluft", Service::Type::TemperatureSensor);
    supply_air_temperature->addCharacteristic(Characteristic::Type::CurrentTemperature);
    std::shared_ptr<Service> extract_air_temperature = ventilation->addService("Abluft", Service::Type::TemperatureSensor);
    extract_air_temperature->addCharacteristic(Characteristic::Type::CurrentTemperature);

    ventilation->setPrimaryServiceKey("Luefter");

    mapper_.mapEntities("helios.fan_stage", "homebridge.Lueftung.Luefter.RotationSpeed", true);
    mapper_.mapEntities("helios.on", "homebridge.Lueftung.Luefter.On", true);
    mapper_.mapEntities("helios.temperature_supply_air", "homebridge.Lueftung.Zuluft.CurrentTemperature", true);
    mapper_.mapEntities("helios.temperature_extract_air", "homebridge.Lueftung.Abluft.CurrentTemperature", true);
  }
}
