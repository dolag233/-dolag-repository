/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*	Skeleton.cpp	

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.
	
	Revision History

	Version		Change													Engineer	Date
	=======		======													========	======
	1.0			(seemed like a good idea at the time)					bbb			6/1/2002

	1.0			Okay, I'm leaving the version at 1.0,					bbb			2/15/2006
				for obvious reasons; you're going to 
				copy these files directly! This is the
				first XCode version, though.

	1.0			Let's simplify this barebones sample					zal			11/11/2010

	1.0			Added new entry point									zal			9/18/2017

*/

#include "Skeleton.h"

//bool myIteration8(PF_LayerDef* inL,)


bool isOnScreen(PF_LayerDef* layer, int x, int y) {
	return (x > -1 && y > -1 && x < layer->width && y < layer->height);
}//�ж��Ƿ�����Ļ��

PF_Pixel16* getPixel16(PF_LayerDef* layer, int x, int y) {
	return (PF_Pixel16*)((char*)layer->data + (y * layer->rowbytes) + (x * sizeof(PF_Pixel16)));
}//16bits��

PF_Pixel8* getPixel8(PF_LayerDef* layer, int x, int y) {
	return (PF_Pixel8*)((char*)layer->data + (y * layer->rowbytes) + (x * sizeof(PF_Pixel8)));
};//���ڻ�ȡָ��xy��������ص��ڴ��ַ

float lerp(float Dmin, float Dmax, float Smin, float Smax, float value) {
	return (float)((value - Smin) / (Smax - Smin) * (Dmax - Dmin) + Dmin);
}//����ӳ�亯��

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE |
		PF_OutFlag_WIDE_TIME_INPUT|
		PF_OutFlag_NON_PARAM_VARY;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUPX("Displace Mode", 4, 1, "Radial|Vector|Direct|Twirl", NULL,isRadial);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX(	"Length", 
							-50000, 
							50000,
							-50000, 
							50000, 
							DIS_LENGTH_DEFT,
							PF_Precision_HUNDREDTHS,
							0,
							0,
							DisplaceLength);

	AEFX_CLR_STRUCT(def);

	PF_ADD_LAYER("Map Layer",PF_LayerDefault_MYSELF, DisplaceMap);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT("Displacement Center", in_data->width / 2, in_data->height / 2,false,DisCenter);
	//AEFX_CLR_STRUCT(def);//���п���ɾ������Ϊcheckboxx������������
	PF_ADD_CHECKBOXX("Apply Transparent",1,0 , isTransparent);
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE("Direct Angle", ANGLE_DEFT, DisAngle);
	AEFX_CLR_STRUCT(def);
	out_data->num_params = PARAMS_NUM;

	return err;
}

static PF_Err
MySimpleGainFunc16 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;
	disInfo* giP = reinterpret_cast<disInfo*>(refcon);

	PF_Pixel16* pix;
	PF_Pixel16* mapP;
	PF_LayerDef map = giP->check.u.ld;
	bool twirlAlpha = false;
	mapP = getPixel16(&map, xL % map.width, yL % map.height);


	if (giP->length != 0) {
		float lumn;
		int x_fin, y_fin;
		float lumn_ori;

		lumn_ori = mapP->red * 0.2 + mapP->green * 0.7 + mapP->blue * 0.1;


		lumn = lumn_ori / (double)PF_MAX_CHAN16 * mapP->alpha / (double)PF_MAX_CHAN16;
		
		int dx;
		int dy;
		float vec_x, vec_y;
		int x_center = giP->center_x;
		int y_center = giP->center_y;
		if (giP->mode == 1) {//radial
			dx = xL - x_center;
			dy = yL - y_center;
			vec_x = sin(atan2((double)dx, (double)dy) + giP->angle);
			vec_y = cos(atan2((double)dx, (double)dy) + giP->angle);
			x_fin = xL + vec_x * giP->length * lumn;
			y_fin = yL + vec_y * giP->length * lumn;
		}
		if (giP->mode == 2) {//vector
			lumn = 0;
			vec_x = mapP->alpha * mapP->red / (double)PF_MAX_CHAN16 / (double)PF_MAX_CHAN16;
			vec_y = mapP->alpha * mapP->blue / (double)PF_MAX_CHAN16 / (double)PF_MAX_CHAN16;
			x_fin = xL + vec_x * giP->length;
			y_fin = yL + vec_y * giP->length;
		}
		if (giP->mode == 3) {//direct
			vec_x = sin(giP->angle);
			vec_y = cos(giP->angle);
			x_fin = xL + vec_x * giP->length * lumn;
			y_fin = yL + vec_y * giP->length * lumn;
		}
		if (giP->mode == 4) {//twirl
			if (mapP->alpha != 0) {
				dx = xL - x_center;
				dy = yL - y_center;
				double scr_angle = atan2(-1 * (double)dy, (double)dx);//��ʼ�����ص����ĺ�x��������нǵĻ�����
				double des_angle = scr_angle + 3.14159265358 / 2.0;
				int dsts = (int)hypot(dx, dy);
				x_fin = floor(dsts * sin(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.width / 2);
				y_fin = floor(dsts * cos(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.height / 2);
			}
			else twirlAlpha = true;
		}
		if (isOnScreen(&giP->layer, x_fin, y_fin)) {
			pix = getPixel16(&giP->layer, x_fin, y_fin);
			outP->alpha = pix->alpha * (1 - twirlAlpha);
			outP->red = pix->red;
			outP->green = pix->green;
			outP->blue = pix->blue;
		}
		else {

			if (giP->isTransparent) {
				outP->alpha = 0;
				outP->red = inP->red;
				outP->green = inP->green;
				outP->blue = inP->blue;
			}
			else {
				outP->alpha = inP->alpha;
				outP->red = inP->red;
				outP->green = inP->green;
				outP->blue = inP->blue;
			}
		}


	}
	else
	{
		outP->alpha = inP->alpha;
		outP->red = inP->red;
		outP->green = inP->green;
		outP->blue = inP->blue;
	}

	return err;
}

static PF_Err
MySimpleGainFunc8 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;
	disInfo	*giP	= reinterpret_cast<disInfo*>(refcon);

	PF_Pixel8* pix;
	PF_Pixel8* mapP;
	PF_LayerDef map = giP->check.u.ld;
	bool twirlAlpha = false;
	mapP = getPixel8(&map, xL % map.width, yL % map.height);


		if (giP->length != 0) {
			float lumn;
			int x_fin, y_fin;
			float lumn_ori;
			//PF_Pixel8* mapP = getPixel8(&giP->map, xL % giP->map.width, yL % giP->map.height);
			 
			lumn_ori = mapP->red * 0.2 + mapP->green * 0.7 + mapP->blue * 0.1; 
			//else lumn_ori= inP->red * 0.2 + inP->green * 0.7 + inP->blue * 0.1;
			//umn_ori = inP->red * 0.2 + inP->green * 0.7 + inP->blue * 0.1;

		//	lumn_ori = (mapP->red + mapP->green + mapP->blue) / 3.0;

			lumn = lumn_ori / (double)PF_MAX_CHAN8*mapP->alpha/ (double)PF_MAX_CHAN8;
			//lumn = getLightness(mapP->red, mapP->green, mapP->blue) * mapP->alpha / PF_MAX_CHAN8 * 3.1415926;
			int dx;
			int dy;
			float vec_x, vec_y;
			int x_center = giP->center_x;
			int y_center = giP->center_y;
			if (giP->mode == 1) {//radial
				dx = xL - x_center;
				dy = yL - y_center;
				vec_x = sin(atan2((double)dx, (double)dy) + giP->angle);
				vec_y = cos(atan2((double)dx, (double)dy) + giP->angle);
				x_fin = xL + vec_x * giP->length * lumn;
				y_fin = yL + vec_y * giP->length * lumn;
			}
			if (giP->mode == 2) {//vector
				
				//lumn = mapP->alpha * (mapP->red) / (double)PF_MAX_CHAN8 / (double)PF_MAX_CHAN8;//new
				//lumn = lerp(0.0, 3.1414926f * 2, 0.0, 1.0, lumn);
				lumn = 0;
				//vec_x = sin(lumn+ giP->angle);
				//vec_y = cos(lumn + giP->angle);
				vec_x = mapP->alpha * mapP->red / (double)PF_MAX_CHAN8 / (double)PF_MAX_CHAN8;
				vec_y = mapP->alpha * mapP->blue / (double)PF_MAX_CHAN8 / (double)PF_MAX_CHAN8;
				x_fin = xL + vec_x * giP->length ;
				y_fin = yL + vec_y * giP->length ;
			}
			if (giP->mode == 3) {//direct
				vec_x = sin(giP->angle);
				vec_y = cos(giP->angle);
				x_fin = xL + vec_x * giP->length * lumn;
				y_fin = yL + vec_y * giP->length * lumn;
			}
			if (giP->mode == 4) {//twirl
				if (mapP->alpha != 0) {
					dx = xL - x_center;
					dy = yL - y_center;
					double scr_angle = atan2(-1 * (double)dy, (double)dx);//��ʼ�����ص����ĺ�x��������нǵĻ�����
					double des_angle = scr_angle + 3.14159265358 / 2.0;
					//double des_angle =  - giP->length * lumn/ 1000.0;
					int dsts = (int)hypot(dx, dy);
					//x_fin = dsts * sin(des_angle + giP->angle)*giP->angle/3.1415926f+giP->layer.width/2;
					//y_fin = dsts * cos(des_angle + giP->angle)*giP->angle/3.1415926f + giP->layer.height / 2;
					x_fin = floor(dsts * sin(des_angle + giP->angle * giP->length * lumn / 1000.0)  + giP->layer.width / 2);
					y_fin = floor(dsts * cos(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.height / 2);
				}
				else twirlAlpha = true;
			}
			//bool change_sign = 1;
			//if (xL == x_fin && yL == y_fin) change_sign = 0;
			if (isOnScreen(&giP->layer, x_fin, y_fin)) {
				pix = getPixel8(&giP->layer, x_fin, y_fin);
				/*if (twirlAlpha) {
					outP->alpha = 0;
					outP->red = pix->red;
					outP->green = pix->green;
					outP->blue = pix->blue;
				}
				else {
					outP->alpha = pix->alpha;
					outP->red = pix->red;
					outP->green = pix->green;
					outP->blue = pix->blue;
				}*/
				outP->alpha = pix->alpha*(1-twirlAlpha);
				outP->red = pix->red;
				outP->green = pix->green;
				outP->blue = pix->blue;
			}
			else {

				if (giP->isTransparent) {
					//outP->alpha = mapP->alpha;
					outP->alpha = 0;
					outP->red = inP->red;
					outP->green = inP->green;
					outP->blue = inP->blue;
				}
				else {
					outP->alpha = inP->alpha;
					outP->red = inP->red;
					outP->green = inP->green;
					outP->blue = inP->blue;
				}
			}
			//outP->alpha = mapP->alpha;//


		}
		else
		{
			outP->alpha = inP->alpha;
			outP->red = inP->red;
			outP->green = inP->green;
			outP->blue = inP->blue;
		}
	
	return err;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	/*	Put interesting code here. */
	disInfo disp;
	AEFX_CLR_STRUCT(disp); 
	//
	PF_EventExtra extra;
	extra.contextH;
	PF_KeyDownEvent key;
	key.keycode = PF_ControlCode_Backspace;
	



	//
	disp.layer = params[0]->u.ld;
	disp.mode = params[1]->u.pd.value;
	disp.length = params[2]->u.fs_d.value;
	disp.center_x = FIX2INT( params[4]->u.td.x_value);
	disp.center_y = FIX2INT(params[4]->u.td.y_value);
	disp.isTransparent = params[5]->u.bd.value;
	disp.angle = (-FIX2FLT( params[6]->u.ad.value))* PF_RAD_PER_DEGREE;//new*115/360/2
	int time = in_data->current_time;
	int step = in_data->time_step;
	int scale = in_data->time_scale;
	PF_CHECKOUT_PARAM(in_data, 3, time, step, scale, &disp.check);
	//{
		//disp.check.u.ld = params[0]->u.ld;
	//}
	A_long				linesL = 0;

	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
	
	if (disp.check.u.ld.data != NULL) {//����ѡͼ��Ϊnone��ʱ�����ʹ��Ұָ��
		//�����Ǹ��ж������Ƿǳ���Ҫ��һ������������ѡ��noneʱ�ı�����Ҳ�����˽���ѡmapͼ��ɾ����ı���
		if (PF_WORLD_IS_DEEP(output)) {
			ERR(suites.Iterate16Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				NULL,							// area - null for all pixels
				(void*)&disp,				// refcon - your custom data pointer
				MySimpleGainFunc16,				// pixel function pointer
				output));
		}
		else {
			ERR(suites.Iterate8Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				NULL,							// area - null for all pixels
				(void*)&disp,					// refcon - your custom data pointer
				MySimpleGainFunc8,				// pixel function pointer
				output));
		}
	}
	else PF_COPY(&params[0]->u.ld, output, NULL, &output->extent_hint);
	ERR(PF_CHECKIN_PARAM(in_data, &disp.check));
	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Skeleton", // Name
		"ADBE Skeleton", // Match Name
		"Sample Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}
