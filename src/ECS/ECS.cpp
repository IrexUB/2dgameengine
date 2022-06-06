#include <algorithm>

#include "../Logger/Logger.h"
#include "ECS.h"

uint32_t ComponentBase::m_componentNo = 0;

uint32_t Entity::Id() const
{
    return m_id;
}

const std::string& Entity::Name() const
{
    return m_name;
}

void Entity::SetName(std::string newName)
{
    m_name = std::move(newName);    
}

void Entity::Kill()
{
    m_registry->KillEntity(*this);
}

bool Entity::operator==(const Entity &rhs) const
{
    return (m_id == rhs.m_id);
}

bool Entity::operator!=(const Entity &rhs) const
{
    return !(*this == rhs);
}

bool Entity::operator<(const Entity &rhs) const
{
    return (m_id < rhs.m_id);
}

bool Entity::operator>(const Entity &rhs) const
{
    return (m_id > rhs.m_id);
}

bool Entity::HasTag(const std::string& tag) const
{
    return m_registry->HasTag(*this, tag);    
}

void Entity::AddTag(const std::string& tag)
{
    m_registry->AddTag(*this, tag);    
}

void System::AddEntity(Entity entity)
{
    m_entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [&entity](const Entity &rhs)
                                    { return entity == rhs; }),
                     m_entities.end());
}

std::vector<Entity> &System::GetSystemEntities()
{
    return m_entities;
}

inline const Signature &System::GetComponentSignature() const
{
    return m_componentSignature;
}

Entity Registry::CreateEntity()
{
    uint32_t entityId;
    if (m_freeIds.empty())
    {
        entityId = m_nEntities++;
    }
    else
    {
        entityId = m_freeIds.front();
        m_freeIds.pop_front();
    }

    if (entityId >= m_entitiesComponentsSignatures.size())
    {
        m_entitiesComponentsSignatures.resize(m_nEntities + 1);
    }

    Entity newEntity(entityId);
    newEntity.m_registry = this;

    m_entitiesToAdd.insert(newEntity);

    Logger::LogDebug("Entity created with id : " + std::to_string(entityId));

    return newEntity;
}

inline void Registry::AddEntityToSystem(Entity entity, System &system)
{
    const auto &targetEntitySignature = m_entitiesComponentsSignatures[entity.Id()];
    const auto &systemSignature = system.GetComponentSignature();

    if ((systemSignature & targetEntitySignature) == systemSignature)
    {
        system.AddEntity(entity);
    }
}

inline void Registry::AddEntityToSystems(Entity entity)
{
    for (auto &[identifier, system] : m_systems)
    {
        AddEntityToSystem(entity, *system);
    }
}

void Registry::Update()
{
    for (auto &entity : m_entitiesToAdd)
    {
        AddEntityToSystems(entity);
    }

    for (auto &entity : m_entitiesToDelete)
    {
        RemoveEntityFromSystems(entity);
        m_entitiesComponentsSignatures[entity.Id()].reset();

        m_freeIds.push_back(entity.Id());
    }

    m_entitiesToDelete.clear();
    m_entitiesToAdd.clear();
}

void Registry:: KillEntity(Entity entity)
{
    m_entitiesToDelete.insert(entity);
    Logger::LogDebug("Entity n°" + std::to_string(entity.Id()) + " destroyed !");
}

inline void Registry::RemoveEntityFromSystems(Entity entity)
{
    for (auto &[identifier, system] : m_systems)
    {
        system->RemoveEntity(entity);
    }
}

void Registry::AddTag(Entity entity, const std::string& tag)
{
    if (!HasTag(entity, tag)) {
        Logger::LogDebug(tag + " tag linked to " + entity.Name());
        m_tagPerEntity.emplace(entity.Id(), tag);
    } else {
        m_tagPerEntity.at(entity.Id()) = tag; 
        Logger::LogDebug(tag + " tag updated for " + entity.Name());
    }
}

bool Registry::HasTag(Entity entity, const std::string& tag) const
{
    if (m_tagPerEntity.find(entity.Id()) != m_tagPerEntity.end())
        return true;
    return false;
}