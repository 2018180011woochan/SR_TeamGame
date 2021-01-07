#include "stdafx.h"
#include "..\Header\SlideBlock.h"


CSlideBlock::CSlideBlock()
{
}

CSlideBlock::CSlideBlock(const CSlideBlock & _rOther)
{
}


CSlideBlock::~CSlideBlock()
{
}

HRESULT CSlideBlock::InitializePrototype()
{
	return E_NOTIMPL;
}

HRESULT CSlideBlock::Awake()
{
	return E_NOTIMPL;
}

HRESULT CSlideBlock::Start()
{
	return E_NOTIMPL;
}

UINT CSlideBlock::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CSlideBlock::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CSlideBlock::Render()
{
	return E_NOTIMPL;
}

CGameObject * CSlideBlock::Clone()
{
	return nullptr;
}

CSlideBlock * CSlideBlock::Create()
{
	return nullptr;
}

void CSlideBlock::Free()
{
}
