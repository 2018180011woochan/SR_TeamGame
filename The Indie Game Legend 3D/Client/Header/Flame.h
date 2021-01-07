#pragma once
#ifndef __FLAME_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CFlame final : public CMonster
{
private:
	explicit CFlame();
	explicit CFlame(const CFlame& other);
	virtual ~CFlame() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;
private:
	HRESULT Movement(float fDeltaTime);
	void Jumping(float fDeltaTime);
	bool isCloseToPlayer();
	
public:
	void SetPos(const _vector _Pos);
	void SetBossDead(const bool _isBossDead);

public:
	virtual CGameObject * Clone() override;
	static CFlame* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	bool				m_bIsBossDead;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __FLAME_H__
#endif // !__FLAME_H__
