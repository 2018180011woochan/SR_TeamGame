#pragma once
#ifndef __FINAL_STAGE_H__
#include "Scene.h"
USING(Engine)
class CFinalStage final : public CScene
{
private:
	explicit CFinalStage();
	virtual ~CFinalStage() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CFinalStage* Create();
	// CScene을(를) 통해 상속됨
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
};
#define __FINAL_STAGE_H__
#endif // !__FINAL_STAGE_H__
