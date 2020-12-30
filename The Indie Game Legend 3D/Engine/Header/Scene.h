#pragma once
#ifndef __SCENE_H__
#include "Base.h"
BEGIN(Engine)
class CGameObject;
class CManagement;
class ENGINE_DLL CScene abstract : public CBase
{
private:
	CManagement*		m_pManagement;
	UINT				m_nSceneID;
	bool				m_bActive;
protected:
	LPDIRECT3DDEVICE9	m_pDevice;
protected:
	explicit CScene(const UINT _nSceneID);
	//template<typename T>
	//explicit CScene();
	virtual ~CScene() = default;
	// CBase을(를) 통해 상속됨
	virtual void		Free() override;
public:
	virtual HRESULT		Awake()								PURE;
	virtual HRESULT		Start()								PURE;
	virtual	UINT		Update(const float _fDeltaTime)		PURE;
	virtual	UINT		LateUpdate(const float _fDeltaTime)	PURE;
	
	HRESULT SetUpCurrentScene(CScene* const _pCurrentScene);

	HRESULT		AddPrototype(CGameObject* const _pGameObject);
	template<typename T>
	CGameObject*	AddGameObject();
	CGameObject*	AddGameObject(const TSTRING& _sTypeName);

public:
	bool		IsActive();
	void		SetActive(const bool _bActive);
};
END
#include "Scene.hpp"
#define __SCENE_H__
#endif // !__SCENE_H__
