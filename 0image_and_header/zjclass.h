#ifndef __ZJCLASS_HPP__
#define __ZJCLASS_HPP__

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>



#include <vector>  
#include<complex>  
#include<bitset>  
#include <iterator>  
#include <algorithm>  
#include <iostream>  
#include <sstream>  
const double pai = 3.1415926535897932384626433832795028841971; // Բ����  

//typedef vector<vector<complex<double> > >VVCD;  
//typedef vector<vector<double> >VVD;  



using namespace cv;
using namespace std;
//-----------------------------------��VerBaseLine�ࡿ---------------------------------------------  
//      ������Ѱ�������߶��еĴ�ֱ��׼�ߣ��ӽ���ֱ���
//---------------------------------------------------------------------------------------------- 

class VerBaseLine
{
public:
	int baseLiN;//��߶α��
	int baseL;//��߶γ���
	Vec4i baseSeg;//��߶�����
	Mat image;//�����ͼ��
	vector<Vec4i> lines;//������߶μ���
	VerBaseLine(){}//Ĭ�Ϲ��캯��
	VerBaseLine(vector<Vec4i> lines)//��ʼ�����������߶μ���
	{
		baseLiN = -1;//��׼�߶α��
		baseL = -1;//��׼�߶γ���
		this->lines = lines;//������߶μ���

		for( size_t i = 0; i < lines.size(); i++ )//Ѱ�һ�׼�߶�
		{
			Vec4i l = lines[i];
			double thisLine = sqrt(pow((double)(l[0]-l[2]),2)+pow((double)(l[1]-l[3]),2));//���߳���
			double angle = atan(abs(double(l[2]-l[0])/double(l[3]-l[1])))/CV_PI*180;//������y�����ʽǶȵľ���ֵ
			if (thisLine > baseL && angle < 45)
			{
				baseL = thisLine;
				baseLiN = i;
				baseSeg = l;
			}
		}
	}
	VerBaseLine(vector<Vec4i> lines,Mat image)//��ʼ���������߶μ�����ͼ��
	{
		baseLiN = -1;//��߶α��
		baseL = -1;//��߶γ���
		this->lines = lines;//������߶μ���
		this->image = image;//��ʼ��ͼ��

		for( size_t i = 0; i < lines.size(); i++ )//Ѱ�һ�׼�߶�
		{
			Vec4i l = lines[i];
			double thisLine = sqrt(pow((double)(l[0]-l[2]),2)+pow((double)(l[1]-l[3]),2));//���߳���
			double angle = atan(abs(double(l[2]-l[0])/double(l[3]-l[1])))/CV_PI*180;//������y�����ʽǶȵľ���ֵ
			if (thisLine > baseL && angle < 45)
			{
				baseL = thisLine;
				baseLiN = i;
				baseSeg = l;
			}
		}
	}
	int baseLineNumber()
	{
		if (baseLiN != -1)
		{
			return baseLiN;
		} 
		else
		{
			cout<<"û���ҵ�����"<<endl;getchar(); return -1;
		}
	}
	int baseLength()
	{
		if (baseL != -1)
		{
			return baseL;
		} 
		else
		{
			cout<<"û���ҵ�����"<<endl;getchar(); return -1;
		}
	}
	Vec4i baseSegment()
	{
		if (baseLiN != -1)
		{
			return baseSeg;
		} 
		else
		{
			cout<<"û���ҵ�����"<<endl;getchar(); return -1;
		}
	}
	void drawOnImage(const Scalar& color = Scalar(0,0,255))
	{
		if (!image.empty())
		{
			line( image, Point(baseSeg[0], baseSeg[1]), Point(baseSeg[2], baseSeg[3]), color, 3, CV_AA);
		} 
		else
		{
			cout<<"��������ͼ��"<<endl;getchar();
		}

	}
};
//-----------------------------------��HorBaseLine�ࡿ---------------------------------------------  
//      ������Ѱ�������߶��е�ˮƽ��׼�ߣ��ӽ�ˮƽ���
//---------------------------------------------------------------------------------------------- 

class HorBaseLine
{
public:
	int baseLiN;//��߶α��
	int baseL;//��߶γ���
	Vec4i baseSeg;//��߶�����
	Mat image;//�����ͼ��
	vector<Vec4i> lines;//������߶μ���
	HorBaseLine(){}//Ĭ�Ϲ��캯��
	HorBaseLine(vector<Vec4i> lines)//��ʼ�����������߶μ���
	{
		baseLiN = -1;//��׼�߶α��
		baseL = -1;//��׼�߶γ���
		this->lines = lines;//������߶μ���

		for( size_t i = 0; i < lines.size(); i++ )//Ѱ�һ�׼�߶�
		{
			Vec4i l = lines[i];
			double thisLine = sqrt(pow((double)(l[0]-l[2]),2)+pow((double)(l[1]-l[3]),2));//���߳���
			double angle = atan(abs(double(l[2]-l[0])/double(l[3]-l[1])))/CV_PI*180;//������y�����ʽǶȵľ���ֵ
			if (thisLine > baseL && angle > 60)
			{
				baseL = thisLine;
				baseLiN = i;
				baseSeg = l;
			}
		}
	}
	HorBaseLine(vector<Vec4i> lines,Mat image)//��ʼ���������߶μ�����ͼ��
	{
		baseLiN = -1;//��߶α��
		baseL = -1;//��߶γ���
		this->lines = lines;//������߶μ���
		this->image = image;//��ʼ��ͼ��

		for( size_t i = 0; i < lines.size(); i++ )//Ѱ�һ�׼�߶�
		{
			Vec4i l = lines[i];
			double thisLine = sqrt(pow((double)(l[0]-l[2]),2)+pow((double)(l[1]-l[3]),2));//���߳���
			double angle = atan(abs(double(l[2]-l[0])/double(l[3]-l[1])))/CV_PI*180;//������y�����ʽǶȵľ���ֵ
			if (thisLine > baseL && angle > 60)
			{
				baseL = thisLine;
				baseLiN = i;
				baseSeg = l;
			}
		}
	}
	int baseLineNumber()
	{
		if (baseLiN != -1)
		{
			return baseLiN;
		} 
		else
		{
			cout<<"û���ҵ�ˮƽ��׼��"<<endl;getchar(); return -1;
		}
	}
	int baseLength()
	{
		if (baseL != -1)
		{
			return baseL;
		} 
		else
		{
			cout<<"û���ҵ�ˮƽ��׼��"<<endl;getchar(); return -1;
		}
	}
	Vec4i baseSegment()
	{
		if (baseLiN != -1)
		{
			return baseSeg;
		} 
		else
		{
			cout<<"û���ҵ�ˮƽ��׼��"<<endl;getchar(); return -1;
		}
	}
	void drawOnImage(const Scalar& color = Scalar(0,0,255))
	{
		if (!image.empty())
		{
			line( image, Point(baseSeg[0], baseSeg[1]), Point(baseSeg[2], baseSeg[3]), color, 2, CV_AA);
		} 
		else
		{
			cout<<"��������ͼ��"<<endl;getchar();
		}

	}
};
//-----------------------------------��RotateImage�ࡿ------------------------------------------  
//      ��������ͼƬ��תָ���Ƕȶ����
//---------------------------------------------------------------------------------------------- 
class RotateImage
{
public:
	Mat inputImage;//����ͼ��
	Vec4i inputLine;//������߶�
	Mat outputImage;//���ͼ��
	int outpulLineCol;//�任����߶�������
	double angle;//��ת�Ƕ�
	Point center;//��ת���ĵ�
	RotateImage(){}//Ĭ�Ϲ��캯��
	RotateImage(Mat inputImage, Vec4i inputLine)//��ʼ���࣬����ͼ���봹ֱ��׼��
	{
		this->inputImage = inputImage;
		this->inputLine = inputLine;
	}

	Mat trans1()//��ת��ʽ1
	{
		Mat rotMat( 2, 3, CV_32FC1 );//��ʼ����ת����

		//��ʼ����ת�Ƕ�
		angle = -atan((double)(inputLine[2]-inputLine[0])/(double)(inputLine[3]-inputLine[1]))/CV_PI*180;

		//��ʼ��Ŀ��ͼ��Ĵ�С
		outputImage = Mat::ones(inputImage.rows, inputImage.cols, inputImage.type());

		//��ͼ�������ת
		int x = (inputLine[2]-inputLine[0])*(inputImage.rows/2-inputLine[1])/(inputLine[3]-inputLine[1])+inputLine[0];
		int y = inputImage.rows/2;
		center = Point( x, y );
		outpulLineCol = x;
		double scale = 1;
		// ͨ���������תϸ����Ϣ�����ת����
		rotMat = getRotationMatrix2D( center, angle, scale );
		// ��ת�����ź��ͼ��
		//��Դͼ��Ӧ�øո���õķ���任
		if (inputImage.channels() == 3)
		{
			warpAffine( inputImage, outputImage, rotMat, outputImage.size(), 1, 0, Scalar(255,255,255) );
			/*	����7������ɫ*/
		}
		if (inputImage.channels() == 1)
		{
			warpAffine( inputImage, outputImage, rotMat, outputImage.size(), INTER_NEAREST, 0, Scalar(0) );
		}
		return outputImage;
	}
};
//-----------------------------------��HorAdjustImage�ࡿ------------------------------------------  
//      ����������ˮƽ��׼�߽�ͼƬ����ˮƽ������ʹ��ˮƽ��׼�߱�ˮƽ
//---------------------------------------------------------------------------------------------- 
class HorAdjustImage
{
public:
	Mat inputImage;//����ͼ��
	Vec4i inputLine;//������߶�
	Mat outputImage;//���ͼ��
	int outpulLineRow;//�任����߶�������
	HorAdjustImage(){}//Ĭ�Ϲ��캯��
	HorAdjustImage(Mat inputImage, Vec4i inputLine)//��ʼ���࣬����ͼ����ˮƽ��׼��
	{
		this->inputImage = inputImage;
		this->inputLine = inputLine;
	}

	Mat trans()//�����л�׼�ߵ���
	{
		outpulLineRow = inputLine[1];

		//��������㣬��������������
		Point2f srcTriangle[3];
		Point2f dstTriangle[3];

		//����Դͼ���Ŀ��ͼ���ϵ�������Լ������任
		srcTriangle[0] = Point2f( inputLine[0],inputLine[1] );
		srcTriangle[1] = Point2f( inputLine[2], inputLine[3] );
		srcTriangle[2] = Point2f( inputLine[0], inputLine[1]+1 );

		dstTriangle[0] = Point2f( inputLine[0],inputLine[1] );
		dstTriangle[1] = Point2f( inputLine[2], inputLine[1] );
		dstTriangle[2] = Point2f( inputLine[0], inputLine[1]+1 );

		//������ת����
		Mat rotMat( 2, 3, CV_32FC1 );

		//�������任
		rotMat = getAffineTransform( srcTriangle, dstTriangle );

		//��Դͼ��Ӧ�øո���õķ���任
		if (inputImage.channels() == 3)
		{
			warpAffine( inputImage, outputImage, rotMat, outputImage.size(), 1, 0, Scalar(255,255,255) );
		}
		if (inputImage.channels() == 1)
		{
			warpAffine( inputImage, outputImage, rotMat, outputImage.size(), INTER_NEAREST, 0, Scalar(0) );
		}
		return outputImage;
	}
};
//-----------------------------------��WordCorrection�ࡿ------------------------------------------  
//      ��������ˮƽ�����Ͻ����ֵĿ��
//---------------------------------------------------------------------------------------------- 
class WordCorrection
{
public:
	vector<int> x;//�ּ���λ��x����������ͼ���ϵ�λ��
	vector<int> wordInterval;//��������xֵ�ļ�ࣨ���ֿ��,����ǰһ�����ֵ����������һ�����ֵ��
	int yh;//��������y����
	int yl;//��������y����
	Mat inputImage;//�����ͼ��
	Mat outputImage;//�����ͼ��
	int targetInterval;//Ŀ��xֵ��ࣨ���ֿ��,����ǰһ�����ֵ����������һ�����ֵ��
	int targetImageWidth;//Ŀ��ͼ��Ŀ��
	int targetImageHeight;//Ŀ��ͼ��ĸ߶�
	int num;//x��Ԫ�ظ���
	WordCorrection(){}//Ĭ�Ϲ��캯��
	WordCorrection(Mat inputImage0,vector<int> a,int yh0,int yl0)//��ʼ���࣬����ͼ���ּ���λ��a����������y���ޡ���������y����
	{
		//Ϊ���еı�������ֵ
		num = sizeof(a)/sizeof(a[0]);
		inputImage = inputImage0;
		if (yh0>yl0)
		{
			yh = yh0;
			yl = yl0;
		}
		else
		{
			yh = yl0;
			yl = yh0;
		}
		if (a[0]<a[1])//�ж�����x��������з�ʽ,��x�����Դ�С����˳������
		{
			for (int i=0;i<num;i++)//Ϊ�ּ��λ��x������ֵ��ֵ
			{
				x.push_back(a[i]);
			}
			for (int i = 1;i<num;i++)//Ϊ��������xֵ�ļ����ֵ��ֵ
			{
				wordInterval.push_back(a[i]-a[i-1]);
			}
		}
		else
		{
			for (int i=num-1;i>=0;i--)//Ϊ�ּ��λ��x������ֵ��ֵ
			{
				x.push_back(a[i]);
			}
			for (int i = num-1;i>0;i--)//Ϊ��������xֵ�ļ����ֵ��ֵ
			{
				wordInterval.push_back(a[i-1]-a[i]);
			}
		}

		targetInterval = wordInterval.at(int(num/2+1));//�����м�ļ����ΪĿ����
		targetImageWidth = (num-1)*targetInterval;//���ͼ��Ŀ��
		targetImageHeight = yh-yl+1;//���ͼ��ĸ߶�
		outputImage = Mat::zeros(Size(targetImageWidth,targetImageHeight),inputImage.type());//���ͼ���ʼ��
	}
	Mat trans()//���к���������
	{
		for (int i=1;i<num;i++)
		{
			//��������㣬��������������
			Point2f srcTriangle[3];
			Point2f dstTriangle[3];

			//����Դͼ���Ŀ��ͼ���ϵ�������Լ������任
			srcTriangle[0] = Point2f( x.at(i-1),0 );
			srcTriangle[1] = Point2f( static_cast<float>(x.at(i) - 1), 0 );
			srcTriangle[2] = Point2f( 0, static_cast<float>(inputImage.cols - 1 ));

			dstTriangle[0] = Point2f( static_cast<float>(targetInterval*(i-1)),0 );
			dstTriangle[1] = Point2f( static_cast<float>(targetInterval*i), 0);
			dstTriangle[2] = Point2f( 0, static_cast<float>(outputImage.cols - 1 ));

			//������ת����
			Mat rotMat( 2, 3, CV_32FC1 );

			//�������任
			rotMat = getAffineTransform( srcTriangle, dstTriangle );

			//��Դͼ��Ӧ�øո���õķ���任
			warpAffine( inputImage, outputImage, rotMat, outputImage.size() );
		}
	}

};

#endif
