// BaseCodeListZK.h: interface for the CBaseCodeListZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASECODELISTZK_H__A34A4DE8_E07B_45D8_8A78_6A0D3469646F__INCLUDED_)
#define AFX_BASECODELISTZK_H__A34A4DE8_E07B_45D8_8A78_6A0D3469646F__INCLUDED_

#include "typeDefine.h"

class CBaseCodeListZK  
{
#define BASE_LIST_NODE_DEFAULT_COUNT                16
public:
    typedef struct tagBASE_LIST_NODE
    {
        nuLONG      nCodeID;
        nuLONG      nParamX;
        nuLONG      nParamY;
        nuLONG      nParamZ;
        nuLONG      nParamEx;
        nuVOID*     pExtend;
    }BASE_LIST_NODE;
    enum emListAction
    {
        ACTION_AT_END         = 0x00,
        ACTION_AT_HEAD,
        ACTION_DEL_ALL_OTHERS,
        ACTION_FULL_COVER,
        ACTION_FULL_DEL
    };
public:
	CBaseCodeListZK();
	virtual ~CBaseCodeListZK();
    nubool  CreateCodeList(nuUINT nCodeCount = BASE_LIST_NODE_DEFAULT_COUNT);
    nuVOID  DeleteCodeList();
    
    nubool  PushNode(const BASE_LIST_NODE& codeNode, nuBYTE nAcType =ACTION_AT_END);
    nubool  PopNode(BASE_LIST_NODE& codeNode, nuBYTE nAcType = ACTION_AT_HEAD);
    nuUINT  GetNodeCount()
    {
        return m_nNowCount;
    }
#ifdef _DEBUG
public:
#else
protected:
#endif
    nuUINT              m_nCodeCount;
    BASE_LIST_NODE*     m_pCodeList;
    nuUINT              m_nNowCount;
    nuUINT              m_nFIndex;
    nuUINT              m_nLIndex;
    nubool              m_bWorking;
};

#endif // !defined(AFX_BASECODELISTZK_H__A34A4DE8_E07B_45D8_8A78_6A0D3469646F__INCLUDED_)
