#include <appliances_backend/manageable_base.h>


namespace appliances_backend
{
  void ManageableBase::start()
  {
    stop();

    initialize();
    
    should_run_ = true;
    worker_thread_ = std::thread(&ManageableBase::run, this);
  }

  void ManageableBase::stop()
  {
    if(should_run_)
    {
      should_run_ = false;

      if(worker_thread_.joinable())
      {
	worker_thread_.join();
      }
    }
  }
}
