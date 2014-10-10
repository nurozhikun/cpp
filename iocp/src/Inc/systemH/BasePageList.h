#ifndef __BASE_PAGE_LIST_H
#define __BASE_PAGE_LIST_H

#include "typeDefine.h"

class CBasePageList
{
public:
	enum emConstant{
		PAGE_SIZE		= 128,
	};
	struct PAGE_NODE{
		nuBYTE*		ppBuffer[PAGE_SIZE];		
		PAGE_NODE*	pNext;
	};
//	typedef nuINT(CBasePageList::*CompareProc)(const nuBYTE*, const nuBYTE*);
public:
	CBasePageList(nuUINT nBufferSize);
	virtual ~CBasePageList();
	int UpdateBuffer(const nuBYTE* pNewBuffer);

	nuBYTE* NewRecord();
	nuVOID  Sort();
private:
	virtual nuINT Compare(const nuBYTE* pBuff1, const nuBYTE* pBuff2);
private:
	nuUINT		m_nBufferSize;
	PAGE_NODE*	m_pHeader;
	PAGE_NODE*	m_pLast;
	nuUINT		m_nCount;
	nuUINT		m_nPages;
};


#endif