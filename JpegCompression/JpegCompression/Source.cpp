#include <iostream>
#include <opencv2\opencv.hpp>
#include <cmath>
#include <math.h>  
#define PI 3.14159265
using namespace std;
using namespace cv;

float _co(int m , int n);
void crate_t();

	void matrix(int num);
	void Q_10();
	void Q_50();
	void Q_90();

	int q_10[8][8];
	int q_50[8][8];
	int q_90[8][8];
	int pixelImg[8][8];
	int N = 8;
	float T[8][8];
	float T_[8][8];
	float val_matrix[8][8];
	float tranform[8][8];
	float qunt[8][8];
	int decimal[64];

void zigzag(){
	int n = 0;
	for(int i = 0;i<2;i++)
	{
		for(int j =0;j<2;j++){
			decimal[n] = pixelImg[i][j];
			n++;
			if(i == 1)
				break;
		}
	}
	for(int i = 2;i<8;i++)
	{
		
			int k = 0,l= i;
			if(i%2 == 0){
			while( k <=i){
				decimal[n] = pixelImg[k][l];
				//cout<<decimal[n]<< " ";
				n++;
				k++;
				l--;
			}
			}
			else{
					while( k <=i){
				decimal[n] = pixelImg[l][k];
				//cout<<decimal[n]<< " ";
				n++;
				k++;
				l--;
			}
			}
		}
	
	cout<<endl<<"****************"<<endl;

	int endloop = 1;
	int start = 7;
	for(int i = start;i>=1;i--)
	{
		int k = endloop,l= 7;
		if(endloop %2 == 1)
		{
			while(k <= start){
				decimal[n] = pixelImg[k][l];
			//	cout<<decimal[n]<< " ";
				n++;
				l--;
				k++;
			}
		}
		else
		{
			while(k <= start){
				decimal[n] = pixelImg[k][l];
			//	cout<<decimal[n]<< " ";
				n++;
				l--;
				k++;
			}
		}
		endloop++;
	}
	for(int i = 0;i<64;i++)
	{
		cout<<decimal[i]<< " ";
	}
	
}
void matrix(int num){

	if (num == 0){
	float temp = 0.0;
	for(int k = 0;k<8;++k){
		for(int l =0;l<8;++l){
			for(int n = 0;n<8;n++)
			{
				temp += T[k][n]*pixelImg[n][l];
			}
			val_matrix[k][l] = temp;
			//cout<<val_matrix[k][l]<< " ";
			temp = 0;
		}
		//cout<<endl;

	}
	}
	else if(num == 1)
	{
			float temp = 0.0;
	for(int k = 0;k<8;++k){
		for(int l =0;l<8;++l){
			for(int n = 0;n<8;n++)
			{
				temp += val_matrix[k][n]*T_[n][l];
			}
			if(temp > - 0.1 && temp <0)
				temp = 0;
			
				
			tranform[k][l] = floor(temp);
			cout<<tranform[k][l]<< " ";
			temp = 0;
		}
		cout<<endl;

	}
	}
	else if(num == 2)
	{
		 cout<<endl<<"********* Quantization ***********"<<endl;
		float temp = 0.0;
	for(int k = 0;k<8;++k){
		for(int l =0;l<8;++l){
			qunt[k][l] = int(tranform[k][l]/q_50[k][l]);
			
			cout<<qunt[k][l]<< " ";
			
		}
		cout<<endl;

	}
	}
}
float _co(int m,int n){
		// m = 0 
		if( m == 0)
		{
			return sqrtf(1.0/N);
		}
		else if(m > 0 )
		{
			return sqrtf(2.0/N);
		}
}
void crate_t(){

  cout<<endl<<"********* Tranform Matrix ***********"<<endl;
	for(int m = 0;m<N;m++)
	{
		for(int n = 0;n<N;n++)
		{
			float val = m*(2*n+1);
			T[m][n] = _co(m,n)*cos(PI*val/16);
			cout<<T[m][n]<<" ";
		}
		cout<<endl;
	}
	 cout<<endl<<"********* Tranform transpost Matrix ***********"<<endl;
	for(int m = 0;m<N;m++)
	{
		for(int n = 0;n<N;n++)
		{
			float val =n*(2*m+1);
			T_[m][n] = _co(n,m)*cos(PI*val/16);
			cout<<T_[m][n]<<" ";
		}
		cout<<endl;
	}
	 cout<<"******************************************"<<endl;
}
int  main(){
	Mat img = imread("C:/Users/sahakornb/Documents/2015/Multimedia/JpegCompress/OpenCV_Jpeg_8PixelCompression/Image8Pixel.jpg",0);
	cout<<endl<<"********* Convert Picture to digital Pixel (0-255) ***********"<<endl<<endl;
	int ddd[64] = {20,30,40,50,60,70,80,90,
	30,40,50,60,70,80,90,100,
	40,50,60,70,80,90,100,110,
	50,60,70,80,90,100,110,120,
	60,70,80,90,100,110,120,130,
	70,80,90,100,110,120,130,140,
	80,90,100,110,120,130,140,150,
	90,100,110,120,130,140,150,160};
	int ni = 0;
	for (int i = 0; i < img.rows; i++)
	{
	
		for (int j = 0; j < img.cols; j++)
		{
			
			cout << static_cast<int>(img.at<uchar>(j, i)) << " ";
			pixelImg[j][i] = static_cast<int>(img.at<uchar>(j, i));
			
			//pixelImg[j][i] = ddd[ni];
			//cout << " "<<pixelImg[j][i] << " ";
			//ni++;
			
		

		}
		cout << endl;
	}
    crate_t();
	matrix(0);
	matrix(1);
	Q_50();
	matrix(2);
	zigzag();
	getchar();
}
void Q_10()
{

	int data[64] = { 80, 60, 50, 80, 120, 200, 255, 255,
		55, 60, 70, 95, 130, 255, 255, 255,
		70, 65, 80, 120, 200, 255, 255, 255,
		70, 85, 110, 145, 255, 255, 255, 255,
		90, 110, 185, 255, 255, 255, 255, 255,
		120, 175, 255, 255, 255, 255, 255, 255,
		245, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255 };
	int n = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			q_10[j][i] = data[n];
			n++;
		}

	}

}
void Q_50()
{
	int data[64] = { 16,11,10,16,24,40,51,61,
	12,12,14,19,26,58,60,55,
	14,13,16,24,40,57,69,56,
	14,17,22,29,51,87,80,62,
	18,22,37,56,68,109,103,77,
	24,35,55,64,81,104,113,92,
	49,64,78,87,103,121,120,101,
	72,92,95,98,112,110,103,99};
	cout<<endl<<"***************************** Q50 *******************"<<endl;
	int n = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			q_50[j][i] = data[n];
			cout<<q_50[j][i]<<" ";
			n++;
		}
		cout<<endl;

	}
	cout<<endl;

}
void Q_90()
{
	int data[64] = { 80, 60, 50, 80, 120, 200, 255, 255,
		55, 60, 70, 95, 130, 255, 255, 255,
		70, 65, 80, 120, 200, 255, 255, 255,
		70, 85, 110, 145, 255, 255, 255, 255,
		90, 110, 185, 255, 255, 255, 255, 255,
		120, 175, 255, 255, 255, 255, 255, 255,
		245, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255 };
	int n = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			q_10[j][i] = data[n];
			n++;
		}

	}


}