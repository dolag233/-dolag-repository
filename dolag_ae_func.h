//����Щ�Զ���ĺ���
/*
extern void Swap(int* dst, int* src);
extern void Swap(A_u_char* dst, A_u_char* src);
extern void Swap(A_u_short* dst, A_u_short* src);


extern bool SwapPixel8(PF_Pixel8* dst, PF_Pixel8* src);

extern bool SwapPixel16(PF_Pixel16* dst, PF_Pixel16* src);

//д�귢�ֲ�û��ʲô��orz��������֮��Ĳ�ֵ��Ȼ����̫����
extern A_Color Bilinear8(PF_LayerDef* layer, PF_FpShort x, PF_FpShort y);



extern A_Color Bilinear16(PF_LayerDef* layer, PF_FpShort x, PF_FpShort y);

extern bool isOnScreen(PF_LayerDef* layer, int x, int y);
extern PF_Pixel16* getPixel16(PF_LayerDef* layer, int x, int y);
//Ȼ���㷢�ֱ�����л�ȡ���صĺ���,�����С��鷳
//PF_GET_PIXEL_DATA8()
// PF_GET_PIXEL_DATA16();�����������*in_data������
extern PF_Pixel8* getPixel8(PF_LayerDef* layer, int x, int y);

extern PF_FpShort lerp(PF_FpShort Dmin, PF_FpShort Dmax, PF_FpShort Smin, PF_FpShort Smax, PF_FpShort value);

extern typedef struct {
	u_short x;//�����32768 * 2
	u_short y;//û�и���
}Do_pixelPos;




//#pragma once
//
//
//#define FIX2FLT(x) (x / ((PF_FpShort)(1L << 16)))//��������PF_FpShort
//
//typedef unsigned char		u_char;
//typedef unsigned short		u_short;
//typedef unsigned short		u_int16;
//typedef unsigned long		u_long;
//typedef short int			int16;
//#define PF_TABLE_BITS	12
//#define PF_TABLE_SZ_16	4096
//
//#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
//// AE_Effect.h checks for this.
*/
#ifndef _H_DOLAG_AE_FUNC_
#define _H_DOLAG_AE_FUNC_

	#include "AEConfig.h"
	//
	#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
	#endif

	#include "entry.h"
	#include "AE_Effect.h"
	#include "AE_EffectCB.h"
	#include "AE_Macros.h"
	#include "Param_Utils.h"
	#include "AE_EffectCBSuites.h"
	#include "String_Utils.h"
	#include "AE_GeneralPlug.h"
	//#include "AEFX_ChannelDepthTpl.h"
	#include "AEGP_SuiteHandler.h"
	#include "AEFX_SuiteHandlerTemplate.h"

#endif
//
//

PF_Pixel8 Bilinear8(PF_LayerDef* layer, PF_FpLong x, PF_FpLong y);

PF_Pixel16 Bilinear16(PF_LayerDef* layer, PF_FpLong x, PF_FpLong y);

void Swap(A_long* dst, A_long* src);
void Swap(A_u_char* dst, A_u_char* src);
void Swap(A_u_short* dst, A_u_short* src);

bool SwapPixel8(PF_Pixel8* dst, PF_Pixel8* src);

bool SwapPixel16(PF_Pixel16* dst, PF_Pixel16* src);

void Normalize(PF_FpShort* x, PF_FpShort* y);

bool isOnScreen(PF_LayerDef* layer, A_long x, A_long y);

PF_Pixel8* getPixel8(PF_LayerDef* layer, A_long x, A_long y);

PF_Pixel16* getPixel16(PF_LayerDef* layer, A_long x, A_long y);

PF_FpShort lerp(PF_FpShort Dmin, PF_FpShort Dmax, PF_FpShort Smin, PF_FpShort Smax, PF_FpShort value);

void DrawWaterMark(PF_LayerDef* output, PF_LayerDef* src_layer, A_long time,bool isLisensed);//��ˮӡ

PF_Err DOLAG_ReleaseSuite(PF_InData* in_data,
	PF_OutData* out_data,
	const char* name,
	int32_t			version,
	const char* error_stringPC0);

enum EdgeRepeatMode//���Ʒ�ʽ��ö��
{
	REPEAT_BASE = 2,
	REPEAT_MIRROR
};

void RepeatEdge(PF_FpShort* x, PF_FpShort* y,short width,short height,A_long mode);

void BlendPix(const PF_Pixel8 src, PF_Pixel8* dst, PF_FpShort coe);
void BlendPix(const PF_Pixel16 src, PF_Pixel16 * dst, PF_FpShort coe);

PF_FpShort GetLumn(PF_Pixel8 pix);
PF_FpShort GetLumn(PF_Pixel16 pix);

void GetPseudoFXAA(PF_LayerDef* layer,short x,short y, PF_FpShort MINLIMIT);

void PseduoFXAA(PF_LayerDef* layer, PF_FpShort MINLIMIT);