/*
 * Multimedia DCT for Jpec Compression 
 * @Since September 9, 2015
 * @author Sahakorn Buangam , Computer Engineering.
 * @email sahakorn.new@gmail.com
 * @Instructor: A. Paisarn Muneesawang , Ph.D.
 *
 */
/*********************************************************************************************/
//		This Program using Visual Studio 2012 and Library Opencv 3.0 on Windows 8.1.		 //
//		I using Opencv for read a image (gray scale) and Convert to int array.				 //
//		If You want to change image can change path at img, Mat file.						 //
//		Copyright MyClass in Falculty of Computer Engineering Naresuan University.			 //
//																							 //
//		Compression Step: Using DCT															 //
//			1.) Read Image from file (Mat) and convert to int Array						     //
//			2.) Send to DCT tranfomer (Lossless)											 //
//			3.) The output of DCT transformation is a matrix of real numbers.				 //
//				We have a matrix of integer number for  Quantization ,q_50,q_10,q_90		 //
//			4.) Using Zigzag Algorithm for reorder of ouput of quantization					 //
//			5.) Convert to number to binary.												 //
//			6.) Using Run-Length Algorithm for Compression binary number.					 //
/*																							 //
/*********************************************************************************************/

#include <iostream>
#include <opencv2\opencv.hpp>  // For using read image 
#include <cmath>			   // For rounding 			
#include <math.h>			   // For using sqrt and cos();
#include<iomanip>

#define PI 3.14159265			// Difine pi value

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

	/******************************************	  Zizag Algorithym    ***********************************************/
	/*			 First Step get value top left of array[][]															*/
	/*			 Second  >> patern 1 , if column is odd, then row will increase and column will decrease index		*/
	/*					 >> patern 2 , if column is evnt, then row will decrease and column will increase index		*/
	/*			 Third   >> patern 1 , if column is odd, then column will increase and row will decrease index		*/
	/*					 >> patern 2 , if column is evnt, then column will decrease and row will increase index		*/
	/*																												*/
	/******************************************	  Zizag Algorithym    ***********************************************/
	int n = 0; // For increas index of decimal[]
	
	//--------- First Step ---------// 
	for(int i = 0;i<2;i++)
	{
		for(int j =0;j<2;j++){
			decimal[n] = pixelImg[i][j];
			n++;
			if(i == 1)
				break;
		}
	}
	//------ End of First Step -------// 


	//--------- Second Step ---------// 
	for(int i = 2;i<8;i++)
	{
			int k = 0,l= i;
			if(i%2 == 0){					// Check if Event number.
			while( k <=i){
				decimal[n] = pixelImg[k][l];
				n++;
				k++;
				l--;
				}
			}
			else{							// Odd number.
					while( k <=i){
				decimal[n] = pixelImg[l][k];
				n++;
				k++;
				l--;
				}
			}
		}
	//--------- End of Second Step ---------// 

	//--------- Third Step ---------// 
	int endloop = 1;
	int start = 7;
	for(int i = start;i>=1;i--)
	{
		int k = endloop,l= 7;
		if(endloop %2 == 1)					// Check if Odd number.
		{
			while(k <= start){
				decimal[n] = pixelImg[k][l];
				n++;
				l--;
				k++;
			}
		}
		else
		{                                      //  Event number.
		while(k <= start){
				decimal[n] = pixelImg[k][l];
				n++;
				l--;
				k++;
			}
		}
		endloop++;
	}
	//--------- End of Third Step ---------// 

	// Show value in reordered
	for(int i = 0;i<64;i++)
	{
		cout<<decimal[i]<< " ";
	}
	
}
void matrix(int num){
	// If num == 0 is T matrix of DCT x pixelImg
	// If num == 1 is T(transpot) of DCT x OUPUT of T x pixelImg
	// If num == 2 is Quantization

	if (num == 0){
	float temp = 0.0;
	for(int k = 0;k<8;++k){
		for(int l =0;l<8;++l){
			for(int n = 0;n<8;n++)
			{
				temp += T[k][n]*pixelImg[n][l];
			}
			val_matrix[k][l] = temp;
			temp = 0;
		}
		}
	} // End of num = 0
	else if(num == 1)
	{
		 cout<<endl<<"********* Real number Value ***********"<<endl;
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
			cout<<tranform[k][l]<<"\t";
			temp = 0;
		}
		cout<<endl;

	}
	}// End of num = 1
	else if(num == 2)
	{
		 cout<<endl<<"********* Quantization ***********"<<endl;
		float temp = 0.0;
	for(int k = 0;k<8;++k){
		for(int l =0;l<8;++l){
			qunt[k][l] = int(tranform[k][l]/q_50[k][l]);
			cout<<qunt[k][l]<<"\t";
			
		}
		cout<<endl;

	}
	}// End of num = 2
}

// =============== C value of DCT ==================== //
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
// ================================================ //

void crate_t(){
  cout<<endl<<"********* Tranform Matrix ***********"<<endl;
	for(int m = 0;m<N;m++)
	{
		for(int n = 0;n<N;n++)
		{
			float val = m*(2*n+1);
			T[m][n] = _co(m,n)*cos(PI*val/16);
			cout << std::fixed;
			cout << std::setprecision(2);
			cout<<T[m][n]<<"\t";
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
			cout<<T_[m][n]<<"\t";
		}
		cout<<endl;
	}
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
			
			cout << static_cast<int>(img.at<uchar>(j, i)) <<"\t";
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
	cout<<endl<<"******************** Quantization 50 *******************"<<endl;
	int n = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			q_50[j][i] = data[n];
			cout<<q_50[j][i]<<"\t";
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