#pragma once
#ifndef __MONSTER_H__
#include "GameObject.h"
USING(Engine)
class CMonster : public CGameObject
{
public:
	explicit CMonster();
	explicit CMonster(const CMonster& other);
	virtual ~CMonster() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	HRESULT IsBillboarding();

public:
	virtual CGameObject * Clone() = 0;
	//부모는 체력 감소 / 사망 체크 / 이팩트 호출 / 사운드 호출 등등 코드 분활이 필요하면 
	//제각각 코드 분할 해서 모듈화 해줄것 요망
	// 테스트 용이라 override 쓰지만 순수가상으로 만들어야  할듯
	virtual void OnCollision(CGameObject* _pGameObject) override;
protected:
	const CTransform*	m_pPlayerTransform;
	class CVIBuffer*	m_pVIBufferCom = nullptr;
	class CTexture*		m_pTextureCom = nullptr;

	_vector				m_vecInitPos;
	//wjm test
	bool				m_bDead = false;
};

#define __MONSTER_H__
#endif // !__MONSTER_H__

