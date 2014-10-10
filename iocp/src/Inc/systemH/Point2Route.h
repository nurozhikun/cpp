// Point2Segment.h: interface for the CPoint2Route class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT2SEGMENT_H__8ADCC149_0A90_40BB_B184_E431F7E748A7__INCLUDED_)
#define AFX_POINT2SEGMENT_H__8ADCC149_0A90_40BB_B184_E431F7E748A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "Windows.h" 

class CPoint2Route  
{
public:
    enum emConst
    {
        DEFAULT_MAX_DISTANCE    = 100,
        DEFAULT_MAX_ANGLE_DIFF  = 45,
    };
    struct NODE_INFO
    {
        long    nIndex;
        long    nDistance;
    };
public:
/*	CPoint2Route(long nPointMax, const POINT *points = NULL, long nNowCount = 0);*/
/*  CPoint2Route(LineCoordinates *points);*/
    CPoint2Route(long nPointMax, long nNodeMax);
    virtual ~CPoint2Route();
    void ResetCount() { m_nPointNow = 0; }
    bool AddPoint(long x, long y);
    bool AddNodeInfo(long nIdx, long ndis);
    long Fit2Line(long nX, long nY, long& nFixedDis, POINT& ptFixed, long nLastFixedIdx = -1, long nAngle = -1);

    inline static bool Point2Segment(long nX, long nY, long nX1, long nY1, long nX2, long nY2, 
        long &nFixedX, long &nFixedY, long& nFixDis, long nAngle = -1);
private:
    inline static long lMin(long x, long y) { return x > y ? y : x; }
    inline static long lMax(long x, long y) { return x > y ? x : y; }
    inline static long lAbs(long x) { return x < 0 ? (-x) : x; }
    inline static bool PointInRect(long x, long y, long left, long top, long right, long bottom){
        return (( x >= left && x <= right ) & ( y >= top && y <= bottom ));
    }
private:
    const static double s_pi;
    POINT*  m_ptList;
    long    m_nPointMax;
    long    m_nPointNow;
    NODE_INFO*  m_pNodeList;
    long        m_nNodeMax;
    long        m_nNodeNow;
    long        m_nLastFixedIdx;
    POINT       m_ptFixed;
};

inline bool CPoint2Route::Point2Segment(long nX, long nY, long nX1, long nY1, long nX2, long nY2, 
                                          long &nFixedX, long &nFixedY, long& nFixDis, long nAngle /* = -1 */)
{
    long nLeft, nRight, nTop, nBottom;
    nLeft      = lMin(nX1, nX2) - nFixDis;
    nRight     = lMax(nX1, nX2) + nFixDis;
    nTop       = lMin(nY1, nY2) - nFixDis;
    nBottom    = lMax(nY1, nY2) + nFixDis;
    if( !PointInRect(nX, nY, nLeft, nTop, nRight, nBottom) )
    {
        return false;
    }
    if( nAngle >= 0 )
    {
        long dx = nX2 - nX1;
        long dy = nY2 - nY1;
        long nSegAngle = (long)(atan2((double)dy, (double)dx) * 180 / s_pi);
        if( nSegAngle < 0 )
        {
            nSegAngle += 360;
        }
        long nDifAngle = (nAngle - nSegAngle) % 360;
        nDifAngle = lAbs(nDifAngle);
        if( nDifAngle > DEFAULT_MAX_ANGLE_DIFF && nDifAngle < 360 - DEFAULT_MAX_ANGLE_DIFF )
        {
            return false;
        }
    }
    long DisToPt1;
	long DisToPt2;
	if( nX1 == nX2 )
    {
        nFixedX = nX1;
		if( nY > nY1 && nY > nY2 )
        {
            nFixedY = lMax(nY1, nY2);
        }
        else if( nY < nY1 && nY < nY2 )
        {
            nFixedY = lMin(nY1, nY2);
        }
        else
        {
            nFixedY = nY;
        }
    }
    else if( nY1 == nY2 )
    {
        nFixedY = nY1;
        if( nX > nX1 && nX > nX2 )
        {
            nFixedX = lMax(nX1, nX2); 
        }
        else if( nX < nX1 && nX < nX2 )
        {
            nFixedX = lMin(nX1, nX2 );
        }
        else
        {
            nFixedX = nX;
        }
    }
	else
	{
		/************************************************************************ 
		点到直线的垂足点的计算公式
		x = ( k^2 * pt1.x + k * (point.y - pt1.y ) + point.x ) / ( k^2 + 1) ，
		y = k * ( x - pt1.x) + pt1.y;
		k 是直线的斜率，pt1是直线上的一点 point是直线外的点
			************************************************************************/
			/*
			k = ( (float)(nY2 - nY1) / (float)(nX2 - nX1) );
			nFixedX =(long)( ( k*k*nX1 + k*(nY-nY1) + nX ) / ( k*k +1 ) );
			nFixedY =(long)( k * ( nFixedX - nX1 ) + nY1 );
			*/
			/*
			k=((nY-nY1)*(nX2 - nX1)-(nX-nX1)*(nY2 - nY1))/((nX2 - nX1)*(nX2 - nX1)+(nY2 - nY1)*(nY2 - nY1));
			nFixedX	=	nX + (long)(k*(float)(nY2 - nY1));
			nFixedY	=	nY - (long)(k*(float)(nX2 - nX1));
			*/
//			k=((nY-nY1)*(nX2 - nX1)-(nX-nX1)*(nY2 - nY1))/((nX2 - nX1)*(nX2 - nX1)+(nY2 - nY1)*(nY2 - nY1));
			nFixedX	=	nX + ((nY2 - nY1)*((nY-nY1)*(nX2 - nX1)-(nX-nX1)*(nY2 - nY1)))/((nX2 - nX1)*(nX2 - nX1)+(nY2 - nY1)*(nY2 - nY1));
			nFixedY	=	nY - ((nX2 - nX1)*((nY-nY1)*(nX2 - nX1)-(nX-nX1)*(nY2 - nY1)))/((nX2 - nX1)*(nX2 - nX1)+(nY2 - nY1)*(nY2 - nY1));
			
			if( nFixedX > lMax(nX1, nX2) || 
				nFixedX < lMin(nX1, nX2) ||
				nFixedY > lMax(nY1, nY2) || 
				nFixedY < lMin(nY1, nY2) )
			{
				DisToPt1 = ( nFixedX - nX1 );
				DisToPt2 = ( nFixedX - nX2 );
				DisToPt1 = lAbs(DisToPt1);
				DisToPt2 = lAbs(DisToPt2);
				if( DisToPt1 > DisToPt2 )
				{
                    nFixedX = nX2;
                    nFixedY = nY2;
				}
				else
				{
                    nFixedX = nX1;
                    nFixedY = nY2;
				}
			}
	}
	
	long dx, dy, dnum;
	dx = (nFixedX - nX);
	dy = (nFixedY - nY);
	dnum=(dx*dx)+(dy*dy);
	nFixDis = (long)sqrt((double) dnum );
	return true;
}

#endif // !defined(AFX_POINT2SEGMENT_H__8ADCC149_0A90_40BB_B184_E431F7E748A7__INCLUDED_)
