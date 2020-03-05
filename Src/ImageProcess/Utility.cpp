#define DLLAPI_EXPORTS

#include "Utility.h"
#include <string.h>

namespace QQ
{
	void CopyMakeBorder(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, int top, int bottom, int left, int right)
	{
		// ������飬�ڴ����
		int newCols = srcImage.cols + left + right;
		int newRows = srcImage.rows + top + bottom;
		dstImage.Create(newRows, newCols, srcImage.numberOfChannels);
		dstImage.SetTo(Scalar(0, 0, 0));

		uchar *dataOfSrc = srcImage.data;
		uchar *dataOfDst = dstImage.data + dstImage.step*top + left*dstImage.numberOfChannels;
		for (int y = 0; y <= srcImage.rows - 1; ++y, dataOfDst += dstImage.step, dataOfSrc += srcImage.step)
		{
			memcpy(dataOfDst, dataOfSrc, srcImage.step);
		}
	} // CopyMakeBorder

	//2015-3-4 16:53:57��by QQ
	//�Ľ��ļ������ػҶ�ֵ����ͼ��
	//������Integral(y,x) = Integral(y-1,x) + rowSum(y);
	void ComputeIntegralImage(const Mat<uchar> &srcImage, Mat<int> &dstImage)
	{
		//��ͼ��Ĵ�С
		int width_Dst = srcImage.cols;
		int height_Dst = srcImage.rows;
		dstImage.Create(Size(width_Dst, height_Dst), 1);//������·��䣬֮ǰ�Ŀռ���ӵ�
		dstImage.SetTo(Scalar(0));

		int width_Src = srcImage.cols;
		int height_Src = srcImage.rows;

		//ÿ�����ص�ͨ����
		int channelCount_Src = srcImage.numberOfChannels;
		int channelCount_Integral = dstImage.numberOfChannels;//ÿ�����ص�ͨ����

		//�е�ͨ����
		int widthStep_Src = channelCount_Src*width_Src;
		int widthStep_Integral = channelCount_Integral*width_Src;

		//��һ��
		uchar *row_Src = srcImage.data;
		int *row_Integral = (int *)dstImage.data;//ע��ָ���ת��
		for (int y = 0; y <= height_Src - 1; ++y)
		{
			int sum = 0;//��ǰ�е��ۼӺ�
			//��
			uchar *col_Src = row_Src;
			int *col_Integral = row_Integral;
			for (int x = 0; x <= width_Src - 1; ++x)
			{
				//���е��ۼ�
				sum += col_Src[0];

				//�����0��,��һ�е�������
				if (y == 0)
				{
					col_Integral[0] = sum;
				}
				else
				{
					//�ǵ�0��
					//��ǰ���ۼӺ�+ͬ�е���һ��Ԫ�ص�ֵ
					col_Integral[0] = sum + col_Integral[0 - widthStep_Integral];//�±�
					//col_Integral[0]=sum+*(col_Integral-image_Integral.cols);//ָ���ƶ�

				}

				//��һ������
				col_Src++;
				col_Integral++;

			}
			//��һ��
			row_Src += widthStep_Src;
			row_Integral += widthStep_Integral;
		}

	}// ComputeIntegralImage





}//namespace QQ
