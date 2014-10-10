#include "BasePageList.h"
#include "inlineFunctionZK.h"

CBasePageList::CBasePageList(nuUINT nBufferSize)
{
	m_nBufferSize	= nBufferSize;
	m_pHeader		= NULL;
	m_pLast			= NULL;
}


CBasePageList::~CBasePageList()
{
}

int CBasePageList::UpdateBuffer(const nuBYTE* pNewBuffer)
{
	if( NULL == pNewBuffer )
	{
		return 0;
	}
	PAGE_NODE* pTmp = m_pHeader;
	nuUINT nCount = m_nCount;
	while( NULL != pTmp && 0 != nCount )
	{
		nuUINT nPageCount = nuFUNC_MIN(nCount, PAGE_SIZE);
		for(nuUINT i = 0; i < nPageCount; ++i)
		{
			if( 0 == Compare(pTmp->ppBuffer[i], pNewBuffer) )
			{
				nuMemcpy(pTmp->ppBuffer[i], pNewBuffer, m_nBufferSize);
				return -1;
			}
		}
		nCount -= nPageCount;
		pTmp = pTmp->pNext;
	}
	nuBYTE* pBuffer = NewRecord();
	if( NULL == pBuffer )
	{
		nuMemcpy(pBuffer, pNewBuffer, m_nBufferSize);
		return 0;
	}
	return 1;
}

nuBYTE* CBasePageList::NewRecord()
{
	if( 0 == m_nBufferSize )
	{
		return NULL;
	}
	if( NULL == m_pHeader || PAGE_SIZE * m_nPages == m_nCount )
	{
		PAGE_NODE* pNew = new PAGE_NODE;
		if( NULL == pNew )
		{
			return NULL;
		}
		nuMemset(pNew, 0, sizeof(PAGE_NODE));
		if( NULL == m_pHeader )
		{
			m_pLast = m_pHeader = pNew;
		}
		else
		{
			m_pLast = m_pLast->pNext = pNew;
		}
		++m_nPages;
	}
	nuUINT nIdx = m_nCount - (m_nPages - 1) * PAGE_SIZE;
	nuBYTE* pBuffer = new nuBYTE[m_nBufferSize];
	if( NULL != pBuffer )
	{
		++m_nCount;
		m_pLast->ppBuffer[nIdx] = pBuffer;
	}
	return pBuffer;
}

nuVOID CBasePageList::Sort()
{

}

nuINT CBasePageList::Compare(const nuBYTE* pBuff1, const nuBYTE* pBuff2)
{
	if( pBuff1 < pBuff2 )
	{
		return -1;
	}
	else if( pBuff1 == pBuff2 )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}