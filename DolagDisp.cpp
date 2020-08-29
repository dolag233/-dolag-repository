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

#include "DolagDispMainHead.h"

//PF_Pixel8 Mirror(PF_LayerDef layer, int x, int y) {

	//PF_GET_PIXEL_DATA8();
	//*in_data->utils->get_pixel_data8();
//};

//bool myIteration8(PF_LayerDef* inL,)

//�ͷ��׼��ڴ�Ķ�������ʵ�� AEFX_SuiteHelper.h��������ֳɵ�



bool isLisensed = 0;/*�ж��Ƿ���Ч�����ȫ�ֱ���*/

static PF_Err
About(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	//short map;
	AEGP_SuiteHandler suites(in_data->pica_basicP);

	suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
		"%s v%d.%d\r%s",
		STR(StrID_Name),
		MAJOR_VERSION,
		MINOR_VERSION,
		STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err
GlobalSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	out_data->my_version = PF_VERSION(MAJOR_VERSION,
		MINOR_VERSION,
		BUG_VERSION,
		STAGE_VERSION,
		BUILD_VERSION);
	/*
	//����ȫ�ֱ���(����һ���������ڴ��)����Ȼ�о�������ֱ�Ӵ���ȫ�ֱ���
	bool* isWaterMarked = 0;
	in_data->global_data = reinterpret_cast<void**>(&isWaterMarked);
	*/
	out_data->out_flags = PF_OutFlag_DEEP_COLOR_AWARE |
		PF_OutFlag_WIDE_TIME_INPUT |
		PF_OutFlag_NON_PARAM_VARY |
		PF_OutFlag_I_EXPAND_BUFFER |
		PF_OutFlag_I_DO_DIALOG;	// just 16bpc, not 32bpc
	out_data->out_flags2 |= PF_OutFlag2_OUTPUT_IS_WATERMARKED | PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG;

	return PF_Err_NONE;
}

static PF_Err
ParamsSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err		err = PF_Err_NONE;
	PF_ParamDef	def;

	static bool* add_params;
	bool NOPARAMS = false;
	add_params = &NOPARAMS;
	AEFX_CLR_STRUCT(def);
	PF_ADD_BUTTON("Water Mark Toggle", "Water Mark Toggle", 0, PF_ParamFlag_SUPERVISE, DisButtonID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUPX("Displace Mode", 5, 1, "Radial|Vector(Only Red and Blue Channel)|Direct|Twirl|Dynamics", NULL, DisModeID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX("Length",
		-250,
		250,
		-100,
		100,
		DIS_LENGTH_DEFT,
		PF_Precision_HUNDREDTHS,
		0,
		//����һ�������flag��ʹ�øñ����ڸı�ʱ�����params changed��cmd
		//�����õģ��������ʵ�
		PF_ParamFlag_SUPERVISE,
		DisLengthID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_LAYER("Map Layer", PF_LayerDefault_MYSELF, DisMapID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT("Displacement Center", in_data->width / 2, in_data->height / 2, false, DisCenterID);
	//AEFX_CLR_STRUCT(def);//���п���ɾ������Ϊcheckboxx�����������
	PF_ADD_CHECKBOXX("Apply Transparent", 1, 0, DisIsTransparentID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOXX("Apply Anti-aliasing", 1, 0, DisApplyAAID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX("Anti-aliasing Threshold", 0, 1, 0, 1, .5f, PF_Precision_HUNDREDTHS, 0, 0, DisAAThresholdID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE("Direct Angle", ANGLE_DEFT, DisAngleID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUPX("Edge Repeat", 3, 1, "None|Repeat|Mirror", NULL, DisEdgeID);//������һ����Ե����ģʽ����û��ʵ��
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("Dynamics Options", DisDynamicsTopicID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX("Displace Velocity", 0. - 1000, 1000.0, 0.0, 10.0, 1, PF_Precision_HUNDREDTHS, 0, PF_ParamFlag_SUPERVISE, DisVelocityID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER("Displace Max Iterations", 0, 1000, 0, 200, 2, DisMaxIterID);
	PF_ADD_FLOAT_SLIDERX("Displace Max Distance", 0, 3500, 0, 2000, 2000, PF_Precision_HUNDREDTHS, 0, 0, DisMaxDistanceID);
	AEFX_CLR_STRUCT(def);
	//PF_ADD_POPUPX("Displace Apply Mode", 2, 1, "Sample|Trans", NULL, DisApplyModeID);
	//PF_ADD_CHECKBOXX("Apply Trans Pixels", 0, 0, DisApplyModeID);
	//AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(DisDynamicsTopicID);
	AEFX_CLR_STRUCT(def);
	/*if (add_params) {
		PF_ADD_TOPICX("PVһ��������", 0, 8);
	}*/

	out_data->num_params = PARAMS_NUM;

	//ѡ�ť
	if (in_data->appl_id != 'PrMr')
	{
		AEFX_SuiteScoper<PF_EffectUISuite1> effect_ui_suiteP = AEFX_SuiteScoper<PF_EffectUISuite1>(
			in_data,
			kPFEffectUISuite,
			kPFEffectUISuiteVersion1,
			out_data);

		ERR(effect_ui_suiteP->PF_SetOptionsButtonName(in_data->effect_ref, "Doalg"));
	}


	return err;
}

static PF_Err
FrameSetup(PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	// Output buffer resizing may only occur during PF_Cmd_FRAME_SETUP. 

	PF_FpLong		border_x = 0,
		border_y = 0,
		border = 500;


	// shrink the border to accomodate decreased resolutions.
	border_x = border * (static_cast<PF_FpLong>(in_data->downsample_x.num) / in_data->downsample_x.den);
	border_y = border * (static_cast<PF_FpLong>(in_data->downsample_y.num) / in_data->downsample_y.den);
	// add 2 times the border width and height to the input width and
	// height to get the output size.

	out_data->width = 2 * params[0]->u.ld.width;
	out_data->height = 1.6 * params[0]->u.ld.height;

	// The origin of the input buffer corresponds to the (border_x, 
	// border_y) pixel in the output buffer.

	out_data->origin.h = params[0]->u.ld.width;;
	out_data->origin.v = 0.8 * params[0]->u.ld.height;

	return PF_Err_NONE;
}


static PF_Err
DisplaceIterFunc16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err		err = PF_Err_NONE;
	disInfo* giP = reinterpret_cast<disInfo*>(refcon);
	int r_mode = giP->repeat_mode;
	PF_Pixel16* pix;
	PF_Pixel16* mapP;
	PF_LayerDef map = giP->check.u.ld;
	bool twirlAlpha = false;
	mapP = getPixel16(&map, xL % map.width, yL % map.height);
	float x_fin = xL;
	float y_fin = yL;
	if (giP->length != 0 || giP->mode == 5) {
		float lumn;
		float lumn_ori;
		if ((mapP->blue == PF_MAX_CHAN16 / 2 || mapP->blue == PF_MAX_CHAN16 / 2 + 1) && (mapP->green == PF_MAX_CHAN16 / 2 || mapP->green == PF_MAX_CHAN16 / 2 + 1) &&
			(mapP->red == PF_MAX_CHAN16 / 2 || mapP->red == PF_MAX_CHAN16 / 2 + 1)) {
			lumn = 0;
		}
		else {
			lumn_ori = mapP->red * 0.299 + mapP->green * 0.587 + mapP->blue * 0.114;
			lumn = lumn_ori / (double)PF_MAX_CHAN16 * mapP->alpha / (double)PF_MAX_CHAN16 - .5;
			lumn *= 2;
		}
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

			//lumn = mapP->alpha * (mapP->red) / (double)PF_MAX_CHAN16 / (double)PF_MAX_CHAN16;//new
			//lumn = lerp(0.0, 3.1414926f * 2, 0.0, 1.0, lumn);
			lumn = 0;
			//vec_x = sin(lumn+ giP->angle);
			//vec_y = cos(lumn + giP->angle);
			vec_x = mapP->alpha * mapP->red / (double)PF_MAX_CHAN16 / (double)PF_MAX_CHAN16 - .5;
			vec_y = mapP->alpha * mapP->blue / (double)PF_MAX_CHAN16 / (double)PF_MAX_CHAN16 - .5;
			vec_x *= 2 * sin(giP->angle);
			vec_y *= 2 * cos(giP->angle);
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
				//double des_angle =  - giP->length * lumn/ 1000.0;
				int dsts = (int)hypot(dx, dy);
				//x_fin = dsts * sin(des_angle + giP->angle)*giP->angle/3.1415926f+giP->layer.width/2;
				//y_fin = dsts * cos(des_angle + giP->angle)*giP->angle/3.1415926f + giP->layer.height / 2;
				x_fin = dsts * sin(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.width / 2;
				y_fin = dsts * cos(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.height / 2;
			}
			else twirlAlpha = true;
		}
		//bool change_sign = 1;
		//if (xL == x_fin && yL == y_fin) change_sign = 0;

		//���µ����ض���ѧģʽ
		if (giP->mode == 5) {//Dynamics
			float vel_x = (mapP->red / (double)PF_MAX_CHAN16 - .5) * 2;//ע�⣬�����и���ֵ�ˣ�Ҳ���Ƿ���
			float vel_y = (mapP->green / (double)PF_MAX_CHAN16 - .5) * 2;
			float norm = mapP->blue / (double)PF_MAX_CHAN16;//blueͨ����ģ����Ϊ>=0��ֵ
			norm *= giP->dis_vel;
			int MAX_ITER_TIMES = giP->max_iter;
			const float distance_square = giP->max_distance;
			float current_distance_square = .0;
			float tmp_x_fin = xL;
			float tmp_y_fin = yL;
			if (vel_x != 0.0 && vel_y != 0.0 && norm != 0.0) {
				//PF_Pixel16* mapPP = (PF_Pixel6*)malloc(sizeof(PF_Pixel16));
				//������ÿ��ѭ�������v��normҪ������0
				for (int i = 0; i < MAX_ITER_TIMES && current_distance_square <= distance_square && vel_x != 0.0 && vel_y != 0.0 && norm != 0.0; i++) {

					tmp_x_fin = x_fin + vel_x * norm;
					tmp_y_fin = y_fin + vel_y * norm;
					current_distance_square += norm * norm * (vel_x * vel_x + vel_y * vel_y);
					if (r_mode == 1) {
						if (isOnScreen(&giP->layer, tmp_x_fin, tmp_y_fin)) {
							x_fin = tmp_x_fin;
							y_fin = tmp_y_fin;
							if (isOnScreen(&map, floor(x_fin), floor(y_fin))) {
								PF_Pixel16* mapPP = getPixel16(&map, floor(x_fin), floor(y_fin));
								if (mapPP) {
									vel_x = mapPP->red / (double)PF_MAX_CHAN16 - .5;
									vel_y = mapPP->green / (double)PF_MAX_CHAN16 - .5;
									norm = mapPP->blue / (double)PF_MAX_CHAN16 * giP->dis_vel;
								}
							}
							else break;

						}
						else break;
					}
					else {
						if(isOnScreen(&map,x_fin,y_fin))
						x_fin = tmp_x_fin;
						y_fin = tmp_y_fin;
						PF_Pixel16* mapPP = getPixel16(&map, x_fin, y_fin);
						if (mapPP) {
							vel_x = (mapPP->red / (double)PF_MAX_CHAN16 - .5) * 2;
							vel_y = (mapPP->green / (double)PF_MAX_CHAN16 - .5) * 2;
							norm = mapPP->blue / (double)PF_MAX_CHAN16 * giP->dis_vel;
						}
					}

				}
				//free(mapPP);
			}
			else {
				x_fin = xL;
				y_fin = yL;
			}
		}


		//������Ե
		if (r_mode != 1) {
			RepeatEdge(&x_fin, &y_fin, giP->layer.width, giP->layer.height, r_mode);
		}


		if (isOnScreen(&giP->layer, floor(x_fin), floor(y_fin)) && isOnScreen(&giP->layer, ceil(x_fin), ceil(y_fin))) {
			pix = getPixel16(&giP->layer, floor(x_fin), floor(y_fin));
			outP->alpha = pix->alpha * (1 - twirlAlpha);
			if (outP->alpha != 0) {

				//��Ϊ��repeat������ʱ���Ե����Щ��
				if (r_mode == 1) {
					PF_Pixel16 tmp = Bilinear16(&giP->layer, x_fin, y_fin);
					outP->red = tmp.red;
					outP->green = tmp.green;
					outP->blue = tmp.blue;
				}
				else {
					outP->red = pix->red;
					outP->green = pix->green;
					outP->blue = pix->blue;
				}
				//PF_Fixed x = INT2FIX(int(x_fin)) + giP->data->width << 15;
				//PF_Fixed y = INT2FIX(int(y_fin)) + giP->data->height << 15;
				//giP->data->utils->subpixel_sample(giP->data->effect_ref, x, y, &giP->samp_pb, outP);
			}
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
DisplaceIterFunc8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err		err = PF_Err_NONE;
	disInfo* giP = reinterpret_cast<disInfo*>(refcon);
	int r_mode = giP->repeat_mode;
	PF_Pixel8* pix;
	PF_Pixel8* mapP;
	PF_LayerDef map = giP->check.u.ld;
	bool twirlAlpha = false;
	mapP = getPixel8(&map, xL % map.width, yL % map.height);
	float x_fin = xL;
	float y_fin = yL;
	if (giP->length != 0 || giP->mode == 5) {
		float lumn;
		float lumn_ori;
		if ((mapP->blue == PF_MAX_CHAN8 / 2 || mapP->blue == PF_MAX_CHAN8 / 2 + 1) && (mapP->green == PF_MAX_CHAN8 / 2 || mapP->green == PF_MAX_CHAN8 / 2 + 1) &&
			(mapP->red == PF_MAX_CHAN8 / 2 || mapP->red == PF_MAX_CHAN8 / 2 + 1)) {
			lumn = 0;
		}
		else {
			lumn_ori = mapP->red * 0.299 + mapP->green * 0.587 + mapP->blue * 0.114;
			lumn = lumn_ori / (double)PF_MAX_CHAN8 * mapP->alpha / (double)PF_MAX_CHAN8 - .5;
			lumn *= 2;
		}
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
			vec_x = mapP->alpha * mapP->red / (double)PF_MAX_CHAN8 / (double)PF_MAX_CHAN8 - .5;
			vec_y = mapP->alpha * mapP->blue / (double)PF_MAX_CHAN8 / (double)PF_MAX_CHAN8 - .5;
			vec_x *= 2 * sin(giP->angle);
			vec_y *= 2 * cos(giP->angle);
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
				//double des_angle =  - giP->length * lumn/ 1000.0;
				int dsts = (int)hypot(dx, dy);
				//x_fin = dsts * sin(des_angle + giP->angle)*giP->angle/3.1415926f+giP->layer.width/2;
				//y_fin = dsts * cos(des_angle + giP->angle)*giP->angle/3.1415926f + giP->layer.height / 2;
				x_fin = dsts * sin(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.width / 2;
				y_fin = dsts * cos(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.height / 2;
			}
			else twirlAlpha = true;
		}
		//bool change_sign = 1;
		//if (xL == x_fin && yL == y_fin) change_sign = 0;

		//���µ����ض���ѧģʽ
		if (giP->mode == 5) {//Dynamics
			float vel_x = (mapP->red / (double)PF_MAX_CHAN8 - .5) * 2;//ע�⣬�����и���ֵ�ˣ�Ҳ���Ƿ���
			float vel_y = (mapP->green / (double)PF_MAX_CHAN8 - .5) * 2;
			float norm = mapP->blue / (double)PF_MAX_CHAN8;//blueͨ����ģ����Ϊ>=0��ֵ
			norm *= giP->dis_vel;
			int MAX_ITER_TIMES = giP->max_iter;
			const float distance_square = giP->max_distance;
			float current_distance_square = .0;
			float tmp_x_fin = xL;
			float tmp_y_fin = yL;
			if (vel_x != 0.0 && vel_y != 0.0 && norm != 0.0) {
				//PF_Pixel8* mapPP = (PF_Pixel8*)malloc(sizeof(PF_Pixel8));
				//������ÿ��ѭ�������v��normҪ������0
				for (int i = 0; i < MAX_ITER_TIMES && current_distance_square <= distance_square && vel_x != 0.0 && vel_y != 0.0 && norm != 0.0; i++) {

					tmp_x_fin = x_fin + vel_x * norm;
					tmp_y_fin = y_fin + vel_y * norm;
					current_distance_square += norm * norm * (vel_x * vel_x + vel_y * vel_y);
					if (r_mode == 1) {
						if (isOnScreen(&giP->layer, tmp_x_fin, tmp_y_fin)) {
							x_fin = tmp_x_fin;
							y_fin = tmp_y_fin;
							if (isOnScreen(&map, floor(x_fin), floor(y_fin))) {
								PF_Pixel8* mapPP = getPixel8(&map, floor(x_fin), floor(y_fin));
								if (mapPP) {
									vel_x = mapPP->red / (double)PF_MAX_CHAN8 - .5;
									vel_y = mapPP->green / (double)PF_MAX_CHAN8 - .5;
									norm = mapPP->blue / (double)PF_MAX_CHAN8 * giP->dis_vel;
								}
							}
							else break;

						}
						else break;
					}
					else {
						x_fin = tmp_x_fin;
						y_fin = tmp_y_fin;
						PF_Pixel8* mapPP = getPixel8(&map, x_fin, y_fin);
						if (mapPP) {
							vel_x = (mapPP->red / (double)PF_MAX_CHAN8 - .5) * 2;
							vel_y = (mapPP->green / (double)PF_MAX_CHAN8 - .5) * 2;
							norm = mapPP->blue / (double)PF_MAX_CHAN8 * giP->dis_vel;
						}
					}

				}
				//free(mapPP);
			}
			else {
				x_fin = xL;
				y_fin = yL;
			}
		}


		//������Ե
		if (r_mode != 1) {
			RepeatEdge(&x_fin, &y_fin, giP->layer.width, giP->layer.height, r_mode);
		}


		if (isOnScreen(&giP->layer, floor(x_fin), floor(y_fin)) && isOnScreen(&giP->layer, ceil(x_fin), ceil(y_fin))) {
			pix = getPixel8(&giP->layer, floor(x_fin), floor(y_fin));
			outP->alpha = pix->alpha * (1 - twirlAlpha);
			if (outP->alpha != 0) {

				//��Ϊ��repeat������ʱ���Ե����Щ��
				if (r_mode == 1) {
					PF_Pixel8 tmp = Bilinear8(&giP->layer, x_fin, y_fin);
					outP->red = tmp.red;
					outP->green = tmp.green;
					outP->blue = tmp.blue;
				}
				else {
					outP->red = pix->red;
					outP->green = pix->green;
					outP->blue = pix->blue;
				}
				//PF_Fixed x = INT2FIX(int(x_fin)) + giP->data->width << 15;
				//PF_Fixed y = INT2FIX(int(y_fin)) + giP->data->height << 15;
				//giP->data->utils->subpixel_sample(giP->data->effect_ref, x, y, &giP->samp_pb, outP);
			}
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

//���������������������ûpoi��
//static PF_Err
//SampleIterFunc8(
//	void* refcon,
//	A_long		xL,
//	A_long		yL,
//	PF_Pixel8* inP,
//	PF_Pixel8* outP) {
//	disInfo *disp = reinterpret_cast<disInfo*>(refcon);
//	int err = 0;
//	ERR(disp->data->utils->subpixel_sample(disp->data->effect_ref, INT2FIX(xL), INT2FIX(yL), &disp->samp_pb, outP));
//	return err;
//}



static PF_Err
MyDrawPath(//��·���ĺ�����û�е���
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP) {
	PF_Err		err = PF_Err_NONE;
	path_info* path = reinterpret_cast<path_info*>(refcon);
	PF_PathVertex* vert = path->p_vert_head;
	int step = path->step;
	PF_FpLong* pntx = path->p_x;
	PF_FpLong* pnty = path->p_y;
	/*for (int i = 0; i < path->vert_num; i++) {
		int deltaX = abs(vert[i].x - xL);
		int deltaY = abs(vert[i].y - yL);
		int fill_flag = 0;
		if (deltaX <= 5 || deltaY <= 5) {
			outP->blue = 255;
			outP->red = 255;
			outP->green = 255;
			outP->alpha = 255;
			fill_flag = 1;
		}
		else if (fill_flag) {
			outP->blue = 0;
			outP->red = 0;
			outP->green = 0;
			outP->alpha = 0;
		}
	}*/
	for (int i = 0; i < path->vert_num * step - step; i++) {
		int deltaX = abs(pntx[i] - xL);
		int deltaY = abs(pnty[i] - yL);
		int fill_flag = 0;
		if (deltaX <= 5 && deltaY <= 5) {
			outP->blue = 255;
			outP->red = 255;
			outP->green = 255;
			outP->alpha = 255;
			fill_flag = 1;
		}
		else if (fill_flag) {
			outP->blue = inP->blue;
			outP->red = inP->red;
			outP->green = inP->green;
			outP->alpha = inP->alpha;

		}
	}
	return err;
}



static PF_Err
Render(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err				err = PF_Err_NONE,
		err2 = PF_Err_NONE;
	//����һ��pica������࣬��������˸��ָ���pica��suite���亯��
	//�����ָ��ĳ���������������Ŀɱ��ָ��
	//�����������ȷ��ֹ��ͨ������������Ĺ��캯��copy constructor
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	disInfo disp;
	AEFX_CLR_STRUCT(disp);
	AEFX_CLR_STRUCT(disp.layer);
	disp.layer = params[DisInputLayer]->u.ld;
	disp.mode = params[DisMode]->u.pd.value;
	float length = params[DisLength]->u.fs_d.value;
	disp.length = length * abs(length);
	disp.center_x = FIX2INT(params[DisCenter]->u.td.x_value);
	disp.center_y = FIX2INT(params[DisCenter]->u.td.y_value);
	disp.dis_vel = params[DisVelocity]->u.fs_d.value;
	disp.max_iter = params[DisMaxIter]->u.sd.value;
	disp.isTransparent = params[DisIsTransparent]->u.bd.value;
	disp.angle = (-FIX2FLT(params[DisAngle]->u.ad.value)) * PF_RAD_PER_DEGREE;//new*115/360/2
	disp.repeat_mode = params[DisEdge]->u.pd.value;//��������ģʽ
	disp.max_distance = params[DisMaxDistance]->u.fs_d.value;

	//disp.sample_suite8 = suites.Sampling8Suite1();
	//disp.apply_mode = params[DisApplyMode]->u.bd.value;//�ǲ������ػ��Ǳ任����
	int time = in_data->current_time;
	int step = in_data->time_step;
	int scale = in_data->time_scale;
	//PF_AdvTimeSuite4 *time_suites;
	//char *time_info = (char*)malloc(sizeof(char) * 32);
	//suites.AdvTimeSuite4()->PF_FormatTime(in_data,output,time,scale,1,time_info);


	//in_data->output_origin_x;//����buffer��ԭ�������buffer�е�xλ��

	//in_data->utils->fill(in_data->effect_ref, NULL,NULL,output);
	//��ͼ��չ��Ⱦ��Χ��ʧ����
	//output->extent_hint.right+=500;
	//output->extent_hint.top += 500;
	//ʹ��mem����
	//AEGP_MemorySuite1 *mem;
	//mem = suites.MemorySuite1();
	A_long				linesL = 0;
	AEFX_CLR_STRUCT(disp.check);
	ERR(PF_CHECKOUT_PARAM(in_data, DisMap, time, step, scale, &disp.check));//��Ҫ��ͼ���������Ϣ����ǩ������� ǩ��
	ERR(PF_ABORT(in_data));
	linesL = output->extent_hint.bottom - output->extent_hint.top;//���½����ظ߶Ȳ�
	if (disp.check.u.ld.data && !err) {//����ѡͼ��Ϊnone��ʱ�����ʹ��Ұָ��
		//�����Ǹ��ж������Ƿǳ���Ҫ��һ������������ѡ��noneʱ�ı�����Ҳ�����˽���ѡmapͼ��ɾ����ı���
		//���ģʽ�����ز����Ļ�������iterate�׼�������
		//if (!disp.apply_mode) {
		if (PF_WORLD_IS_DEEP(output)) {
			ERR(suites.Iterate16Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				&(in_data->extent_hint),							// area - null for all pixels
				//refcon������reference configuration������ṹ������õ�����
				(void*)&disp,				// refcon - your custom data pointer
				DisplaceIterFunc16,				// pixel function pointer
				output));
			ERR(PF_ABORT(in_data));
		}
		else {
			ERR(suites.Iterate8Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				&(in_data->extent_hint),							// area - null for all pixels
				(void*)&disp,					// refcon - your custom data pointer
				DisplaceIterFunc8,				// pixel function pointer
				output));
			ERR(PF_ABORT(in_data));
		}
		//}
		//���ģʽ�������ƶ�����ֱ��д�������
		//����ע�͵�����ֱ�Ӷ�����ָ��Ĳ�������û��ʹ��ae��iterate�׼��������õģ��������iterate����Ĳ�����һ������֯��һ�㻺���������Ժ���
		/******************************�������вο���ֵ�Ĵ���********************************/
		/*
		else{
			if (disp.mode == 5) {
				if (!PF_WORLD_IS_DEEP(output)) {
					Do_pixelPos* pixel_pos = (Do_pixelPos*)malloc(sizeof(Do_pixelPos) * output->width * output->height);
					int MAX_ITER_TIMES = disp.max_iter;
					//���ȼ���ÿ�����ص�����λ��
					//����λ�õ���һ����û�����
					for (u_short x = 0; x < output->width; x++) {
						for (u_short y = 0; y < output->height; y++) {
							float x_fin = x;
							float y_fin = y;
							if (isOnScreen(&disp.check.u.ld, x, y)) {//������
								PF_Pixel8* mapP = getPixel8(&disp.check.u.ld, x, y);
								float vel_x = mapP->red / (double)PF_MAX_CHAN8 - .5;
								float vel_y = mapP->green / (double)PF_MAX_CHAN8 - .5;
								float norm = mapP->blue / (double)PF_MAX_CHAN8;
								norm *= disp.dis_vel;
								const float distance_square = 2000.0;
								float current_distance_square = .0;
								float tmp_x_fin = x;
								float tmp_y_fin = y;
								if (vel_x != 0.0 && vel_y != 0.0 && norm != 0.0) {
									for (int i = 0; i < MAX_ITER_TIMES && current_distance_square <= distance_square && vel_x != 0.0 && vel_y != 0.0 && norm != 0.0; i++) {
										tmp_x_fin = x_fin + vel_x * norm;
										tmp_y_fin = y_fin + vel_y * norm;
										current_distance_square += (tmp_x_fin - x_fin) * (tmp_x_fin - x_fin) + (tmp_y_fin - y_fin) * (tmp_y_fin - y_fin);

										if (isOnScreen(&params[0]->u.ld, tmp_x_fin, tmp_x_fin)) {
											x_fin = tmp_x_fin;
											y_fin = tmp_y_fin;
											if (isOnScreen(&disp.check.u.ld, x_fin, y_fin)) {
												PF_Pixel8* mapPP = getPixel8(&disp.check.u.ld, x_fin, y_fin);
												vel_x = mapPP->red / (double)PF_MAX_CHAN8 - .5;
												vel_y = mapPP->green / (double)PF_MAX_CHAN8 - .5;
												norm = mapPP->blue / (double)PF_MAX_CHAN8 * disp.dis_vel;
											}
											else {
												break;
											}
										}
										else {
											break;
										}
									}
								}

							}
							pixel_pos[y * params[0]->u.ld.width + x].x = x_fin;
							pixel_pos[y * params[0]->u.ld.width + x].y = y_fin;
						}
					}//��ȡ
					//Ȼ���������λ�ý����ƶ�

					//PF_EffectWorld* copy_buffer = NULL;
					//AEFX_CLR_STRUCT(*copy_buffer);
					//ERR(PF_NEW_WORLD(in_data->width, in_data->height, 0, copy_buffer));//�������������������ڴ���8bpc������
					//PF_Pixel col = { 255,255,255,255 };
					//ERR(PF_FILL(&col, NULL, copy_buffer));
					//PF_Rect copy_rect = { 0,0,in_data->width ,in_data->height };

					int rowbytes = params[DisInputLayer]->u.ld.rowbytes;
					PF_Pixel8* copy_buffer = (PF_Pixel8*)malloc(in_data->height * rowbytes);
					memcpy(copy_buffer, params[DisInputLayer]->u.ld.data, in_data->height * rowbytes);
					//ERR(PF_COPY(&disp.layer, copy_buffer, NULL, NULL));

					PF_Pixel8* pix = NULL;
					for (u_short x = 0; x < output->width; x++) {
						for (u_short y = 0; y < output->height; y++) {
							int dstx = pixel_pos[y * params[DisInputLayer]->u.ld.width + x].x;
							int dsty = pixel_pos[y * params[DisInputLayer]->u.ld.width + x].y;
							if (isOnScreen(&params[0]->u.ld, floor(dstx), floor(dsty)) && isOnScreen(&params[0]->u.ld, ceil(dstx), ceil(dsty))) {
								pix = (PF_Pixel8*)((char*)copy_buffer + dsty * rowbytes + dstx * sizeof(PF_Pixel8));
								getPixel8(output, x, y)->red = pix->red;
								getPixel8(output, x, y)->green = pix->green;
								getPixel8(output, x, y)->blue = pix->blue;
								getPixel8(output, x, y)->alpha = pix->alpha;
							}
							else {
								getPixel8(output, x, y)->red = getPixel8(&params[0]->u.ld, x, y)->red;
								getPixel8(output, x, y)->green = getPixel8(&params[0]->u.ld, x, y)->green;
								getPixel8(output, x, y)->blue = getPixel8(&params[0]->u.ld, x, y)->blue;
								getPixel8(output, x, y)->alpha = getPixel8(&params[0]->u.ld, x, y)->alpha;

							}
						}
					}

					//if (copy_buffer->data) {
					//	PF_DISPOSE_WORLD(copy_buffer);
					//}			//free(copy_buffer);
					free(copy_buffer);
					free(pixel_pos);


				}
				else {

				}
			}
			ERR(PF_ABORT(in_data));

		}
		*/
		/******************************�������вο���ֵ�Ĵ���********************************/
	}
	else PF_COPY(&params[0]->u.ld, output, NULL, NULL);
	ERR2(PF_CHECKIN_PARAM(in_data, &disp.check));
	ERR2(PF_ABORT(in_data));
	//��ˮӡ
	//������ֵ���֤ˮӡ����
	//�������
	//disp.data = in_data;
	//disp.samp_pb.src = output;
	//ERR(suites.Iterate8Suite1()->iterate(in_data,
	//	0,								// progress base
	//	linesL,							// progress final
	//	&params[0]->u.ld,	// src 
	//	&(in_data->extent_hint),							// area - null for all pixels
	//	//refcon������reference configuration������ṹ������õ�����
	//	(void*)&disp,				// refcon - your custom data pointer
	//	SampleIterFunc8,				// pixel function pointer
	//	output));
	ERR(PF_ABORT(in_data));


	if (in_data->utils->ansi.fabs(params[DisLength]->u.fs_d.value - 147) <= 0.05 && params[DisMaxIter]->u.sd.value == 244) {
		isLisensed = 1;
	}


	if (params[DisApplyAA]->u.pd.value) {//��װ�����
		PseduoFXAA(output, params[DisAAThreshold]->u.fs_d.value);
	}

	DrawWaterMark(output, &params[0]->u.ld, time, isLisensed);

	ERR(PF_ABORT(in_data));
	return err;
}





extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite * inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Pixel Dynamics", // Name
		"Pixel Dynamics", // Match Name
		"Dolag Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info
	//PF_SampPB;
	return result;
}
static
PF_Err
DoDialog(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	PF_SPRINTF(out_data->return_msg, "Dolag is watching you");
	out_data->out_flags2 |= out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
	return PF_Err_NONE;
}
static
PF_Err
ChangeParam(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	PF_UserChangedParamExtra* extra) {
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	//��Ү��
	//����״̬
	//PF_Context con;
	//con.w_type = PF_Window_LAYER;
	char* time_info = (char*)malloc(sizeof(char) * 32);
	int time = in_data->current_time;
	int step = in_data->time_step;
	int scale = in_data->time_scale;
	/*double vel = params[DisVelocity]->u.fs_d.value;
	char vel_info[12];
	itoa(vel, vel_info, 10);//��������ֵ����û����
	*/
	//��������ܷ��ظ�ʽ����ʱ�䣬��ν��ʽ������ָ00:00:13:05֮��

	//if (params[DisMode]->u.pd.value == 5) {
	//	params[DisDynamicsTopic]->flags |= PF_ParamFlag_START_COLLAPSED;//չ��topic
	//}


	//PF_TimeDisplayPrefVersion3 *tdp = (PF_TimeDisplayPrefVersion3*)malloc(sizeof(PF_TimeDisplayPrefVersion3));
	//ע����ʹ���Զ�������͵ĺ���֮ǰҪ��malloc���������Ұָ��ʹ���������
	//A_long time_start = 0;
	//suites.AdvTimeSuite4()->PF_GetTimeDisplayPref(tdp, &time_start);
	//suites.AdvTimeSuite4()->PF_FormatTime(in_data,output,time,scale,1,time_info);
	//char* err_info=0;
	//strcpy_s(err_info,10, (char*)out_data->return_msg);
	//suites.AdvAppSuite2()->PF_InfoDrawText("Support by Dolag kira��", err_info);
	suites.AdvAppSuite2()->PF_InfoDrawText("Support by Dolag", "kira��_(:�١���)_");

	if (extra->param_index == DisButton) {
		isLisensed = !isLisensed;
		//params[DisButton]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		out_data->out_flags |= PF_OutFlag_FORCE_RERENDER;
	}

	PF_Err		err = PF_Err_NONE;

	return err;
}

PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	void* extra)
{
	PF_Err		err = PF_Err_NONE;
	/*AEGP_Command* icon;//aegps������⣬�о�ûɶ��
	AEGP_CommandSuite1 cmdSuite;
	cmdSuite.AEGP_GetUniqueCommand(icon);
	//InsertMenuItem()
	cmdSuite.AEGP_InsertMenuCommand(*icon, "drawUI", AEGP_Menu_WINDOW, AEGP_MENU_INSERT_SORTED);
	cmdSuite.AEGP_EnableCommand(*icon);
	AEGP_RegisterSuite5 rgstSuite;
	//AEGP_GlobalRefcon
	//rgstSuite.AEGP_RegisterCommandHook(AEGP_HP_BeforeAE);
	*/
	try {
		switch (cmd) {
		case PF_Cmd_USER_CHANGED_PARAM:

			err = ChangeParam(in_data,
				out_data,
				params,
				output,
				(PF_UserChangedParamExtra*)(extra));
			break;

		case PF_Cmd_ABOUT:

			err = About(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_GLOBAL_SETUP:

			err = GlobalSetup(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_PARAMS_SETUP:

			err = ParamsSetup(in_data,
				out_data,
				params,
				output);
			break;

			/*case  PF_Cmd_FRAME_SETUP:

				err = FrameSetup(in_data,
								out_data,
								params,
								output);
				break;
			*/
			/*case PF_Cmd_EVENT:

				err = HandleEvent(in_data,
					out_data,
					params,
					output,
					reinterpret_cast<PF_EventExtra*>(extra));
				break;
			*/
		case PF_Cmd_DO_DIALOG:

			err = DoDialog(in_data, out_data, params, output);
			break;

		case PF_Cmd_RENDER:

			err = Render(in_data,
				out_data,
				params,
				output);
			break;
		}
	}
	catch (PF_Err & thrown_err) {
		err = thrown_err;
	}
	return err;
}

