#ifndef __ZJMETHOD_HPP__
#define __ZJMETHOD_HPP__

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class ZjMethod
{
public:
	//-----------------------------------��expandLine( )������----------------------------------  
	//      ����������ֵ��ͼ��İ�ɫ���ֽ�������
	//---------------------------------------------------------------------------------------------- 
	static Mat expandLine(const Mat &image)
	{
		Mat expImage = Mat::zeros( image.rows, image.cols, image.type() );//��ʼ��һ����imageͬ���͵Ŀհף�ȫ�ڣ�ͼ��
		int sum=0;//ͳ��Ŀ�귽����2��2�����Ƿ��а�ɫ��

		for (int i=1;i<image.rows-1;i++)
		{
			for (int j=1;j<image.cols-1;j++,sum=0)//����ͼ���еĵ�
			{
				if (image.at<uchar>(i,j)==255)//�����(i,j)Ϊ��ɫ����(i+1,j)��(i,j+1)ҲͿ�ɰ�ɫ
				{
					expImage.at<uchar>(i+1,j)=255;
					expImage.at<uchar>(i,j+1)=255;
				}
			}

		}
		return expImage;
	}

	//-----------------------------------��thin( )������----------------------------------  
	//      �������� DEPTH_8U�Ͷ�ֵͼ�����ϸ��  �����Zhang���п���ϸ���㷨
	//		���룺����ͼ�����ͼ�񡢵�������
	//---------------------------------------------------------------------------------------------- 
	static void thin(const Mat &src, Mat &dst, const int iterations)
	{
		const int height =src.rows -1;
		const int width  =src.cols -1;

		//����һ���������һ������
		if(src.data != dst.data)
		{
			src.copyTo(dst);
		}


		int n = 0,i = 0,j = 0;
		Mat tmpImg;
		uchar *pU, *pC, *pD;
		bool isFinished = false;

		for(n=0; n<iterations; n++)
		{
			dst.copyTo(tmpImg); 
			isFinished = false;   //һ�� ���к���ɨ�� ��ʼ
			//ɨ�����һ ��ʼ
			for(i=1; i<height;  i++) 
			{
				pU = tmpImg.ptr<uchar>(i-1);
				pC = tmpImg.ptr<uchar>(i);
				pD = tmpImg.ptr<uchar>(i+1);
				for(int j=1; j<width; j++)
				{
					if(pC[j] > 0)
					{
						int ap=0;
						int p2 = (pU[j] >0);
						int p3 = (pU[j+1] >0);
						if (p2==0 && p3==1)
						{
							ap++;
						}
						int p4 = (pC[j+1] >0);
						if(p3==0 && p4==1)
						{
							ap++;
						}
						int p5 = (pD[j+1] >0);
						if(p4==0 && p5==1)
						{
							ap++;
						}
						int p6 = (pD[j] >0);
						if(p5==0 && p6==1)
						{
							ap++;
						}
						int p7 = (pD[j-1] >0);
						if(p6==0 && p7==1)
						{
							ap++;
						}
						int p8 = (pC[j-1] >0);
						if(p7==0 && p8==1)
						{
							ap++;
						}
						int p9 = (pU[j-1] >0);
						if(p8==0 && p9==1)
						{
							ap++;
						}
						if(p9==0 && p2==1)
						{
							ap++;
						}
						if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
						{
							if(ap==1)
							{
								if((p2*p4*p6==0)&&(p4*p6*p8==0))
								{                           
									dst.ptr<uchar>(i)[j]=0;
									isFinished = true;                            
								}

								//   if((p2*p4*p8==0)&&(p2*p6*p8==0))
								//    {                           
								//         dst.ptr<uchar>(i)[j]=0;
								//         isFinished =TRUE;                            
								//    }

							}
						}                    
					}

				} //ɨ�����һ ����


				dst.copyTo(tmpImg); 
				//ɨ����̶� ��ʼ
				for(i=1; i<height;  i++)  //һ�� ���к���ɨ�� ��ʼ
				{
					pU = tmpImg.ptr<uchar>(i-1);
					pC = tmpImg.ptr<uchar>(i);
					pD = tmpImg.ptr<uchar>(i+1);
					for(int j=1; j<width; j++)
					{
						if(pC[j] > 0)
						{
							int ap=0;
							int p2 = (pU[j] >0);
							int p3 = (pU[j+1] >0);
							if (p2==0 && p3==1)
							{
								ap++;
							}
							int p4 = (pC[j+1] >0);
							if(p3==0 && p4==1)
							{
								ap++;
							}
							int p5 = (pD[j+1] >0);
							if(p4==0 && p5==1)
							{
								ap++;
							}
							int p6 = (pD[j] >0);
							if(p5==0 && p6==1)
							{
								ap++;
							}
							int p7 = (pD[j-1] >0);
							if(p6==0 && p7==1)
							{
								ap++;
							}
							int p8 = (pC[j-1] >0);
							if(p7==0 && p8==1)
							{
								ap++;
							}
							int p9 = (pU[j-1] >0);
							if(p8==0 && p9==1)
							{
								ap++;
							}
							if(p9==0 && p2==1)
							{
								ap++;
							}
							if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
							{
								if(ap==1)
								{
									//   if((p2*p4*p6==0)&&(p4*p6*p8==0))
									//   {                           
									//         dst.ptr<uchar>(i)[j]=0;
									//         isFinished =TRUE;                            
									//    }

									if((p2*p4*p8==0)&&(p2*p6*p8==0))
									{                           
										dst.ptr<uchar>(i)[j]=0;
										isFinished = true;                            
									}

								}
							}                    
						}

					}

				} //һ�� ���к���ɨ�����          
				//�����ɨ�������û��ɾ���㣬����ǰ�˳�
				if(isFinished == false)
				{
					break; 
				}
			}

		}
	}
	//-----------------------------------��showregion( )������----------------------------------  
	//      ������������ͼ��ֳ�10����ʾ
	//---------------------------------------------------------------------------------------------- 
	static void showRegion(Mat &thresholdImage)
	{
		Mat imageROI[10];
		imageROI[0]=thresholdImage(Rect(1,1,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[1]=thresholdImage(Rect(thresholdImage.cols/2-1,1,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[2]=thresholdImage(Rect(1,thresholdImage.rows/5-1,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[3]=thresholdImage(Rect(thresholdImage.cols/2-1,thresholdImage.rows/5-1,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[4]=thresholdImage(Rect(1,(thresholdImage.rows/5-1)*2,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[5]=thresholdImage(Rect(thresholdImage.cols/2-1,(thresholdImage.rows/5-1)*2,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[6]=thresholdImage(Rect(1,(thresholdImage.rows/5-1)*3,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[7]=thresholdImage(Rect(thresholdImage.cols/2-1,(thresholdImage.rows/5-1)*3,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[8]=thresholdImage(Rect(1,(thresholdImage.rows/5-1)*4,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imageROI[9]=thresholdImage(Rect(thresholdImage.cols/2-1,(thresholdImage.rows/5-1)*4,thresholdImage.cols/2-1,thresholdImage.rows/5-1));
		imshow("���ֲ�ͼ0��", imageROI[0]); 
		imshow("���ֲ�ͼ1��", imageROI[1]); 
		imshow("���ֲ�ͼ2��", imageROI[2]); 
		imshow("���ֲ�ͼ3��", imageROI[3]); 
		imshow("���ֲ�ͼ4��", imageROI[4]); 
		imshow("���ֲ�ͼ5��", imageROI[5]); 
		imshow("���ֲ�ͼ6��", imageROI[6]); 
		imshow("���ֲ�ͼ7��", imageROI[7]); 
		imshow("���ֲ�ͼ8��", imageROI[8]); 
		imshow("���ֲ�ͼ9��", imageROI[9]);
	}

	//-----------------------------------��modeling()������----------------------------------  
	//      ��������������ȡ����ͶӰ������ֽ��ģ��
	//		���룺����㼯������ȡ�ߣ���ֽ����ռ������ˮƽ��׼��������������Ӱ�߶ȡ�ֽ�Ÿ߶�
	//		���������㼯��xΪ����㼯��x��yΪģ�͸߶ȣ���λ��ƫ�ã�
	//----------------------------------------------------------------------------------------------
	static vector<vector<Point3f>> modeling(const vector<vector<Point3f>> &input, int pagerows, int linerows, float height, float pagehight)
	{
		float angle;//����������������봹ֱͶӰ�ߵļн�
		vector<vector<Point3f>> output(input);//����㼯
		for (int i=0;i<input.size();i++)//ÿ����ȡ��
		{
			angle = -atan((pagehight / pagerows) * (input[i][0].y - linerows) / height );//�����еĽǶ�
				//cout<<"angle"<<i<<":"<<angle<<endl;

			for (int j=0;j<input[i].size();j++)//ÿ�е�Ԫ��
			{
				//input[i][j].y = (input[i][j].y - input[i][0].y) / sin(angle);//�����е�ʵ����״������ƫ��
				output[i][j].y = (input[i][j].y - input[i][0].y) / sin(angle) + input[i][0].y;//�����е�ʵ����״������ƫ��
			}
		}
		return output;
	}

	//-----------------------------------��lengthCalculation()������----------------------------------  
	//      ���������������ĳ�����Ϣ������
	//		���룺�޳��ȵ㼯��ģ����Ϣ�����Ȼ�׼��
	//		������г��ȵ㼯
	//---------------------------------------------------------------------------------------------- 
	static vector<vector<Point3f>> lengthCalculation(const vector<vector<Point3f>> &input, const vector<vector<Point3f>> &model,int baseline)
	{
		vector<vector<Point3f>> output(input);//����㼯

		//��ʼ��ĳ��ȶ���Ϊ�����ڵ�����
		for (int i=0;i<input.size();i++)//ÿ����ȡ��
		{
			output[i][0].z = input[i][0].x;
		}

		//������ĳ���Ϊ�����ۻ�����
		for (int i=0;i<input.size();i++)//ÿ����ȡ��
		{
			for (int j=1;j<input[i].size();j++)//ÿ�е�Ԫ��
			{
				if (j < input[baseline-1].size())//��baselineΪ����
				{
					//��baselineΪ����
					float dx = model[baseline-1][j].x - model[baseline-1][j-1].x;
					float dy = model[baseline-1][j].y - model[baseline-1][j-1].y;
					output[i][j].z = output[i][j-1].z + sqrt(pow(dx,float(2))+pow(dy,float(2)));
				} 
				else
				{
					output[i][j].z = output[i][j-1].z+1;
				}
			}
		}
		return output;
	}
	//-----------------------------------��paperCorrection()������----------------------------------  
	//      ����������ͼ��
	//		���룺����Դͼ�񡢿հ����ͼ�񡢽�����Ϣ�������ȵ�����ȡ�㼯��
	//		�����������ͼ��
	//---------------------------------------------------------------------------------------------- 
	static void paperCorrection(const Mat &inputImage, Mat &outputImage, const vector<vector<Point3f>> &massage)
	{
		int ref_cols = massage[0].size()/5;//�ο��У����һ��1/5���ȴ���y��
		int height0 = (massage[0][0].y - massage[massage.size()-1][0].y) / (massage.size()-1);//��ƽ���߶�
		int thisx;//���еĲο���
		int nextx;//��һ�еĲο���
		int height ;//����ͬһ���������ߵĿ�߶�
		int bias;//�����ж�Ӧ�Ĵ���y�����ϵ�λ��

		//������
		for (int i = massage.size()-1;i >= 0;i--)//�����ĸ���ȡ�У��������£�
		{
			//���ָ߶���ϢΪͬһ�ο���
			thisx = massage[i][0].x - massage[0][0].x + ref_cols;
			if (thisx < 0)
			{
				thisx = 0;
			}
			//�����߶�
			if (i == massage.size()-1)//��һ��ĸ߶�Ϊƽ���߶ȵ�����
			{
				height = 2 * height0;
			}
			else//������߶�Ϊ��һ�вο��е�y��ȥ���вο��е�y
			{
				//height = massage[i][0].y - massage[i+1][0].y;
				//���ָ߶���ϢΪͬһ�ο���
				nextx = massage[i+1][0].x - massage[0][0].x + ref_cols;
				if (nextx < 0)
				{
					nextx = 0;
				}
				height = massage[i][thisx].y - massage[i+1][nextx].y;
			}
			//�����ж�Ӧ�Ĵ���y�����ϵ�λ��
			if (height >1.5 * height0)//Ĭ������ƽ���߶ȳɱ���
			{
				bias = height0 * 0.2;
			} 
			else//һ���뱾��ƽ���߶ȳɱ���
			{
				bias = height * 0.2;
			}

			int j =0;//����ȡ�е�һ�п�ʼ����
			for (int io = massage[i][0].x;io < massage[i][massage[i].size()-2].z;io++)//���е��м���
			{
				for (int jo = massage[i][thisx].y, height_count = 0;height_count < height;jo--,height_count++)//���е��м���
				{
					for (;massage[i][j+1].z < io;j++){}//�ҵ����Բ�ֵ�ĵ�
					int k = (io - massage[i][j].z) / (massage[i][j+1].z - massage[i][j].z);//��ֵϵ��
					uchar B,G,R;//����
					Point	targetPoint1 = Point(massage[i][j].x,massage[i][j].y - height_count + bias),
							targetPoint2 = Point(massage[i][j+1].x,massage[i][j+1].y - height_count + bias);//��ֵ���˵ĵ�����
					if (targetPoint1.inside(Rect(0,0,inputImage.cols,inputImage.rows)) && targetPoint2.inside(Rect(0,0,inputImage.cols,inputImage.rows)))
					{//��ֹ����㳬������ͼ��Χ
						B = k * inputImage.at<Vec3b>(targetPoint1)[0] + (1 - k)*inputImage.at<Vec3b>(targetPoint2)[0];
						G = k * inputImage.at<Vec3b>(targetPoint1)[1] + (1 - k)*inputImage.at<Vec3b>(targetPoint2)[1];
						R = k * inputImage.at<Vec3b>(targetPoint1)[2] + (1 - k)*inputImage.at<Vec3b>(targetPoint2)[2];
						outputImage.at<Vec3b>(jo,io) = Vec3b(B,G,R);
					}
				}
			}
		}

		//����ĩ�У���ֹĩ��̫��
		int i = 0;//ĩ��
		int j =0;//����ȡ�е�һ�п�ʼ����
		height = (massage[0][0].y - massage[massage.size()-1][0].y) / (massage.size()-1);//����ͬһ���������ߵĿ�߶ȣ����һ��ĸ߶�Ϊƽ���߶�
		thisx = massage[i][0].x - massage[0][0].x + ref_cols;
		bias = height*0.2;//�����ж�Ӧ�Ĵ���y�����ϵ�λ��
		for (int io = massage[i][0].x;io < massage[i][massage[i].size()-2].z;io++)//���е��м���
		{
			for (int jo = massage[i][thisx].y, height_count = 0;height_count < height;jo++,height_count++)//���е��м���
			{
				for (;massage[i][j+1].z < io;j++){}//�ҵ����Բ�ֵ�ĵ�
				int k = (io - massage[i][j].z) / (massage[i][j+1].z - massage[i][j].z);//��ֵϵ��
				uchar B,G,R;//����
				Point	targetPoint1 = Point(massage[i][j].x,massage[i][j].y + height_count + bias),
					targetPoint2 = Point(massage[i][j+1].x,massage[i][j+1].y + height_count + bias);//��ֵ���˵ĵ�����
				if (targetPoint1.inside(Rect(0,0,inputImage.cols,inputImage.rows)) && targetPoint2.inside(Rect(0,0,inputImage.cols,inputImage.rows)))
				{//��ֹ����㳬������ͼ��Χ
					B = k * inputImage.at<Vec3b>(targetPoint1)[0] + (1 - k)*inputImage.at<Vec3b>(targetPoint2)[0];
					G = k * inputImage.at<Vec3b>(targetPoint1)[1] + (1 - k)*inputImage.at<Vec3b>(targetPoint2)[1];
					R = k * inputImage.at<Vec3b>(targetPoint1)[2] + (1 - k)*inputImage.at<Vec3b>(targetPoint2)[2];
					outputImage.at<Vec3b>(jo,io) = Vec3b(B,G,R);
				}
			}
		}
	}
	//-----------------------------------��smooth()������----------------------------------  
	//      �������㼯�˲�
	//		���룺δ�˲��㼯����ֵ�˲�����
	//		������˲���㼯
	//---------------------------------------------------------------------------------------------- 
	static vector<vector<Point3f>> smooth(const vector<vector<Point3f>> &src,int range)
	{
		vector<vector<Point3f>> dst(src);//����㼯
		int sum = 0,//��ֵ��Χy�ܺ�
			average = 0;//��ֵ��Χyƽ��

		for (int i = 0; i < src.size(); i++)//��ȡÿһ��
		{
			//range/2~src[i].size()-range/2���㣬ȡ��ΧΪrang�ľ�ֵ
			for (int j = 0; j <= src[i].size() - range; j++)//jΪ�ۼ���ʼ��
			{
				for (int k = 0; k < range; k++)//rang��Χ���ۼ�
				{
					sum += src[i][j + k].y;
				}
				average = sum / range;
				dst[i][j + range/2].y = average;//j+range/2Ϊ����ֵ��
				sum = 0;
			}
			//0~range/2-1����
			for (int j = 0;j < range/2;j++)//jΪ����ֵ��
			{
				for (int k = 0;k < 2*j + 1;k++)//�ۼӷ�ΧΪ0~2j
				{
					sum += src[i][k].y;
				}
				average = sum / (2*j + 1);
				dst[i][j].y = average;
				sum = 0;
			}
			//range/2+1~src[i].size()-1����
			for (int j = src[i].size() - range + range/2 + 1;j < src[i].size();j++)//jΪ����ֵ��
			{
				for (int k = 2*j - src[i].size() + 1;k < src[i].size();k++)//�ۼӷ�ΧΪ2*j-src[i].size()+1~src[i].size()-1
				{
					sum += src[i][k].y;
				}
				average = sum / (2*src[i].size() - 2*j - 1);
				dst[i][j].y = average;
				sum = 0;
			}
		}
		return dst;

	}
	//-----------------------------------��showAndSaveImage()������----------------------------------  
	//      ��������ʾ�����ͼ��
	//		���룺ͼ�����ƣ�����λ��
	//		�������
	//---------------------------------------------------------------------------------------------- 
	static void  showAndSaveImage(const Mat &inputImage,string nameI,string placeI)
	{
		namedWindow(nameI,WINDOW_NORMAL);//��ʾԭʼͼ��
		imshow(nameI, inputImage);  
		imwrite(placeI+nameI+".jpg",inputImage);
	}

	//-----------------------------------��resizeMassage()������----------------------------------  
	//      �������ı�ģ����Ϣ�ĳߴ�
	//		���룺ԭʼ�㼯�����ű���
	//		������ı�����ĵ㼯
	//---------------------------------------------------------------------------------------------- 
	static vector<vector<Point3f>>  resizeMassage(const vector<vector<Point3f>> &src,float proportion)
	{
		vector<vector<Point3f>> dst;//����㼯
		int outputSize = (src[0].size()-1)*proportion+1;//����㼯����

		for (int i = 0;i<src.size();i++)//i:�м���
		{
			vector<Point3f> thisline;//����
			for (int jout =0;jout<outputSize;jout++ )//j:����㼯�м���
			{
				
				int xout,yout;//��ǰ�����������xy��x-j��y-i��
				float zout;//��ǰ����������ĳ�����Ϣ

				if (jout == 0)//��0����Ϣ����
				{
					yout = src[i][0].y * proportion;
					xout = src[i][0].x * proportion;
					zout = src[i][0].z * proportion;
				} 
				else//��0���������Ϣ����
				{
					if ( (jout/proportion) == int(jout/proportion))
						//���������Ӧ�����±������Ļ�������ֵ����ֹ����պ�Ϊ�����±�ʱ����
					{
						yout = src[i][int(jout/proportion)].y * proportion;
						xout = src[i][int(jout/proportion)].x * proportion;
						zout = src[i][int(jout/proportion)].z * proportion;
					} 
					else//������Ӧ�������±������Ļ�����ֵ��
					{
						float jin = jout/proportion;//��Ӧԭͼ�ϵ��±�
						int jin0 = jin,jin1 = jin+1;
						float k0 = jin1-jin;
						float k1 = jin - jin0;

						yout = (k0*src[i][jin0].y + k1*src[i][jin1].y) * proportion;
						xout = (k0*src[i][jin0].x + k1*src[i][jin1].x) * proportion;
						zout = (k0*src[i][jin0].z + k1*src[i][jin1].z) * proportion;

					}
				}
				thisline.push_back(Point3f(xout,yout,zout));//����˵�
			}
			dst.push_back(thisline);//���뱾��
		}
		return dst;
	}



};
#endif