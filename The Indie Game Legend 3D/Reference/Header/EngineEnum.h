#pragma once

#ifndef __ENGINE_ENUM_H__

BEGIN(Engine)

enum class EDisplayMode
{
	Full,
	Window
};

enum class ERenderID
{
	Priority,
	NoAlpha,
	Alpha,
	UI,
	End
};


enum class ETextureType
{
	Normal,
	Cube,
	End

};


END

#define __ENGINE_ENUM_H__
#endif // !__ENGINE_ENUM_H__
