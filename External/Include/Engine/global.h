#pragma once


#include <Windows.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <iterator>
#include <array>
#include <iterator>
#include <algorithm>
#include <queue>
#include <iostream>
#include <stdlib.h>

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::back_insert_iterator;

using std::string;
using std::wstring;

using std::array;
using std::queue;

using std::begin;
using std::cbegin;

using std::end;
using std::cend;

using std::cin;
using std::cout;
using std::endl;

#include <typeinfo>

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
#include "func.h"
#include "d3dUtil.h"
#include "singleton.h"