// System
#include <atomic>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <unistd.h>

// Private
#include <appliances_backend/appliances/helios_kwl.h>
//#include <appliances_backend/helios_kwl_modbus_client.h>

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

  /*std::cout << "Connecting to modbus server" << std::endl;
  ModbusClient modbus_client("192.168.100.14", 502, 180);
  std::cout << "Connected" << std::endl;*/

  //HeliosKwlModbusClient helios("192.168.100.14");

  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  std::shared_ptr<ApplianceBase> appliance = std::make_shared<appliances::HeliosKwl>("192.168.100.14");

  std::cout << "Starting appliance" << std::endl;
  appliance->start();
  std::cout << "Started appliance" << std::endl;

  should_run = true;

  while(should_run)
  {
    std::cout << "Duty cycle" << std::endl;

    
    /*std::cout << "Output: " << helios.readFanStage() << std::endl;

      helios.writeFanStage(2);*/

    usleep(500000);
  }

  std::cout << "Stopping appliance" << std::endl;
  appliance->stop();
  std::cout << "Stopped appliance" << std::endl;

  return EXIT_SUCCESS;
}
