//
// Created by NePutin on 8/16/2021.
//

#ifndef PALKA_RAWPTRWRAPPER_H
#define PALKA_RAWPTRWRAPPER_H

#include <rttr/type>
#include <rttr/wrapper_mapper.h>
#include <rttr/registration_friend>

template<typename T>
class Raw_Ptr
{
public:
    Raw_Ptr() : m_data(nullptr) {}
    explicit Raw_Ptr(T* obj) : m_data(obj) {}
    explicit Raw_Ptr(T) : m_data(nullptr) {}

    T* get_data() const { return m_data; }
    void set_data(T* obj) { m_data = obj; }
private:
    T* m_data;
};

namespace rttr
{
    template<typename T>
    struct wrapper_mapper<Raw_Ptr<T>>
    {
        using wrapped_type  = decltype(std::declval<Raw_Ptr<T>>().get_data());
        using type          = Raw_Ptr<T>;
        inline static wrapped_type get(const type& obj)
        {
            return obj.get_data();
        }
        inline static type create(const wrapped_type& value)
        {
            return Raw_Ptr<T>(std::move(value));
        }
        template<typename U>
        inline static Raw_Ptr<U> convert(const type& source, bool& ok)
        {
            if (auto obj = rttr_cast<typename Raw_Ptr<U>::wrapped_type*>(&source.get_data()))
            {
                ok = true;
                return Raw_Ptr<U>(*obj);
            }
            else
            {
                ok = false;
                return Raw_Ptr<U>();
            }
        }
    };
}
#endif //PALKA_RAWPTRWRAPPER_H
