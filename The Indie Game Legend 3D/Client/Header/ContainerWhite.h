#pragma once
#ifndef __CONTAINERWHITE_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CContainerWhite final : public CGameObject
{
private:
	explicit CContainerWhite();
	explicit CContainerWhite(const CContainerWhite& other);
	virtual ~CContainerWhite() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;

public:
	HRESULT IsBillboarding();

public:
	virtual CGameObject * Clone() override;
	static CContainerWhite* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
	bool m_bIsHit;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;
	UINT nIndex;
};

#define __CONTAINERWHITE_H__
#endif // !__CONTAINERWHITE_H__

