#pragma comment(lib,"vfw32.lib ")
#pragma comment(lib,"comctl32.lib")

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "ml.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Parse.h"
#include "StandardKNN.h"
#include "StandardSVM.h"
#include "MyKNN.h"
using namespace std;
using namespace cv;
clock_t start,finish;
IplImage* imagen;
int red,green,blue;
IplImage* screenBuffer;
int drawing;
int r,last_x, last_y;
int size;

void draw(int x,int y)	//画图
{
	cvCircle(imagen, cvPoint(x,y), r, CV_RGB(red,green,blue), -1, 4, 0);
	screenBuffer=cvCloneImage(imagen);
	cvShowImage( "手写板", screenBuffer );
}
void drawCursor(int x, int y)
{
	//保存之前图像
	screenBuffer=cvCloneImage(imagen);
	cvCircle(screenBuffer, cvPoint(x,y), r, CV_RGB(0,0,0), 1, 4, 0);	//画该点
}
void on_mouse( int event, int x, int y, int flags, void* param )	//鼠标事件
{
	last_x=x;
	last_y=y;
	drawCursor(x,y);
	
	if(event==CV_EVENT_LBUTTONDOWN)	//鼠标按下
	{
		drawing=1;
		draw(x,y);
	}
	else if(event==CV_EVENT_LBUTTONUP)	//鼠标空闲
	{
		drawing=0;
	}
	else if(event == CV_EVENT_MOUSEMOVE  &&  flags & CV_EVENT_FLAG_LBUTTON)	//按下鼠标并移动
	{
		if(drawing)
			draw(x,y);
	}
}
void PrintBasic()
{
	printf( "                                 手写数字识别\n"
		"快捷键: \n"
		"\tr - 重置白版\n"
		"\tc - 开始识别\n"
		"\tESC - 返回\n");
}
void PrintWelcome()
{
	printf("			请输入你想应用的机器学习算法：\n"
		"1.我们自己的KNN算法\n"
		"2.opencv的KNN算法\n"
		"3.opencv的SVM算法\n"
		"ESC键退出\n" );
}
void doMyKNN()
{
	MyKNN myknn;
	bool out=false;
	while(1)
	{
		if(out)
			break;
		int c;
		cvShowImage( "手写板", screenBuffer );
		c = cvWaitKey(10);
		switch(char(c))
		{
			case 27: 	//esc
				system("cls");
				out=true;
				break;
			case '-':
				if(r>1)
				{
					--r;
					drawCursor(last_x,last_y);
				}
				break;
			case '+':
				++r;
				drawCursor(last_x,last_y);
				break;
			case 'r':
				cvSet(imagen, CV_RGB(255,255,255),NULL);
				drawCursor(last_x,last_y);
				break;
			case 'c':
					IplImage prs_image, *dst;   
					prs_image = preprocessing(imagen, size, size,3);
					float scale = 0.25;
					CvSize dst_cvsize;    
					dst_cvsize.width = imagen->width * scale;  //目标图像的宽为源图象宽的scale倍
					dst_cvsize.height = imagen->height * scale; //目标图像的高为源图象高的scale倍
					dst = cvCreateImage( dst_cvsize, imagen->depth, imagen->nChannels); //构造目标图象
					cvResize(&prs_image, dst, CV_INTER_LINEAR); //缩放源图像到目标图像				
					cvSaveImage("out32.bmp", dst);
					myknn.classify();
					break;
		}
	}
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	drawCursor(last_x,last_y);
	PrintWelcome();
}
void doStandardKNN()
{
	StandardKNN sknn;
	bool out=false;
	while(1)
	{
		if(out)
			break;
		int c;
		cvShowImage( "手写板", screenBuffer );
		c = cvWaitKey(10);
		switch(char(c))
		{
			case 27: 	//esc
				system("cls");
				out=true;
				break;
			case '-':
				if(r>1)
				{
					--r;
					drawCursor(last_x,last_y);
				}
				break;
			case '+':
				++r;
				drawCursor(last_x,last_y);
				break;
			case 'r':
				cvSet(imagen, CV_RGB(255,255,255),NULL);
				drawCursor(last_x,last_y);
				break;
			case 'c':
				IplImage* temp;
				temp = cvCreateImage( cvGetSize( imagen ), imagen->depth, 1 );
				cvCvtColor( imagen, temp, CV_BGR2GRAY );
				sknn.classify(temp,1);
				break;
		}
	}
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	drawCursor(last_x,last_y);
	PrintWelcome();
}
void doStandardSVM()
{
	StandardSVM ssvm;
	bool out=false;
	while(1)
	{
		if(out)
			break;
		int c;
		cvShowImage( "手写板", screenBuffer );
		c = cvWaitKey(10);
		switch(char(c))
		{
			case 27: 	//esc
				system("cls");
				out=true;
				break;
			case '-':
				if(r>1)
				{
					--r;
					drawCursor(last_x,last_y);
				}
				break;
			case '+':
				++r;
				drawCursor(last_x,last_y);
				break;
			case 'r':
				cvSet(imagen, CV_RGB(255,255,255),NULL);
				drawCursor(last_x,last_y);
				break;
			case 'c':
				IplImage prs_image;
				prs_image = preprocessing(imagen, size, size,3);
				ssvm.classify(&prs_image,1);
				break;
		}
	}
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	drawCursor(last_x,last_y);
	PrintWelcome();
}

int main( int argc, char** argv )
{
	size=128;
	drawing=0;	//画画状态
	r=5;	//画笔半径
	last_x=last_y=red=green=blue=0;
	//创建窗口，大小128*128
	imagen=cvCreateImage(cvSize(128,128),8,3); 
	cvSet(imagen, CV_RGB(255,255,255),NULL);
	screenBuffer=cvCloneImage(imagen);
	cvNamedWindow( "手写板", 0 );
	cvResizeWindow("手写板", 512,512);
	//加入鼠标事件	
	cvSetMouseCallback("手写板",&on_mouse, 0 );
	PrintWelcome();		//提示欢迎信息
	while(1)
	{
		int c;
		c =  cvWaitKey(10);
		if(char(c)=='1')
		{
			system("cls");
			PrintBasic();
			doMyKNN();
		}
		else if(char(c)=='2')
		{
			system("cls");
			PrintBasic();
			doStandardKNN();
		}
		else if(char(c)=='3')
		{
			system("cls");
			PrintBasic();
			doStandardSVM();
		}
		else if(char(c)==27)
			break;
	}
	cvDestroyWindow("手写板");
	return 0;
}

#ifdef _EiC
main(1,"mouseEvent.c");
#endif
