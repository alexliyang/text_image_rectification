#ifndef __JLCLASS_HPP__
#define __JLCLASS_HPP__

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#define BLACK 50	// <50��Ϊ��ɫ
#define WHITE 200	//>200��Ϊ��ɫ
#define MAX_OFFSET src.rows/100	//һ�������������ֱ����  10//�޸�300
#define MIN_LEN src.rows/6		//��������г���	500
#define MINYS src.rows/150		//��������г���	20

using namespace cv;
using namespace std;


//-----------------------------------��VerProject�ࡿ---------------------------------------------  
//      ��������ֱͶӰѰ���ּ��  
//---------------------------------------------------------------------------------------------- 

class VerProject
{
public:

	VerProject(Point p1, Point p2, Mat garyImage)//��ʼ��
	{
		image=garyImage;
		this->p1=p1;
		this->p2=p2;
	}
	
	vector<int> findPosition()
	{
		//find local peaks, key and value stored in vector wPoints, reSumps
		int pos;
		int Pd=20;
		int k=0;
		double sump=0;
		int i;
		for(i=p1.x;i<p2.x;i++) {
			
			int numj=0;
			for(int j=p1.y;j<p2.y;j++) {
				sump+=image.at<uchar>(j,i);
				numj++;
			}
			sump/=numj;
			sumps.push_back(sump);
			//cout<<sump<<"	";
			
			if (k>1 && sumps[k-1]>sumps[k-2] && sumps[k-1]>sumps[k]) {
				reSumps.push_back(sump);
				wPoints.push_back(i);
				//cout<<sump<<","<<i<<"	";
				
			}
			k++;
					
		}
		
		// delete peaks too closed between each other. which mean fliter to noise.
		// Pd : the min neighbor distance. it should be auto-changed with different pictures. 
		while(wPoints.size() >1)
		{
			int kk=1;
			pos = (int) ( max_element(reSumps.begin(),reSumps.end()) - reSumps.begin() );
			//cout<<reSumps[pos]<<"	"<<wPoints[pos]<<endl;
			delPoints.push_back(wPoints[pos]);
			delSumps.push_back(reSumps[pos]);
			//cout<<"pos:"<<pos<<endl;
			//if (pos>0)
			//cout<< wPoints[pos]-wPoints[pos-1]<<endl;

			while(pos-1>=0 && wPoints[pos]-wPoints[pos-1]<Pd)	//forward delete
			{
				
				wPoints.erase(wPoints.begin()+(--pos));

				reSumps.erase(reSumps.begin()+pos);
				
			}
			while(pos+kk<wPoints.size() && wPoints[pos+kk]-wPoints[pos]<Pd)	//backward delete
			{
				
				wPoints.erase(wPoints.begin()+pos+kk);
				
				reSumps.erase(reSumps.begin()+pos+kk);
				
			}

			
			wPoints.erase(wPoints.begin()+pos);
		
			reSumps.erase(reSumps.begin()+pos);

		}
		
		//delPoints.push_back(wPoints[0]);
		
		for (int i=0;i<delPoints.size();i++)
			cout<<delPoints[i]<<"	";
		return delPoints;

	}

private:

	Point p1,p2;//region of text
	double maxL;//��߶γ���
	Vec4i maxSeg;//��߶�����
	Mat image;//�����ͼ��
	vector<int> wPoints;	//vertical position of text
	vector<int> resPoints;
	vector<int> delPoints;	//position after delete
	vector<double> sumps;	//vertical value of text
	vector<double> reSumps;
	vector<double> delSumps;//value after delete
};

class jlmethod 
{
public:
	static vector<vector<Point3f>> findline(const Mat &src)
	{
		int x_scale=0, y_scale=5,max_p=0;	//	ͼ����Χ���ٷֱȣ�
		int x1,x2,y1,y2,x0,y0,fx,fy;
		Point3f p;
		int inver_flag=0;	//�����־��0����    1����

		//ROI�߽�
		y1=src.rows*y_scale*1.0/100;
		y2=src.rows*(1-y_scale*1.0/100);
		x1=src.cols*x_scale*1.0/100;
		x2=src.cols*(1-x_scale*1.0/100);

		//��ʼ�����
		y0=src.rows/2+1;
		x0=src.cols*3/4+1;

		int i,j,k;

		//��ֱ�������ڽ���
		int flag=1,offset=1;

		p.x=x0;p.y=y0;
		vector <Point3f> aline;
		vector<vector<Point3f>> lines;
		vector<int> ys;
		fy=y1;

		//��ֱɨ�裬ȷ��ÿ�л���
		while (fy>=y1&&fy<=y2)
		{
			if(src.at<uchar>(fy,x0)>=WHITE)
				if(!ys.empty())
				{
					if(fy-ys.back()>MINYS)
						ys.push_back(fy);
					else
					{
						ys.pop_back();
						ys.push_back(fy);
					}
				}
				else
					ys.push_back(fy);

			fy++;
		}

		//��ÿ�л��㿪ʼ�����Һ���ˮƽɨ��
		for(i=0;i<ys.size();i++)
		{



			p.x=x0;
			p.y=ys[i];
			inver_flag=0;

			if(p.x>=x0)
				aline.push_back(p);
			else
				aline.insert(aline.begin(),p);

			while (p.x<=x2 && p.x>=x1)
			{
				if(inver_flag==0)
					p.x++;
				else
					p.x--;
				if(p.x==x2)
				{
					inver_flag=1;
					p.x=x0-1;
					p.y=ys[i];
				}
				flag=1,offset=1;
				while(src.at<uchar>(p.y,p.x)<=BLACK)
				{
					p.y+=(offset*flag);
					offset++;
					flag=-flag;
					if(offset>=MAX_OFFSET)
						break;

				}
				if(offset>=MAX_OFFSET)
				{
					if(inver_flag==0)
					{
						inver_flag=1;
						p.x=x0-1;
						p.y=ys[i];
					}
					else
						break;
				}
				if(p.x>=x0)
					aline.push_back(p);
				else
					aline.insert(aline.begin(),p);
				fx=p.x;
				fy=p.y;

			}
			if(aline.size()>max_p)
				max_p=aline.size();
			if(aline.size()>max_p/2)
			{

				if(p.x>x0)
					lines.push_back(aline);
				else
					lines.insert(lines.begin(),aline);
			}
			aline.clear();





		}
		for(i=0;i<lines.size();i++)
		{
			if(lines[i].size()<max_p/2)
				lines.erase(lines.begin()+i);
		}
		return lines;

	}
	//-----------------------------------��cutEnds()������----------------------------------  
	//      ������ȥ�������������˸��Ŷ�
	//		���룺δ����㼯��ȥ������
	//		����������㼯
	//---------------------------------------------------------------------------------------------- 
	static vector<vector<Point3f>> cutEnds(const vector<vector<Point3f>> &src,int length)
	{
		vector<vector<Point3f>> dst;
		int i,j;
		dst = src;//��ʱ
		for (i=0;i<src.size();i++)
		{
			for(j=0;j<length;j++)
			{
				dst[i].erase(dst[i].begin());
				dst[i].pop_back();
			}
		}

		return dst;
	}
	//-----------------------------------��extendEnds()������----------------------------------  
	//      ���������������ߵ㼯��չ�ɱ�׼����
	//		���룺δ��չ�㼯��minX��maxX
	//		�������չ��㼯
	//---------------------------------------------------------------------------------------------- 
	static vector<vector<Point3f>> extendEnds(const vector<vector<Point3f>> &src,int minX,int maxX)
	{
		int m,n;
		m=src.size();
		n=maxX-minX+1;
		vector<vector<Point3f>> dst(m,n);
		int i,j;

		//dst = src;//��ʱ
		for (i=0;i<src.size();i++)
		{
			for(j=0;j<src[i].size();j++)
			{
				dst[i][src[i][j].x-minX].y=src[i][j].y;
				dst[i][src[i][j].x-minX].x=src[i][j].x;
			}
		}

		for (i=0;i<dst.size();i++)
		{
			for(j=0;j<dst[i].size();j++)
			{
				if(dst[i][j].x==0)
					dst[i][j].x=minX+j;
			}
		}


		return dst;
	}
	//-----------------------------------��lineInterpolation()������----------------------------------  
	//      ��������չ�ν��ж��κ�����ֵ
	//		���룺δ������ֵ�ĵ㼯��ȡ����
	//		�������ֵ��㼯
	//---------------------------------------------------------------------------------------------- 
	static vector<vector<Point3f>> lineInterpolation(const vector<vector<Point3f>> &src,int interval)
	{
		vector<vector<Point3f>> dst;

		dst = src;//��ʱ
		int i,j,k;
		float x1,x2,x3,y1,y2,y3,x,y;
		
		for (i=0;i<src.size();i++)
		{
			for(k=0;k<src[i].size();k++)
			{
				if(src[i][k].y!=0)
					break;
			}
			x1=src[i][k].x;
			y1=src[i][k].y;
			x2=src[i][k+interval].x;
			y2=src[i][k+interval].y;
			x3=src[i][k+interval*2].x;
			y3=src[i][k+interval*2].y;
			for(j=0;dst[i][j].y==0;j++)
			{
				x=dst[i][j].x;
				dst[i][j].y= y1*(x2-x)*(x3-x)/((x1-x2)*(x1-x3))+
					y2*(x1-x)*(x-x3)/((x1-x2)*(x2-x3))+
					y3*(x1-x)*(x-x2)/((x1-x3)*(x3-x2));
			}

			
			for(k=src[i].size()-1;k>=0;k--)
			{
				if(src[i][k].y!=0)
					break;
			}
			x1=src[i][k].x;
			y1=src[i][k].y;
			x2=src[i][k-interval].x;
			y2=src[i][k-interval].y;
			x3=src[i][k-interval*2].x;
			y3=src[i][k-interval*2].y;
			k=src[i].size()-1;
			for(j=k;dst[i][j].y==0;j--)
			{
				x=dst[i][j].x;
				dst[i][j].y= y1*(x2-x)*(x3-x)/((x1-x2)*(x1-x3))+
					y2*(x1-x)*(x-x3)/((x1-x2)*(x2-x3))+
					y3*(x1-x)*(x-x2)/((x1-x3)*(x3-x2));
			}



		}




		return dst;
	}
	static void showline( Mat &src,vector<vector<Point3f>> RES,int val)
	{
		int i,j;
		//cout<<RES.size()<<endl;
		for (i=0;i<RES.size();i++)
		{
			for(j=0;j<RES[i].size();j++)
			{
				//cout<<RES[i].size();
				if (RES[i][j].x>1 && RES[i][j].x<src.cols-1 && RES[i][j].y>1 && RES[i][j].y<src.rows-1)
				{
					src.at<uchar>(RES[i][j].y,RES[i][j].x)=val;
				}
				//cout<<"("<<RES[i][j].x<<","<<RES[i][j].y<<")  ";
			}
		}
		//cout<<RES.size();
	}
	static void showline( Mat &src,vector<vector<Point3f>> RES,const Scalar& color)
	{
		int i,j;
		//cout<<RES.size()<<endl;
		for (i=0;i<RES.size();i++)
		{
			for(j=0;j<RES[i].size();j++)
			{
				if (RES[i][j].x>1 && RES[i][j].x<src.cols-1 && RES[i][j].y>1 && RES[i][j].y<src.rows-1)
				{
					//cout<<RES[i].size();
					src.at<Vec3b>(RES[i][j].y,RES[i][j].x)=Vec3b(color[0],color[1],color[2]);
					src.at<Vec3b>(RES[i][j].y+1,RES[i][j].x)=Vec3b(color[0],color[1],color[2]);
					//line( src, Point(RES[i][j].x,RES[i][j].y), Point(RES[i][j].x+1,RES[i][j].y), color, 2, CV_AA);//point:(�У���)
					//cout<<"("<<RES[i][j].x<<","<<RES[i][j].y<<")  ";
				}
			}
		}
		//cout<<RES.size();

	}
	static void showline( Mat &src,Mat &th,const Scalar& color)
	{
		int i,j;
		//cout<<RES.size()<<endl;
		for (i=1;i<src.cols-2;i++)
		{
			for(j=1;j<src.rows-2;j++)
			{
				if (th.at<uchar>(j,i) == 255)
				{
					//line( src, Point(i,j), Point(i+1,j), color, 2, CV_AA);//point:(�У���)
					src.at<Vec3b>(j,i)=Vec3b(color[0],color[1],color[2]);
					src.at<Vec3b>(j+1,i)=Vec3b(color[0],color[1],color[2]);
				}
			}
		}
		//cout<<RES.size();

	}
};
#endif