#pragma once
#ifndef __GAMEOBJECT_MANAGER_H__
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)
class CGameObject;
class CCollisionManager;
class CGameObjectManager final : public CBase
{
	DECLARE_SINGLETON(CGameObjectManager)
private:
	typedef unordered_map<TSTRING, CGameObject*>		PROTOTYPE, *LPPROTOTYPE;
	typedef unordered_map<size_t, list<CGameObject*>>	GAMEOBJECTS;
	PROTOTYPE			m_Prototype;
	GAMEOBJECTS			m_GameObjects;
	CCollisionManager*	m_pCollisionManager;
private:
	explicit CGameObjectManager();
	virtual ~CGameObjectManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	HRESULT Awake(const size_t _nSceneID);
	HRESULT Start(const size_t _nSceneID);
	UINT	Update(const size_t _nSceneID, const float _fDeltaTime);
	UINT	LateUpdate(const size_t _nSceneID, const float _fDeltaTime);

	HRESULT				AddPrototype(CGameObject* const _pGameObject);
	template<typename T>
	CGameObject*		AddGameObject(const size_t _nSceneID);
	CGameObject*		AddGameObject(const size_t _nSceneID, const TSTRING& _sTypeName);
	template<typename T>
	CGameObject*		Instantiate(const size_t _nSceneID);
	template<typename T>
	CGameObject*		FindGameObjectOfType(const size_t _nSceneID);
	template<typename T>
	list<CGameObject*>	FindGameObjectsOfType(const size_t _nSceneID);
};
END
#include "GameObjectManager.hpp"
#define __GAMEOBJECT_MANAGER_H__
#endif // !__GAMEOBJECT_MANAGER_H__