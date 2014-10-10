// SocketThreadZK.h: interface for the CSocketThreadZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETTHREADZK_H__C88BA84B_11F3_4E8D_AC40_4DB43AC4A60A__INCLUDED_)
#define AFX_SOCKETTHREADZK_H__C88BA84B_11F3_4E8D_AC40_4DB43AC4A60A__INCLUDED_

#include "CrossThreadZK.h"

class CSocketThreadZK : public CCrossThreadZK
{
public:
	CSocketThreadZK();
	virtual ~CSocketThreadZK();

    nuFINAL StartThread(class CSockServerZK* pSockServer);
    nuFINAL CloseThread();
protected:
//    virtual nuDWORD ThreadBegin();
    virtual nuDWORD ThreadEnd();
    virtual nuDWORD ThreadAction();
//    virtual nuDWORD MessageProc(CBaseCodeListZK::BASE_LIST_NODE& node);
private:
    class  CSockServerZK*   m_pSockServer;
};

#endif // !defined(AFX_SOCKETTHREADZK_H__C88BA84B_11F3_4E8D_AC40_4DB43AC4A60A__INCLUDED_)
