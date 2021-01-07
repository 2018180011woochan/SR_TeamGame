#pragma once
#ifndef __GREENBOYRIGHTHAND_H__

#include "Monster.h"
#include "TexturePoolManager.h"
USING(Engine)
class CGreenBoyRightHand final : public CMonster
{
private:
	explicit CGreenBoyRightHand();
	explicit CGreenBoyRightHand(const CGreenBoyRightHand& other);
	virtual ~CGreenBoyRightHand() = default;

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
	void BulletFire();
	void Crush(float fDeltaTime);

public:
	void SetPos(const _vector _Pos);
	void SetIsAttack(const bool _bAttack);
	void SetIsCrush(const bool _bAttack);

public:
	virtual CGameObject * Clone() override;
	static CGreenBoyRightHand* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;

	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fFireSpeed;
	float				m_fFireDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;
	bool				m_bisAttack;
	bool				m_bCrush;

	float				m_fJumpSpeed;
	float				m_fJumpDeltaTime;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __GREENBOYRIGHTHAND_H__
#endif // !__GREENBOYRIGHTHAND_H__