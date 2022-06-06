#pragma once

#include <bitset>
#include <array>
#include <optional>
#include <deque>
#include <set>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <vector>

constexpr uint32_t MAX_COMPONENTS{32};
using Signature = std::bitset<MAX_COMPONENTS>;

class Registry;

class Entity
{
    friend class Registry;

public:
    Entity(uint32_t id) : m_id(id) 
    {
        m_name = "Entity " + std::to_string(m_id);
    };

    uint32_t Id() const;
    const std::string& Name() const;
    inline void SetName(std::string newName);

    bool HasTag(const std::string& tag) const;
    void AddTag(const std::string& tag);

    bool operator==(const Entity &rhs) const;
    bool operator!=(const Entity &rhs) const;
    bool operator<(const Entity &rhs) const;
    bool operator>(const Entity &rhs) const;

    void Kill();

    template <typename T, typename... TArgs>
    inline void AddComponent(TArgs &&...args);

    template <typename T>
    inline void RemoveComponent();

    template <typename T>
    inline bool HasComponent() const;

    template <typename T>
    inline T &GetComponent() const;

private:
    Registry *m_registry;
    uint32_t m_id;
    std::string m_name;
};

struct ComponentBase
{
protected:
    static uint32_t m_componentNo;
};

template <typename T>
class Component : public ComponentBase
{
public:
    static uint32_t GetComponentNo()
    {
        static auto no = m_componentNo++;
        return no;
    }
};

class System
{
public:
    System() = default;
    virtual ~System() = default;

    void AddEntity(Entity entity);
    void RemoveEntity(Entity entity);
    std::vector<Entity> &GetSystemEntities();
    inline const Signature &GetComponentSignature() const;

    template <typename T>
    inline void RequireComponent();

private:
    Signature m_componentSignature;
    std::vector<Entity> m_entities;
};

class PoolBase
{
public:
    virtual ~PoolBase() = default;
};

template <typename T>
class Pool : public PoolBase
{
public:
    Pool(uint32_t size = 100);
    virtual ~Pool() = default;

    void Resize(uint32_t newSize);
    uint32_t GetSize() const;
    bool IsEmpty() const;
    void Clear();
    void Add(T object);
    void Set(std::size_t index, T object);
    T &operator[](std::size_t index);

private:
    std::vector<T> m_data;
};

class Registry
{
public:
    void Update();
    Entity CreateEntity();
    void KillEntity(Entity entity);
    void AddTag(Entity entity, const std::string& tag);
    bool HasTag(Entity entity, const std::string& tag) const;

    inline void AddEntityToSystem(Entity entity, System &system);
    inline void AddEntityToSystems(Entity entity);
    inline void RemoveEntityFromSystems(Entity entity);

    template <typename T, typename... TArgs>
    void AddSystem(TArgs &&...args);

    template <typename T>
    inline void RemoveSystem();

    template <typename T>
    inline bool HasSystem() const;

    template <typename T>
    T &GetSystem();

    template <typename T, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&...args);

    template <typename T>
    inline void RemoveComponent(Entity entity);

    template <typename T>
    inline bool HasComponent(Entity entity) const;

    template <typename T>
    inline T &GetComponent(Entity entity) const;

private:
    uint32_t m_nEntities;
    std::set<Entity> m_entitiesToAdd;
    std::set<Entity> m_entitiesToDelete;

    std::array<std::shared_ptr<PoolBase>, MAX_COMPONENTS> m_componentsPools;
    std::deque<int> m_freeIds;
    std::vector<Signature> m_entitiesComponentsSignatures;
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;

    std::unordered_map<int, std::string> m_tagPerEntity;
};

#include "ECS.inl"