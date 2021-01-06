#pragma once
#ifndef __GAMEOBJECT_H__
#include "Base.h"
BEGIN(Engine)
class CManagement;
class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CManagement*	m_pManagement;
protected:
	typedef unordered_map<TSTRING, CComponent*> COMPONENTS;
	COMPONENTS m_Components;

	LPDIRECT3DDEVICE9	m_pDevice;
	UINT				m_nSceneID;
	ERenderID			m_eRenderID;
	class CTransform*   m_pTransform;
	_int				m_nTag;

	bool				m_bEnable;
	TSTRING				m_sName;
	//  [1/5/2021 wades]
	bool				m_bDelete;
protected:
	explicit CGameObject();
	explicit CGameObject(const CGameObject& _rOther);
	virtual ~CGameObject() = default;
protected:
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;

public:
	void			SetTag(const _int& _nTag);
	_int			GetTag();
	TSTRING			GetName();

public:
	virtual HRESULT			InitializePrototype()					PURE;
	virtual HRESULT			Awake()									PURE;
	virtual HRESULT			Start()									PURE;
	virtual UINT			Update(const float _fDeltaTime)			PURE;
	virtual UINT			LateUpdate(const float _fDeltaTime)		PURE;
	virtual HRESULT			Render()								PURE;

	virtual CGameObject*	Clone()									PURE;

	virtual void			OnCollision(CGameObject* _pGameObject);
	virtual void			OnTrigger(CGameObject* _pGameObject);

	template<typename T>
	CGameObject*			AddGameObject();
	template<typename T> 
	CGameObject*			FindGameObjectOfType();
	template<typename T>
	list<CGameObject*>		FindGameObjectsOfType();

	template<typename T>
	CComponent*				AddComponent();
	template<typename T>
	CComponent*				GetComponent();
	CComponent*				GetComponent(const TSTRING& _sTypeName);

	void					SetSceneID(const UINT _nSceneID);

	bool					IsEnable();

	virtual void			OnEnable();
	virtual void			OnDisable();

	void					SetEnable(const bool _bEnable);
	//  [1/5/2021 wades]
	HRESULT					GetWorldPos(OUT D3DXVECTOR3& _vWorldPos);
	HRESULT					SetPosition(const D3DXVECTOR3& _vPosition);
};
END
#include "GameObject.hpp"
#define __GAMEOBJECT_H__
#endif // !__GAMEOBJECT_H__