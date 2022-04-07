
#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <class T>
class Singleton
{
   static T* m_pInstance;

protected:

   Singleton();

public:

   virtual ~Singleton();

   static T* GetInstance();
};

template <class T>
T* Singleton<T>::m_pInstance = nullptr;

template <class T>
Singleton<T>::Singleton()
{
}

template <class T>
Singleton<T>::~Singleton()
{
}

template <class T>
T* Singleton<T>::GetInstance()
{
   if (m_pInstance == nullptr)
   {
      m_pInstance = new T();
   }
   return m_pInstance;
}

#endif // SINGLETON_HPP
