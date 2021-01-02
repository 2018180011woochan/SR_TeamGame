#pragma once
#ifndef __SUBJECT_H__
class CObserver;
class CSubject
{
public:
	explicit CSubject();
	virtual ~CSubject();
public:
	void AddObserver( CObserver* _pObserver);
	void RemoveObserver(CObserver* _pObserver);
protected:
	void Notify(void* pAgr);
private:
	list<CObserver*> m_listObservers;
};

#define __SUBJECT_H__
#endif
