// System
#include <atomic>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <unistd.h>

// Private
#include <appliances_backend/appliances/helios_kwl/helios_kwl.h>
#include <appliances_backend/interfaces/homebridge/homebridge_mqtt.h>
#include <appliances_backend/appliances_manager.h>
#include <appliances_backend/interfaces_manager.h>


std::atomic<bool> should_run;


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

  should_run = true;

  while(should_run)
  {
    std::map<std::string, std::shared_ptr<property::RawData>> changed_variables = appliances_manager.getChangedVariables();

    for(const std::pair<std::string, std::shared_ptr<property::RawData>>& changed_variable_pair : changed_variables)
    {
      std::cout << "Changed: " << changed_variable_pair.first << " = " << changed_variable_pair.second << std::endl;
    }

    usleep(50000);
  }

  appliances_manager.stop();
  interfaces_manager.stop();

  return EXIT_SUCCESS;
}
