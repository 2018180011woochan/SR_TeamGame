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
	Rapid,
	Normal,
	End
};

enum class EItemID : unsigned int
{
	Heart,
	Ammo,
	sprCoin,
	sprBigCoin,
	End
};
#define  __CLIENTENUM_H__
#endif // !__CLIENTENUM_H__
