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
	HRESULT AddPlayerLayer(const wstring& LayerTag);
	HRESULT AddMonsterLayer(const wstring& LayerTag);
	HRESULT AddCameraLayer(const wstring& LayerTag);

	HRESULT AddUIObject();
public:
	static CStage* Create();
	virtual void Free() override;
};

#define __STAGE_H__
#endif // !__STAGE_H__

