// CrossSharedMemoryZK.h: interface for the CCrossSharedMemoryZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSSHAREDMEMORYZK_H__F9636748_EEBA_44F4_9B95_EE6F35D85606__INCLUDED_)
#define AFX_CROSSSHAREDMEMORYZK_H__F9636748_EEBA_44F4_9B95_EE6F35D85606__INCLUDED_

#include "typeDefine.h"

class CCrossSharedMemoryZK  
{
public:
	CCrossSharedMemoryZK();
	virtual ~CCrossSharedMemoryZK();
    
    nuVOID* CreateSharedMemory(const nuCHAR* psName);
    nuVOID  CloseShareMemory();
private:
    nuVOID* OpenSharedMemory(const nuCHAR* psName);
private:
    nuVOID*         m_pMemory;
    nuHOBJECT       m_pFile;
    nuHOBJECT       m_pMapping;
};

#endif // !defined(AFX_CROSSSHAREDMEMORYZK_H__F9636748_EEBA_44F4_9B95_EE6F35D85606__INCLUDED_)
