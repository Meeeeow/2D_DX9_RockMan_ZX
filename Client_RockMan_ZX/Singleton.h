#pragma once
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <typename T>
class CSingleton abstract
{
protected:
	explicit CSingleton() = default;
	virtual ~CSingleton() = default;

public:
	static T* Get_Instance();
	static void Destroy_Instance();
	static T* m_pInstance;
};
template <typename T>
T* CSingleton<T>::m_pInstance = nullptr;


template<typename T>
inline T * CSingleton<T>::Get_Instance()
{
	if (!m_pInstance)
		m_pInstance = new T;
	return m_pInstance;
}

template<typename T>
inline void CSingleton<T>::Destroy_Instance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}


#endif // !__SINGLETON_H__
