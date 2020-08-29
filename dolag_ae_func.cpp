//����Щ�Զ���ĺ���
#include "dolag_ae_func.h"

PF_Pixel8 Bilinear8(PF_LayerDef* layer, double x, double y) {//�ĸ�8bpc����֮���˫���Բ�ֵ
	int xL = floor(x);
	int yL = floor(y);
	int rowbytes = layer->rowbytes;
	if (xL < layer->width && xL >= 0 && yL>0 && yL < layer->height) {//���ڱ�Ե

		PF_Pixel8* left_bottom = (PF_Pixel8*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel8)));
		PF_Pixel8* left_top = (PF_Pixel8*)((char*)left_bottom - rowbytes);
		PF_Pixel8* right_bottom = (PF_Pixel8*)((char*)left_bottom + sizeof(PF_Pixel8));
		PF_Pixel8* right_top = (PF_Pixel8*)((char*)left_bottom + sizeof(PF_Pixel8) - rowbytes);

		double dx = x - xL;
		double dy = y - yL;
		PF_Pixel8 tmp;
		tmp.red = ((1 - dx) * left_bottom->red + dx * right_bottom->red) * (1 - dy) + ((1 - dx) * left_top->red + dx * right_top->red) * dy;
		tmp.blue = ((1 - dx) * left_bottom->blue + dx * right_bottom->blue) * (1 - dy) + ((1 - dx) * left_top->blue + dx * right_top->blue) * dy;
		tmp.green = ((1 - dx) * left_bottom->green + dx * right_bottom->green) * (1 - dy) + ((1 - dx) * left_top->green + dx * right_top->green) * dy;
		tmp.alpha = ((1 - dx) * left_bottom->alpha + dx * right_bottom->alpha) * (1 - dy) + ((1 - dx) * left_top->alpha + dx * right_top->alpha) * dy;
		return tmp;
	}
	else if (xL == layer->width && xL >= 0 && yL > 0 && yL < layer->height) {//���ұ�Ե
		PF_Pixel8* left_bottom = (PF_Pixel8*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel8)));
		PF_Pixel8* left_top = (PF_Pixel8*)((char*)left_bottom - rowbytes);
		double dy = y - yL;
		PF_Pixel8 tmp;
		tmp.red = (1 - dy) * left_bottom->red + dy * left_top->red;
		tmp.blue = (1 - dy) * left_bottom->blue + dy * left_top->blue;
		tmp.green = (1 - dy) * left_bottom->green + dy * left_top->green;
		tmp.alpha = (1 - dy) * left_bottom->alpha + dy * left_top->alpha;
		return tmp;
	}
	else if (xL < layer->width && xL >= 0 && yL > 0 && yL == layer->height) {//���ϱ�Ե
		PF_Pixel8* left_bottom = (PF_Pixel8*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel8)));
		PF_Pixel8* right_bottom = (PF_Pixel8*)((char*)left_bottom + sizeof(PF_Pixel8));
		double dx = x - xL;
		PF_Pixel8 tmp;
		tmp.red = (1 - dx) * left_bottom->red + dx * right_bottom->red;
		tmp.blue = (1 - dx) * left_bottom->blue + dx * right_bottom->blue;
		tmp.green = (1 - dx) * left_bottom->green + dx * right_bottom->green;
		tmp.alpha = (1 - dx) * left_bottom->alpha + dx * right_bottom->alpha;
		return tmp;
	}
	else if (xL == layer->width && yL == layer->height) {
		PF_Pixel8* left_bottom = (PF_Pixel8*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel8)));
		PF_Pixel8 tmp;
		tmp.red = left_bottom->red;
		tmp.blue = left_bottom->blue;
		tmp.green = left_bottom->green;
		tmp.alpha = left_bottom->alpha;
		return tmp;
	}
	else {
		PF_Pixel8 tmp;
		tmp.red = 0;
		tmp.blue = 0;
		tmp.green = 0;
		tmp.alpha = 0;
		return tmp;
	}
}

PF_Pixel16 Bilinear16(PF_LayerDef* layer, double x, double y) {//�ĸ�16bpc����֮���˫���Բ�ֵ
	int xL = floor(x);
	int yL = floor(y);
	int rowbytes = layer->rowbytes;
	if (xL < layer->width && xL >= 0 && yL>0 && yL < layer->height) {//���ڱ�Ե

		PF_Pixel16* left_bottom = (PF_Pixel16*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel16)));
		PF_Pixel16* left_top = (PF_Pixel16*)((char*)left_bottom - rowbytes);
		PF_Pixel16* right_bottom = (PF_Pixel16*)((char*)left_bottom + sizeof(PF_Pixel16));
		PF_Pixel16* right_top = (PF_Pixel16*)((char*)left_bottom + sizeof(PF_Pixel16) - rowbytes);

		double dx = x - xL;
		double dy = y - yL;
		PF_Pixel16 tmp;
		tmp.red = ((1 - dx) * left_bottom->red + dx * right_bottom->red) * (1 - dy) + ((1 - dx) * left_top->red + dx * right_top->red) * dy;
		tmp.blue = ((1 - dx) * left_bottom->blue + dx * right_bottom->blue) * (1 - dy) + ((1 - dx) * left_top->blue + dx * right_top->blue) * dy;
		tmp.green = ((1 - dx) * left_bottom->green + dx * right_bottom->green) * (1 - dy) + ((1 - dx) * left_top->green + dx * right_top->green) * dy;
		tmp.alpha = ((1 - dx) * left_bottom->alpha + dx * right_bottom->alpha) * (1 - dy) + ((1 - dx) * left_top->alpha + dx * right_top->alpha) * dy;
		return tmp;
	}
	else if (xL == layer->width && xL >= 0 && yL > 0 && yL < layer->height) {//���ұ�Ե
		PF_Pixel16* left_bottom = (PF_Pixel16*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel16)));
		PF_Pixel16* left_top = (PF_Pixel16*)((char*)left_bottom - rowbytes);
		double dy = y - yL;
		PF_Pixel16 tmp;
		tmp.red = (1 - dy) * left_bottom->red + dy * left_top->red;
		tmp.blue = (1 - dy) * left_bottom->blue + dy * left_top->blue;
		tmp.green = (1 - dy) * left_bottom->green + dy * left_top->green;
		tmp.alpha = (1 - dy) * left_bottom->alpha + dy * left_top->alpha;
		return tmp;
	}
	else if (xL < layer->width && xL >= 0 && yL > 0 && yL == layer->height) {//���ϱ�Ե
		PF_Pixel16* left_bottom = (PF_Pixel16*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel16)));
		PF_Pixel16* right_bottom = (PF_Pixel16*)((char*)left_bottom + sizeof(PF_Pixel16));
		double dx = x - xL;
		PF_Pixel16 tmp;
		tmp.red = (1 - dx) * left_bottom->red + dx * right_bottom->red;
		tmp.blue = (1 - dx) * left_bottom->blue + dx * right_bottom->blue;
		tmp.green = (1 - dx) * left_bottom->green + dx * right_bottom->green;
		tmp.alpha = (1 - dx) * left_bottom->alpha + dx * right_bottom->alpha;
		return tmp;
	}
	else if (xL == layer->width && yL == layer->height) {
		PF_Pixel16* left_bottom = (PF_Pixel16*)((char*)layer->data + (yL * rowbytes) + (xL * sizeof(PF_Pixel16)));
		PF_Pixel16 tmp;
		tmp.red = left_bottom->red;
		tmp.blue = left_bottom->blue;
		tmp.green = left_bottom->green;
		tmp.alpha = left_bottom->alpha;
		return tmp;
	}
	else {
		PF_Pixel16 tmp;
		tmp.red = 0;
		tmp.blue = 0;
		tmp.green = 0;
		tmp.alpha = 0;
		return tmp;
	}
}

bool isOnScreen(PF_LayerDef* layer, int x, int y) {//�ж��Ƿ�����Ļ��
	return (x > -1 && y > -1 && x < layer->width && y < layer->height);
}


bool isOnScreen(PF_LayerDef* layer, float x, float y) {//�ж��Ƿ�����Ļ��
	return (x >= 0 && y >= 0 && x < layer->width - 1 && y < layer->height - 1);
}


void Swap(int* dst, int* src) {//������������ֵ
	*dst = *dst ^ *src;
	*src = *dst ^ *src;
	*dst = *dst ^ *src;
}
void Swap(A_u_char* dst, A_u_char* src) {//������������ֵ
	*dst = *dst ^ *src;
	*src = *dst ^ *src;
	*dst = *dst ^ *src;
}
void Swap(A_u_short* dst, A_u_short* src) {//������������ֵ
	*dst = *dst ^ *src;
	*src = *dst ^ *src;
	*dst = *dst ^ *src;
}


bool SwapPixel8(PF_Pixel8* dst, PF_Pixel8* src) {//��������8bpc����ָ���ֵ
	if (dst && src) {
		Swap(&dst->red, &src->red);
		Swap(&dst->blue, &src->blue);
		Swap(&dst->green, &src->green);
		Swap(&dst->alpha, &src->alpha);
		return 1;
	}
	return 0;
}


bool SwapPixel16(PF_Pixel16* dst, PF_Pixel16* src) {//��������16bpc����ָ���ֵ
	if (dst && src) {
		Swap(&dst->red, &src->red);
		Swap(&dst->blue, &src->blue);
		Swap(&dst->green, &src->green);
		Swap(&dst->alpha, &src->alpha);
		return 1;
	}
	return 0;
}

void Normalize(float* x, float* y) {//��һ��
	double square = sqrt(*x * *x + *y * *y);
	*x /= square;
	*y /= square;
}



//Ȼ���㷢�ֱ�����л�ȡ���صĺ���,�����С��鷳
//PF_GET_PIXEL_DATA8()
// PF_GET_PIXEL_DATA16();�����������*in_data������
PF_Pixel8* getPixel8(PF_LayerDef* layer, int x, int y) {
	//������������p��
	//��Ϊ��char*��ֻռ1�ֽڣ����Կ�����ô�ã�����������е�
	//�ɴ˿��Կ���ae������һ���������ڴ������ָ�룬�ܷ�����ܹ��ƶ�ָ���Է�������
	//���⿪��һ��鴢��������Ϣ�������ڴ�(����������Ļ���Ҫһ��һ���������ʣ����ģ����Զ�������һЩ�ռ��Ա�֤Ч��)

	//��ȷ�Ľ����ǣ����Ƕ�ָ�����ֽڲ�������Ϊָ����ָ�Ĳ�ȫ��������Ϣ�����ֱ����ָ���ƫ�ƻᵼ������
	//����Ҫ��ae�����ṩ��rowbytes������
	if (isOnScreen(layer, x, y)) {
		return (PF_Pixel8*)((char*)layer->data + (y * layer->rowbytes) + (x * sizeof(PF_Pixel8)));
	}
	else return NULL;
};//���ڻ�ȡָ��xy��������ص��ڴ��ַ

PF_Pixel16* getPixel16(PF_LayerDef* layer, int x, int y) {
	if (isOnScreen(layer, x, y)) {
		return (PF_Pixel16*)((char*)layer->data + (y * layer->rowbytes) + (x * sizeof(PF_Pixel16)));
	}
	else return NULL;
}

float lerp(float Dmin, float Dmax, float Smin, float Smax, float value) {
	return (float)((value - Smin) / (Smax - Smin) * (Dmax - Dmin) + Dmin);
}//����ӳ�亯��

void DrawWaterMark(PF_LayerDef* output, PF_LayerDef* src_layer, int time, bool isLisensed) {
	if (!isLisensed) {
		if (!PF_WORLD_IS_DEEP(output)) {
			float ratio = output->width / float(output->height);
			int rowbytes = src_layer->rowbytes;
			const int mark_wid = 8;
			PF_Pixel8* pix = NULL;
			for (int i = 0; i < output->width; i++) {
				int j = i * ratio;

				for (int k = 0; k < mark_wid; k++, j++) {
					pix = (PF_Pixel8*)((char*)output->data + i * rowbytes + j * sizeof(PF_Pixel8));
					if (isOnScreen(output, j, i)) {
						srand(i + j + time * 114);
						pix->red = float(rand()) / (RAND_MAX + 1.0) * 55 + 150;
						pix->green = float(rand()) / (RAND_MAX + 1.0) * 100;
						srand(i + j + time * 514);
						pix->blue = float(rand()) / (RAND_MAX + 1.0) * 100;
						pix->alpha = 255;
					}
					pix = (PF_Pixel8*)((char*)output->data + (i + 1) * rowbytes - j * sizeof(PF_Pixel8));
					if (isOnScreen(output, j, output->height - i)) {
						srand(i + j + time * 19);
						pix->red = float(rand()) / (RAND_MAX + 1.0) * 55 + 150;
						pix->green = float(rand()) / (RAND_MAX + 1.0) * 100;
						srand(i + +j + time * 810);
						pix->blue = float(rand()) / (RAND_MAX + 1.0) * 100;
						pix->alpha = 255;
					}
				}
			}
		}
		else if (PF_WORLD_IS_DEEP(output)) {
			float ratio = output->width / float(output->height);
			int rowbytes = src_layer->rowbytes;
			const int mark_wid = 8;
			PF_Pixel16* pix = NULL;
			for (int i = 0; i < output->width; i++) {
				int j = i * ratio;

				for (int k = 0; k < mark_wid; k++, j++) {
					pix = (PF_Pixel16*)((char*)output->data + i * rowbytes + j * sizeof(PF_Pixel16));
					if (isOnScreen(output, j, i)) {
						srand(i + j + time * 114);
						pix->red = float(rand()) / (RAND_MAX + 1.0) * 12768 + 20000;
						pix->green = float(rand()) / (RAND_MAX + 1.0) * 12768;
						srand(i + j + time * 514);
						pix->blue = float(rand()) / (RAND_MAX + 1.0) * 12768;
						pix->alpha = 32768;
					}
					pix = (PF_Pixel16*)((char*)output->data + (i + 1) * rowbytes - j * sizeof(PF_Pixel16));
					if (isOnScreen(output, j, output->height - i)) {
						srand(i + j + time * 19);
						pix->red = float(rand()) / (RAND_MAX + 1.0) * 12768 + 20000;
						pix->green = float(rand()) / (RAND_MAX + 1.0) * 12768;
						srand(i + +j + time * 810);
						pix->blue = float(rand()) / (RAND_MAX + 1.0) * 12768;
						pix->alpha = 32768;
					}
				}
			}
		}
	}
}


PF_Err DOLAG_ReleaseSuite(PF_InData* in_data,
	PF_OutData* out_data,
	const char* name,
	int32_t			version,
	const char* error_stringPC0)
{
	PF_Err			err = PF_Err_NONE;
	SPBasicSuite* bsuite;

	bsuite = in_data->pica_basicP;

	if (bsuite) {
		(*bsuite->ReleaseSuite)((char*)name, version);
	}
	else {
		err = PF_Err_BAD_CALLBACK_PARAM;
	}

	if (err) {
		const char* error_stringPC = error_stringPC0 ? error_stringPC0 : "Not able to release AEFX Suite.";

		out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;

		PF_SPRINTF(out_data->return_msg, error_stringPC);
	}

	return err;
}


void RepeatEdge(float* x, float* y, short width, short height, int mode) {
	switch (mode)
	{
	case REPEAT_BASE:
	{
		if (*x < 0) {
			*x = *x + width - 1;
		}
		else if (*x > width)
		{
			*x = *x - width + 1;
		}
		if (*y < 0) {
			*y = *y + height - 1;
		}
		else if (*y > height)
		{
			*y = *y - height + 1;
			//*y -= 2;
		}

		break;
	}
	case REPEAT_MIRROR:
	{
		if (*x < 0) {
			*x = -*x;
		}
		else if (*x > width ) {
			*x = 2 * width - *x - 1;
		}
		if (*y < 0) {
			*y = -*y;
		}
		else if (*y > height) {
			*y = 2 * height - *y - 1;
		}
	}
	default:
		break;
	}
}


void BlendPix(const PF_Pixel8 src, PF_Pixel8* dst, float coe) {
	dst->red = dst->red * (1 - coe) + src.red * coe;
	dst->green = dst->green * (1 - coe) + src.green * coe;
	dst->blue = dst->blue * (1 - coe) + src.blue * coe;
	dst->alpha = dst->alpha * (1 - coe) + src.alpha * coe;

}

void BlendPix(const PF_Pixel16 src, PF_Pixel16* dst, float coe) {
	dst->red = dst->red * (1 - coe) + src.red * coe;
	dst->green = dst->green * (1 - coe) + src.green * coe;
	dst->blue = dst->blue * (1 - coe) + src.blue * coe;
	dst->alpha = dst->alpha * (1 - coe) + src.alpha * coe;

}

float GetLumn(PF_Pixel8 pix) {
	return (0.299 * pix.red + pix.green * 0.584 + pix.blue * 0.114) * pix.alpha / PF_MAX_CHAN8 / PF_MAX_CHAN8;
}
float GetLumn(PF_Pixel16 pix) {
	return (0.299 * pix.red + pix.green * 0.584 + pix.blue * 0.114) * pix.alpha / PF_MAX_CHAN16 / PF_MAX_CHAN16;
}

//#define PIX_FLOAT_VALUE8(CHANNEL) \
//(CHANNEL / float(PF_MAX_CHAN8) + 1)
//
//#define PIX_FLOAT_VALUE16(CHANNEL) \
//(CHANNEL / float(PF_MAX_CHAN16) + 1)

#define CLAMP01(value) \
(value > 1?1:value)

float PixDiff(PF_Pixel8 Pa, PF_Pixel8 Pb) {

	float r, g, b, a;
	float delta = 0;
	r = CLAMP01(ABS(float(Pa.red - Pb.red) / max(Pa.red + 1, Pb.red + 1)));
	g = CLAMP01(ABS(float(Pa.green - Pb.green) / max(Pa.green + 1, Pb.green + 1)));
	b = CLAMP01(ABS(float(Pa.blue - Pb.blue) / max(Pa.blue + 1, Pb.blue + 1)));
	a = 1 - ABS(float(Pa.alpha - Pb.alpha) / PF_MAX_CHAN8);
	delta = (r + g + b) * a / 3.0f;
	return delta;
}

float PixDiff(PF_Pixel16 Pa, PF_Pixel16 Pb) {
	float r, g, b, a;
	float delta = 0;
	r = CLAMP01(ABS(float(Pa.red - Pb.red) / max(Pa.red + 1, Pb.red + 1)));
	g = CLAMP01(ABS(float(Pa.green - Pb.green) / max(Pa.green + 1, Pb.green + 1)));
	b = CLAMP01(ABS(float(Pa.blue - Pb.blue) / max(Pa.blue + 1, Pb.blue + 1)));
	a = 1 - ABS(float(Pa.alpha - Pb.alpha) / PF_MAX_CHAN16);
	delta = (r + g + b) * a / 3.0f;
	return delta;
}


const float BLEND = .5;
void GetPseudoFXAA(PF_LayerDef* layer, short x, short y, float MINLIMIT) {
	if (!PF_WORLD_IS_DEEP(layer)) {
		PF_Pixel8* pix = getPixel8(layer, x, y);
		PF_Pixel8* up = getPixel8(layer, x, y + 1);
		PF_Pixel8* bottom = getPixel8(layer, x, y - 1);
		PF_Pixel8* right = getPixel8(layer, x + 1, y);
		PF_Pixel8* left = getPixel8(layer, x - 1, y);
		float diff = 0;
		if (up) {
			diff = PixDiff(*pix, *up);
			if (diff > MINLIMIT) {
				BlendPix(*pix, up, BLEND * diff);
			}
		}
		if (bottom) {
			diff = PixDiff(*pix, *bottom);
			if (diff > MINLIMIT) {
				BlendPix(*pix, bottom, BLEND * diff);
			}
		}
		if (right) {
			diff = PixDiff(*pix, *right);
			if (diff > MINLIMIT) {
				BlendPix(*pix, right, BLEND * diff);
			}
		}
		if (left) {
			diff = PixDiff(*pix, *left);
			if (diff > MINLIMIT) {
				BlendPix(*pix, left, BLEND * diff);
			}
		}

	}
	else {
		PF_Pixel16* pix = getPixel16(layer, x, y);
		PF_Pixel16* up = getPixel16(layer, x, y + 1);
		PF_Pixel16* bottom = getPixel16(layer, x, y - 1);
		PF_Pixel16* right = getPixel16(layer, x + 1, y);
		PF_Pixel16* left = getPixel16(layer, x - 1, y);
		float diff = 0;
		if (up) {
			diff = PixDiff(*pix, *up);
			if (diff > MINLIMIT) {
				BlendPix(*pix, up, BLEND * diff);
			}
		}
		if (bottom) {
			diff = PixDiff(*pix, *bottom);
			if (diff > MINLIMIT) {
				BlendPix(*pix, bottom, BLEND * diff);
			}
		}
		if (right) {
			diff = PixDiff(*pix, *right);
			if (diff > MINLIMIT) {
				BlendPix(*pix, right, BLEND * diff);
			}
		}
		if (left) {
			diff = PixDiff(*pix, *left);
			if (diff > MINLIMIT) {
				BlendPix(*pix, left, BLEND * diff);
			}
		}
	}
}


void PseduoFXAA(PF_LayerDef* layer, float MINLIMIT) {
	int i, j;
	for (i = 0; i < layer->height; i++) {
		for (j = 0; j < layer->width; j++) {
			GetPseudoFXAA(layer, j, i, MINLIMIT);
		}
	}
}
