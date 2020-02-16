#define DLLAPI_EXPORTS

#include "GeometryTransformation.h"
#include "Utility.h"

#define  ROTATE_SHIFT  8

namespace QQ
{
	void Rotate(const Mat<uchar> &srcImage, Point center, double theta, double scale, Size dstSize, Mat<uchar> &dstImage)
	{
		// ������飬�ڴ����
		dstImage.Create(dstSize, srcImage.numberOfChannels);
		dstImage.SetTo(Scalar(0, 0, 0));

		// ԭͼ�����ת����
		int x0 = center.x;
		int y0 = center.y;
		theta = DEGREE2RADIAN(theta);

		// dx,dy����dstImage���ĵ���srcImage����ת���ĵľ��룬Ҳ����ƽ�Ƶľ��� 
		int dx = dstImage.cols / 2 - x0; 
		int dy = dstImage.rows / 2 - y0;
		int numberOfChannels = srcImage.numberOfChannels;

		int widthOfDst = dstImage.cols;
		int heightOfDst = dstImage.rows;

		Mat<uchar> extendedImage;
		CopyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1); // ʹ��0���߽�

		////////////////////////////////// �Ż�����/////////////////////////////
		// ��ѭ���ڵĲ�������ȡ����
		double sinTheta = sin(theta);
		double cosTheta = cos(theta);
		scale = 1.0 / scale;

		// �ı���ѭ���ڲ������ķ�ʽ
		double temp1 = (0 - y0 - dy)*sinTheta;
		double temp2 = (0 - y0 - dy)*cosTheta;
		double dtemp1 = sinTheta;
		double dtemp2 = cosTheta;

		for (int y = 0; y <= heightOfDst - 1; ++y, temp1 += dtemp1, temp2 += dtemp2)
		{
			// �ı���ѭ���ڲ������ķ�ʽ
			double temp3 = ((0 - x0 - dx)*cosTheta + temp1)*scale + x0;
			double temp4 = (-(0 - x0 - dx)*sinTheta + temp2)*scale + y0;
			double dtemp3 = (cosTheta)*scale;
			double dtemp4 = (-sinTheta)*scale;
			for (int x = 0; x <= widthOfDst - 1; ++x, temp3 += dtemp3, temp4 += dtemp4)
			{
				// ����ԭͼ����
				double srcX = temp3;
				double srcY = temp4;

				// ��1,�õ���extendedImage�е�����
				srcX++;
				srcY++;

				// get the nearest coordinate of src
				int x1 = (int)(srcX);
				int y1 = (int)(srcY);

				// ����ת��Ϊ����
				// ���ֱ��ʹ��float dx1= srcX - x1;��(1 - dx1)*(1 - dy1)��Ϊ����˷���������ĳ˷�����
				int dx1 = (srcX - x1)*(1 << ROTATE_SHIFT);
				int dy1 = (srcY - y1)*(1 << ROTATE_SHIFT);

				if (numberOfChannels == 1)
				{
					// !����ע������ķ�Χ����extendedImage�У�ԭͼ�ķ�Χ����1~cols - 2��
					if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
					{
						//˫���Բ�ֵ
						//��Χ4����
						//a�������������
						//a   b
						//  p
						//c   d
						uchar a = extendedImage.At<uchar>(y1,x1);
						uchar b = extendedImage.At<uchar>(y1,x1+1);
						uchar c = extendedImage.At<uchar>(y1+1,x1);
						uchar d = extendedImage.At<uchar>(y1+1,x1+1);

						int p = a*((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b*dx1*((1 << ROTATE_SHIFT) - dy1) + c*((1 << ROTATE_SHIFT) - dx1)*dy1 + d*dx1*dy1;
						p = DESCALE(p, 2 * ROTATE_SHIFT);

						dstImage.At<uchar>(y, x) = p;
					}
					else
					{
						// Խ�縳ֵ0
						dstImage.At<uchar>(y,x) = 0;
					}
				}
				else
				{
					if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
					{
						//˫���Բ�ֵ
						//��Χ4����
						//a�������������
						//a   b
						//  p
						//c   d
						Vec3b a = extendedImage.At<Vec3b>(y1,x1);
						Vec3b b = extendedImage.At<Vec3b>(y1,x1+1);
						Vec3b c = extendedImage.At<Vec3b>(y1+1,x1);
						Vec3b d = extendedImage.At<Vec3b>(y1+1,x1+1);

						int p1 = a[0] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[0] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[0] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[0] * dx1*dy1;
						p1 = DESCALE(p1, 2 * ROTATE_SHIFT);
						int p2 = a[1] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[1] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[1] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[1] * dx1*dy1;
						p2 = DESCALE(p2, 2 * ROTATE_SHIFT);
						int p3 = a[2] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[2] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[2] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[2] * dx1*dy1;
						p3 = DESCALE(p3, 2 * ROTATE_SHIFT);


						dstImage.At<Vec3b>(y, x) = Vec3b(p1, p2, p3);
					}
					else
					{
						dstImage.At<Vec3b>(y, x) = Vec3b(0, 0, 0);
					}

				}
			}
		}
	} // Rotate

	void Rotate90(const Mat<uchar> &srcImage, Mat<uchar> &dstImage)
	{
		// ������飬�ڴ����
		dstImage.Create(srcImage.cols,srcImage.rows, srcImage.numberOfChannels);
		dstImage.SetTo(Scalar(0, 0, 0));

		// ��ת���ģ�����ȡԭͼ���ģ�Ҳ��������Ϊ������
		int x0 = srcImage.cols / 2;
		int y0 = srcImage.rows / 2;

		Mat<uchar> extendedImage;
		CopyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1); // ʹ��0���߽�

		// dx,dy����dst��srcͼ�����ĵľ��� 
		int dx = dstImage.cols / 2 - srcImage.cols / 2;
		int dy = dstImage.rows / 2 - srcImage.rows / 2;
		int numberOfChannels = srcImage.numberOfChannels;

		int widthOfDst = dstImage.cols;
		int heightOfDst = dstImage.rows;

		////////////////////////////////// �Ż�����/////////////////////////////
		// �ı���ѭ���ڲ������ķ�ʽ
		double temp1 = 0 - y0 - dy;

		for (int y = 0; y <= heightOfDst - 1; ++y, ++temp1)
		{
			// �ı���ѭ���ڲ������ķ�ʽ
			double temp3 = temp1 + x0;
			double temp4 = x0 + dx + y0;
			for (int x = 0; x <= widthOfDst - 1; ++x, --temp4)
			{
				// ����ԭͼ����
				double srcX = temp3;
				double srcY = temp4;

				// ��1,�õ���extendedImage�е�����
				int x1 = ++srcX;
				int y1 = ++srcY;


				//// get the nearest coordinate of src
				//int x1 = (int)(srcX);
				//int y1 = (int)(srcY);

				//// ����ת��Ϊ����
				//int dx1 = (srcX - x1)*(1 << ROTATE_SHIFT);
				//int dy1 = (srcY - y1)*(1 << ROTATE_SHIFT);

				if (numberOfChannels == 1)
				{
					// !����ע������ķ�Χ����extendedImage�У�ԭͼ�ķ�Χ����1~cols - 2��
					if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
					{
						//˫���Բ�ֵ
						//��Χ4����
						//a�������������
						//a   b
						//  p
						//c   d
						/*uchar a = extendedImage.At<uchar>(y1, x1);
						uchar b = extendedImage.At<uchar>(y1, x1 + 1);
						uchar c = extendedImage.At<uchar>(y1 + 1, x1);
						uchar d = extendedImage.At<uchar>(y1 + 1, x1 + 1);

						int p = a*((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b*dx1*((1 << ROTATE_SHIFT) - dy1) + c*((1 << ROTATE_SHIFT) - dx1)*dy1 + d*dx1*dy1;
						p = DESCALE(p, 2 * ROTATE_SHIFT);*/

						dstImage.At<uchar>(y, x) = extendedImage.At<uchar>(y1, x1);
					}
					else
					{
						// Խ�縳ֵ0
						dstImage.At<uchar>(y, x) = 0;
					}
				}
				else
				{
					if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
					{
						////˫���Բ�ֵ
						////��Χ4����
						////a�������������
						////a   b
						////  p
						////c   d
						//Vec3b a = extendedImage.At<Vec3b>(y1, x1);
						//Vec3b b = extendedImage.At<Vec3b>(y1, x1 + 1);
						//Vec3b c = extendedImage.At<Vec3b>(y1 + 1, x1);
						//Vec3b d = extendedImage.At<Vec3b>(y1 + 1, x1 + 1);

						//int p1 = a[0] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[0] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[0] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[0] * dx1*dy1;
						//p1 = DESCALE(p1, 2 * ROTATE_SHIFT);
						//int p2 = a[1] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[1] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[1] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[1] * dx1*dy1;
						//p2 = DESCALE(p2, 2 * ROTATE_SHIFT);
						//int p3 = a[2] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[2] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[2] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[2] * dx1*dy1;
						//p3 = DESCALE(p3, 2 * ROTATE_SHIFT);


						dstImage.At<Vec3b>(y, x) = extendedImage.At<Vec3b>(y1, x1);
					}
					else
					{
						dstImage.At<Vec3b>(y, x) = Vec3b(0, 0, 0);
					}

				}
			}
		}
	}// Roate90

	void Rotate270(const Mat<uchar> &srcImage, Mat<uchar> &dstImage)
	{
		// ������飬�ڴ����
		dstImage.Create(srcImage.cols,srcImage.rows, srcImage.numberOfChannels);
		dstImage.SetTo(Scalar(0, 0, 0));

		// ��ת���ģ�����ȡԭͼ���ģ�Ҳ��������Ϊ������
		int x0 = srcImage.cols / 2;
		int y0 = srcImage.rows / 2;
		//theta = DEGREE2RADIAN(theta);

		Mat<uchar> extendedImage;
		CopyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1); // ʹ��0���߽�

		// dx,dy����dst��srcͼ�����ĵľ��� 
		int dx = dstImage.cols / 2 - srcImage.cols / 2;
		int dy = dstImage.rows / 2 - srcImage.rows / 2;
		int numberOfChannels = srcImage.numberOfChannels;

		int widthOfDst = dstImage.cols;
		int heightOfDst = dstImage.rows;

		////////////////////////////////// �Ż�����/////////////////////////////
		// �ı���ѭ���ڲ������ķ�ʽ
		double temp1 = y0 + dy;

		for (int y = 0; y <= heightOfDst - 1; ++y, --temp1)
		{
			// �ı���ѭ���ڲ������ķ�ʽ
			double temp3 = temp1 + x0;
			double temp4 = 0 - x0 - dx + y0;
			for (int x = 0; x <= widthOfDst - 1; ++x, ++temp4)
			{
				// ����ԭͼ����
				double srcX = temp3;
				double srcY = temp4;

				// ��1,�õ���extendedImage�е�����
				int x1 = ++srcX;
				int y1 = ++srcY;

				//// get the nearest coordinate of src
				//int x1 = (int)(srcX);
				//int y1 = (int)(srcY);

				//// ����ת��Ϊ����
				//int dx1 = (srcX - x1)*(1 << ROTATE_SHIFT);
				//int dy1 = (srcY - y1)*(1 << ROTATE_SHIFT);

				if (numberOfChannels == 1)
				{
					// !����ע������ķ�Χ����extendedImage�У�ԭͼ�ķ�Χ����1~cols - 2��
					if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
					{
						//˫���Բ�ֵ
						//��Χ4����
						//a�������������
						//a   b
						//  p
						//c   d
						/*uchar a = extendedImage.At<uchar>(y1, x1);
						uchar b = extendedImage.At<uchar>(y1, x1 + 1);
						uchar c = extendedImage.At<uchar>(y1 + 1, x1);
						uchar d = extendedImage.At<uchar>(y1 + 1, x1 + 1);

						int p = a*((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b*dx1*((1 << ROTATE_SHIFT) - dy1) + c*((1 << ROTATE_SHIFT) - dx1)*dy1 + d*dx1*dy1;
						p = DESCALE(p, 2 * ROTATE_SHIFT);*/

						dstImage.At<uchar>(y, x) = extendedImage.At<uchar>(y1, x1);
					}
					else
					{
						// Խ�縳ֵ0
						dstImage.At<uchar>(y, x) = 0;
					}
				}
				else
				{
					if ((x1 >= 1 && x1 <= extendedImage.cols - 2) && (y1 >= 1 && y1 <= extendedImage.rows - 2))
					{
						////˫���Բ�ֵ
						////��Χ4����
						////a�������������
						////a   b
						////  p
						////c   d
						//Vec3b a = extendedImage.At<Vec3b>(y1, x1);
						//Vec3b b = extendedImage.At<Vec3b>(y1, x1 + 1);
						//Vec3b c = extendedImage.At<Vec3b>(y1 + 1, x1);
						//Vec3b d = extendedImage.At<Vec3b>(y1 + 1, x1 + 1);

						//int p1 = a[0] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[0] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[0] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[0] * dx1*dy1;
						//p1 = DESCALE(p1, 2 * ROTATE_SHIFT);
						//int p2 = a[1] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[1] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[1] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[1] * dx1*dy1;
						//p2 = DESCALE(p2, 2 * ROTATE_SHIFT);
						//int p3 = a[2] * ((1 << ROTATE_SHIFT) - dx1)*((1 << ROTATE_SHIFT) - dy1) + b[2] * dx1*((1 << ROTATE_SHIFT) - dy1) + c[2] * ((1 << ROTATE_SHIFT) - dx1)*dy1 + d[2] * dx1*dy1;
						//p3 = DESCALE(p3, 2 * ROTATE_SHIFT);

						dstImage.At<Vec3b>(y, x) = extendedImage.At<Vec3b>(y1, x1);
					}
					else
					{
						dstImage.At<Vec3b>(y, x) = Vec3b(0, 0, 0);
					}

				}
			}
		}
	} // Rotate270

	void VerticalFlip(const Mat<uchar> &srcImage, Mat<uchar> &dstImage)
	{
		dstImage.Create(srcImage.rows,srcImage.cols, srcImage.numberOfChannels);

		// ��ֱ��ת
		int widthStep = srcImage.cols*srcImage.numberOfChannels;
		int width = srcImage.cols;
		int height = srcImage.rows;
		uchar *dataOfSrc = srcImage.data;
		uchar *dataOfDst = dstImage.data + widthStep*(dstImage.rows - 1);
		for (int y = 0; y <= height - 1; ++y, dataOfSrc += widthStep, dataOfDst -= widthStep)
		{
			// ����һ��
			memcpy(dataOfDst, dataOfSrc, widthStep);
		}
	}// VerticalFlip

	void HorizontalFlip(const Mat<uchar> &srcImage, Mat<uchar> &dstImage)
	{
		// �����ڴ�
		dstImage.Create(srcImage.rows, srcImage.cols, srcImage.numberOfChannels);

		// ÿһ�н������ұ�(��ÿһ��ͨ����������)
		int width = srcImage.cols;
		int channelCount = srcImage.numberOfChannels;
		int widthStep = width*channelCount;
		int *table = new int[widthStep];
		for (int i = 0; i <= width - 1; ++i)
		{
			int index1 = i*channelCount;
			int index2 = (width - 1 - i)*channelCount;
			for (int j = 0; j <= channelCount - 1; ++j)
				table[index1 + j] = index2 + j;
		}


		uchar *dataOfSrc = srcImage.data;
		uchar *dataOfDst = dstImage.data;
		int limit = ((width + 1) >> 1)*channelCount; // ע����������ȼ�,Ϊʲô((width - 1) >>1)*channelCount ���ԣ���ʾ:��Ϊ��ͨ����ԭ�򣬶԰�ʹ��(x+1)/2
		for (int y = 0; y <= srcImage.rows - 1; ++y, dataOfSrc += widthStep, dataOfDst += widthStep)
		{
			for (int x = 0; x <= limit; ++x)
			{
				int index = table[x];
				dataOfDst[index] = dataOfSrc[x];
				dataOfDst[x] = dataOfSrc[index];
			}
		}

		delete[] table;

	}

}//namespace QQ