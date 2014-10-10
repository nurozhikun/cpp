#ifndef CCRITICAL_LOCK_H
#define CCRITICAL_LOCK_H
#include "windows.h"

class  CCriticalLock
{
public:
    CCriticalLock(CRITICAL_SECTION* pSection)
    {
        m_pSection = pSection;
        EnterCriticalSection(m_pSection);
    }
    ~CCriticalLock()
    {
        LeaveCriticalSection(m_pSection);
    }
private:
    CRITICAL_SECTION* m_pSection;
};
#endif