#pragma once

#include "chunkgroup.h"
extern ChunkGroup* pool;
/*
* inline���� ���� ����
* inline�� ODR(One definion rull) �ذ� ����
* inline Ű���� �ٸ� �ߺ� ������ �����Ѵ�.
* #include �ǹ� �ڵ带 �ٿ��� �ְٴ�
*/
/*  �̰ͺ��� �� ���� �̱����� ���� �ִ�.
	��Ȳ�� �´� ���� �̱����� ����ϱ� �ٶ���. */
template <size_t OBJ_SIZE, unsigned char MAX_OBJ_NUM>
ChunkGroup* ChunkGroupInstance()
{
	if (!pool)
		pool = new ChunkGroup(OBJ_SIZE, MAX_OBJ_NUM);
	return pool;
}
/* ������θ� ����Ѵٸ�, ���ø����� �ھ� �д�. */
template <typename T, size_t MAX_OBJ_NUM>
class SmallObjAllocator
{
public:
	static void* operator new(size_t s)
	{
		return ChunkGroupInstance<sizeof(T), MAX_OBJ_NUM>()->alloc();
	}

	static void operator delete(void* pdelloc_target, std::size_t obj_size)
	{
		if (NULL == pdelloc_target)
			return;

		ChunkGroupInstance<sizeof(T), MAX_OBJ_NUM>()->delloc(pdelloc_target);
	}
};
