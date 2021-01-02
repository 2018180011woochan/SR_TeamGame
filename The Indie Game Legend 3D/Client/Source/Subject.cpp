#include "stdafx.h"
#include "..\Header\Subject.h"
#include "Observer.h"

CSubject::CSubject()
{
}


CSubject::~CSubject()
{
}

void CSubject::AddObserver(CObserver * _pObserver)
{
	m_listObservers.emplace_back(_pObserver);
}

void CSubject::RemoveObserver(CObserver * _pObserver)
{
	m_listObservers.remove(_pObserver);
}

void CSubject::Notify(void* pAgr)
{
	for (auto& pObs : m_listObservers)
	{
		pObs->OnNotify(pAgr);
	}
}
