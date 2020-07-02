#include"ImageProcessTest.h"
#include<stdio.h>
#include<time.h>
#include"Bmp.h"
#include"Edge.h"
#include"Filter.h"
#include"Mat.h"
#include"GeometryTransformation.h"
#include"GrayTransformation.h"
#include"Histogram.h"
#include"Utility.h"

using namespace QQ;
using namespace std;

// ͼ��·��
#define LENA_GRAY		"../../../../Resource/Image/Gray/Lena512.bmp"
#define LENA_COLOR		"../../../../Resource/Image/Color/Lena800.bmp"
#define BEAUTY_COLOR	"../../../../Resource/Image/Color/Beauty.bmp"
#define BEAUTY_GRAY		"../../../../Resource/Image/Gray/Beauty.bmp"
#define CAMERA_GRAY		"../../../../Resource/Image/Gray/Camera256.bmp"
#define CAMERA_COLOR	"../../../../Resource/Image/Color/Camera256.bmp"

#define NUM_LOOP 100

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
    // read image
	Mat<uchar> srcImage;
	ReadBmp(BEAUTY_COLOR, srcImage);

    // rotate
	Mat<uchar> dstImage;
	double time1=clock();
    Rotate(srcImage, Point(srcImage.cols/2,srcImage.rows/2), 90, 1,Size(srcImage.rows, srcImage.cols), dstImage); // 16
	//Rotate270(srcImage, dstImage); // 7ms
	//VerticalFlip(srcImage, dstImage);
	double time2=clock();
	printf("rotate time:%f\n", (time2 - time1));

    // save result
	WriteBmp("Result.bmp", dstImage);
}
void ImageProcessTest::TestResize()
{

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

//�������ظ�ֵ������
static void TestAssignmentOperator(Mat<uchar> &srcImage)
{
    //���������ص�ʱ��srcImage���ü���Ҳ���0�ˣ����ʱ�򣬾�Ҫ�ͷſռ���
    //��ֵ
    Mat<uchar> dstImage,dstImage2;
    dstImage2=dstImage=srcImage;

}
void ImageProcessTest::TestEqualizeHistogram()//����ֱ��ͼ���⻯
{
    // read
	Mat<uchar> srcImage;
    ReadBmp(BEAUTY_GRAY,srcImage);

	Mat<uchar> dstImage;
	EqualizeHistogram(srcImage,dstImage);

    // save result
    WriteBmp("Result.bmp",dstImage);
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

void ImageProcessTest::TestFilter()
{
	Mat<uchar> srcImage;
	ReadBmp(LENA_GRAY,srcImage);

	// ��ֵ�˲�
	/*Mat<float> kernel(5, 5, 1);
	int kernelSize = kernel.cols;
	for (int y = 0; y <= kernelSize-1; ++y)
	{
		for (int x = 0; x <= kernelSize - 1; ++x)
			kernel.At<float>(y, x) = 1.0/(kernelSize*kernelSize);
	}*/

	// ��Ե���
	Mat<float> kernel(3, 3, 1);
	int kernelSize = kernel.cols;
	kernel.At<float>(0, 0) = -1;
	kernel.At<float>(0, 1) = -2;
	kernel.At<float>(0, 2) = -1;
	kernel.At<float>(1, 0) = 0;
	kernel.At<float>(1, 1) = 0;
	kernel.At<float>(1, 2) = 0;
	kernel.At<float>(2, 0) = 1;
	kernel.At<float>(2, 1) = 2;
	kernel.At<float>(2, 2) = 1;
	
	Mat<uchar> dstImage;
	clock_t t1;
	clock_t t2;
	double sum=0;
	for (int i = 0; i < NUM_LOOP; ++i)
	{
		t1 = clock();

		//Blur(srcImage, dstImage, Size(13, 13));// 3*3��2ms��5*5��2ms�������������˴�С�����仯
		Convolution(srcImage, kernel, dstImage); // 3*3:7ms,5*5:51ms
		//MedianBlur(srcImage, dstImage, 5); // 3*3:9ms,5*5:12ms
		//GaussianBlur(srcImage, dstImage, 0.84089642);
		t2 = clock();
		sum += (t2 - t1);
	}
	printf("time:%f ms\n", sum / NUM_LOOP);
	WriteBmp("Result.bmp", dstImage);
}

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
    // read image
	Mat<uchar> srcImage;
	ReadBmp(LENA_GRAY, srcImage);

    // canny
    Mat<uchar> dstImage;
    Canny(srcImage, dstImage, 50, 150);

    // save result
    WriteBmp("Result.bmp", dstImage);

}

