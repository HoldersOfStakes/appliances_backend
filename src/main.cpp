// System
#include <atomic>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <list>

// Private
#include <appliances_backend/appliances/helios_kwl/helios_kwl.h>
#include <appliances_backend/interfaces/homebridge/homebridge_mqtt.h>
#include <appliances_backend/appliances_manager.h>
#include <appliances_backend/interfaces_manager.h>
#include <appliances_backend/accessories_manager.h>
#include <appliances_backend/mapper.h>


std::atomic<bool> should_run;


std::list<std::string> split(const std::string& str, char delimeter)
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

void signalHandler(int signal)
{
  if(signal == SIGINT || signal == SIGTERM)
  {
    std::cout << "\rReceived stop signal" << std::endl;
    should_run = false;
  }
}

int main(int argc, char** argv)
{
  using namespace appliances_backend;

  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  InterfacesManager interfaces_manager;
  interfaces_manager.addInterface<interfaces::HomebridgeMqtt>("homebridge", "192.168.100.2", 1883);
  interfaces_manager.start();

  AppliancesManager appliances_manager;
  appliances_manager.addAppliance<appliances::HeliosKwl>("helios", "192.168.100.14");
  appliances_manager.start();

  AccessoriesManager accessories_manager;
  std::shared_ptr<Accessory> ventilation = accessories_manager.addAccessory("ventilation");
  std::shared_ptr<Service> fan = ventilation->addService("Speed", Service::Type::Fan);
  std::shared_ptr<Characteristic> rotation_speed = fan->addCharacteristic(Characteristic::Type::RotationSpeed);
  rotation_speed->setProperty("min_value", 1);
  rotation_speed->setProperty("max_value", 4);
  std::shared_ptr<Characteristic> on = fan->addCharacteristic(Characteristic::Type::On);
  on->setProperty("always_on", true);

  ventilation->setPrimaryServiceKey("Speed");

  Mapper mapper;
  mapper.mapEntities("helios.fan_stage", "homebridge.ventilation.Speed.RotationSpeed", true);
  mapper.mapEntities("helios.on", "homebridge.ventilation.Speed.On", true);

  std::map<std::string, std::string> appliance_interface_mapping =
  {
    { "helios.fan_stage", "homebridge.ventilation.Speed.RotationSpeed" },
    { "helios.on", "homebridge.ventilation.Speed.On" }
  };
  
  std::map<std::string, std::string> interface_appliance_mapping =
  {
    { "homebridge.ventilation.Speed.RotationSpeed", "helios.fan_stage" },
    { "homebridge.ventilation.Speed.On", "helios.on" }
  };
  
  for(const std::string accessory_name : accessories_manager.getAccessoryNames())
  {
    interfaces_manager.registerAccessory(accessories_manager.getAccessory(accessory_name));
  }

  should_run = true;

  while(should_run)
  {
    std::map<std::string, nlohmann::json> changed_variables = appliances_manager.getChangedVariables();

    for(const std::pair<std::string, nlohmann::json>& changed_variable_pair : changed_variables)
    {
      std::cout << "From appliance: " << changed_variable_pair.first << " = " << changed_variable_pair.second << std::endl;

      for(const std::string& mapped_entity : mapper.getMappedEntities(changed_variable_pair.first))
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

	  std::shared_ptr<Accessory> accessory = accessories_manager.getAccessory(accessory_key);

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
		  interfaces_manager.setVariable(interface_key, accessory, service, characteristic, changed_variable_pair.second);
		}
	      }
	    }
	  }
	}
      }
    }

    changed_variables = interfaces_manager.getChangedVariables();

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

	  std::cout << interface_key << ", " << accessory_key << ", " << service_key << ", " << characteristic_key << ": " << changed_variable_pair.second << std::endl;

	  std::shared_ptr<Accessory> accessory = accessories_manager.getAccessory(accessory_key);

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
		    interfaces_manager.setVariable(interface_key, accessory, service, characteristic, true);
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
	for(const std::string& mapped_entity : mapper.getMappedEntities(changed_variable_pair.first))
	{
	  std::list<std::string> path_parts = split(mapped_entity, '.');

	  if(path_parts.size() > 0)
	  {
	    std::string appliance_key = path_parts.front();
	    path_parts.pop_front();
	    
	    appliances_manager.setVariable(appliance_key, path_parts, changed_variable_pair.second);
	  }
	}
      }
    }

    usleep(50000);
  }

  for(const std::string accessory_name : accessories_manager.getAccessoryNames())
  {
    interfaces_manager.deregisterAccessory(accessory_name);
  }

  appliances_manager.stop();
  interfaces_manager.stop();

  return EXIT_SUCCESS;
}
