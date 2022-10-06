#pragma once

#include "CRes.h"

template<typename T>
class Ptr
{
public :

	T* const Get()
	{
		return m_pRes;
	}

	T** const GetAdressOf()
	{
		return &m_pRes;
	}
	void operator =(T* ptr)
	{
		if(m_pRes)
			m_pRes->Release();

		m_pRes = ptr;
		if(m_pRes)
		m_pRes->AddRef();
	}

	void operator =(const Ptr<T>& ptr)
	{
		if (m_pRes)
			m_pRes->Release();

		m_pRes = ptr.m_pRes;
		if (m_pRes)
			m_pRes->AddRef();
	}
	
	T* operator->()
	{
		return m_pRes;
	}

	bool operator == (void* _pRes)
	{
		if (m_pRes == _pRes)
			return true;
		else
			return false;
	}

	Ptr()
		:m_pRes(nullptr)
	{

	}
	
	Ptr(T* _pRes)
		:m_pRes(_pRes)
	{
		if (m_pRes)
			m_pRes->AddRef();
	}
	
	Ptr(const Ptr<T>& _pRes)
		:m_pRes(_pRes.m_pRes)
	{
		if (m_pRes)
			m_pRes->AddRef();
	}

	~Ptr()
	{
		if (m_pRes)
			m_pRes->Release();
	}
	
private :
	T* m_pRes;
};

template<typename T>
bool operator == (void* _Res, const Ptr<T>& _Ptr)
{
	if (_Res == _Ptr.Get())
		return true;
	else
		return false;
}

template<typename T>
bool operator != (void* _Res, const Ptr<T>& _Ptr)
{
	if (_Res == _Ptr.Get())
		return false;
	else
		return true;
}
