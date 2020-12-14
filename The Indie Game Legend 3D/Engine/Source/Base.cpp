#include "..\Header\Base.h"

USING(Engine)

CBase::CBase()
	: m_nRefCount(0)
{
}

UINT CBase::AddRef()
{
	return (++m_nRefCount);
}

UINT CBase::Release()
{
	if (0 == m_nRefCount)
	{
		Free();
		delete this;
		return 0;
	}
	return (--m_nRefCount);
}

