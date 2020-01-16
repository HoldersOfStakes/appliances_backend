// System
#include <atomic>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <list>

// Private
#include <appliances_backend/backend.h>


// Globals
std::unique_ptr<appliances_backend::Backend> backend_;


void signalHandler(int signal)
{
  if(signal == SIGINT || signal == SIGTERM)
  {
    if(backend_ != nullptr)
    {
      std::cout << "\rReceived stop signal" << std::endl;
      backend_->stop();
    }
  }
}

int main(int argc, char** argv)
{
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  std::string config_file_path = "";

  backend_ = std::make_unique<appliances_backend::Backend>(config_file_path);

  backend_->initialize();
  backend_->run();
  backend_->deinitialize();

  return EXIT_SUCCESS;
}
