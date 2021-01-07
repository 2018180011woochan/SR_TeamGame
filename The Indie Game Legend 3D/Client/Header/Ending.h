#pragma once
#ifndef __ENDING_H__
#include "Scene.h"
USING(Engine)
class CEnding final : public CScene
{
private:
	explicit CEnding();
	virtual ~CEnding() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CEnding* Create();
public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
};
#define __ENDING_H__
#endif // !__ENDING_H__
