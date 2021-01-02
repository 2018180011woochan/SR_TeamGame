#pragma once
#ifndef __LIGHTMANAGER_H_
#include "Base.h"
USING(Engine)
#define LIGHT_MAX 10
class CLightMananger final : public CBase
{
	DECLARE_SINGLETON(CLightMananger)
public:
	enum LightID  : int
	{
		Player, World1, World2,World3, World4, World5, World6, World7, World8, End
	};
	// 이넘버전이 
	D3DLIGHT9*	m_pLightChannel[(_uint)LightID::End]; 
	//맵으로 짜는것도 되긴한데 이넘 버전이 더 땡김 혼용은 막히는 부분이 있어서 맵버전은 제작중단
	map<wstring, D3DLIGHT9>		m_mapLights;

	_uint						m_nUseLightCount; //켜져있는 라이트 수
	bool						m_bLightEnable;
	LPDIRECT3DDEVICE9		    m_pDevice;
public:
	HRESULT CreatePoint(const LightID& _eID,const _vector& _vPosition ,const D3DXCOLOR& _Ambient
		, const D3DXCOLOR& _Diffuse, const D3DXCOLOR& _Specular);
	HRESULT CreateDirction(const LightID& _eID, const _vector& _vDirection, const D3DXCOLOR& _Ambient
		, const D3DXCOLOR& _Diffuse, const D3DXCOLOR& _Specular);
	HRESULT CreateSpotlight(const LightID& _eID, const _vector& _vPosition, const _vector& _vDirection, const D3DXCOLOR& _Ambient
		, const D3DXCOLOR& _Diffuse, const D3DXCOLOR& _Specular,const float& _fTheta , const float& _fPhi);

	HRESULT LightEnable(const LightID & _eID, const bool& _bEnable);

	D3DLIGHT9* GetLight(const LightID& _eID);
	void	   SetLight(const LightID& _eID);
	void	   LightOn();
	void	   LightOff();
private:
	explicit CLightMananger();
	virtual ~CLightMananger() = default;
	virtual void Free() override;
};
#define __LIGHTMANAGER_H_
#endif
