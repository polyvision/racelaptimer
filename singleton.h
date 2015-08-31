/**
 * UtopiaX - Client based on SDL for the UtopiaX Terminal Server
 * 
 * (c) 2005 by mind-plug
 * 
 * Author: Alexander Bierbrauer
 */
#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdlib.h>

template <typename T>
class Singleton
{
public:
	static T*	instance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new T;
		}

		return m_pInstance;
	};

	static void	delInstance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	};

protected:
	Singleton(){};
	virtual ~Singleton(){};

private:
	static T	*m_pInstance;
};

template <typename T> T* Singleton<T>::m_pInstance = NULL;
#endif