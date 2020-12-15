#pragma once
#ifndef __INTRO_H__
#include "Scene.h"
USING(Engine)
class CIntro final : public CScene
{
private:
	explicit CIntro();
	virtual ~CIntro() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CIntro* Create();
public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
};
#define __INTRO_H__
#endif // !__INTRO_H__
