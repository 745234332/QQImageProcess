#include"ImageProcessTest.h"
#include"Bmp.h"
#include"Edge.h"
#include"Filter.h"
#include"GeometryTransformation.h"
#include"GrayTransformation.h"
#include"Histogram.h"
#include<time.h>
using namespace QQ;
using namespace std;

void ImageProcessTest::TestCreateImage()//���Դ���ͼ��
{
	Mat<uchar> srcImage(512,512,3,QQ::Scalar(0,0,255));//����һ����ͼ��
	
	////����ͼ��ֵ
	srcImage.SetTo(Scalar(255,0,0));
	//srcImage.SetTo(Scalar(255, 255, 255, 0));

	WriteBmp("D:/1.bmp", srcImage);

}

void ImageProcessTest::TestReadAndWrite()
{
//	//��ȡ
//	Mat<uchar> srcImage;//����һ����ͼ��
//	ReadBmp(LENA_COLOR, srcImage);
//	
//	//// ����100������Ϊ0
//	for (int i = 0; i <= srcImage.cols - 1; ++i)
//	{
//		srcImage.At<Vec3b>(100, i) = Vec3b(255,255,255);
//	}
//
//	// ����߽�
//	/*Image dstImage;
//	CopyMakeBorder(srcImage, dstImage, 2, 2, 2, 2);
//*/
//	//����
//	WriteBmp("D:/1.bmp", srcImage);


	//��ȡ
	Mat<uchar> srcImage;//����һ����ͼ��
    ReadBmp(LENA_GRAY, srcImage);

	//// ����100������Ϊ0
	for (int i = 0; i <= srcImage.cols - 1; ++i)
	{
		srcImage.At<uchar>(100, i) = 255;
	}

	// ����߽�
	/*Image dstImage;
	CopyMakeBorder(srcImage, dstImage, 2, 2, 2, 2);
	*/
	//����
	WriteBmp("Result.bmp", srcImage);

	
}
void ImageProcessTest::TestRotate()//������ת
{
	Mat<uchar> srcImage;
	ReadBmp(BEAUTY_COLOR, srcImage);

	Mat<uchar> dstImage;
	double time1=clock();
	Rotate(srcImage, Point(0,0), 30, 1,Size(srcImage.rows, srcImage.cols), dstImage); // 16
	//Rotate270(srcImage, dstImage); // 7ms
	//VerticalFlip(srcImage, dstImage);
	double time2=clock();
	printf("rotate time:%f\n", (time2 - time1));
	WriteBmp("Result.bmp", dstImage);
}
void ImageProcessTest::TestResize()
{
	////ʹ��OpenCV����Resizeʱ��
	//clock_t time1,time2;

	//Image srcImage;
	//ReadBmp((string(COLOR)+"Road.bmp").c_str(),srcImage);

}

void ImageProcessTest::TestCvtColor()//����CvtColor
{
	
	Mat<uchar> srcImage;
	ReadBmp(LENA_COLOR, srcImage);
	double sum=0;
	Mat<uchar> dstImage;
	for (int i=1;i<=16;++i)
	{
		clock_t time1=clock();
		RGB2Gray(srcImage,dstImage);
		clock_t time2=clock();
		sum+=(time2-time1);

	}
	printf("CvtColor:%f ms\n",sum/16);
	WriteBmp("D:/1.bmp",dstImage);


}
void ImageProcessTest::TestAssignmentOperator(Mat<uchar> &srcImage)//�������ظ�ֵ������
{
	//���������ص�ʱ��srcImage���ü���Ҳ���0�ˣ����ʱ�򣬾�Ҫ�ͷſռ���
	//��ֵ
	Mat<uchar> dstImage,dstImage2;
	dstImage2=dstImage=srcImage;

}
void ImageProcessTest::TestEqualizeHistogram()//����ֱ��ͼ���⻯
{
	//����ֱ��ͼ
	Mat<uchar> srcImage;
	ReadBmp("D:/Image/Gray/White.bmp",srcImage);

	//����ֱ��ͼ
	Mat<int> histogram;
	CalculateHistogram(srcImage, histogram);
	
	//���ֱ��ͼ
	int *dataOfHistogram =(int *)histogram.data;
	for (int i=0;i<=255;++i)
	{
		printf("%d\n", dataOfHistogram[i]);
	}

	////ֱ��ͼ���⻯C++��
	Mat<uchar> dstImage;
	EqualizeHistogram(srcImage,dstImage);
	WriteBmp("D:/Equalize.bmp",dstImage);
}

void ImageProcessTest::TestMemoryLeak()//�����Ƿ����ڴ�й¶
{
	while (true)
	{
		Mat<uchar> srcImage;
		ReadBmp(LENA_COLOR, srcImage);
		printf("%d\n", srcImage.cols);
	}
	
}

void ImageProcessTest::TestSmooth()//�����˲�
{
	//���Ծ�ֵ�˲�
	Mat<uchar> srcImage;
	ReadBmp(LENA_GRAY,srcImage);

	Mat<float> kernel(5, 5, 1);
	int kernelSize = kernel.cols;

	// ��ֵ�˲���ʱ��������ڹ��󣬻���־����½������ʱ�����Ҫһ�������ݴ�����
	for (int y = 0; y <= kernelSize-1; ++y)
	{
		for (int x = 0; x <= kernelSize - 1; ++x)
			kernel.At<float>(y, x) = 1.0/(kernelSize*kernelSize);
	}
	
	Mat<uchar> dstImage;
	clock_t t1;
	clock_t t2;
	int sum=0;
	for (int i=1;i<=16;i++)
	{
		t1=clock();

		// �����˲�������ʹ��Blur
		//Blur(srcImage, dstImage, Size(13, 13));// 3*3��2ms��5*5��2ms�������������˴�С�����仯
		//Convolution(srcImage, kernel, dstImage); // 3*3:7ms,5*5:51ms
		//MedianBlur(srcImage, dstImage, 5); // 3*3:9ms,5*5:12ms
		GaussianBlur(srcImage, dstImage, 0.84089642);
		t2=clock();
		sum+=(t2-t1);
	}
	printf("My Blur time:%d ms\n",sum>>4);
	WriteBmp("D:/Blur.bmp", dstImage);

}

// 2016-5-30 15:07:08��by QQ
// �����ڴ�ķ�����ͷ�
void ImageProcessTest::TestMallocAndFree()
{
	//BYTE *data = (BYTE *)AlignedMalloc(100, 16);

	//// ���ڴ�д����
	//for (int i = 0; i <= 99; ++i)
	//{
	//	data[i] = 255;
	//}

	//// ��������100������
	//// ���ڴ�д����
	//for (int i = 0; i <= 99; ++i)
	//{
	//	printf("%d\n", data[i]);
	//}

	//AlignedFree(data);
	//getchar();



}

void ImageProcessTest::TestEdge()
{
	Mat<uchar> srcImage;
	ReadBmp(LENA_GRAY, srcImage);

	//Mat<uchar> dstImage;
	//Sobel(srcImage, dstImage);
	//Canny(srcImage, dstImage, 50, 150);
	//WriteBmp("D:/Edge.bmp", dstImage);

	Mat<int> dstImage;
	ComputeIntegralImage(srcImage, dstImage);
	printf("%d\n", dstImage.At<int>(dstImage.rows - 1, dstImage.cols - 1));

	

}

