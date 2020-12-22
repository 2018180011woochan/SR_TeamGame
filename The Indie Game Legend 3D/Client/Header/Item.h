#pragma once
#ifndef __ITEM_H__
#include "GameObject.h"
USING(Engine)
class CItem final: public CGameObject
{
private:
	CMeshRenderer*				 m_pMeshRenderer = nullptr;
	class CTexturePool*			 m_pTexturePool = nullptr;
	CCollider*				   	 m_pCollider = nullptr;
	_uint						m_nMaxFrame = 0;
	float						m_fAnimateOneCycleTime = 0.f;
	float						m_fAnimateSpeed = 0.f;
	float						m_fTextureIndex = 0.f;
	TSTRING						m_sTextureName;
	bool						m_bDelete = false;
	EItemID						m_eType;
private:
	HRESULT Animate(const float _fDeltaTime);
	HRESULT	BillBord();
public:
	void    SetItemType(const EItemID& _eID);

public:
	const EItemID& GetItemID() { return m_eType; }

public:
	explicit CItem();
	explicit CItem(const CItem& _rOther);
	virtual ~CItem() = default;
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;
public:
	virtual void Free();
	static CItem* Create();
};
#define __ITEM_H__
#endif // !__ITEM_H__
