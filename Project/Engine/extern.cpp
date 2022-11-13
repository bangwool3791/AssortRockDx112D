#include "pch.h"

#include "global.h"

tTransform g_transform = {};
tObjectRender g_objectInfo = {};
tGlobalData g_global = {};
ChunkGroup* pool;

const Vector3 Vector3::Zero{ 0.f, 0.f, 0.f };
const Vector3 Vector3::One{ 1.f, 1.f, 1.f };
const Vector3 Vector3::UnitX{ 1.f, 0.f, 0.f };
const Vector3 Vector3::UnitY{ 0.f, 1.f, 0.f };
const Vector3 Vector3::UnitZ{ 0.f, 0.f, 1.f };
const Vector3 Vector3::Up{ 0.f, 1.f, 0.f };
const Vector3 Vector3::Down{ 0.f, -1.f, 0.f };
const Vector3 Vector3::Right{ 1.f, 0.f, 0.f };
const Vector3 Vector3::Left{ -1.f, 0.f, 0.f };
const Vector3 Vector3::Forward;
const Vector3 Vector3::Front{ 0.f, 0.f, -1.f };
const Vector3 Vector3::Backward{ 0.f, 0.f, 1.f };