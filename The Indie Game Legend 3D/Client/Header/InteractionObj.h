#pragma once
#ifndef _INTERACTIONOBJ_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CInteractionObj abstract : public CGameObject
{
protected:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool = nullptr;
	CCollider*	   m_pCollider = nullptr;
	list<CInteractionObj*>			m_listObservers;
	_uint		   m_nAttachID;
public:
	void	SetAttachID(const _uint& _nID) {  m_nAttachID = _nID; };
	_uint   GetAttachID() { return m_nAttachID; }
protected:
	explicit CInteractionObj();
	explicit CInteractionObj(const CInteractionObj& _rOther);
	virtual ~CInteractionObj() = default;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() PURE;
	virtual HRESULT Awake() PURE;
	virtual HRESULT Start() PURE;
	virtual UINT Update(const float _fDeltaTime) PURE;
	virtual UINT LateUpdate(const float _fDeltaTime) PURE;
	virtual HRESULT Render() PURE;
	virtual CGameObject * Clone() PURE;
	virtual void OnCollision(CGameObject* _pGameObject) PURE;
	virtual void Free() override;
protected:
	virtual void OnNotify()PURE;
public:
	void AddObserver(CInteractionObj* _pObj);
	void RemoveObserver(CInteractionObj* _pObj);
	void Notify();
};

#define _INTERACTIONOBJ_H__
#endif
