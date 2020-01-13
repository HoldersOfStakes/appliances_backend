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
  std::shared_ptr<Service> fan = ventilation->addService("speed", "Speed", Service::Type::Fan);
  std::shared_ptr<Characteristic> rotation_speed = fan->addCharacteristic(Characteristic::Type::RotationSpeed);
  rotation_speed->setProperty("min_value", 1);
  rotation_speed->setProperty("max_value", 3);
  fan->addCharacteristic(Characteristic::Type::AlwaysOn);

  ventilation->setPrimaryServiceKey("speed");

  std::map<std::string, std::string> appliance_interface_mapping =
  {
    { "helios.fan_stage", "homebridge.ventilation.Speed.RotationSpeed" }
  };
  
  std::map<std::string, std::string> interface_appliance_mapping =
  {
    { "homebridge.ventilation.Speed.RotationSpeed", "helios.fan_stage" }
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

      if(appliance_interface_mapping.find(changed_variable_pair.first) != appliance_interface_mapping.end())
      {
	interfaces_manager.setVariable(split(appliance_interface_mapping[changed_variable_pair.first], '.'), changed_variable_pair.second);
      }
    }

    changed_variables = interfaces_manager.getChangedVariables();

    for(const std::pair<std::string, nlohmann::json>& changed_variable_pair : changed_variables)
    {
      std::cout << "From interface: " << changed_variable_pair.first << " = " << changed_variable_pair.second << std::endl;

      if(interface_appliance_mapping.find(changed_variable_pair.first) != interface_appliance_mapping.end())
      {
	appliances_manager.setVariable(split(interface_appliance_mapping[changed_variable_pair.first], '.'), changed_variable_pair.second);
      }
      else if(accessories_manager.isServiceAlwaysOn(changed_variable_pair.first))
      {
	//interfaces_manager.setVariable(changed_variable_pair.first, true);
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
