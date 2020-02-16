#define DLLAPI_EXPORTS

#include"Edge.h"
#include<queue>
#include"Utility.h"

namespace QQ
{

	void Sobel(const Mat<uchar> &srcImage, Mat<uchar> &dstImage)
	{
		// Ŀ��ͼ��
		int widthOfDst = srcImage.cols;
		int heightOfDst = srcImage.rows;
		dstImage.Create(heightOfDst, widthOfDst, 1);

		// ����ԭͼ
		Mat<uchar> extendedImage;
		CopyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1);

		uchar *rowOfExtendedImage = extendedImage.data + extendedImage.cols + 1;
		uchar *rowOfDstImage = dstImage.data;
		for (int y = 0; y <= heightOfDst - 1; ++y,rowOfExtendedImage+=extendedImage.cols,rowOfDstImage+=dstImage.cols)
		{
			uchar *colOfExtendedImage = rowOfExtendedImage;
			uchar *colOfDstImage = rowOfDstImage;
			for (int x = 0; x <= widthOfDst - 1; ++x,++colOfExtendedImage,++colOfDstImage)
			{
				int fx = (colOfExtendedImage[0 + 1 - extendedImage.cols] + 2 * colOfExtendedImage[0 + 1] + colOfExtendedImage[0 + 1 + extendedImage.cols]) -
					(colOfExtendedImage[0 - 1 - extendedImage.cols] + 2 * colOfExtendedImage[0 - 1] + colOfExtendedImage[0 - 1 + extendedImage.cols]);

				int fy = (colOfExtendedImage[0 + extendedImage.cols - 1] + 2 * colOfExtendedImage[0 + extendedImage.cols] + colOfExtendedImage[0 + extendedImage.cols + 1]) -
					(colOfExtendedImage[0 - extendedImage.cols - 1] + 2 * colOfExtendedImage[0 - extendedImage.cols] + colOfExtendedImage[0 - extendedImage.cols + 1]);

				colOfDstImage[0] = MIN(255, abs(fx) + abs(fy));
			}

		}
	}

	void Canny(const Mat<uchar> &srcImage, Mat<uchar> &dstImage, double lowThreshold, double highThreshold, bool L2)
	{
		dstImage.Create(srcImage.rows,srcImage.cols, 1);

		// L2���������Եǿ�ȵ�ʱ�򣬾������ƽ���ķ�ʽ��������ֵҲ��Ҫ����ƽ��
		if (L2)
		{
			lowThreshold = MIN(32767.0, lowThreshold);
			highThreshold = MIN(32767.0, highThreshold);

			if (lowThreshold > 0) lowThreshold *= lowThreshold;
			if (highThreshold > 0) highThreshold *= highThreshold;
		}

		// ����fx,fy,ǿ��ͼ
		Mat<int> fx(srcImage.rows, srcImage.cols, 1);
		Mat<int> fy(srcImage.rows, srcImage.cols, 1);
		Mat<uchar> enlargedImage;
		Mat<int> magnitudeImage(srcImage.rows + 2, srcImage.cols + 2, 1);
		magnitudeImage.SetTo(Scalar(0));
		CopyMakeBorder(srcImage, enlargedImage, 1, 1, 1, 1);
		int stepOfEnlargedImage = enlargedImage.cols;
		int stepOffx = fx.cols;
		int height = srcImage.rows;
		int width = srcImage.cols;
		uchar *rowOfEnlargedImage = enlargedImage.data + stepOfEnlargedImage + 1;
		int *rowOffx = (int *)fx.data;
		int *rowOffy = (int *)fy.data;
		int *rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		for (int y = 0; y <= height - 1; ++y, rowOfEnlargedImage += stepOfEnlargedImage, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx)
		{
			uchar *colOfEnlargedImage = rowOfEnlargedImage;
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			for (int x = 0; x <= width - 1; ++x, ++colOfEnlargedImage, ++colOffx, ++colOffy, ++colOfMagnitudeImage)
			{
				// fx
				colOffx[0] = colOfEnlargedImage[1 - stepOfEnlargedImage] + 2 * colOfEnlargedImage[1] + colOfEnlargedImage[1 + stepOfEnlargedImage] -
					colOfEnlargedImage[-1 - stepOfEnlargedImage] - 2 * colOfEnlargedImage[-1] - colOfEnlargedImage[-1 + stepOfEnlargedImage];

				// fy
				colOffy[0] = colOfEnlargedImage[stepOfEnlargedImage - 1] + 2 * colOfEnlargedImage[stepOfEnlargedImage] + colOfEnlargedImage[stepOfEnlargedImage + 1] -
					colOfEnlargedImage[-stepOfEnlargedImage - 1] - 2 * colOfEnlargedImage[-stepOfEnlargedImage] - colOfEnlargedImage[-stepOfEnlargedImage + 1];

				// �����Եǿ�ȣ�����ֻ�����ڱȽϣ�Ϊ�˼ӿ��ٶȣ�ֻ����ƽ����
				if (L2)
				{
					colOfMagnitudeImage[0] = colOffx[0] * colOffx[0] + colOffy[0] * colOffy[0];
				}
				else
				{
					colOfMagnitudeImage[0] = abs(colOffx[0]) + abs(colOffy[0]);
				}

			}
		}


		// �Ǽ������ƣ�ͬʱ���ͼ�����,˫��ֵ����
		//   0 - �����Ǳ�Ե
		//   1 - ���Ǳ�Ե
		//   2 - һ���Ǳ�Ե
		Mat<uchar> labelImage(srcImage.rows + 2, srcImage.cols + 2, 1);
		memset(labelImage.data, 1, labelImage.rows*labelImage.cols);
		rowOffx = (int *)fx.data;
		rowOffy = (int *)fy.data;
		rowOfMagnitudeImage = (int *)magnitudeImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		queue<uchar*> queueOfEdgePixel;
		for (int y = 0; y <= height - 1; ++y, rowOfMagnitudeImage += stepOfEnlargedImage, rowOffx += stepOffx, rowOffy += stepOffx, rowOfLabelImage += stepOfEnlargedImage)
		{
			int *colOffx = rowOffx;
			int *colOffy = rowOffy;
			int *colOfMagnitudeImage = rowOfMagnitudeImage;
			uchar *colOfLabelImage = rowOfLabelImage;
			for (int x = 0; x <= width - 1; ++x, ++colOffx, ++colOffy, ++colOfMagnitudeImage, ++colOfLabelImage)
			{
				int fx = colOffx[0];
				int fy = colOffy[0];

				// ȡ����ֵ��ת��Ϊ����
#if (defined WIN32 || defined _WIN32)
				int abs_fx = std::abs(fx);
				int abs_fy = std::abs(fy);
#else  // Linux��C������׼���в���
				int abs_fx = abs(fx);
				int abs_fy = abs(fy);
#endif
				

				// �����ص���п����Ǳ�Ե��
				if (colOfMagnitudeImage[0] > lowThreshold)
				{
					// �Ǽ�������
					abs_fy = abs_fy << CANNY_SHIFT; // ������ֱ����λ

					// �ڳ��Խ�С�ĸ�������ʱ����Ҫ������������ת��
					int tan225 = abs_fx * TAN_225;
					int tan675 = abs_fx * TAN_675;

					// �ݶȷ���0��
					if (abs_fy< tan225)
					{
						// ����ֵ���п����Ǳ�Ե
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[-1] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[1])
						{
							// ���ڸ���ֵ���Ǳ�Ե�����Ϊ2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// ������У������ñ��
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// �п����Ǳ�Ե�����Ϊ0
								colOfLabelImage[0] = 0;

							}
						}
					}

					// �ݶȷ���90��
					if (abs_fy>tan675)
					{
						// ����ֵ���п����Ǳ�Ե
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage])
						{
							// ���ڸ���ֵ���Ǳ�Ե�����Ϊ2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// ������У������ñ��
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// �п����Ǳ�Ե�����Ϊ0
								colOfLabelImage[0] = 0;

							}

						}

					}

					// �ݶȷ���45��/-45��
					if (abs_fy > tan225&&abs_fy<tan675)
					{
						int s = (fy*fx) < 0 ? -1 : 1;
						// ����ֵ���п����Ǳ�Ե
						if (colOfMagnitudeImage[0] >= colOfMagnitudeImage[-stepOfEnlargedImage - s] && colOfMagnitudeImage[0] >= colOfMagnitudeImage[stepOfEnlargedImage + s])
						{
							// ���ڸ���ֵ���Ǳ�Ե�����Ϊ2
							if (colOfMagnitudeImage[0] > highThreshold)
							{
								// ������У������ñ��
								colOfLabelImage[0] = 2;
								queueOfEdgePixel.push(colOfLabelImage);
							}
							else
							{
								// �п����Ǳ�Ե�����Ϊ0
								colOfLabelImage[0] = 0;

							}

						}

					}

				}
			}

		}

		// �����Է���,������ö���ʵ�֣�Ҳ���Բ���ջʵ��
		// ע�⣬������õݹ����
		// ���A����Ϊ0����2��8�������ӣ���ô���Ϊ2�������ʱ������ֵΪ0��������A���ڣ��򱻱��Ϊ2,
		while (!queueOfEdgePixel.empty())
		{
			uchar *m = queueOfEdgePixel.front();
			queueOfEdgePixel.pop();

			// ��8��������
			if (!m[-1])
			{
				m[-1] = 2;
				queueOfEdgePixel.push(m - 1);
			}

			if (!m[1])
			{
				m[1] = 2;
				queueOfEdgePixel.push(m + 1);
			}
			if (!m[-stepOfEnlargedImage - 1])
			{
				m[-stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage - 1);
			}
			if (!m[-stepOfEnlargedImage])
			{
				m[-stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage);
			}
			if (!m[-stepOfEnlargedImage + 1])
			{
				m[-stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m - stepOfEnlargedImage + 1);
			}
			if (!m[stepOfEnlargedImage - 1])
			{
				m[stepOfEnlargedImage - 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage - 1);
			}
			if (!m[stepOfEnlargedImage])
			{
				m[stepOfEnlargedImage] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage);
			}
			if (!m[stepOfEnlargedImage + 1])
			{
				m[stepOfEnlargedImage + 1] = 2;
				queueOfEdgePixel.push(m + stepOfEnlargedImage + 1);
			}
		}

		// ������ɱ�Եͼ
		rowOfLabelImage = labelImage.data + stepOfEnlargedImage + 1;
		uchar *rowOfDst = dstImage.data;
		for (int y = 0; y <= height - 1; ++y, rowOfLabelImage += stepOfEnlargedImage, rowOfDst += stepOffx)
		{
			uchar *colOfLabelImage = rowOfLabelImage;
			uchar *colOfDst = rowOfDst;
			for (int x = 0; x <= width - 1; ++x, ++colOfDst, ++colOfLabelImage)
			{
				if (colOfLabelImage[0] == 2)
					colOfDst[0] = 255;
				else
				{
					colOfDst[0] = 0;
				}
			}
		}

	}

}// namespace QQ