#pragma once
#ifndef __RUB_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CRub final : public CMonster
{
private:
	explicit CRub();
	explicit CRub(const CRub& other);
	virtual ~CRub() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

private:
	HRESULT Movement(float fDeltaTime);
	void Jumping(float fDeltaTime);

public:

public:
	virtual CGameObject * Clone() override;
	static CRub* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fJumpingCnt = 0;
	float				m_fJumpSpeed = 10;
	float				m_fMaxJump = 5.f;
	float				m_fMoveSpeed = 8.f;
	bool				m_isMaxJump = false;
	bool				m_isJumping = false;

	UINT nIndex = 0;

	bool m_bIsTPS;
};

#define __RUB_H__
#endif // !__RUB_H__