#define DLLAPI_EXPORTS

#include "GrayTransformation.h"
#include"Histogram.h"

namespace QQ
{

void RGB2Gray(const Mat<uchar> &srcImage, Mat<uchar> &dstImage)
{
	// ������飬�ڴ����
	assert(srcImage.numberOfChannels == 3 && "srcImage.numberOfChannels==3");
	dstImage.Create(srcImage.rows,srcImage.cols,1);
	

	//�������ұ�
	int LUTR[256],LUTG[256],LUTB[256];
	for (int i=0;i<=255;++i)
	{
		LUTR[i]=0.299*(i<<16);
		LUTG[i]=0.587*(i<<16);
		LUTB[i]=0.114*(i<<16);
	}

	//���ò��ұ�ҶȻ�
	int pixelCount=srcImage.rows*srcImage.cols;
	uchar *dataOfSrc=srcImage.data;
	uchar *dataOfDst=dstImage.data;
	for (int i = 0; i <= pixelCount - 1; ++i, dataOfSrc += 3, ++dataOfDst)
	{

		int b = LUTB[dataOfSrc[0]];
		int g = LUTG[dataOfSrc[1]];
		int r = LUTR[dataOfSrc[2]];
		dataOfDst[0] = (b + g + r) >> 16;
	}
}

int ComputeMeanGray(const Mat<uchar> &srcImage)
{
	// �������
	assert(srcImage.numberOfChannels == 1 && "srcImage.numberOfChannels==1");

	uchar *data = srcImage.data;
	int numberOfPixels = srcImage.rows*srcImage.cols;
	int sum = 0;
	for (int i = 0; i <= numberOfPixels - 1; ++i)
		sum += data[i];

	return sum / numberOfPixels;
}

void EqualizeHistogram(const Mat<uchar> &srcImage, Mat<uchar> &dstImage)
{
	dstImage.Create(srcImage.rows, srcImage.cols, 1);

	// ��Ҷ�ֱ��ͼ
	Mat<int> historgam;
	CalculateHistogram(srcImage, historgam);

	// ���ۼƷֲ�ֱ��ͼ�����ۼ�ֱ��ͼ��Ϊ�Ҷȱ任����(LUT)
	int *dataOfHistogram = (int *)historgam.data;
	int LUT[256];
	LUT[0] = 0;
	int sum = dataOfHistogram[0];
	int pixelCount = srcImage.rows*srcImage.cols;
	for (int i = 1; i <= 255; ++i)
	{
		sum += dataOfHistogram[i];
		LUT[i] = 255 * sum / pixelCount;//�ۼӸ���
	}


	// �Ҷȱ任
	uchar *dataOfSrc = srcImage.data;
	uchar *dataOfDst = dstImage.data;
	for (int i = 0; i <= pixelCount - 1; ++i)
	{
		int gray = dataOfSrc[i];
		dataOfDst[i] = LUT[gray];
	}

}


}//namespace QQ
