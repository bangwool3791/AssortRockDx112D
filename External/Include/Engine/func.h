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
		if (nullptr != iter->second)
			delete iter->second.Get();
	}
	_map.clear();
}

template<typename InputIter>
void Safe_Del_Array(InputIter begin, InputIter end)
{
	for (auto Iter{ begin }; Iter != end; ++Iter)
	{
		delete* Iter;
	}
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