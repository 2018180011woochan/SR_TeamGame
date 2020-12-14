#pragma once
#ifndef __MAINAPP_H__
#include "Base.h"
#include "TexturePoolManager.h"
USING(Engine)
class CMainApp : public CBase
{
private:
	CManagement*	m_pManagement;
	CTexturePoolManager* m_pTexturePoolManager;
	LPDIRECT3DDEVICE9 m_pDevice;

	//private:
	explicit CMainApp();
	virtual ~CMainApp() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

public:
	HRESULT	Initialize();
	UINT	Update();

	static CMainApp* Create();
private:
	// 스태틱 아님 일단 스테이지로만듬
	HRESULT ReadyStageResources();
	HRESULT ReadyDefaultSetting();


};
#define __MAINAPP_H__
#endif // !__MAINAPP_H__
