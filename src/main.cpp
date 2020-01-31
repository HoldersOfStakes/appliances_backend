// System
#include <atomic>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <list>
#include <getopt.h>

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

  static int dry_run_flag = false;
  int c;

  while(true)
  {
    static struct option long_options[] =
    {
     { "dry-run", no_argument, &dry_run_flag, 1 },
     { 0, 0, 0, 0 }
    };

    int option_index = 0;
    c = getopt_long(argc, argv, "", long_options, &option_index);

    if(c == -1)
    {
      break;
    }

    switch(c)
    {
    case 0:
      if(long_options[option_index].flag != 0)
      {
	break;
      }

      // ...

      break;

    case '?':
      break;

    default:
      abort();
      break;
    }
  }

  std::string config_file_path = "../configs/default.cfg";

  appliances_backend::Backend::RunMode run_mode =
    (dry_run_flag ?
     appliances_backend::Backend::RunMode::DryRun :
     appliances_backend::Backend::RunMode::Normal);

  backend_ = std::make_unique<appliances_backend::Backend>(config_file_path, run_mode);

  backend_->initialize();
  backend_->run();
  backend_->deinitialize();

  return EXIT_SUCCESS;
}
