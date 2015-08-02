#include "cv.h"
#include "highgui.h"
#include "ml.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <iostream>
#include "StandardSVM.h"
#include "Parse.h"
using namespace std;
using namespace cv;
StandardSVM::StandardSVM()
{
	svm = CvSVM();
	sprintf(file_path , "SVMOCR/");//存储图片的地方
	train_samples = 80;
	classes= 10;//识别十个数字
	size=128;//图片长/宽	
	printf("Start Training...\n");
	svm.load("HOG_SVM_DATA.xml");//加载训练好的xml文件
	time_t st,ed;
	st=time(NULL);
	test();
	ed=time(NULL);
	printf("time:%.2f\n",difftime(ed,st));
	printf("Finish Training...\n");
	printf(" ------------------------\n");
	printf("|\t识别结果\t|\n");
	printf(" ------------------------\n");
}
float StandardSVM::classify(IplImage *test,int showResult)
{ 
	IplImage* trainTempImg=cvCreateImage(cvSize(28,28),8,3);
	cvZero(trainTempImg);    
	//prs_image = preprocessing(trainTempImg, size, size,3);
	cvResize(test,trainTempImg);
	//cout<<2<<endl;
	HOGDescriptor *hog=new HOGDescriptor(cvSize(28,28),cvSize(14,14),cvSize(7,7),cvSize(7,7),9);
	vector<float>descriptors;//存放结果         
	//cout<<3<<endl;
	hog->compute(trainTempImg, descriptors,Size(1,1), Size(0,0)); //Hog特征计算   
	CvMat* SVMtrainMat=cvCreateMat(1,descriptors.size(),CV_32FC1);   
	int n=0;
//	cout<<descriptors.size()<<endl;
	for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++)    
	{    
		cvmSet(SVMtrainMat,0,n,*iter);    
		n++;    
	}   
	float ret = svm.predict(SVMtrainMat);//检测结果
	if(showResult==1)
	{
		printf("|\t    %.0f    \t|\n",ret);
		printf(" -----------------------\n");
	}
	return ret;
}
void StandardSVM::test()
{
	IplImage* src_image;
	IplImage prs_image;
	char file[255];
	int i,j;
	int error=0;
	int testCount=(100-train_samples)*10;
	for(i =0; i<classes; i++)
	{
		for( j = train_samples; j< 100; j++)//五十个测试样本，计算一下错误率
		{
			sprintf(file,"%s%d-%d.bmp",file_path, i , j);
			src_image = cvLoadImage(file,-1);
			if(!src_image)
			{
				printf("Error: Cant load image %s\n", file);
				exit(-1);
			}
			//printf("Given:%d\n",i);
			//预处理图片
			prs_image = preprocessing(src_image, size, size,3);
			float r=classify(&prs_image,0);
			//printf("Returned:%.0f\n",r);
			if((int)r!=i)
				error++;
		}
	}
	float totalerror=100*(float)error/(float)testCount;
	printf("错误率: %.2f%%\n", totalerror);
	
}