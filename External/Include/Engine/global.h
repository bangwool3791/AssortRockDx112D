#pragma once

#include <d3d11.h>
/*
* Compile HLSL code or an effect file into bytecode for a given target.
* https://learn.microsoft.com/en-us/windows/win32/api/d3dcompiler/nf-d3dcompiler-d3dcompile
*/
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#include "define.h"
#include "struct.h"