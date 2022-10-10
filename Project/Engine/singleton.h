#pragma once

template<typename T>
class CSingletone
{
private :
	static T* g_Mgr;

public :
	static T* GetInst()
	{
		if (g_Mgr == nullptr)
		{
			g_Mgr = new T;
			void(*pFunc)() = &T::Destroy;
			atexit(pFunc);
		}
		
		return g_Mgr;
	}
	static void Destroy()
	{
		if (g_Mgr != nullptr)
		{
			delete g_Mgr;
			g_Mgr = nullptr;
		}
	}
protected:
	CSingletone()
	{
	}

	~CSingletone()
	{
	}
};

template<typename T>
T* CSingletone<T>::g_Mgr = nullptr;