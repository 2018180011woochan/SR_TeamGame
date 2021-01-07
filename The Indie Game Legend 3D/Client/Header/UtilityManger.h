#pragma once
#ifndef __PEEKINGMANAGER_H__
USING(Engine)
class CUtilityManger
{
public:
	static list<CGameObject*>  m_RoomMobList;
public:
	static bool CrossHairPicking(_uint _nSceneID,OUT _vector& _vPickingPos/*,const _uint _nRoomID*/);
	static bool AirstrikePicking(_uint _nSceneID, OUT _vector& _vPickingPos/*,const _uint _nRoomID*/);

	static bool AutoAim(_uint _nSceneID, OUT list<CGameObject*>& _listGameObj);
	static bool AutoAim(_uint _nSceneID, OUT CGameObject*&  _pGameObject);

	static void ObjectCulling(_uint _nSceneID , _uint _nTag);
	static void Release();
private:
	static void ReleaseList();
};
#define __PEEKINGMANAGER_H__
#endif __PEEKINGMANAGER_H__
