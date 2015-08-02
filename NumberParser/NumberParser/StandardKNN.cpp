#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "StandardKNN.h"
#include "Parse.h"
#include <iostream>
using namespace std;
StandardKNN::StandardKNN()//构造函数
{
	sprintf(file_path , "OCR/");//存储图片的地方
	train_samples = 80;
	classes= 10;//识别十个数字
	size=128;//图片长/宽	
	printf("Start Training...\n");
	trainData = cvCreateMat(train_samples*classes, size*size, CV_32FC1);//训练数据的矩阵
	trainClasses = cvCreateMat(train_samples*classes, 1, CV_32FC1);//标签矩阵
	getData();//将已有图片数据装载到训练矩阵中
	//进行训练
	train();
	//测试默认数据集正确率
	time_t st,ed;
	st=time(NULL);
	test();
	ed=time(NULL);
	printf("time:%.2f\n",difftime(ed,st));
	printf("Finish Training...\n");
	printf(" -----------------------------------------------\n");
	printf("|\t识别结果\t|\t  准确率\t|\n");
	printf(" -----------------------------------------------\n");
}

void StandardKNN::getData()
{
	IplImage* src_image;
	IplImage prs_image;
	CvMat row,data;
	char file[255],outname[255];
	int i,j;
	for(i=0; i<classes; i++)
	{
		for( j = 0; j< train_samples; j++)
		{
			//加载pbm格式图像，作为训练
			if(j<10)
				sprintf(file,"%s%d/%d0%d.pbm",file_path, i, i , j);
			else
				sprintf(file,"%s%d/%d%d.pbm",file_path, i, i , j);

			
			/*sprintf(outname,"%d-%d.bmp",i , j);
			IplImage *imgMono,*imgClr;
			//得到单通道图象
			imgMono=cvLoadImage(file,0);
			//定义一个3通道图象
			imgClr = cvCreateImage(cvSize(imgMono->width,imgMono->height), IPL_DEPTH_8U, 3);
			cvCvtColor(imgMono, imgClr, CV_GRAY2BGR);
			cvSaveImage(outname, imgClr);
			*/

			src_image = cvLoadImage(file,0);
			if(!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				exit(-1);
			}
			//对图片进行预处理
			prs_image = preprocessing(src_image, size, size,1);
			//设置标签（0-9）
			//cvGetRow函数：将row和trainClasses联系起来（改一个会同时改另一个），下同
			cvGetRow(trainClasses, &row, i*train_samples + j);
			cvSet(&row, cvRealScalar(i));
			
			cvGetRow(trainData, &row, i*train_samples + j);

			IplImage* img = cvCreateImage( cvSize( size, size ), IPL_DEPTH_32F, 1 );
			//将图片prs_image缩放后存进img
			//0.0039215=1/255
			cvConvertScale(&prs_image, img, 0.0039215, 0);	
			//将所需大小的图片放进data中
			cvGetSubRect(img, &data, cvRect(0,0, size,size));
			
			CvMat row_header, *row1;
			//转换成1维向量
			row1 = cvReshape( &data, &row_header, 0, 1 );
			//存入数据矩阵中
			cvCopy(row1, &row, NULL);
		}
	}
}
void StandardKNN::train()
{
	knn=new CvKNearest( trainData, trainClasses, 0, false, K );
}

float StandardKNN::classify(IplImage* img, int showResult)
{
	IplImage prs_image;
	CvMat data;
	CvMat* nearest=cvCreateMat(1,K,CV_32FC1);
	float result;
	//处理输入的图像
	prs_image = preprocessing(img, size, size,1);
	IplImage* img32 = cvCreateImage( cvSize( size, size ), IPL_DEPTH_32F, 1);
	cvConvertScale(&prs_image, img32, 0.0039215, 0);
	cvGetSubRect(img32, &data, cvRect(0,0, size,size));
	CvMat row_header, *row1;
	row1 = cvReshape( &data, &row_header, 0, 1 );
	result=knn->find_nearest(row1,K,0,0,nearest,0);
	int accuracy=0;
	for(int i=0;i<K;i++)
	{
		if( (nearest->data.fl[i]) == result)
                    accuracy++;
	}
	if(showResult==1)
	{
		printf("|\t    %.0f    \t| \t %d of %d \t| \n",result,accuracy,K);
		printf(" -----------------------------------------------\n");
	}

	return result;

}

void StandardKNN::test()
{
	IplImage* src_image;
	IplImage prs_image;
	char file[255];
	int i,j;
	int error=0;
	int testCount=0;
	for(i =0; i<classes; i++)
	{
		for( j = train_samples; j< 100; j++)//五十个测试样本，计算一下错误率
		{
			sprintf(file,"%s%d/%d%d.pbm",file_path, i, i , j);
			src_image = cvLoadImage(file,0);
			if(!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				exit(-1);
			}

			//预处理图片
			prs_image = preprocessing(src_image, size, size,1);
			float r=classify(&prs_image,0);
			if((int)r!=i)
				error++;
			testCount++;
		}
	}
	float totalerror=100*(float)error/(float)testCount;
	printf("错误率: %.2f%%\n", totalerror);
	
}