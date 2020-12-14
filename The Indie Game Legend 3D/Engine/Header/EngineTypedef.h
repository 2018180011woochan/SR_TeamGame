#pragma once

#ifndef __ENGINE_TYPEDEF_H__

#pragma region TSTRING

#ifdef UNICODE
#ifndef _TSTRING_DEFINED
#define _TSTRING_DEFINED
typedef wstring TSTRING;
#endif // !_TSTRING_DEFINED
#else
#ifndef _TSTRING_DEFINED
#define _TSTRING_DEFINED
typedef string	TSTRING;
#endif // !_TSTRING_DEFINED
#endif

#pragma endregion

#pragma region STANDARD

typedef bool	_bool;
typedef char	_char;
typedef int		_int;

typedef unsigned int	_uint;
typedef unsigned char	_uchar;

#pragma endregion

#pragma region DIRECT3D

typedef D3DXVECTOR3  _vector;
typedef D3DXMATRIX   _matrix;

#pragma endregion

#pragma region TEXTURE
typedef vector<IDirect3DBaseTexture9*> TEXTURES;
#pragma endregion


#define __ENGINE_TYPEDEF_H__
#endif // !__ENGINE_TYPEDEF_H__

