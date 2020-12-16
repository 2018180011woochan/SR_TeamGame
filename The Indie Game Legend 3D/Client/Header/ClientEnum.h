#pragma once
#ifndef __CLIENTENUM_H__

enum class ESceneID
{
	Static,
	Stage,
	MaxCount
};

enum class EWeaponType : unsigned int
{
	Big,
	Multiple,
	Normal,
	End
};

#define  __CLIENTENUM_H__
#endif // !__CLIENTENUM_H__
