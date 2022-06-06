#include <cassert>
#include "ECS.h"
#include "../Logger/Logger.h"

template <typename T, typename... TArgs>
inline void Entity::AddComponent(TArgs &&...args)
{
    m_registry->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template <typename T>
inline void Entity::RemoveComponent()
{
    m_registry->RemoveComponent<T>(*this);
}

template <typename T>
inline bool Entity::HasComponent() const
{
    return m_registry->HasComponent<T>(*this);
}

template <typename T>
inline T &Entity::GetComponent() const
{
    return m_registry->GetComponent<T>(*this);
}

template <typename T>
inline void System::RequireComponent()
{
    const auto componentNo = Component<T>::GetComponentNo();
    m_componentSignature.set(componentNo);
}

template <typename T>
Pool<T>::Pool(uint32_t size)
{
    Resize(size);
};

template <typename T>
void Pool<T>::Resize(uint32_t newSize)
{
    m_data.resize(newSize);
}

template <typename T>
bool Pool<T>::IsEmpty() const
{
    return m_data.empty();
}

template <typename T>
void Pool<T>::Clear()
{
    m_data.clear();
}

template <typename T>
uint32_t Pool<T>::GetSize() const
{
    return m_data.size();
}

template <typename T>
void Pool<T>::Add(T object)
{
    m_data.push_back(object);
}

template <typename T>
void Pool<T>::Set(std::size_t index, T object)
{
    assert(index < m_data.size());
    m_data[index] = object;
}

template <typename T>
T &Pool<T>::operator[](std::size_t index)
{
    assert(index < m_data.size());
    return m_data[index];
}

template <typename T, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs&&... args)
{
    const auto entityId = entity.Id();
    const auto componentNo = Component<T>::GetComponentNo();

    if (!m_componentsPools[componentNo])
    {
        m_componentsPools[componentNo] = std::make_shared<Pool<T>>();
    }

    auto componentPool = std::static_pointer_cast<Pool<T>>(m_componentsPools[componentNo]);
    if (entityId >= componentPool->GetSize())
    {
        componentPool->Resize(m_nEntities);
    }

    T newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, std::move(newComponent));

    m_entitiesComponentsSignatures[entityId].set(componentNo);

    Logger::Log("Component n°" + std::to_string(componentNo) + " added to " + entity.Name());
}

template <typename T>
inline void Registry::RemoveComponent(Entity entity)
{
    const auto entityId = entity.Id();
    const auto componentNo = Component<T>::GetComponentNo();

    m_entitiesComponentsSignatures[entityId].set(componentNo, false);
}

template <typename T>
inline bool Registry::HasComponent(Entity entity) const
{
    const auto entityId = entity.Id();
    const auto componentNo = Component<T>::GetComponentNo();

    return m_entitiesComponentsSignatures[entityId].test(componentNo);
}

template <typename T, typename... TArgs>
inline void Registry::AddSystem(TArgs &&...args)
{
    std::shared_ptr<T> newSystem{std::make_shared<T>(std::forward<TArgs>(args)...)};
    m_systems.insert(std::make_pair(std::type_index(typeid(T)), std::move(newSystem)));
}

template <typename T>
inline void Registry::RemoveSystem()
{
    const auto it = m_systems.find(std::type_index(typeid(T)));
    if (it != m_systems.end())
        m_systems.erase(it);
}

template <typename T>
inline bool Registry::HasSystem() const
{
    return m_systems.find(std::type_index(typeid(T))) != m_systems.end();
}

template <typename T>
T &Registry::GetSystem()
{
    auto system = m_systems.find(std::type_index(typeid(T)));
    if (system != m_systems.end())
        return *(std::static_pointer_cast<T>(system->second));
    else
        Logger::LogError("Missing : " + std::string(typeid(T).name()) + " system");
}

template <typename T>
inline T &Registry::GetComponent(Entity entity) const
{
    const auto entityId = entity.Id();
    const auto componentNo = Component<T>::GetComponentNo();

    if (HasComponent<T>(entity))
    {
        auto componentTPool = std::static_pointer_cast<Pool<T>>(m_componentsPools[componentNo]);
        return componentTPool->operator[](entityId);
    }
    else
    {
        Logger::LogError("Entity n°" + std::to_string(entityId) + " doesn\'t have component n°" + std::to_string(componentNo));
    }
}