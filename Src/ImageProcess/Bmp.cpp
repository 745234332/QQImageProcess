#define DLLAPI_EXPORTS
#include "Bmp.h"
#if (defined WIN32 || defined _WIN32)
#include "BmpDefinition_Windows.h" // ��Linux��ʹ�� BmpDefinition_Linux.h
#else
#include "BmpDefinition_Linux.h"
#endif
namespace QQ
{


void ReadBmp(const string &fileName, Mat<uchar> &image)
{
	//ͼ�����
	int width,height,bitCount_PerPixel;
	
	//��BMP
	uchar *data=ReadBmp_C(fileName.c_str(),width,height,bitCount_PerPixel);
	
	//����ͼ��ͷ
	int numberOfChannels = bitCount_PerPixel >> 3;
	Mat<uchar> temp(height, width, numberOfChannels, data, true);

	image = temp;

	delete[] data;

}

void WriteBmp(const string &fileName, const Mat<uchar> &image)
{
	//д��bmp
	int bitCount_PerPixel=image.numberOfChannels<<3;
	WriteBmp_C(image.data,image.cols,image.rows,fileName.c_str(),bitCount_PerPixel);
}
void WriteMarkedBMP(const string &fileName, const Mat<uchar> &image)
{
	WriteMarkedBmp_C(image.data, image.cols, image.rows, fileName.c_str());
}

///��ȡ8λ����24λBmp
///����ԭʼͼ�����ݣ�δ���������
uchar *ReadBmp_C(const char * fileName,
	int &width, int &height,//ͼ���С(���صĿ�Ⱥ͸߶�)
	int &bitCount_PerPixel//����ͼ���ÿ����λ��
	)
{
	FILE *fp;
	BITMAPFILEHEADER bitmap_FileHeader;
	BITMAPINFOHEADER bitmap_InfoHeader;
	RGBQUAD *colorTable;
	bool isSuccess = true;

	width = height = 0;
	if ((fp = fopen(fileName, "rb")) == NULL)
		return NULL;

	// �����ļ�ͷ����Ϣͷ�ڵ���Ϣ
	if (fread((void *)&bitmap_FileHeader, 1, sizeof(BITMAPFILEHEADER), fp) != sizeof(BITMAPFILEHEADER))
		isSuccess = false;
	if (fread((void *)&bitmap_InfoHeader, 1, sizeof(BITMAPINFOHEADER), fp) != sizeof(BITMAPINFOHEADER))
		isSuccess = false;
	if ((isSuccess == false) || (bitmap_FileHeader.bfOffBits<sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)))
	{
		fclose(fp);
		return NULL;
	}
	width = bitmap_InfoHeader.biWidth;
	height = bitmap_InfoHeader.biHeight;
	bitCount_PerPixel = bitmap_InfoHeader.biBitCount;//ÿ����λ��
	int ucharCount_PerPixel = bitCount_PerPixel / 8;
	int dataSize = bitmap_InfoHeader.biSizeImage;

	// ������ɫ��(8λ���У�24λ��û��)
	if (bitCount_PerPixel == 8)
	{
		//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
		colorTable = new RGBQUAD[256];
		int count = fread(colorTable, sizeof(RGBQUAD), 256, fp);
		if (count != 256)//��ȡ��ɫ�����
		{
			isSuccess = false;
			return NULL;
		}
		//�����ɫ����Ϣ�ṹ��
		/*FILE *outFile;
		fopen_s(&outFile, "D:output.txt", "w");
		for (int i = 0; i < 256; i++)
		{
		fprintf_s(outFile, "colorTable[%d]\n", i);
		fprintf_s(outFile, "\tB:%d\n", colorTable[i].rgbBlue);
		fprintf_s(outFile, "\tG:%d\n", colorTable[i].rgbGreen);
		fprintf_s(outFile, "\tR:%d\n", colorTable[i].rgbRed);
		}
		fclose(outFile);*/
	}


	// ��ȡͼ������	
	// ��ȡͼ�����ݣ�����BMPɨ�跽ʽ�Ǵ��µ��ϣ�����BMPͼ��洢��ʽ�Ǵ��µ��ϣ����Զ���ʱ����Ҫ���Ŷ�ȡ
	int ucharCount_PerLine = (width*ucharCount_PerPixel + 3) / 4 * 4;//�洢�У�ÿ���ֽ���(4�ֽڶ���)
	uchar *image_Src = new uchar[height*width*ucharCount_PerPixel];
	uchar temp;
	int extend = ucharCount_PerLine - width*ucharCount_PerPixel;//ÿ���������ֽڶ������Ҫ������ֽ���
	for (int i = 0; i<height; i++)
	{
		int readCount = fread(image_Src + (height - 1 - i)*width * ucharCount_PerPixel, sizeof(uchar), width* ucharCount_PerPixel, fp);
		if (readCount != width * ucharCount_PerPixel)
		{
			fclose(fp);
			delete[]image_Src;
			image_Src = NULL;
			return NULL;
		}
		for (int k = 0; k<extend; k++) // ���������
		{
			if (fread(&temp, sizeof(uchar), 1, fp) != 1)
			{
				fclose(fp);
				delete[] image_Src;
				image_Src = NULL;
				return NULL;

			}
		}
	}
	fclose(fp);
	return image_Src;
}



////д��ԭʼͼ�����ݣ�δ���������
///8λ����24λ
bool WriteBmp_C(uchar *imageData, //ԭʼͼ������
	int width, int height, //Bmpͼ���С
	const char *fileName,
	int bitCount_PerPixel//Bmpÿ�����ص�λ��
	)
{
	FILE * fp;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int colorTableSize = 0;
	int i;
	bool isSuccess = true;
	uchar p[4];//��ɫ��

	if ((fp = fopen(fileName, "wb")) == NULL)
	{
		return false;
	}

	// д�ļ�ͷfileHeader
	// �ļ�ͷ+��Ϣͷ+��ɫ��
	bitmapFileHeader.bfType = ((WORD)('M' << 8) | 'B');
	if (bitCount_PerPixel == 8)//8bit Bmp
	{
		colorTableSize = 1024;
	}
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+colorTableSize;
	int ucharCount_PerPixel = bitCount_PerPixel / 8;
	int ucharCount_PerLine = (width*ucharCount_PerPixel + 3) / 4 * 4;//4�ֽڶ���
	bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + ucharCount_PerLine*height;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	if (fwrite((void *)&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), fp) != sizeof(BITMAPFILEHEADER))
		isSuccess = false;


	// д��Ϣͷ
	bitmapInfoHeader.biSize = 40;
	bitmapInfoHeader.biWidth = width;
	bitmapInfoHeader.biHeight = height;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = bitCount_PerPixel;
	bitmapInfoHeader.biCompression = 0;
	bitmapInfoHeader.biSizeImage = height*ucharCount_PerLine;//ͼ�����ݲ��֣�4�ֽڶ���
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	if (fwrite((void *)&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), fp) != sizeof(BITMAPINFOHEADER))
		isSuccess = false;

	// д����ɫ��(8λ����ɫ��24λ��û����ɫ��)
	///8λBMPͼ�����ݲ��ִ洢������ɫ�������
	if (bitCount_PerPixel == 8)
	{
		for (i = 0, p[3] = 0; i<256; i++)//8λ��ɫ����256��4λΪ16
		{
			p[3] = 0;
			p[0] = i;//B
			p[1] = i;//G
			p[2] = i; //R
			if (fwrite((void *)p, 1, 4, fp) != 4)
			{
				isSuccess = false;
				break;
			}
		}
	}


	// д��ͼ������
	// ����BMPɨ�跽ʽ�Ǵ��µ��ϣ�����BMPͼ��洢��ʽ�Ǵ��µ��ϣ�����д��ʱ����Ҫ����д
	int extend = ucharCount_PerLine - width * ucharCount_PerPixel;
	int writeCount;
	uchar *temp;
	if (extend == 0)
	{
		for (temp = imageData + (height - 1) * ucharCount_PerPixel * width; temp >= imageData; temp -= ucharCount_PerPixel * width)
		{
			writeCount = fwrite((void *)temp, 1, width * ucharCount_PerPixel, fp);
			if (writeCount != (unsigned int)(ucharCount_PerPixel * width))
				isSuccess = false; // ��ʵ������
		}
	}
	else
	{
		for (temp = imageData + (height - 1) * ucharCount_PerPixel * width; temp >= imageData; temp -= ucharCount_PerPixel * width)
		{
			writeCount = fwrite((void *)temp, 1, width * ucharCount_PerPixel, fp);
			if (writeCount != (unsigned int)(ucharCount_PerPixel * width))
				isSuccess = false; // ��ʵ������
			for (i = 0; i<extend; i++) // ��������ݣ��������ݲ���
			{
				writeCount = fwrite((void *)(temp + ucharCount_PerPixel* (width - 1)), 1, 1, fp);
				if (writeCount != 1)
					isSuccess = false;
			}
		}
	}
	// Return;
	fclose(fp);
	return isSuccess;
}

//�Ҷ�ͼ������ɫ�����Ϣ
bool WriteMarkedBmp_C(uchar *image,
	int width, int height, //Bmpͼ���С
	const char * filename
	)
{

	FILE * fp;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int colorTableSize = 0;
	int i;
	bool isSuccess = true;
	uchar colorTableItem[4];//��ɫ����
	int bitCount_PerPixel = 8;//Bmpÿ�����ص�λ��

	if ((fp = fopen(filename, "w+b")) == NULL)
	{
		return false;
	}

	// д�ļ�ͷfileHeader
	// �ļ�ͷ+��Ϣͷ+��ɫ��
	bitmapFileHeader.bfType = ((WORD)('M' << 8) | 'B');
	if (bitCount_PerPixel == 8)//8bit Bmp
	{
		colorTableSize = 1024;
	}
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+colorTableSize;
	int ucharCount_PerPixel = bitCount_PerPixel / 8;
	int ucharCount_PerLine = (width*ucharCount_PerPixel + 3) / 4 * 4;//4�ֽڶ���
	bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + ucharCount_PerLine*height;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	if (fwrite((void *)&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), fp) != sizeof(BITMAPFILEHEADER))
		isSuccess = false;

	// д��Ϣͷ
	bitmapInfoHeader.biSize = 40;
	bitmapInfoHeader.biWidth = width;
	bitmapInfoHeader.biHeight = height;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = bitCount_PerPixel;
	bitmapInfoHeader.biCompression = 0;
	bitmapInfoHeader.biSizeImage = height*ucharCount_PerLine;//ͼ�����ݲ��֣�4�ֽڶ���
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	if (fwrite((void *)&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), fp) != sizeof(BITMAPINFOHEADER))
		isSuccess = false;

	// д����ɫ��(8λ����ɫ��24λ��û����ɫ��),ע��ͨ��˳��BGR
	///8λBMPͼ�����ݲ��ִ洢������ɫ�������
	if (bitCount_PerPixel == 8)
	{
		for (i = 0; i < 256; i++)
		{
			colorTableItem[3] = 0;
			switch (i) //�����Զ���������ɫR = 0��B = 255, G = 255:��ɫ
			{
			case 255://��ɫ
			{
						 colorTableItem[0] = 0;
						 colorTableItem[1] = 0;
						 colorTableItem[2] = 255;//R
						 break;
			}
			case 254://��ɫ
			{
						 colorTableItem[0] = 0;
						 colorTableItem[1] = 255;//G
						 colorTableItem[2] = 0;
						 break;
			}
			case 253://��ɫ
			{
						 colorTableItem[0] = 255;//B
						 colorTableItem[1] = 0;
						 colorTableItem[2] = 0;
						 break;
			}
			case 252://��ɫ
			{
						 colorTableItem[0] = 0;//B
						 colorTableItem[1] = 255;//G
						 colorTableItem[2] = 255;//R
						 break;
			}
			case 251://��ɫ
			{
						 colorTableItem[0] = 255;//B
						 colorTableItem[1] = 0;//G
						 colorTableItem[2] = 255;//R
						 break;
			}
			default:
			{
					   colorTableItem[0] = i;//B
					   colorTableItem[1] = i;//G
					   colorTableItem[2] = i; //R
					   break;
			}
			}
			if (fwrite((void *)colorTableItem, 1, 4, fp) != 4)
			{
				isSuccess = false;
				break;
			}
		}
	}


	// д��ͼ������
	// ����BMPɨ�跽ʽ�Ǵ��µ��ϣ�����BMPͼ��洢��ʽ�Ǵ��µ��ϣ�����д��ʱ����Ҫ����д���ļ�
	int extend = ucharCount_PerLine - width * ucharCount_PerPixel;
	uchar *temp;
	if (extend == 0)
	{
		for (temp = image + (height - 1) * ucharCount_PerPixel * width; temp >= image; temp -= ucharCount_PerPixel * width)
		{
			if (fwrite((void *)temp, 1, width * ucharCount_PerPixel, fp) != (unsigned int)(ucharCount_PerPixel * width))
				isSuccess = false; // ��ʵ������
		}
	}
	else
	{
		for (temp = image + (height - 1) * ucharCount_PerPixel * width; temp >= image; temp -= ucharCount_PerPixel * width)
		{
			if (fwrite((void *)temp, 1, width * ucharCount_PerPixel, fp) != (unsigned int)(ucharCount_PerPixel * width))
				isSuccess = false; // ��ʵ������
			for (i = 0; i < extend; i++) // ��������ݣ��������ݲ���
			{
				if (fwrite((void *)(temp + ucharCount_PerPixel* (width - 1)), 1, 1, fp) != 1)
					isSuccess = false;
			}
		}
	}

	// Return;
	fclose(fp);
	return isSuccess;
}

}// namespace QQ