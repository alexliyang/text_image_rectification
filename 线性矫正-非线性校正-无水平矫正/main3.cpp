/*����У�����з�����У��
����˵��������ͼƬ����СͼƬ���д����ٻָ���Ϣ���н���
���ڣ�2016.4.29

�������ţ�
_o:ԭʼͼ������
_e:�����Ӵֵ�ͼ������
_r:��תͼ������
_h:ˮƽ����ͼ������
_s:�������˲��㼯����
_c:ȥ�����˸��Ŷε㼯����
_i:������ֵ�ĵ㼯����


����RGBͨ����ɫ��
Scalar(255,0,0)		����ɫ
Scalar(0,255,0)		����ɫ
Scalar(0,0,255)		����ɫ
Scalar(0,255,255)	����ɫ
Scalar(255,0,255)	����ɫ
Scalar(255,255,0)	������
Scalar(255,255,255)	����ɫ

�Զ�����������˵����
vector<vector<Point3f>> RES;
*      RESΪ2ά������RES.size()����������RES[i].size()����ÿ�е���Ŀ
*      RES[i][j]Ϊ��i�е�j���㣬��������Point,���� RES[i][j].x��  
*      RES[i][j].y ��RES[i][j].z���x,y�����Լ��Զ��峤��z��
*      һ�㣬RES�����ݾ���˳�����У���Ϊ������Ϊ˳��

*/
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "zjclass.h"
#include "zjmethod.h"
#include "jlclass.h"

using namespace cv;
using namespace std;

//-----------------------------------�����ֲ����������֡�---------------------------------------
//		����������������ʹ�õĲ��ֲ���
//-----------------------------------------------------------------------------------------------
#define figureName "../0image_and_header/1.jpg"//����ͼ�������
#define targetHeight 800.0//����ͼ��ĸ߶�//�޸�
#define Kpaperheight 0.8//ֽ�Ÿ߶���ͼƬ�߶ȵı���
#define CameraHeight 0.3//��Ӱ�߶ȣ�m��
#define PaperHeight 0.25//ֽ�Ÿ߶ȣ�m��
#define BaselineNumber 4//�г��ȼ����׼��
#define max(a,b) (((a)>(b))?(a):(b))//�޸�

//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-----------------------------------------------------------------------------------------------
int main( )
{
	//-----------------------------------������У�����֡�--------------------------------------------
	//		��������ԭͼ��������У��
	//-----------------------------------------------------------------------------------------------

	//��1����������
	Mat origImage_o,		//ԭʼͼ��
		resizeImage_o,		//�ı�ߴ��ͼ��//fixed5.2
		grayImage_o,		//�Ҷ�ͼ
		thresholdImage_o,	//��ֵͼ
		medianBlurImage_o,	//��ֵ�˲�ͼ��
		medianBlurImage_o_e,//��չ����ֵ�˲�ͼ��
		lineateImage_o,		//��תǰЧ��ͼ�������ߣ�
		rotatedImage_r,		//��ת��Ч��ͼ
		rotatedImage_r_o,	//��ת���ԭͼ//fixed5.2
		medianBlurImage_r;	//��ת����ֵ�˲����ֵͼ
	float resizeProportion;//ͼ��ı����//fixed5.2
	int i;				//���ü�������
	int blockSize;		//�ֲ���ֵ����ֵ���㴰�ڱ߳�������Ϊ����
	int ksize;//��ֵ�˲����ڱ߳�������Ϊ����
	vector<Vec4i> vlines;//Ϊ�������ߣ�����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������(�У��У��У���)
	VerBaseLine vbasel;	//Ѱ�Ҵ�ֱ���߶ε���
	Vec4i	vlin,			//resizeͼƬ�Ĵ�ֱ���߶�
			vlin_o;			//ԭʼͼƬ�Ĵ�ֱ���߶�//fixed5.2
	RotateImage rotatedIm,	//ͼ����ת��
				rotatedIm_o,//ԭʼͼ����ת��//fixed5.2
				rotatedIm_t;//��ֵͼ����ת��
	int disp_flag1;		//������У�����֡���ʾ�������־

	//��2������ԭʼͼ
	origImage_o = imread(figureName);
	if(!origImage_o.data ) { cout<<"��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ����ͼƬ����~"<<endl;getchar(); return false; }

	//��2.1���ı�ͼ���С//fixed5.2
	resizeProportion = targetHeight/origImage_o.rows;//ͼ��ı����

	Size outputSize = Size(origImage_o.cols*resizeProportion,targetHeight);
	resize(origImage_o, resizeImage_o, outputSize);//fixed5.2

	cout<<"ͼƬ������"<<resizeImage_o.cols<<endl;//2445
	cout<<"ͼƬ������"<<resizeImage_o.rows<<endl;//3264

	//��3��ת��Ϊ�Ҷ�ͼ�����⻯
	cvtColor( resizeImage_o, grayImage_o, CV_BGR2GRAY );//ת��Ϊ�Ҷ�ͼ
	equalizeHist( grayImage_o, grayImage_o );//�Ҷ�ͼ���⻯

	//��4�����оֲ���ֵ��
	blockSize = (grayImage_o.cols+grayImage_o.rows)*0.5*0.05;//��ֵ���㴰�ڱ߳�������Ϊ����
	if (blockSize%2==0){blockSize++;}
		//cout<<"blockSize:"<<blockSize<<endl;
	adaptiveThreshold(grayImage_o,thresholdImage_o,255,CV_ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV, blockSize, 20);
		/*	����5
			For the method ADAPTIVE_THRESH_MEAN_C , the threshold value T(x,y) is a mean of the neighborhood of (x, y) minus C .
			For the method ADAPTIVE_THRESH_GAUSSIAN_C , the threshold value T(x, y) is a weighted sum (cross-correlation with a 
			Gaussian window) of the neighborhood of (x, y) minus C . The default sigma (standard deviation) is used for the specified blockSize . */

	//��5��������ֵ�˲���ȥ������������
	ksize = (thresholdImage_o.cols+thresholdImage_o.rows)*0.5/800;//�˲����ڱ߳�������Ϊ����
	if (ksize%2==0){ksize++;}
	if (ksize<1){ksize = 1;}//�޸�
	if (ksize>7){ksize = 7;}
	//cout<<"ksize:"<<ksize<<endl;
	if (ksize>1)//�޸�
	{
		medianBlur(thresholdImage_o,medianBlurImage_o,ksize);
	} 
	else
	{
		thresholdImage_o.copyTo(medianBlurImage_o);
	}
	

	//��6�����л����߱任���ֵͼ����չ
	i=0;//���㹫�ü�������
	medianBlurImage_o.copyTo(medianBlurImage_o_e);
	do 
	{
		//�޸� ����3
		HoughLinesP(medianBlurImage_o_e, vlines, medianBlurImage_o_e.rows*0.02, CV_PI/360, medianBlurImage_o_e.rows*0.1, medianBlurImage_o_e.rows*0.3, medianBlurImage_o_e.rows*0.006 );//���л���任
		/*	�����������Accumulator threshold parameter. Only those lines are returned that get enough votes (  ).
			������������double���͵�minLineLength����Ĭ��ֵ0����ʾ����߶εĳ��ȣ�������趨�����̵��߶ξͲ��ܱ����ֳ�����
			���߸�������double���͵�maxLineGap����Ĭ��ֵ0������ͬһ�е����֮���������������ľ���*/
		if (!vlines.data() && i<5)
		{
			medianBlurImage_o_e = ZjMethod::expandLine(medianBlurImage_o_e);//���û���ҵ����ʵ����ߣ�����չ��ֵͼ��
			i++;
		}
	} while (!vlines.data() && i<5);
	if (i==5)
	{
		cout<<"��ֵͼ����չ��Ч��û���ҵ�����"<<endl;
		getchar();
		return -1;
	}
		//cout<<"��ֵͼ����չ������"<<i<<endl;

	//��7��������ͼ�л��Ƴ�ÿ���߶�
	resizeImage_o.copyTo(lineateImage_o);//������ͼ���Ƹ�Ч��ͼ
	for( size_t i = 0; i < vlines.size(); i++ )
	{
		Vec4i l = vlines[i];
		line( lineateImage_o, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,255), 2, CV_AA);//point:(�У���)
	}

	//��8���ҳ�����
	vbasel = VerBaseLine(vlines,lineateImage_o);//��ʼ��Ѱ�һ�׼�߶ε���
	vbasel.drawOnImage(Scalar(0,0,255));//��ͼ�л�����׼�߶�

	//��9����*ԭͼ���ֵͼ*���з���任
	vlin = vbasel.baseSegment();//�ҵ�ԭʼͼƬ�Ĵ�ֱ���߶�
	rotatedIm = RotateImage(resizeImage_o,vlin);//������ת��
	rotatedIm_t = RotateImage(medianBlurImage_o,vlin);//������ת��
	rotatedImage_r = rotatedIm.trans1();//����ת�任
	medianBlurImage_r = rotatedIm_t.trans1();//����ת�任

	//---------------------ԭͼ��ת------------//fixed5.2
	vlin_o = Vec4i(vlin[0]/resizeProportion,vlin[1]/resizeProportion,vlin[2]/resizeProportion,vlin[3]/resizeProportion);
	rotatedIm_o = RotateImage(origImage_o,vlin_o);
	rotatedImage_r_o = rotatedIm_o.trans1();
	
	//��10����ʾͼ��
	cout<<"�Ƿ���ʾ�����������У�����֡���1��,0��:";
	cin>>disp_flag1;
	if (disp_flag1 ==1)
	{
		ZjMethod::showAndSaveImage(resizeImage_o,"��1ԭʼͼ��","../0image_and_header/");//��ʾԭʼͼ��
		ZjMethod::showAndSaveImage(grayImage_o,"��1�Ҷ�ͼ��","../0image_and_header/");//��ʾֱ��ͼ���⻯��ĻҶ�ͼ��
		ZjMethod::showAndSaveImage(thresholdImage_o,"��1�ֲ���ֵ��ͼ��","../0image_and_header/");//��ʾ�ֲ���ֵ�����ͼ��
		ZjMethod::showAndSaveImage(medianBlurImage_o,"��1��ֵ�˲�ͼ��","../0image_and_header/");//��ʾ��ֵ�˲�ͼ��
		ZjMethod::showAndSaveImage(lineateImage_o,"��1��תǰЧ��ͼ��","../0image_and_header/");//��ʾ��תǰЧ��ͼ
		ZjMethod::showAndSaveImage(rotatedImage_r,"��1��ת��Ч��ͼ��","../0image_and_header/");//��ʾ����Ч��ͼ
		ZjMethod::showAndSaveImage(medianBlurImage_r,"��1��ת���ֵͼ��","../0image_and_header/");//��ʾ���ն�ֵЧ��ͼ

	}

	cout<<"���ڼ��������Ե�"<<endl;
	waitKey(0);

	//-----------------------------------���з�����У������-ϸ��ͼ��----------------------------------------
	//		������������У�����ͼ���еõ�ϸ��ͼ��
	//-----------------------------------------------------------------------------------------------

	//��1����ת���������
	Mat floodFillImage_r,			//ȥ�����ߺ�Ķ�ֵͼ��
		morphologyImage_r,			//��̬ѧ�˲�ͼ��
		thinImage_r,				//ϸ����ֵͼ��
		rotatedWithThinImage_r;		//ϸ����ɫͼ��
	float unit;//��̬ѧ�˲���ͼƬ�߶�������ĵ�λ����//�޸�
	int disp_flag2;//��������У��-ϸ��ͼ�񲿷֡���ʾ�������־

	//��2��ȥ����ֵͼ�е�����
	medianBlurImage_r.copyTo(floodFillImage_r);
	floodFill(floodFillImage_r, rotatedIm.center, Scalar(0));
	/*	��һ��������InputOutputArray���͵�image, ����/���1ͨ����3ͨ����8λ�򸡵�ͼ�񣬾��������֮��Ĳ�������ָ����
		�ڶ��������� InputOutputArray���͵�mask�����ǵڶ����汾��floodFill����Ĳ�������ʾ������ģ,����Ӧ��Ϊ��ͨ����8λ�����Ϳ��϶�������ͼ�� image ���������ص��ͼ�񡣵ڶ����汾��floodFill��Ҫʹ���Լ�������Ĥ���������mask��������һ��Ҫ����׼���ò����ڴ˴�����Ҫע����ǣ���ˮ��䲻�������Ĥmask�ķ��������������磬һ����Ե������ӵ��������������Ϊ��Ĥ���Է�ֹ��䵽��Ե��ͬ���ģ�Ҳ�����ڶ�εĺ���������ʹ��ͬһ����Ĥ���Ա�֤�������򲻻��ص���������Ҫע����ǣ���Ĥmask���������ͼ������� mask ��������ͼ��(x,y)���ص����Ӧ�ĵ������Ϊ(x+1,y+1)��
		������������Point���͵�seedPoint����ˮ����㷨����ʼ�㡣
		���ĸ�������Scalar���͵�newVal�����ص㱻Ⱦɫ��ֵ�������ػ��������ص���ֵ��
		�����������Rect*���͵�rect����Ĭ��ֵ0��һ����ѡ�Ĳ�������������floodFill������Ҫ�ػ��������С�߽��������
		������������Scalar���͵�loDiff����Ĭ��ֵScalar( )����ʾ��ǰ�۲�����ֵ���䲿����������ֵ���ߴ�����ò�������������֮������Ȼ���ɫ֮���lower brightness/color difference�������ֵ�� 
		���߸�������Scalar���͵�upDiff����Ĭ��ֵScalar( )����ʾ��ǰ�۲�����ֵ���䲿����������ֵ���ߴ�����ò�������������֮������Ȼ���ɫ֮���lower brightness/color difference�������ֵ��
		�ڰ˸�������int���͵�flags��������־�����˲��������������֣��Ƚϸ��ӣ�����һ����ϸ������

		�Ͱ�λ����0~7λ�����ڿ����㷨����ͨ�ԣ���ȡ4 (4Ϊȱʡֵ) ���� 8�������Ϊ4����ʾ����㷨ֻ���ǵ�ǰ����ˮƽ����ʹ�ֱ��������ڵ㣻�����Ϊ 8�����������ڵ��⣬��������Խ��߷�������ڵ㡣
		�߰�λ���֣�16~23λ������Ϊ0 ������������ѡ���ʶ������ϣ�     

		FLOODFILL_FIXED_RANGE - �������Ϊ�����ʶ���Ļ����ͻῼ�ǵ�ǰ��������������֮��Ĳ����Ϳ��ǵ�ǰ���������������صĲҲ����˵�������Χ�Ǹ����ġ�
		FLOODFILL_MASK_ONLY - �������Ϊ�����ʶ���Ļ�����������ȥ���ı�ԭʼͼ�� (Ҳ���Ǻ��Ե���������newVal), ����ȥ�����ģͼ��mask���������ʶ��ֻ�Եڶ����汾��floodFill���ã����һ���汾����ѹ����û��mask������

		�м��λ���֣�������ڸ߰�λFLOODFILL_MASK_ONLY��ʶ�����Ѿ�˵�ĺ����ԣ���Ҫ�������Ҫ������롣Floodfill��flags�������м��λ��ֵ��������ָ���������ͼ���ֵ�ġ������flags�м��λ��ֵΪ0�����������1����䡣
		������flags������or��������������������|�������磬�������8������䣬�����̶�����ֵ��Χ�����������������Դͼ���Լ������ֵΪ38����ô����Ĳ�����������
		*/

	//��3��������̬ѧ�˲�
	unit = floodFillImage_r.rows*0.00025;//��ͼƬ�߶�������ĵ�λ����//�޸�



		//cout<<"unit:"<<unit<<endl;
	dilate(floodFillImage_r,morphologyImage_r,getStructuringElement(MORPH_RECT, Size(max(70*unit,1), max(2*unit,1))));//����//�޸�
	erode(morphologyImage_r,morphologyImage_r,getStructuringElement(MORPH_RECT, Size(max(1*unit,1), max(7*unit,1))));//��ʴ//�޸�

	//��4������ϸ��
	ZjMethod::thin(morphologyImage_r,thinImage_r,max(100*unit,1));//�޸�

	//��5����ʾϸ��ͼ��
	rotatedImage_r.copyTo(rotatedWithThinImage_r);
	jlmethod::showline(rotatedWithThinImage_r,thinImage_r,Scalar(255,0,0));//��ϸ��ͼ�񻭵���ת���ͼ����

	cout<<"�Ƿ���ʾ�������������У��-ϸ��ͼ�񲿷֡���1��,0��:";
	cin>>disp_flag2;
	if (disp_flag2 ==1)
	{
		ZjMethod::showAndSaveImage(floodFillImage_r,"��2ȥ�����ߺ�Ķ�ֵͼ��","../0image_and_header/");//��ʾȥ�����ߺ�Ķ�ֵͼ��
		ZjMethod::showAndSaveImage(morphologyImage_r,"��2��̬ѧ�˲�ͼ��","../0image_and_header/");//��ʾ��̬ѧ�˲�ͼ��
		ZjMethod::showAndSaveImage(thinImage_r,"��2ϸ����ֵͼ��","../0image_and_header/");//��ʾ��̬ѧ�˲�ͼ��
		ZjMethod::showAndSaveImage(rotatedWithThinImage_r,"��2ϸ����ɫͼ��","../0image_and_header/");//��ʾϸ����ɫͼ��
	}
	cout<<"���ڼ��������Ե�"<<endl;
	waitKey(0);

	//-----------------------------------���з�����У������-����ȡ��----------------------------------------
	//		��������ϸ����ͼ������ȡ��������
	//-----------------------------------------------------------------------------------------------

	//��1������ȡ��������
	Mat lineres_r,				//����ȡ��ֵͼ��
		rotatedWithLineImage_r;	//����ȡ��ɫͼ��
		
	vector<vector<Point3f>> RES_r;//��תͼ����������
	int disp_flag3;//��������У��-����ȡ���֡���ʾ�������־

	//��2������������ȡ
	RES_r=jlmethod::findline(thinImage_r);

	//��3����ʾ����ȡͼ��
	lineres_r = Mat(thinImage_r.rows,thinImage_r.cols,CV_8U,Scalar(0));//�����հ�ͼ��
	jlmethod::showline(lineres_r,RES_r,255);//������ȡ��������հ�ͼ����

	rotatedWithThinImage_r.copyTo(rotatedWithLineImage_r);
	jlmethod::showline(rotatedWithLineImage_r,RES_r,Scalar(0,0,255));//����ȡ���л�����ת���ͼ����

	cout<<"�Ƿ���ʾ�������������У��-����ȡ���֡���1��,0��:";
	cin>>disp_flag3;
	if (disp_flag3 ==1)
	{
		ZjMethod::showAndSaveImage(lineres_r,"��3����ȡ��ֵͼ��","../0image_and_header/");//��ʾ����ȡͼ��
		ZjMethod::showAndSaveImage(rotatedWithLineImage_r,"��3����ȡ��ɫͼ��","../0image_and_header/");//��ʾ��̬ѧ�˲�ͼ��
	}
	cout<<"���ڼ��������Ե�"<<endl;
	waitKey(0);

	//-----------------------------------���з�����У������-Ѱ��ˮƽ��׼�ߡ�----------------------------------------
	//		������������ȡ����������Ѱ��ˮƽ��׼��
	//-----------------------------------------------------------------------------------------------
	//��1����������
	Mat lineres_r_e,				//��չ�������ȡ��ֵͼ��
		horizontalBaselineImage_r;	//ˮƽ��׼����ȡ��ɫͼ��
	vector<Vec4i> hlines;//Ϊ����ˮƽ��׼�ߣ�����һ��ʸ���ṹlines2���ڴ�ŵõ����߶�ʸ������(�У��У��У���)
	Vec4i hlin;			//ˮƽ��׼�߶�
	HorBaseLine hbasel;//Ѱ��ˮƽ��׼�ߵ���
	int disp_flag4;//��������У��-Ѱ��ˮƽ��׼�ߡ���ʾ�������־

	//��2��������ȡ��ֵͼ����л����߱任����ȡ����չ
	i=0;//���㹫�ü�������
	lineres_r.copyTo(lineres_r_e);
	do 
	{
		HoughLinesP(lineres_r_e, hlines, 1, CV_PI/720, lineres_r_e.cols*0.1, lineres_r_e.cols*0.3, lineres_r_e.cols*0.006 );//���л���任
		/*	�����������Accumulator threshold parameter. Only those lines are returned that get enough votes (  ).
			������������double���͵�minLineLength����Ĭ��ֵ0����ʾ����߶εĳ��ȣ�������趨�����̵��߶ξͲ��ܱ����ֳ�����
			���߸�������double���͵�maxLineGap����Ĭ��ֵ0������ͬһ�е����֮���������������ľ���*/
		if (!hlines.data() && i<5)
		{
			lineres_r_e = ZjMethod::expandLine(lineres_r_e);//���û���ҵ����ʵ�ˮƽ��׼�ߣ�����չ��ȡ��
			i++;
		}
	} while (!hlines.data() && i<5);
	if (i==5)
	{
		cout<<"��ֵͼ����չ��Ч��û���ҵ�ˮƽ��׼��"<<endl;
		getchar();
		return -1;
	}
		//cout<<"��ȡ����չ������"<<i<<endl;

	//��3��������ͼ�л��Ƴ�ÿ���߶�
	rotatedWithLineImage_r.copyTo(horizontalBaselineImage_r);;//������ȡ��ɫͼ���Ƹ�ˮƽ��׼��ͼ
	for( size_t i = 0; i < hlines.size(); i++ )
	{
		Vec4i l = hlines[i];
		line( horizontalBaselineImage_r, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,0), 2, CV_AA);//point:(�У���)
	}

	//��4���ҳ�ˮƽ��׼��
	hbasel = HorBaseLine(hlines,horizontalBaselineImage_r);//��ʼ��Ѱ��ˮƽ��׼�ߵ���
	hlin = hbasel.baseSegment();//�ҵ���תͼ���ˮƽ��׼�߶�
	hbasel.drawOnImage(Scalar(0,255,255));//��ͼ�л�����׼�߶�

	//��5����ʾͼ��
	cout<<"�Ƿ���ʾ�������������У��-Ѱ��ˮƽ��׼�����������1��,0��:";
	cin>>disp_flag4;
	if (disp_flag4 ==1)
	{
		ZjMethod::showAndSaveImage(horizontalBaselineImage_r,"��4ˮƽ��׼����ȡ��ɫͼ��","../0image_and_header/");//��ʾˮƽ��׼����ȡ��ɫͼ��
	}
	cout<<"���ڼ��������Ե�"<<endl;
	waitKey(0);

	//-----------------------------------���з�����У������-ҳ�潨ģ�������----------------------------------------
	//		������������ȡ���������߽���ҳ�潨ģ�����
	//-----------------------------------------------------------------------------------------------

	//��1����������
	Mat modImage_r,			//��ת���ģ��ͼ��
		cutAndSmoothImage,	//ȥ�������������˸��Ŷβ������˲����ͼ��
		interpolationImage,	//��ֵ��ͼ��
		correctedImage;		//����������ͼƬ
	vector<vector<Point3f>> RES_h_c,//ȥ�������������˸��Ŷεĵ㼯
							RES_h_c_s,//ȥ�������������˸��Ŷβ�����˲��ĵ㼯
							RES_h_e,//��չ��ı�׼�㼯
							RES_h_i,//������ֵ��ĵ㼯
							RES_mod_h,//����ģ����Ϣ�ĵ㼯������˵��:1.x��2.ģ�͸߶ȣ�3.��
							RES_mod_h_s,//����ģ����Ϣ�������˲��ĵ㼯
							RES_leng_h;//��������ѧϰ�ĵ㼯������˵��:1.x��2.y��3.length
	int disp_flag5;//��������У��-ҳ�潨ģ���������ʾ�������־
	int minX,maxX;//��׼�㼯�Ķ˵�x����


	//��2��ȥ�������������˸��Ŷ�
	RES_h_c = jlmethod::cutEnds(RES_r,lineres_r.cols * 0.02);

	//��3��ȥ�����Ŷ˺�����˲�
	RES_h_c_s = ZjMethod::smooth(RES_h_c,lineres_r.cols*0.02);

	//��4����չΪ��׼����������
	minX = RES_r[0][0].x;
	maxX = RES_r[0][RES_r[0].size()-1].x;
	for (int i = 1;i<RES_r.size();i++)
	{
		if (RES_r[i][0].x < minX)//��minX
		{
			minX = RES_r[i][0].x;
		}
		if (RES_r[i][RES_r[i].size()-1].x > maxX)//��maxX
		{
			maxX = RES_r[i][RES_r[i].size()-1].x;
		}
	}

	RES_h_e = jlmethod::extendEnds(RES_h_c_s,minX,maxX);
	
	//��5����������չ�˶��κ�����ֵ
	RES_h_i = jlmethod::lineInterpolation(RES_h_e,lineres_r.cols * 0.05);

	//��6������ģ����Ϣ
	RES_mod_h = ZjMethod::modeling(RES_h_i,lineres_r.rows*Kpaperheight,hlin[1],CameraHeight,PaperHeight);
		//cout<<"RES_r.size() "<<RES_r.size()<<endl;
		//cout<<"lineres_r.rows*0.8 "<<lineres_r.rows*0.8<<endl;
		//cout<<"hlin[0]"<<hlin[0]<<endl;

	//��7��ģ����Ϣ�˲�
	RES_mod_h_s = ZjMethod::smooth(RES_mod_h,lineres_r.cols*0.06);//fixed5.2

	//��8�������г���
	RES_leng_h = ZjMethod::lengthCalculation(RES_h_i,RES_mod_h_s,BaselineNumber);
		//cout<<RES_leng_h[0]<<endl;


	//��8.1���ָ�������Ϣ------------------------------------------//fixed5.2
	vector<vector<Point3f>> RES_leng_h_ext;
	RES_leng_h_ext = ZjMethod::resizeMassage(RES_leng_h,1.0/resizeProportion);
	
	//��9������ҳ��
	correctedImage = Mat(rotatedImage_r_o.rows,rotatedImage_r_o.cols*1.5,rotatedImage_r_o.type(),Scalar(255,255,255));//�����հ�ͼ��
	ZjMethod::paperCorrection(rotatedImage_r_o,correctedImage,RES_leng_h_ext);

	//------------------------------------------------

	////��9������ҳ��
	//correctedImage = Mat(rotatedImage_r.rows,rotatedImage_r.cols*1.5,rotatedImage_r.type(),Scalar(255,255,255));//�����հ�ͼ��
	//ZjMethod::paperCorrection(rotatedImage_r,correctedImage,RES_leng_h);

	//��10����ʾͼ��
	modImage_r = Mat(lineres_r.rows,lineres_r.cols,CV_8U,Scalar(0));//�����հ�ͼ��
	jlmethod::showline(modImage_r,RES_mod_h_s,255);//������ȡ��������հ�ͼ����

	rotatedImage_r.copyTo(cutAndSmoothImage);
	jlmethod::showline(cutAndSmoothImage,RES_h_c_s,Scalar(0,255,0));//ȥ�������������˸��Ŷβ������˲����ͼ��

	cutAndSmoothImage.copyTo(interpolationImage);
	jlmethod::showline(interpolationImage,RES_h_i,Scalar(255,0,0));//������ֵ���ͼ��

	cout<<"�Ƿ���ʾ�������������У��-ҳ�潨ģ���������1��,0��:";
	cin>>disp_flag5;
	if (disp_flag5 ==1)
	{
		ZjMethod::showAndSaveImage(modImage_r,"��5�˲���ģ�Ͷ�ֵͼ��","../0image_and_header/");//��ʾģ��ͼ��
		ZjMethod::showAndSaveImage(cutAndSmoothImage,"��5ȥ�����˸��Ŷβ��˲���ͼ��","../0image_and_header/");//��ʾȥ�����˸��Ŷβ��˲���ͼ��
		ZjMethod::showAndSaveImage(interpolationImage,"��5������ֵ���ͼ��","../0image_and_header/");//��ʾ������ֵ���ͼ��
		ZjMethod::showAndSaveImage(correctedImage,"��5����ͼ��","../0image_and_header/");//��ʾ������ͼ��
	}
	cout<<"���ڼ��������Ե�"<<endl;
	waitKey(0);

	return 0;  
}
