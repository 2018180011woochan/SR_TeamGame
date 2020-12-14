#pragma once
#ifndef __BASE_H__
#define __BASE_H__
#include "EngineInclude.h"
BEGIN(Engine)
class ENGINE_DLL CBase abstract
{
protected:
	UINT	m_nRefCount;

protected:
	explicit CBase();
	virtual ~CBase() = default;

public:
	UINT	AddRef();
	UINT	Release();

protected:
	virtual void Free() PURE;
};
END
#endif // !__BASE_H__