#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

class StandardKNN
{
	public:
		float classify(IplImage* img,int showResult);
		StandardKNN ();
		void test();	
	private:
		char file_path[255];
		int train_samples;
		int classes;
		CvMat* trainData;	//数据集
		CvMat* trainClasses;	//标签集
		int size;
		static const int K=5;//最大邻居个数
		CvKNearest *knn;
		void getData();
		void train();
};
