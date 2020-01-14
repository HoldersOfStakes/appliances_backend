#include <appliances_backend/mapper.h>


namespace appliances_backend
{
  void Mapper::mapEntities(const std::string& from_entity, const std::string& to_entity, bool bidirectional)
  {
    if(std::find(mappings_[from_entity].begin(), mappings_[from_entity].end(), to_entity) == mappings_[from_entity].end())
    {
      mappings_[from_entity].emplace_back(to_entity);
    }

    if(bidirectional)
    {
      mapEntities(to_entity, from_entity, false);
    }
  }

  void Mapper::unmapEntities(const std::string& from_entity, const std::string& to_entity, bool bidirectional)
  {
    if(mappings_.find(from_entity) != mappings_.end())
    {
      mappings_[from_entity].remove(to_entity);
    }

    if(bidirectional)
    {
      unmapEntities(to_entity, from_entity, false);
    }
  }

  void Mapper::removeEntity(const std::string& entity)
  {
    mappings_.erase(entity);

    for(std::map<std::string, std::list<std::string>>::iterator mapping_iterator = mappings_.begin();
	mapping_iterator != mappings_.end(); ++mapping_iterator)
    {
      mapping_iterator->second.remove(entity);
    }
  }

  std::list<std::string> Mapper::getMappedEntities(const std::string& from)
  {
    if(mappings_.find(from) != mappings_.end())
    {
      return mappings_[from];
    }

    return std::list<std::string>();
  }
}
