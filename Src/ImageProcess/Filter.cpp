#define DLLAPI_EXPORTS

#include "Filter.h"
#include<vector>
#include "Utility.h"
#include "Histogram.h"
using namespace std;

namespace QQ
{
static void GetMedianValueAndPixelCountLowerMedian(const Mat<uchar> &image, int &medianValue, int &pixelCountLowerMedian)
{
	//��ȡֱ��ͼ
	Mat<int> histogramMat;
	CalculateHistogram(image, histogramMat);

	int *histogram = (int *)histogramMat.data;
	pixelCountLowerMedian = 0;
	int pixelCount = image.cols*image.rows;
	for (int i = 0; i <= 255; ++i)
	{
		//
		pixelCountLowerMedian += histogram[i];
		if (2 * pixelCountLowerMedian>pixelCount)//���ó���(sum>pixelCount/2)
		{
			medianValue = i;
			break;
		}
	}

}

//2015-1-1 20:23:53��by QQ
//ʹ��copyMakeBorder����ͼ��߽磬�����˲��߽�
void Blur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, Size kernelSize)
{
	//��ͼ��Ĵ�С
	int width_Dst = srcImage.cols;
	int height_Dst = srcImage.rows;
	dstImage.Create(Size(width_Dst, height_Dst), 1);//������·��䣬֮ǰ�Ŀռ���ӵ�

	//����Ĵ�С
	int width_Extend = kernelSize.width >> 1;
	int height_Extend = kernelSize.height >> 1;

	//������ͼ��
	Mat<uchar> image_New;
	CopyMakeBorder(srcImage, image_New, height_Extend, height_Extend, width_Extend, width_Extend);

	//��ͼ�����
	int width_New = image_New.cols;
	int height_New = image_New.rows;
	int widthStep_New = width_New;

	//��������
	int width_Aperture = kernelSize.width;
	int height_Aperture = kernelSize.height;
	int pixelCount = width_Aperture*height_Aperture;

	//������Ҫ�˲����ص��������
	int startX = width_Aperture >> 1;
	int startY = height_Aperture >> 1;

	//ÿ�еĻҶ�ֵ��
	int *sum_PerCol = new int[width_New];

	//����ͼ�����˲�����
	uchar *row_New = image_New.data + startY*widthStep_New + startX;
	uchar *row_Dst = dstImage.data;//��һ��
	uchar *row_Aperture_New = image_New.data;
	for (int y = startY; y <= height_New - startY - 1; ++y)
	{
		//��
		uchar *col_Dst = row_Dst;
		uchar *col_Aperture_New = row_Aperture_New;

		//����ÿ��height_Aperture�����صĻҶ�ֵ��
		//��һ�У����������еĺ�
		if (y == startY)
		{

			for (int k = 0; k <= width_New - 1; ++k)
			{
				sum_PerCol[k] = 0;
				//ÿ�е�һ��ָ��
				uchar *col_PerLine = col_Aperture_New + k;
				for (int t = 0; t <= height_Aperture - 1; ++t)
				{
					sum_PerCol[k] += col_PerLine[0];
					col_PerLine += widthStep_New;//��һ��
				}

			}
		}
		else//�ǵ�һ��
		{
			for (int k = 0; k <= width_New - 1; ++k)
			{
				//ÿ�е�һ��ָ��
				uchar *col_ = col_Aperture_New + k;
				sum_PerCol[k] -= col_[0 - widthStep_New];//������
				sum_PerCol[k] += col_[0 + (height_Aperture - 1)*widthStep_New];//������
			}

		}

		//����width_Aperture�е����ܺ�
		int sum_Aperture = 0;
		for (int x = startX; x <= width_New - startX - 1; ++x)
		{
			//ÿ�е�һ��Ԫ�أ���width_Aperture���к�
			if (x == startX)
			{
				for (int k = 0; k <= width_Aperture - 1; ++k)
				{
					sum_Aperture += sum_PerCol[k];
				}
			}
			else//�ǵ�һ��Ԫ��
			{
				//��ȥ���
				sum_Aperture -= sum_PerCol[x - startX - 1];

				//�����ұ�
				sum_Aperture += sum_PerCol[x + startX];
			}

			//���ֵ
			uchar meanValue = sum_Aperture / pixelCount;
			col_Dst[0] = meanValue;

			//����һ������
			col_Dst++;
			//col_New++;//���ﲻ��Ҫ��Դͼ��ָ����л���
		}

		//��һ��
		row_Dst += width_Dst;
		row_Aperture_New += width_New;
	}
}// Blur


//2015-4-22 19:51:29,by QQ
void MedianBlur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, int width_Aperture)
{
	
	int width_Dst = srcImage.cols;
	int height_Dst = srcImage.rows;
	dstImage.Create(Size(width_Dst, height_Dst), 1);//������·��䣬֮ǰ�Ŀռ���ӵ�
	//dstImage.setTo(Scalar(0));//��Ϊ0

	//��������
	int pixelCount = width_Aperture*width_Aperture;//�����������ܸ���
	Mat<uchar> image_Aperture(width_Aperture, width_Aperture, 1);//��������ͼ��
	int thresholdValue = pixelCount / 2 + 1;//step1.������ֵ(����ο���ͼ��ĸ�Ч���Ҫ��֮��)

	//�����������
	int startX = width_Aperture / 2;
	int startY = width_Aperture / 2;

	int height_Extend = startY;
	int width_Extend = startX;
	Mat<uchar> image_New;
	CopyMakeBorder(srcImage, image_New, height_Extend, height_Extend, width_Extend, width_Extend);//Ĭ�ϲ���BORDER_REFLECT
	int height_New = image_New.rows;
	int width_New = image_New.cols;
	//��һ��
	//������Ҫ����3��ָ��,һ�𻬶�
	//1.Դͼ���б���������� 
	//2.Ŀ��ͼ�񱻴�������� 
	//3.Դͼ�񻬶�����
	uchar *row_New = image_New.data + startY*width_New + startX;//��ͼ��
	uchar *row_Dst = dstImage.data;//Ŀ��ͼ��
	uchar *row_Aperture_New = image_New.data;//Դͼ���еĻ�������

	for (int y = startY; y <= height_New - startY - 1; ++y)
	{
		//��
		uchar *col_New = row_New;
		uchar *col_Dst = row_Dst;
		uchar *col_Aperture_New = row_Aperture_New;//����������������

		///////////////////////////////�Ի������ڲ���///////////////////////////////////////////
		//����ÿ�е�һ����������ֱ��ͼ
		//��ȡ��������ͼ��
		uchar *row_Aperture = image_Aperture.data;//��������ͼ��
		uchar *row_Aperture_New_2 = col_Aperture_New;
		for (int k = 0; k <= width_Aperture - 1; ++k)
		{
			//��
			uchar *col_ApertureImage = row_Aperture;
			uchar *col_Aperture_New_2 = row_Aperture_New_2;

			for (int w = 0; w <= width_Aperture - 1; ++w)
			{
				//����ÿ������
				col_ApertureImage[0] = col_Aperture_New_2[0];

				//��һ������
				col_ApertureImage++;
				col_Aperture_New_2++;
			}

			//��һ��
			row_Aperture += width_Aperture;
			row_Aperture_New_2 += width_New;
		}

		//step 2.ȷ����ֵ������¼����<=��ֵ�����ص����
		//��ֱ��ͼ
		Mat<int> histogramMat;
		CalculateHistogram(image_Aperture, histogramMat);

		//��������ֵ��<=��ֵ�����ص����
		int medianValue, pixelCountLowerMedian;
		GetMedianValueAndPixelCountLowerMedian(image_Aperture, medianValue, pixelCountLowerMedian);
		//////////////////////////////�������ڲ�������////////////////////////////////////////////

		//�˲�
		col_Dst[0] = (uchar)medianValue;
		int *histogram = (int *)histogramMat.data;

		//����һ������(����ָ����һ���ƶ�)
		col_Dst++;
		col_New++;
		col_Aperture_New++;
		for (int x = startX + 1; x <= width_New - startX - 1; ++x)//��ÿ�еڶ����˲����ؿ�ʼ
		{
			//////////////////////////////////ά�ֻ�������ֱ��ͼ////////////////////////////////////////
			//step 3.ȥ�����
			uchar *col_Left = col_Aperture_New - 1;
			for (int k = 0; k <= width_Aperture - 1; ++k)
			{
				int gray = col_Left[0];
				histogram[gray] -= 1;
				if (gray <= medianValue)
				{
					pixelCountLowerMedian--;
				}
				col_Left += width_New;
			}

			//step 4.�����Ҳ�
			uchar *col_Right = col_Aperture_New + width_Aperture - 1;
			for (int k = 0; k <= width_Aperture - 1; ++k)
			{
				int gray = col_Right[0];
				histogram[gray] += 1;
				if (gray <= medianValue)
				{
					pixelCountLowerMedian++;
				}
				col_Right += width_New;
			}

			//�����µ���ֵ
			if (pixelCountLowerMedian>thresholdValue)//step 6.
			{
				while (1)
				{
					pixelCountLowerMedian -= histogram[medianValue];
					medianValue--;
					if (pixelCountLowerMedian <= thresholdValue)
					{
						break;
					}
				}

			}
			else
			{
				while (pixelCountLowerMedian<thresholdValue)//step 5
				{
					medianValue++;
					pixelCountLowerMedian += histogram[medianValue];

				}

			}

			//�˲�
			col_Dst[0] = (uchar)medianValue;

			//����һ������
			col_New++;
			col_Dst++;
			col_Aperture_New++;
		}//end of x

		//��һ��
		row_New += width_New;
		row_Dst += width_Dst;
		row_Aperture_New += width_New;
	}//end of y

}// MedianBlur

// 2016-10-4,by QQ
void GaussianBlur(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, double sigma)
{

	// compute the size of gaussian kernel
	int kSize = (int)(sigma * 3) << 1 + 1;// must be odd

	double coefficient = -1 / (2 * sigma*sigma);
	double coefficient2 = -1 * coefficient / PI;

	// the center of gaussian kernel
	int x0 = (kSize - 1) >> 1;
	int y0 = (kSize - 1) >> 1;

	// compute gaussian kernel
	Mat<float> gaussianKernel(kSize, kSize, 1);
	float *rowOfKernel = (float *)gaussianKernel.data;
	double sum = 0;// for normalization
	for (int y = 0; y <= kSize - 1; ++y, rowOfKernel += kSize)
	{
		float *colOfKernel = rowOfKernel;
		for (int x = 0; x <= kSize - 1; ++x, ++colOfKernel)
		{
			double distance = (x - x0)*(x - x0) + (y - y0)*(y - y0);
			colOfKernel[0] = coefficient2*exp(coefficient*(distance));
			sum += colOfKernel[0];
		}

	}

	// normalize the gaussian kernel
	int numberOfPixels = kSize*kSize;
	float *dataOfKernel = (float *)gaussianKernel.data;
	for (int i = 0; i <= numberOfPixels - 1; ++i)
	{
		dataOfKernel[i] /= sum;
	}

	// ���������,����Library_OpenCV�е�Convolution
	// Convolution(srcImage, gaussianKernel,dstImage);

}// GaussianBlur

Mat<float> GetGaborKernel(Size ksize, double sigma, double theta, double lambd, double gamma, double psi)
{
	double sigma_x = sigma;
	double sigma_y = sigma / gamma;
	int nstds = 3;
	int xmin, xmax, ymin, ymax;
	double c = cos(theta), s = sin(theta);

	if (ksize.width > 0)
		xmax = ksize.width / 2;
	else
		xmax = MAX(fabs(nstds*sigma_x*c), fabs(nstds*sigma_y*s));

	if (ksize.height > 0)
		ymax = ksize.height / 2;
	else
		ymax = MAX(fabs(nstds*sigma_x*s), fabs(nstds*sigma_y*c));

	xmin = -xmax;
	ymin = -ymax;


	Mat<float> kernel(ymax - ymin + 1, xmax - xmin + 1,1);
	double scale = 1;
	double ex = -0.5 / (sigma_x*sigma_x);
	double ey = -0.5 / (sigma_y*sigma_y);
	double cscale = PI * 2 / lambd;

	for (int y = ymin; y <= ymax; y++)
	for (int x = xmin; x <= xmax; x++)
	{
		double xr = x*c + y*s;
		double yr = -x*s + y*c;

		double v = scale*exp(ex*xr*xr + ey*yr*yr)*cos(cscale*xr + psi);
		kernel.At<float>(ymax - y, xmax - x) = (float)v;
	}

	return kernel;

}
// �������,Kernel�����ξ�����ұ߳�Ϊ����
void Convolution(const Mat<uchar> &srcImage, const Mat<float> &kernel, Mat<uchar> &dstImage)
{
	// Ŀ��ͼ���С
	int stride = 1;// Ĭ�ϻ������ڲ���Ϊ1
	int kernelSize = kernel.rows;
	int widthOfDst = (srcImage.cols + kernelSize - 1 - kernelSize) / stride + 1;
	int heightOfDst = (srcImage.rows + kernelSize - 1 - kernelSize) / stride + 1;
	dstImage.Create(heightOfDst, widthOfDst, 1);

	// ����ԭͼ
	Mat<uchar> extendedImage;
	CopyMakeBorder(srcImage, extendedImage, kernelSize / 2, kernelSize / 2, kernelSize / 2, kernelSize / 2);

	// ���������������ز��ұ�,���㻬��������ÿ��������ͼ��������ڻ�������ָ��ƫ��
	std::vector<int> pixelOffset;
	pixelOffset.resize(kernelSize*kernelSize);
	memset(&pixelOffset[0], 0, pixelOffset.size()*sizeof(int));
	int index = 0;
	for (int y = 0; y < kernelSize; ++y)
	{
		int yOffset = y*extendedImage.cols;
		for (int x = 0; x < kernelSize; ++x)
		{
			// ����ÿ�����ص�ƫ��
			pixelOffset[index++] = yOffset + x;
		}
	}

	// ������
	uchar *rowOfDst = dstImage.data;
	for (int y = 0; y <= heightOfDst - 1; ++y, rowOfDst += widthOfDst)
	{
		uchar *colOfDst = rowOfDst;

		// ��������(��һ��Ԫ��)��y�����ϵ�ƫ��
		int offsetOfY = y*stride;

		for (int x = 0; x <= widthOfDst - 1; ++x, ++colOfDst)
		{
			// ���㻬��������ͼ���е�ƫ��,��ת��Ϊָ��û������ڵ�ָ��
			int offsetOfX = x*stride;
			uchar *dataOfROI = extendedImage.data + offsetOfY*extendedImage.cols + offsetOfX;

			// �����
			float *dataOfKernel = (float *)kernel.data;
			float sum = 0;
			for (int i = 0; i < pixelOffset.size(); ++i)
			{
				sum += (dataOfROI[pixelOffset[i]] * dataOfKernel[i]);
			}

			// ��������ֵΪ���ͼ��,ע������Ĵ���
			colOfDst[0] = SATURATE((int)sum);
		}
	}
}

















}//end of namespace QQ
