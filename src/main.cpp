// System
#include <atomic>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <unistd.h>

// Private
#include <appliances_backend/appliances/helios_kwl.h>
#include <appliances_backend/interfaces/mqtt.h>


std::atomic<bool> should_run;


void signalHandler(int signal)
{
  if(signal == SIGINT || signal == SIGTERM)
  {
    std::cout << "Received stop signal" << std::endl;
    should_run = false;
  }
}

int main(int argc, char** argv)
{
  using namespace appliances_backend;

  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  std::shared_ptr<InterfaceBase> interface = std::make_shared<interfaces::Mqtt>("192.168.100.2", 1883);

  std::cout << "Starting interface" << std::endl;
  //interface->start();
  std::cout << "Started interface" << std::endl;

  std::shared_ptr<ApplianceBase> appliance = std::make_shared<appliances::HeliosKwl>("192.168.100.14");

  std::cout << "Starting appliance" << std::endl;
  appliance->start();
  std::cout << "Started appliance" << std::endl;

  should_run = true;

  while(should_run)
  {
    if(appliance->wasVariableChanged())
    {
      std::pair<std::string, std::shared_ptr<property::RawData>> changed_variable = appliance->getChangedVariable();

      std::cout << "Variable changed: " << changed_variable.first << " = " << changed_variable.second << std::endl;

      // emit variable change to interface 
    }
    
    usleep(50000);
  }

  std::cout << "Stopping appliance" << std::endl;
  appliance->stop();
  std::cout << "Stopped appliance" << std::endl;

  std::cout << "Stopping interface" << std::endl;
  //interface->stop();
  std::cout << "Stopped interface" << std::endl;

  return EXIT_SUCCESS;
}
