#pragma once
#ifndef __STAGE_H__
#include "Scene.h"
USING(Engine)
class CStage : public CScene
{
private:
	explicit CStage();
	virtual ~CStage() = default;

public:
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;

private:
	HRESULT AddLight();
	HRESULT AddUIObject();
public:
	static CStage* Create();
	virtual void Free() override;
};

#define __STAGE_H__
#endif // !__STAGE_H__

