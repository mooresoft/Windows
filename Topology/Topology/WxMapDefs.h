#pragma once

#include <atlimage.h>   // CImage

#include "HPVEC.H"

#define WM_MAP_CTRL			(WM_USER + 103)

// 外部数据的绘制模式
#define MDRAW_DEFAULT	1
#define MDRAW_OUTPUT    2
#define MDRAW_MASKCOLOR    9


#define MOBJECT_POINTIN   3
#define MOBJECT_SELECT   4
#define MOBJECT_CHGPOS	5
#define MOBJECT_MOVE	6
#define MPOINT_POS		7
#define MOBJECT_ADD		8
#define MOBJECT_RMENU	10


#define TIMER_PICVIEW_MOVE 1001

#define MAPCTRL_SHOW_HRULER (0x1)
#define MAPCTRL_SHOW_VRULER (0x2)
#define MAPCTRL_SHOW_BRULER (0x3)

#define MAPCTRL_RULER_WIDTH (20)

#define MAPCTRL_IMGBDR_WIDTH (100)


typedef enum PIC_RESIZE_STRATEGY{
	prsMM = 0x00,
	prsML = 0x01,
	prsMR = 0x02,
	prsTM = 0x10,
	prsTL = 0x11,
	prsTR = 0x12,
	prsBM = 0x20,
	prsBL = 0x21,
	prsBR = 0x22
};

typedef enum PictureViewItemSelect{
	pvisPic = 0x4000,
	pvisUser = 0x800,
	pvisRuler = 0x40,
	pvisThmb = 0x20,
	pvisNone = 0x8000
}PvItmSel;

//黑背景
static COLORREF colormap[] = {
	RGB(255,255,255), RGB(0,0,46), RGB(0,0,51), RGB(0,5,55), 
	RGB(0,10,60), RGB(0,16,64), RGB(0,23,68), RGB(0,30,72),
	RGB(0,38,77), RGB(0,47,81), RGB(0,57,85), RGB(0,67,89),
	RGB(0,78,94), RGB(0,90,98), RGB(0, 102,102),RGB(0, 106, 97), 
	RGB(0,111,92), RGB(0,115,86), RGB(0, 119,79),RGB(0, 123, 72), 
	RGB(0,128,64), RGB(0,132,55), RGB(0, 136,45),RGB(0, 140, 35), 
	RGB(0,145,24), RGB(0,149,12), RGB(0, 153,0),RGB(13, 157, 0), 
	RGB(27, 162, 0),RGB(41, 166, 0),RGB(57, 170, 0),RGB(73, 174, 0),
	RGB(89, 179, 0),RGB(107, 183, 0),RGB(125, 187, 0),RGB(143, 191, 0),
	RGB(163, 195, 0),RGB(183, 200, 0),RGB(204, 204, 0),RGB(208, 191, 0),
	RGB(212, 177, 0),RGB(217, 163, 0),RGB(221, 147, 0),RGB(225, 131, 0),
	RGB(229, 115, 0),RGB(234, 97, 0),RGB(238, 79, 0),RGB(242, 61, 0),
	RGB(246, 41, 0),RGB(251, 21, 0),RGB(255, 0, 0)
};

static COLORREF colormap1[] = {
	RGB(0,0,255), //blue
	RGB(0,255,0), //green
	RGB(0,255,255), //cyan
	RGB(255,0,0), //red
	RGB(255,0,255), //mangenta
	RGB(255,255,0), //yellow
	RGB(0,0,128), //dark-blue
	RGB(0,128,0), //dark-green
	RGB(0,128,128), //blue-green
	RGB(128,0,0), //brown
	RGB(128,0,128),//purpur
	RGB(128,128,0),//oliv
	RGB(166,202,240),//light-blue
	RGB(192,192,192), //light-gray
	RGB(160,160,164), //middle-gray
	RGB(128,128,128), //dark-gray
	RGB(255,251,240),//dark-white
	RGB(255,255,255), //white
	RGB(0,0,0), //black
};

class ImageObject
{
public:
	ImageObject(){};
	virtual ~ImageObject(){if(!img.IsNull()) img.Destroy();};
public:
	CImage img;
	long offX;
	long offY;
	double zoomX;
	double zoomY;
public:
	int GetWidth(){return (img.IsNull()?0:img.GetWidth());}
	int GetHeight(){return (img.IsNull()?0:img.GetHeight());}
	BOOL DrawTo(HDC hDstDC, RECT &rectDst){return (img.IsNull()?FALSE:img.Draw(hDstDC, rectDst, CRect(0,0,GetWidth(),GetHeight())));};
	LRESULT Load(LPCTSTR str)
	{
		if(!img.IsNull()) img.Destroy();
		return img.Load(str);
	};
};
