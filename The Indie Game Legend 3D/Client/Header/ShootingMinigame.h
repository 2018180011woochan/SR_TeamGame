#pragma once
#ifndef __SHOOTINGMINIGAME_H__
#define __SHOOTINGMINIGAME_H__
#include "Scene.h"
USING(Engine)
class CShootingMinigame final : public CScene
{
private:
	explicit CShootingMinigame();
	virtual ~CShootingMinigame() = default;

public:
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;

public:
	static CShootingMinigame* Create();
	virtual void Free() override;
};

#endif // !__SHOOTINGMINIGAME_H__

