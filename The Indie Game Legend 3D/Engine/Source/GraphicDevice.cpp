#include "..\Header\GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
	: m_pD3D9(nullptr)
	, m_pDevice(nullptr)
{
}

HRESULT CGraphicDevice::Initialize(const HWND _hWnd, const UINT _nWinCX, const UINT _nWinCY, const EDisplayMode _eDisplayMode)
{
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (FAILED(m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		PrintLog(TEXT("Error"), TEXT("Faile to GetDeviceCaps"));
		return E_FAIL;
	}

	DWORD vp = 0;
	if (DeviceCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = _nWinCX;
	d3dpp.BackBufferHeight = _nWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = _hWnd;
	d3dpp.Windowed = (BOOL)_eDisplayMode;// TRUE일 경우 창모드, FALSE 전체화면. 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, vp, &d3dpp, &m_pDevice)))
	{
		PrintLog(TEXT("Error"), TEXT("Failed to create graphic device."));
		return E_FAIL;
	}

	return S_OK;
}

void CGraphicDevice::Free()
{
	SafeRelease(m_pDevice);
	SafeRelease(m_pD3D9);
}
