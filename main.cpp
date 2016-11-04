#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <sys/time.h>


using namespace std;
using namespace cv;

void inline db_Copy9(double xd[9],const double xs[9])
{
    xd[0]=xs[0];xd[1]=xs[1];xd[2]=xs[2];
    xd[3]=xs[3];xd[4]=xs[4];xd[5]=xs[5];
    xd[6]=xs[6];xd[7]=xs[7];xd[8]=xs[8];
}

void to_m(double M[9]){

	double m_H_ref_to_ins[9]={2,2,2,54, 1,0,0, 1,0};

 	db_Copy9(M,m_H_ref_to_ins);

}


bool YV12ToBGR24_OpenCV(unsigned char* pYUV,unsigned char* pBGR24,int width,int height)
{
    if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
        return false;
    Mat dst(height,width,CV_8UC3,pBGR24);
    Mat src(height + height/2,width,CV_8UC1,pYUV);
    cvtColor(src,dst,CV_YUV2BGR_YV12);
    return true;
}

int main(int argc, char* argv[])
{

    Mat img1 = imread("1.jpg");
    Mat img2 = imread("2.jpg");
	
    Mat img_yuv2 ;
    cvtColor(img2,img_yuv2,COLOR_RGB2YUV);
    
    imwrite("image.jpg",img_yuv2);

	
 /*   
    Mat H = (Mat_<double>(3,3)<<
      1, 0.0, 0,
      0, 1,  0,
      0, 0,  1);
*/
  static const double coeffs[3][3] =
    {
        {cos(CV_PI / 6), -sin(CV_PI / 6), 100.0},
        {sin(CV_PI / 6), cos(CV_PI / 6), -100.0},
        {0.0, 0.0, 1.0}
    };
    Mat H(3, 3, CV_64F, (void *)coeffs);
		
  
    Mat xformed;
    warpPerspective(img2,xformed,H,img1.size(),INTER_LINEAR);
    imwrite("out1.jpg",xformed);
    Mat xformed2;
    warpPerspective(img_yuv2,xformed2,H,img1.size(),INTER_LINEAR);
    imwrite("outyuv.jpg",xformed2);
    Mat dst;
    cvtColor(xformed2,dst,COLOR_YUV2RGB);
    imwrite("out2.jpg",dst);

    
    return 0;
}
