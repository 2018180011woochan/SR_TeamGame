#pragma once
#ifndef __CRYDER_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CCryder final : public CMonster
{
private:
	explicit CCryder();
	explicit CCryder(const CCryder& other);
	virtual ~CCryder() = default;

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
	static CCryder* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fMoveSpeed;
	bool				m_isMaxJump;
	bool				m_isJumping;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __CRYDER_H__
#endif // !__CRYDER_H__