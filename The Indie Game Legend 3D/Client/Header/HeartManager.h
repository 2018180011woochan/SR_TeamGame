#pragma once
#ifndef __HEART_MANAGER_H__
#include "GameObject.h"
USING(Engine)
class CHeart;
class CHeartManager final : public CGameObject
{
private:
	//최대 하트 개수.
	const UINT m_nMaxHeart;

	UINT m_nHeartCount;
	UINT m_nGauge;
	CHeart** m_pHeart;
private:
	explicit CHeartManager();
	explicit CHeartManager(const CHeartManager& _rOther);
	virtual ~CHeartManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CHeartManager* Create();
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
	void SetHeartCount(const UINT _nHeartCount);
	void SetGauge(const UINT _nGauge);
};
#define __HEART_MANAGER_H__
#endif // !__HEART_MANAGER_H__
