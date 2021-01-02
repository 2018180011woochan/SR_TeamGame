#pragma once
#ifndef __OBSERVER_H__

class CObserver 
{
public:
	explicit CObserver();
	virtual ~CObserver();
public:
	virtual void OnNotify(void* pAgr)PURE;
};

#define __OBSERVER_H__
#endif
