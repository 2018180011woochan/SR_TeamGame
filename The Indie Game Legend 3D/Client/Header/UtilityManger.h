#pragma once
#ifndef __PEEKINGMANAGER_H__
USING(Engine)
//이름 바꿀거임
 class CUtilityManger
{
public:
	static bool CrossHairPicking(_uint _nSceneID,OUT _vector& _vPickingPos/*,const _uint _nRoomID*/);
	static void ObjectCulling(_uint _nSceneID , _uint _nTag);
};
#define __PEEKINGMANAGER_H__
#endif __PEEKINGMANAGER_H__
