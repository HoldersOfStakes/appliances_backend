#include <appliances_backend/backend.h>


namespace appliances_backend
{
  Backend::Backend(std::string config_file_path)
    : config_file_path_{ config_file_path }
    , log_{ "Backend" }
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
        log_ << Log::Severity::Debug << "From appliance: " << changed_variable_pair.first << " = " << changed_variable_pair.second << std::endl;

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
        log_ << Log::Severity::Debug << "From interface: " << changed_variable_pair.first << " = " << changed_variable_pair.second << std::endl;

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

  bool Backend::fileExists(std::string file_path)
  {
    return access(file_path.c_str(), F_OK) != -1;
  }

  void Backend::loadConfiguration(std::string config_file_path)
  {
    if(fileExists(config_file_path))
    {
      log_ << Log::Severity::Info << "Loading file '" << config_file_path << "'" << std::endl;
      Log file_loading_log = log_.deriveLogLevel();

      using namespace libconfig;

      Config cfg;
      cfg.readFile(config_file_path.c_str());

      const Setting& root = cfg.getRoot();

      // Load appliances.
      file_loading_log << Log::Severity::Info << "Loading appliances" << std::endl;

      try
      {
	const Setting& appliances = root["appliances"];
	size_t appliances_count = appliances.getLength();

	for(unsigned int appliance_index = 0; appliance_index < appliances_count; ++appliance_index)
	{
	  const Setting& appliance = appliances[appliance_index];
	  loadAppliance(appliance, file_loading_log.deriveLogLevel());
	}
      }
      catch(const SettingNotFoundException& ex)
      {
	// Ignore.
      }

      // Load interfaces.
      file_loading_log << Log::Severity::Info << "Loading interfaces" << std::endl;

      try
      {
	const Setting& interfaces = root["interfaces"];
	size_t interfaces_count = interfaces.getLength();

	for(unsigned int interface_index = 0; interface_index < interfaces_count; ++interface_index)
	{
	  const Setting& interface = interfaces[interface_index];
	  loadInterface(interface, file_loading_log.deriveLogLevel());
	}
      }
      catch(const SettingNotFoundException& ex)
      {
	// Ignore.
      }

      // Load accessories.
      file_loading_log << Log::Severity::Info << "Loading accessories" << std::endl;

      try
      {
	const Setting& accessories = root["accessories"];
	size_t accessories_count = accessories.getLength();

	for(unsigned int accessory_index = 0; accessory_index < accessories_count; ++accessory_index)
	{
	  const Setting& accessory = accessories[accessory_index];
	  loadAccessory(accessory, file_loading_log.deriveLogLevel());
	}
      }
      catch(const SettingNotFoundException& ex)
      {
	// Ignore.
      }

      // Load mappings.
      file_loading_log << Log::Severity::Info << "Loading mappings" << std::endl;

      try
      {
	const Setting& mappings = root["mappings"];
	size_t mappings_count = mappings.getLength();

	for(unsigned int mapping_index = 0; mapping_index < mappings_count; ++mapping_index)
	{
	  const Setting& mapping = mappings[mapping_index];
	  loadMapping(mapping, file_loading_log.deriveLogLevel());
	}
      }
      catch(const SettingNotFoundException& ex)
      {
	// Ignore.
      }

      /*interfaces_manager_.addInterface<interfaces::HomebridgeMqtt>("homebridge", "192.168.100.2", 1883);
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
      mapper_.mapEntities("helios.temperature_extract_air", "homebridge.Lueftung.Abluft.CurrentTemperature", true);*/

      log_ << Log::Severity::Info << "Finished loading file '" << config_file_path << "'" << std::endl;
    }
    else
    {
      throw std::runtime_error("Config file does not exist: '" + config_file_path + "'");
    }
  }

  void Backend::loadAppliance(const libconfig::Setting& appliance_description, Log log)
  {
    using namespace libconfig;

    std::string appliance_key = appliance_description.getName();

    log << Log::Severity::Info << "Instantiating appliance '" << appliance_key << "'" << std::endl;

    // ...
  }
  
  void Backend::loadInterface(const libconfig::Setting& interface_description, Log log)
  {
    using namespace libconfig;

    std::string interface_key = interface_description.getName();

    log << Log::Severity::Info << "Instantiating interface '" << interface_key << "'" << std::endl;

    // ...
  }
  
  void Backend::loadAccessory(const libconfig::Setting& accessory_description, Log log)
  {
    using namespace libconfig;

    std::string name;
    size_t loaded_services_count = 0;
    std::string primary_service = "";
    bool primary_service_set = accessory_description.lookupValue("primary_service", primary_service);

    if(accessory_description.lookupValue("name", name))
    {
      log << Log::Severity::Info << "Loading accessory '" << name << "'" << std::endl;

      std::shared_ptr<Accessory> accessory_object = accessories_manager_.addAccessory(name);

      try
      {
	const Setting& services = accessory_description["services"];
	size_t services_count = services.getLength();

	Log service_loading_log = log.deriveLogLevel();

	for(unsigned int service_index = 0; service_index < services_count; ++service_index)
	{
	  const Setting& service = services[service_index];

	  std::string service_name;
	  std::string service_type;
	  
	  if(service.lookupValue("name", service_name) &&
	     service.lookupValue("type", service_type))
	  {
	    Service::Type service_type_symbol = Service::parseTypeString(service_type);

	    if(service_type_symbol != Service::Type::Undefined)
	    {
	      service_loading_log << Log::Severity::Info << "Adding service '" << service_name << "' of type '" << service_type << "'." << std::endl;
	      std::shared_ptr<Service> service_object = accessory_object->addService(service_name, service_type_symbol);

	      try
	      {
		const Setting& characteristics = service["characteristics"];
		size_t characteristics_count = characteristics.getLength();

		Log characteristic_loading_log = service_loading_log.deriveLogLevel();

		for(unsigned int characteristic_index = 0; characteristic_index < characteristics_count; ++characteristic_index)
		{
		  const Setting& characteristic = characteristics[characteristic_index];
		  Characteristic::Type characteristic_type = Characteristic::parseTypeString(characteristic.getName());

		  if(characteristic_type != Characteristic::Type::Undefined)
		  {
		    characteristic_loading_log << Log::Severity::Info << "Adding characteristic of type '" << characteristic.getName() << "'" << std::endl;

		    std::shared_ptr<Characteristic> characteristic_object = service_object->addCharacteristic(characteristic_type);

		    size_t parameters_count = characteristic.getLength();
		    Log parameter_loading_log = characteristic_loading_log.deriveLogLevel();

		    for(unsigned int parameter_index = 0; parameter_index < parameters_count; ++parameter_index)
		    {
		      const Setting& parameter = characteristic[parameter_index];
		      std::string parameter_name = parameter.getName();

		      bool type_ok = true;

		      switch(parameter.getType())
		      {
		      case Setting::TypeInt:
		      case Setting::TypeInt64:
			characteristic_object->setProperty(parameter_name, static_cast<int>(parameter));
			break;

		      case Setting::TypeFloat:
			characteristic_object->setProperty(parameter_name, static_cast<double>(parameter));
			break;

		      case Setting::TypeString:
			characteristic_object->setProperty(parameter_name, static_cast<const char*>(parameter));
			break;

		      case Setting::TypeBoolean:
			characteristic_object->setProperty(parameter_name, static_cast<bool>(parameter));
			break;

		      default:
		        parameter_loading_log << Log::Severity::Error << "Value of parameter '" << parameter_name << "' has unknown type." << std::endl;
			type_ok = false;
			break;
		      }

		      if(type_ok)
		      {
		        parameter_loading_log << Log::Severity::Info << "Setting parameter '" << parameter_name << "' = " << characteristic_object->getProperty(parameter_name) << std::endl;
		      }
		    }
		  }
		  else
		  {
		    log << Log::Severity::Error << "Unknown characteristic type '" << characteristic.getName() << "'" << std::endl;
		  }
		}
	      }
	      catch(const SettingNotFoundException& ex)
	      {
		// Ignore.
	      }
	    }
	    else
	    {
	      log << Log::Severity::Error << "Service '" << service_name << "' has unknown type '" << service_type << "'." << std::endl;
	    }

	    loaded_services_count++;
	  }
	}
      }
      catch(const SettingNotFoundException& ex)
      {
        log << Log::Severity::Error << "Faulty accessory description." << std::endl;
      }
    }
    else
    {
      log << Log::Severity::Error << "Accessory definition lacks name property." << std::endl;
    }
  }

  void Backend::loadMapping(const libconfig::Setting& mapping_description, Log log)
  {
    using namespace libconfig;

    try
    {
      const Setting& from = mapping_description["from"];
      const Setting& to = mapping_description["to"];

      log << Log::Severity::Info << "Mapping variables bidirectionally:" << std::endl;
      Log variable_log = log.deriveLogLevel();

      std::string from_string = static_cast<const char*>(from);
      std::string to_string = static_cast<const char*>(to);

      variable_log << Log::Severity::Info << from_string << std::endl;
      variable_log << Log::Severity::Info << to_string << std::endl;

      mapper_.mapEntities(from_string, to_string, true);
    }
    catch(const SettingNotFoundException& ex)
    {
      log << Log::Severity::Warning << "Invalid mapping format" << std::endl;
    }
  }
}
