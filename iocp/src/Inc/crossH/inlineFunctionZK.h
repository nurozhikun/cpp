#ifndef _INLINE_FUNCTION_ZK_H_20110829
#define _INLINE_FUNCTION_ZK_H_20110829

#include "crossFunctionsZK.h"

class CBaseLock
{
public:
    CBaseLock(nubool *pbLock)
    {
		m_pbLock = pbLock;
		while( *m_pbLock )
		{
			nuSleep(15);
		}
		*m_pbLock = nuTrue;
    }
    ~CBaseLock()
    {
        *m_pbLock = nuFalse;
    }
private:
    nubool*     m_pbLock;
};

class CBaseAlloc
{
public:
    CBaseAlloc()
    {
        m_pBuffer   = nuNULL;
    }
    ~CBaseAlloc()
    {
        if( m_pBuffer )
        {
            nuFree(m_pBuffer);
            m_pBuffer   = nuNULL;
        }
    }
    nuVOID* Alloc(nuUINT nSize)
    {
        if( m_pBuffer )
        {
            nuFree(m_pBuffer);
            m_pBuffer = nuNULL;
        }
        m_pBuffer = nuMalloc(nSize);
        return m_pBuffer;
    }
private:
    nuVOID* m_pBuffer;
};

#define nuFUNC_ABS(a) (((a) < 0) ? (-(a)) : (a))
#define nuFUNC_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define nuFUNC_MIN(a, b) (((a) > (b)) ? (b) : (a))

#define nuFUNC_SETSTATUS(a, s)  a |= (s)
#define nuFUNC_CLEANSTATUS(a, s) a &= ~(s)
#define nuFUNC_ISSTATUS(a, s) ((a)&(s))

#endif