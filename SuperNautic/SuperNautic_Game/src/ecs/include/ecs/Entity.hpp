#pragma once

#include <vector>

#include "../../impl/Ids.hpp"

namespace ecs
{
    namespace impl
    {
        class EcsHelper;
    }
    using namespace impl;

    class Entity
    {
        public:
            Entity();

            template<typename T>
            operator T*();

            template<typename T>
            operator const T*() const;

            template<typename T>
            bool has() const;

            template<typename T>
            T* get();

            template<typename T>
            const T* get() const;

            bool operator!() const;
            bool is_null() const;
            bool operator==(std::nullptr_t null_ptr) const;
            operator bool() const;

            Entity& operator=(std::nullptr_t null_ptr);

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
            friend class ::ecs::impl::EcsHelper;
            Entity(EntityId id);
            
            void* get(TypeIndex id) const;
            void* attach(TypeIndex id);
            bool detach(TypeIndex id, void* data);
            std::vector<void*> detach(TypeIndex id);

            EntityId _id;
    };
}


#include "../../impl/Entity.inl"
