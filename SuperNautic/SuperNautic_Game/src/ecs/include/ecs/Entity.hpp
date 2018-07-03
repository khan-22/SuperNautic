#pragma once

#include <vector>

#include "../../impl/Ids.hpp"

namespace ecs
{
    using namespace impl;

    class Entity
    {
        public:
            Entity();

            static Entity create();
            static void destroy(Entity& entity);
            void destroy();

            template<typename... T>
            static std::vector<Entity> get_with();

            static std::vector<Entity> get_like(const Entity& entity);

            std::vector<Entity> get_like() const;

            template<typename T>
            operator T*();

            template<typename T>
            operator const T*() const;

            template<typename T>
            bool has() const;

            template<typename T>
            T& operator+=(const T& component);

            template<typename T>
            T& attach(const T& component);

            template<typename T>
            T& attach();

            template<typename T>
            bool operator-=(T* component);

            template<typename T>
            bool detach(T* component);
            
            template<typename T>
            bool detach();

        private:
            Entity(EntityId id);
            
            static std::vector<Entity> get_with(const std::vector<TypeIndex>& ids);
            static std::vector<Entity> from_ids(const std::vector<EntityId>& ids);
            void* get(TypeIndex id) const;
            void* attach(TypeIndex id);
            bool detach(TypeIndex id, void* data);
            std::vector<void*> detach(TypeIndex id);

            EntityId _id;
    };
}


#include "../../impl/Entity.inl"
