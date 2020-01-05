#include <appliances_backend/appliance_base.h>


namespace appliances_backend
{
  void ApplianceBase::start()
  {
    stop();
    
    should_run_ = true;
    worker_thread_ = std::thread(&ApplianceBase::run, this);
  }

  void ApplianceBase::stop()
  {
    if(should_run_)
    {
      should_run_ = false;
      worker_thread_.join();
    }
  }
}
