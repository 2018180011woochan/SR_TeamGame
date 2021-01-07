#pragma once
#ifndef __COMPUTER_H__
#include "InteractionObj.h"

USING(Engine)
class CComputer  final : public CInteractionObj
{
private:
	float		m_fInteracitonDis;

public:
	explicit CComputer();
	explicit CComputer(const CComputer& _rOther);
	virtual ~CComputer() = default;

	// CInteractionObj을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual void OnCollision(CGameObject * _pGameObject) override;
	virtual void OnNotify() override;
	virtual CGameObject * Clone() override;

	static CComputer* Create();
private:
	virtual void Free();
};
#define __COMPUTER_H__
#endif
