#pragma once

#include <list>
#include <functional>
#include <utility>

template <typename TValue>
class ValueObserver
{
public:
    typedef std::function<void(const TValue&)> Handler;

    ValueObserver() = default;
    ValueObserver(const ValueObserver&) = delete;
    ValueObserver& operator = (const ValueObserver&) = delete;

    ValueObserver( ValueObserver && param ) = default;
    ValueObserver& operator = ( ValueObserver && param ) = default;

    void addListener(const Handler& func)
    {
        m_listeners.push_back(func);
    }

    template <typename TObj, typename TFunc>
    void addListener(TObj obj, TFunc func)
    {
       auto functor = std::bind(func, obj, std::placeholders::_1);
        m_listeners.push_back(functor);
    }

    void notifyListeners(const TValue& value) const
    {
        for(const auto& listener: m_listeners)
        {
            if(listener)
            {
                listener(value);
            }
        }
    }

private:
    std::list<Handler> m_listeners;
};

template <typename TValue>
class ValueSetter: public ValueObserver<TValue>
{
public:

    class ScopedSetter
    {
    public:
        explicit ScopedSetter(ValueSetter<TValue>& setter):
            m_setter(setter)
        {
        }

        ~ScopedSetter()
        {
            m_setter.notifyListeners(m_setter.m_value);
        }
        ScopedSetter(const ScopedSetter&) = delete;
        ScopedSetter& operator = (const ScopedSetter&) = delete;

        TValue& getRef()
        {
            return m_setter.m_value;
        }

        TValue& operator *()
        {
            return m_setter.m_value;
        }
        TValue* operator ->()
        {
            return &(m_setter.m_value);
        }
        
        ValueSetter<TValue>& m_setter;
    };

    ValueSetter() = default;

    ValueSetter(TValue defaultValue)
    : m_value(defaultValue)
    {
    }

    ValueSetter(const ValueSetter&) = delete;
    ValueSetter& operator = (const ValueSetter&) = delete;

    ValueSetter( ValueSetter &&) = default;
    ValueSetter& operator = ( ValueSetter &&) = default;


    ValueObserver<TValue>& getObserver()
    {
        return *this;
    }

    void setValue(const TValue& value)
    {
        m_value = value;
        ValueObserver<TValue>::notifyListeners(m_value);
    }
    const TValue& getValue() const
    {
        return m_value;
    }

    ValueSetter<TValue>& operator = (const TValue& value)
    {
        setValue(value);
        return *this;
    }

    operator TValue() const
    {
        return getValue();
    }

    bool operator == (const TValue& value) const
    {
        return m_value == value;
    }

    bool operator != (const TValue& value)  const
    {
        return m_value != value;
    }

    const TValue& operator *() const
    {
        return m_value;
    }
    const TValue* operator ->() const
    {
        return &m_value;
    }
private:
    TValue m_value;
};
