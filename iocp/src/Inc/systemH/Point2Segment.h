// Point2Segment.h: interface for the CPoint2Segment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT2SEGMENT_H__8ADCC149_0A90_40BB_B184_E431F7E748A7__INCLUDED_)
#define AFX_POINT2SEGMENT_H__8ADCC149_0A90_40BB_B184_E431F7E748A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <math.h>

#define LMAX(x, y)   ( x > y ? x : y )  
#define LMIN(x, y)    ( x < y ? x : y )
#define LABS(x)        (x < 0 ? (-x) : x)
#define POINT_IN_RECT(x, y, left, top, right, bottom) (( x >= left && x <= right ) && ( y >= top && y <= bottom ))

class CPoint2Segment  
{
public:
	/// 点坐标信息
	typedef struct _Point
	{
		/// 道路坐标信息 B
		int x;        ///<X坐标(经度)，转换成m单位的整数
		int y;        ///<Y坐标(纬度)，转换成m单位的整数
	}Point;

    enum emConst
    {
        DEFAULT_MAX_DISTANCE    = 100,
        DEFAULT_MAX_ANGLE_DIFF  = 60,
    };
public:
    CPoint2Segment(long nPointMax, const Point *points = NULL, long nNowCount = 0);
    virtual ~CPoint2Segment();
    void ResetCount() { m_nPointNow = 0; }
    bool AddPoint(long x, long y);
    long Fit2Line(long nX, long nY, long& nFixedDis, Point& ptFixed, long nLastFixedIdx = -1, long nAngle = -1);
    long Fit2LineEx(long nX, long nY, bool& bDeflect, long& nFixedDis, Point& ptFixed, long nLastFixedIdx = -1, long nAngle = -1);  
    inline static bool Point2Segment(long nX, long nY, long nX1, long nY1, long nX2, long nY2, 
        long &nFixedX, long &nFixedY, long& nFixDis);
	long up_count;
	long down_count;
private:
    inline static long lMin(long x, long y) { return x > y ? y : x; }
    inline static long lMax(long x, long y) { return x > y ? x : y; }
    inline static long lAbs(long x) { return x < 0 ? (-x) : x; }
    inline static bool PointInRect(long x, long y, long left, long top, long right, long bottom){
        return (( x >= left && x <= right ) & ( y >= top && y <= bottom ));
    }
private:
    const static double s_pi;
    Point*  m_ptList;
    long    m_nPointMax;
    long    m_nPointNow;
};

inline bool CPoint2Segment::Point2Segment(long nX, long nY, long nX1, long nY1, long nX2, long nY2, 
                                          long &nFixedX, long &nFixedY, long& nFixDis)
{
    long DisToPt1;
	long DisToPt2;
	if( nX1 == nX2 )
    {
        nFixedX = nX1;
		if( nFixedY > nY1 && nFixedY > nY2 )
        {
            nFixedY = lMax(nY1, nY2);
        }
        else if( nFixedY < nY1 && nFixedY < nY2 )
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
        if( nFixedX > nX1 && nFixedX > nX2 )
        {
            nFixedX = lMax(nX1, nX2); 
        }
        else if( nFixedX < nX1 && nFixedX < nX2 )
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
	nFixDis = (long)sqrt( (float)dnum );
	return true;
}

#endif // !defined(AFX_POINT2SEGMENT_H__8ADCC149_0A90_40BB_B184_E431F7E748A7__INCLUDED_)
