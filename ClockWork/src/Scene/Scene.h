#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <vector>

#include "ECS/ECS.h"

namespace CW
{
    class Scene
    {
        class Entity;
        using EntityPtr = std::shared_ptr<Entity>;
        using WeakEntityPtr = std::weak_ptr<Entity>;

    public:
        EntityPtr& CreateEntity(const std::string& name, EntityPtr parent = nullptr)
        {
            auto id = ECS::Instance().CreateEntity();

            auto entity = std::make_shared<Entity>(id,name, parent);
            if (parent != nullptr)
                parent->SetChild(entity);

            _entities.push_back(entity);
            return entity;
        }

        EntityPtr& CopyEntity(const EntityPtr& base)
        {
            return CreateEntity(base->_name, base->_parent.lock());
        }

        void DestroyEntity(EntityPtr& entity)
        {
            auto it = std::find(_entities.begin(), _entities.end(), entity);
            if (it != _entities.end())
            {
                DestroyRecursive(entity);
            }
            entity.reset();
        }

        template<typename... T>
        void AddComponents(EntityPtr& entity, T... components)
        {
            (AddComponent<T>(entity, components), ...);
        }

        template<typename T>
        void RemoveComponent(EntityPtr& entity, T component)
        {
            ECS::Instance().RemoveComponent(entity->_id, component);
        }

        void Clear()
        {
            _entities.clear();
        }

    private:

        class Entity : public std::enable_shared_from_this<Entity>
        {
            friend class Scene;

        public:
            Entity(EntityId id, const std::string& name, const EntityPtr& parent = nullptr)
                : _id(id), _name(name), _parent(parent) {
                std::cout << "creating " << _id << " " << _name << "\n";
            }

            ~Entity()
            {
                std::cout << "destroying " << _id << " " << _name << "\n";
            }

            inline const std::string& GetName() const { return _name; }

            inline EntityPtr GetParent() const { return _parent.lock(); }

            void SetParent(EntityPtr& parent)
            {
                _parent = parent;
                parent->_children.push_back(shared_from_this());
            }

            void SetChild(EntityPtr& child)
            {
                if (child->GetParent() != nullptr)
                {
                    child->GetParent()->RemoveChild(child);
                }
                auto a = shared_from_this();
                child->SetParent(a);
            }

            void RemoveChild(const EntityPtr& child)
            {
                auto it = std::find(_children.begin(), _children.end(), child);
                if (it != _children.end())
                {
                    (*it)->_parent = WeakEntityPtr();
                    _children.erase(it);
                }
            }

        private:
            EntityId _id;
            std::string _name;
            WeakEntityPtr _parent;
            std::vector<EntityPtr> _children;
        };

        template<typename... T>
        void AddComponent(EntityPtr& entity, T... component)
        {
            ECS::Instance().AddComponent(entity->_id, component...);
        }

        void DestroyRecursive(EntityPtr& entity)
        {
            //first remove entity from parent's children.
            if (auto parent = entity->GetParent())
            {
                parent->RemoveChild(entity);
            }

            // reset entity parent
            entity->_parent = WeakEntityPtr();

            // do the same for every children of entity.
            while (!entity->_children.empty())
            {
                auto child = entity->_children.back();
                entity->_children.pop_back();
                DestroyRecursive(child);
            }

            // remove entity from scene entities
            auto itr = std::find(_entities.begin(), _entities.end(), entity);
            if (itr != _entities.end())
            {
                _entities.erase(itr);
            }

            entity.reset();
        }

        std::vector<EntityPtr> _entities;
    };
}
