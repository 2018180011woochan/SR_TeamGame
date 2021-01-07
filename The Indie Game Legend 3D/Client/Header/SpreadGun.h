#pragma once
#ifndef __SPREADGUN_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CSpreadGun final : public CGameObject
{
private:
	explicit CSpreadGun();
	explicit CSpreadGun(const CSpreadGun& other);
	virtual ~CSpreadGun() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;

public:
	HRESULT IsBillboarding();
	void Set_ItemPos(const _vector _vItemPos);

public:
	virtual CGameObject * Clone() override;
	static CSpreadGun* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
	bool			m_bDead;
};

#define __SPREADGUN_H__
#endif // !__SPREADGUN_H__

