#pragma once
#ifndef __TOOL_SINGLETON_H__
#define __TOOL_SINGLETON_H__

template <typename T>
class CToolSingleton abstract
{
protected:
	explicit CToolSingleton() = default;
	virtual ~CToolSingleton() = default;

public:
	static T* Get_Instance();
	static void Destroy_Instance();
	static T* m_pInstance;
};
template <typename T>
T* CToolSingleton<T>::m_pInstance = nullptr;


template<typename T>
inline T * CToolSingleton<T>::Get_Instance()
{
	if (!m_pInstance)
		m_pInstance = new T;
	return m_pInstance;
}

template<typename T>
inline void CToolSingleton<T>::Destroy_Instance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}


#endif // !__SINGLETON_H__
