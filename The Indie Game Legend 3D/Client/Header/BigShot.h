#pragma once
#ifndef __BIGSHOT_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CBigShot final : public CGameObject
{
private:
	explicit CBigShot();
	explicit CBigShot(const CBigShot& other);
	virtual ~CBigShot() = default;

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
	static CBigShot* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
};

#define __BIGSHOT_H__
#endif // !__BIGSHOT_H__

