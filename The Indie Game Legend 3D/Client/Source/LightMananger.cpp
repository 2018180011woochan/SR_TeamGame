#include "stdafx.h"
#include "LightMananger.h"

IMPLEMENT_SINGLETON(CLightMananger)

HRESULT CLightMananger::CreatePoint(const LightID & _eID, const _vector & _vPosition, const D3DXCOLOR & _Ambient, const D3DXCOLOR & _Diffuse, const D3DXCOLOR & _Specular)
{
	if (nullptr != m_pLightChannel[_eID])
		return E_FAIL;

	D3DLIGHT9* pLight = new D3DLIGHT9;
	memset(pLight, 0,sizeof(D3DLIGHT9));
	pLight->Type = D3DLIGHT_POINT;
	pLight->Ambient = _Ambient;
	pLight->Diffuse = _Diffuse;
	pLight->Diffuse.a = 1.f;
	pLight->Specular = _Specular;
	pLight->Position = _vPosition;
	pLight->Range = 500.0f;
	pLight->Falloff = 1.0f;
	pLight->Attenuation0 = 0.0f;
	pLight->Attenuation1 = 0.0035f;
	pLight->Attenuation2 = 0.0f;
	m_pLightChannel[_eID] = pLight;
	m_pDevice->SetLight(_eID, m_pLightChannel[_eID]);
	return S_OK;
}

HRESULT CLightMananger::CreateDirction(const LightID & _eID, const _vector & _vDirection, const D3DXCOLOR & _Ambient, const D3DXCOLOR & _Diffuse, const D3DXCOLOR & _Specular)
{
	if (nullptr != m_pLightChannel[_eID])
		return E_FAIL;
	D3DLIGHT9* pLight = new D3DLIGHT9;
	memset(pLight, 0, sizeof(D3DLIGHT9));
	pLight->Type = D3DLIGHT_DIRECTIONAL;
	pLight->Ambient = _Ambient;
	pLight->Diffuse = _Diffuse;
	pLight->Diffuse.a = 1.f;
	pLight->Specular = _Specular;
	_vector vDir = _vDirection;
	D3DXVec3Normalize(&vDir, &vDir);
	pLight->Direction = vDir;
	
	m_pLightChannel[_eID] = pLight;
	m_pDevice->SetLight(_eID, m_pLightChannel[_eID]);
	return S_OK;
}

HRESULT CLightMananger::CreateSpotlight(const LightID & _eID, const _vector & _vPosition, const _vector & _vDirection, const D3DXCOLOR & _Ambient, const D3DXCOLOR & _Diffuse, const D3DXCOLOR & _Specular, const float& _fTheta, const float& _fPhi)
{
	if (nullptr != m_pLightChannel[_eID])
		return E_FAIL;
	D3DLIGHT9* pLight = new D3DLIGHT9;
	memset(pLight, 0, sizeof(D3DLIGHT9));

	pLight->Type = D3DLIGHT_SPOT;
	pLight->Ambient = _Ambient;
	pLight->Diffuse = _Diffuse;
	pLight->Diffuse.a = 1.f;


	pLight->Specular = _Specular;

	pLight->Position = _vPosition;
	pLight->Direction = _vDirection;

	pLight->Attenuation0 = 0.00000001f;
	pLight->Attenuation1 = 0.00000001f;
	pLight->Attenuation2 = 0.00000001f;
	pLight->Range = 1000.0f;
	pLight->Falloff = 0.0001f;

	pLight->Theta = _fTheta;
	pLight->Phi = _fPhi;

	m_pLightChannel[_eID] = pLight;
	m_pDevice->SetLight(_eID, m_pLightChannel[_eID]);
	return S_OK;
}



HRESULT CLightMananger::LightEnable(const LightID & _eID, const bool & _bEnable)
{
	if (_bEnable && LIGHT_MAX <= m_nUseLightCount)
		return E_FAIL;
	if (!_bEnable && 0 == m_nUseLightCount)
		return E_FAIL;
	m_pDevice->LightEnable(_eID, _bEnable);

	_bEnable ? ++m_nUseLightCount : --m_nUseLightCount;

	return S_OK;
}

D3DLIGHT9 * CLightMananger::GetLight(const LightID & _eID)
{
	return m_pLightChannel[_eID];
}

void CLightMananger::SetLight(const LightID & _eID)
{
	if (nullptr == m_pLightChannel[_eID] || CLightMananger::End == _eID)
		return;
	m_pDevice->SetLight((DWORD)_eID, m_pLightChannel[_eID]);
}

void CLightMananger::LightOn()
{
	if (m_bLightEnable)
		return;
	m_bLightEnable = true;
	m_pDevice->SetRenderState(D3DRS_LIGHTING, m_bLightEnable);
}

void CLightMananger::LightOff()
{
	if (!m_bLightEnable)
		return;
	m_bLightEnable = false;
	m_pDevice->SetRenderState(D3DRS_LIGHTING, m_bLightEnable);
}

CLightMananger::CLightMananger()
	:m_bLightEnable(false)
	, m_nUseLightCount(0)
	, m_pDevice(nullptr)
{
	for (auto& pChannel : m_pLightChannel)
	{
		pChannel = nullptr;
	}
	m_pDevice = nullptr;
	m_pDevice = CManagement::GetInstance()->GetDevice();
	//장치가 알아서 법선백터 관리 
	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS,TRUE);
	SafeAddRef(m_pDevice);
}

void CLightMananger::Free()
{
	for (auto& pChannel : m_pLightChannel)
	{
		SafeDelete(pChannel);
	}
	SafeRelease(m_pDevice);

	m_mapLights.clear();
}
