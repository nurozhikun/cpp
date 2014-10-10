// CrossFileZK.h: interface for the CCrossFileZK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSFILEZK_H__ED9BC726_E5D2_4E32_A4B4_8F17693AFD8E__INCLUDED_)
#define AFX_CROSSFILEZK_H__ED9BC726_E5D2_4E32_A4B4_8F17693AFD8E__INCLUDED_

#include "typeDefine.h"

class CCrossFileZK  
{
public:
	CCrossFileZK();
	virtual ~CCrossFileZK();
    enum emFilePathLen
    {
        FS_PATH_MAX = 256,
    };
    
    enum emFileOpenMode
    {
        FS_OPEN_R_EXIST     = 0,//open an existent file for reading
        FS_OPEN_RW_EXIST    = 1,//open an existent file for reading and writing
        FS_OPEN_W_EMPTY     = 2,//open an empty file for writing
        FS_OPEN_RW_EMPTY    = 3,//open an empty file for reading and writing
        FS_OPEN_A           = 4,//open an existent file or create a new file for appending.
        FS_OPEN_RA          = 5,//open an existent file or create a new file for reading and appending.
        FS_OPEN_TEXT        = 0x80,//open the file with 'text' mode
    };
    nubool  fOpen(nuTCHAR* tsName, nuBYTE nMode);
    nuVOID  fClose();
    enum emFileSeekMode
    {
        FS_SEEK_SET,
        FS_SEEK_CUR,
        FS_SEEK_END,
    };
    nubool  fSeek(nuLONG nOffSet, nuBYTE nMode);
    //@return the count of Items actually read
    nuUINT  fRead(nuVOID* pBuffer, nuUINT nItemSize, nuUINT nItemCount);
    //@return the number of full items actually written
    nuUINT  fWrite(const nuVOID* pBuffer, nuUINT nItemSize, nuUINT nItemCount);
    //
    nuUINT  fLength();
    //@return success or failure
    nubool  fRemove(nuTCHAR* tsName);
    //
    nubool  fFlush();
    //
    nubool  fIsOpen() { return (nuNULL != m_file); }
protected:
    nuHOBJECT   m_file;
};

#endif // !defined(AFX_CROSSFILEZK_H__ED9BC726_E5D2_4E32_A4B4_8F17693AFD8E__INCLUDED_)
