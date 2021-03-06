#include <appliances_backend/manageable_base.h>


namespace appliances_backend
{
  ManageableBase::ManageableBase(Log log)
    : LoggingBase{ log }
  {
  }

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

  void ManageableBase::waitFor(std::chrono::steady_clock::duration duration)
  {
    std::this_thread::sleep_for(duration);
  }
}
