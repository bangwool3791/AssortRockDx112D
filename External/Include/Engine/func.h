#pragma once

template<typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
			iter->second = nullptr;
	}
	_map.clear();
}

template<typename T, int SIZE>
void Safe_Del_Array(T* (&_arr)[SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (nullptr != _arr[i])
			delete _arr[i];
	}
}

template<typename T>
void Safe_Del_Array(T& arr)
{
	for (auto Iter{ arr.begin()}; Iter != arr.end(); ++Iter)
	{
		delete* Iter;
		(*Iter) = nullptr;
	}
}

template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	typename vector<T*>::iterator iter = _vec.begin();
	for (; iter != _vec.end(); ++iter)
	{
		if (nullptr != (*iter))
			delete (*iter);
	}
	_vec.clear();
}


template<typename T>
void tick_function(T& container)
{
	for (auto iter{ container.begin() }; iter != container.end(); ++iter)
	{
		iter->tick();
	}
}

template<typename T>
void finaltick_function(T& container)
{
	for (auto iter{ container.begin() }; iter != container.end(); ++iter)
	{
		iter->finaltick();
	}
}

template<typename T>
void render_function(T& container)
{
	for (auto iter{ container.begin() }; iter != container.end(); ++iter)
	{
		iter->render();
	}
}

template<typename T>
void clear_function(T& container)
{
	for (auto iter{ container.begin() }; iter != container.end(); ++iter)
	{
		iter->clear();
	}
}

class CGameObject;
void Instantiate(CGameObject* _pNewObj, int _iLayerIdx = 0);
void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx = 0);
void Instantiate(CGameObject* _pNewObj, CGameObject* _pOwner, int _iLayerIdx = 0);
#ifdef _DEBUG
void DebugDrawRect(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuration = 0.f);
void DebugDrawCircle(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration = 0.f);
#endif