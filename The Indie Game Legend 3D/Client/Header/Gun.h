#pragma once
#ifndef __GUN_H__
#include "GameObject.h"
USING(Engine)
class CPlayerCamera;
class CGun final : public CGameObject
{
public:
	enum STATE { NONE, MOVE, FIRE };
private:
	CPlayerCamera*	m_pPlayerCamera;
	CMeshRenderer*	m_pMeshRenderer;
	STATE			m_eState;
	bool			m_bFire;
private:
	explicit CGun();
	explicit CGun(const CGun& _rOther);
	virtual ~CGun() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CGun* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	void SetState(CGun::STATE _eState);
private:
	void Animate(const float _fDeltaTime);
	void Move(const float _fDeltaTime);
	void Fire(const float _fDeltaTime);
public:
	bool SetFire();
	void SetActive(const bool _bActive);
};
#define __GUN_H__
#endif // !__GUN_H__
