//--------------------------------------
// LuminaEngine belongs to Aria Bonczek
//--------------------------------------
#ifndef COMMON_HPP
#define COMMON_HPP

#if DX11

#define DELETECOM(x) {if(x) {x->Release(); x = 0;} }

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_4.h>
#endif

#if DX12

#define DELETECOM(x) {if(x) {x->Release(); x = 0;} }

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#endif

#if GL43
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <pthread.h>

typedef unsigned int GLHANDLE;
#endif

#define NS_BEGIN namespace Lumina{
#define NS_END }

#include <string>
#include <vector>

#include <Math\Math.hpp>
using namespace LMath;

#include <Utility\Timer.hpp>

#if _WIN32 && _DEBUG
#include <Utility\Debug.hpp>
#endif

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned long long uint64;
typedef long long int64;
NS_BEGIN
typedef int GUID;
NS_END

#endif