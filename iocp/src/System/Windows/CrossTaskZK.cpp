// CrossTaskZK.cpp: implementation of the CCrossTaskZK class.
//
//////////////////////////////////////////////////////////////////////

#include "CrossTaskZK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

nuUINT CCrossTaskZK::CreateThread(nuUINT nThdPriority /* = 0 */, 
                                  nuUINT nCreateFlags /* = 0 */, 
                                  nuUINT nStackSize /* = 1024*1024 */)
{
    if( m_handle )
    {
        return 1;
    }
    /*
    m_handle = CreateThread( NULL
        , nStackSize
        , (LPTHREAD_START_ROUTINE)ThreadProc//thdAttrs.staCbFunc
        , this//(LPVOID)thdAttrs.staParam
        , nCreateFlags
        , &m_nThdId );
        */
    m_handle = (void*)_beginthreadex(NULL
        , nStackSize
        , (unsigned int (__stdcall *)(void *))ThreadProc
        , this
        , 0
        , (unsigned int*)&m_nThdId );
    if( m_handle )
    {
        int nPriority = 0;
        bool bSet = true;
        if( 0 == nThdPriority )
        {
            bSet = false;
        }
        else if( 30 > nThdPriority )
        {
            nPriority = THREAD_PRIORITY_BELOW_NORMAL;
        }
        else
        {
            nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
        }
        if( bSet )
        {
            SetThreadPriority((HANDLE)m_handle, nPriority);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

nuVOID CCrossTaskZK::DeleteThread()
{
    m_handle = nuNULL;
    m_nThdId = 0;
    if( m_pStackAddr )
    {
        delete[] m_pStackAddr;
        m_pStackAddr = nuNULL;
    }
}

nuVOID CCrossTaskZK::EndWaiting()
{
}