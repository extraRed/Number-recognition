#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

class StandardSVM
{
	public:
		StandardSVM();
		float classify(IplImage *test, int showResult);
		void test();
	private:
		char file_path[255];
		int size;
		int train_samples;
		int classes;
		CvSVM svm;
};