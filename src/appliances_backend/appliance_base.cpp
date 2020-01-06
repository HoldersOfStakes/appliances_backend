#include <appliances_backend/appliance_base.h>


namespace appliances_backend
{
  bool ApplianceBase::wasVariableChanged()
  {
    std::lock_guard<std::mutex> lock(variable_access_mutex_);

    return !changed_variables_.empty();
  }

  std::pair<std::string, std::shared_ptr<property::RawData>> ApplianceBase::getChangedVariable()
  {
    std::lock_guard<std::mutex> lock(variable_access_mutex_);

    std::string variable_name = changed_variables_.front();
    changed_variables_.pop_front();

    return std::make_pair(variable_name, variable_states_[variable_name]->copy());
  }

  void ApplianceBase::setRawVariableState(std::string variable_path, std::shared_ptr<property::RawData> content)
  {
    std::lock_guard<std::mutex> lock(variable_access_mutex_);

    bool changed = false;

    if(variable_states_.find(variable_path) != variable_states_.end())
    {
      if(!variable_states_[variable_path]->equals(content))
      {
	changed = true;
      }
    }
    else
    {
      changed = true;
    }

    if(changed)
    {
      variable_states_[variable_path] = content;
      
      changed_variables_.push_back(variable_path);
    }
  }
}
