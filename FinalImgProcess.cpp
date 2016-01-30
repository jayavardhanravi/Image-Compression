#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

// This function is used to check the Mat is continious or not
// Returns  TRUE - if continious
//			FALSE - if not continious
bool check_Cont_Mat(Mat mVar)
{
	if(mVar.isContinuous())
	{
		cout << "The Matrix data is continious" << endl;
		return true;
	}	
	else
	{
		cout << "The Matrix data is NOT continious" << endl;
		return false;
	}	
}

// This function is used to read the image to Mat form
// Returns Mat 'mat'
Mat read_Input_ImageData(string filePath)
{
	Mat mat = imread(filePath,CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_ANYCOLOR);
	return mat;
}

// This function is used to display the properties of image
// Returns void
void display_Image_prop(Mat mVar)
{
	int d = mVar.depth();
		string var;
		switch(d)
		{
			case 0 : 
			{
				var = "8-bit unsigned";
				break;
			}
			case 1 : 
			{
				var = "8-bit signed";
				break;
			}
			case 2 : 
			{
				var = "16-bit unsigned";
				break;
			}
			case 3 : 
			{
				var = "16-bit signed";
				break;
			}
			case 4 : 
			{
				var = "32-bit signed";
			}
			case 5 : 
			{
				var = "32-bit floating-point";
			}
			case 6 : 
			{
				var = "64-bit floating-point";
			}
			default :
			{
				var = "None of the case";
				break;
			}
		}
		cout << "The number of channels/depth for the image is : "<< var << " " << mVar.channels() << " channel" << endl;
		cout << "Rows = "<<mVar.rows<< " Columns = "<<mVar.cols<< endl;
}

// This function is used to compute the average pixel intensity
// Returns double 'api'
double avg_Pixel_Intensity(Mat m)
{
	double api,tot=0;
	double *arrayVar;
	arrayVar = new double[m.rows*m.cols];
	for(int i=0; i<m.rows; i++)
	{
		for(int j=0; j<m.cols; j++)
		{
			arrayVar[i+m.rows*j]=double(m.at<uchar>(i,j));
		}
	}
	for (int i=0;i<m.rows*m.cols;i++)
	 {
		tot+= arrayVar[i];
	 }
    api = tot/(m.rows*m.cols);
    cout << "The average pixel intensity of the image is "<< api <<endl;
	return api;
}

// This function is used to generate the diagonal matrix
// Returns Mat 'm'
Mat generate_Diag_Matrix(Mat W, Mat matObj)
{
	Mat m;
	m = matObj.zeros(W.rows,W.rows,CV_64FC1);
	for(int i=0;i<W.rows;i++)
	{
		m.at<double>(i,i)=W.at<double>(i);
	}
	return m;
}

// Start the main function
int main(int argc, char *argv[])
{
	// Defining the variables
	Mat myImage, myImage_gray, myImage_svd, W, U, VT, Wdiag, compressedImage;
	SVD svdObj;
	Mat matObj;
	Range rangeObj;
	bool checkCont, checkCont_gray;
	int rank;
	//double api, original_Api;
	// Reading the image file
	myImage = read_Input_ImageData(argv[1]);
	// Converting the image to grayscale
	cvtColor(myImage,myImage_gray,CV_RGB2GRAY);
	//original_Api = avg_Pixel_Intensity(myImage_gray);
	// checking if image data is loaded or not
	if(myImage.empty())
	{
		cout << "Loading the given image has FAILED !!!" << endl;
		return 0;
	}
	else
	{
		// Printing the properties of images
		display_Image_prop(myImage);
		display_Image_prop(myImage_gray);
		// Checking if image data is continious or not
		checkCont = check_Cont_Mat(myImage);
		checkCont_gray = check_Cont_Mat(myImage_gray);
		if(checkCont && checkCont_gray)
		{
			// Changing the property of the image
			myImage_gray.convertTo(myImage_svd, CV_64F,1.0/255);
			// Computing the SVD
			svdObj.compute(myImage_svd,W,U,VT);
			// Computing the diagonal matrix
			Wdiag =  generate_Diag_Matrix(W,matObj);
			rank = atoi(argv[2]);
			// Selecting the particular range of values
			Wdiag=Wdiag(Range(0,rank),Range(0,rank));
			U=U(rangeObj.all(),Range(0,rank));
			VT=VT(Range(0,rank),rangeObj.all());
			// Reconstructing the image data
			compressedImage = U*Wdiag*VT;
			//api = avg_Pixel_Intensity(compressedImage);
			cout << "Properties of W : # of rows = "<< W.rows << " # of cols = "<< W.cols <<endl;
			cout << "Properties of U : # of rows = "<< U.rows << " # of cols = "<< U.cols <<endl;
			cout << "Properties of VT : # of rows = "<< VT.rows << " # of cols = "<< VT.cols <<endl;
			// String manipulation for displaying the names
			//  for windows and files 
			stringstream x1,x2;
			string sVar = "Reconstructed for N : ";
			string sVar1 = "ReconImg";
			x1 << sVar << rank;
			x2 << sVar1 << rank << ".jpeg";
			String disp = x1.str();
			String pDisp = x2.str();
			// Creating the window to display
			namedWindow("Original Image",CV_WINDOW_AUTOSIZE);
			namedWindow("Input Gray Image",CV_WINDOW_AUTOSIZE);
			namedWindow(disp,CV_WINDOW_AUTOSIZE);
			// Displaying the image in window
			imshow("Original Image",myImage);
			imshow("Input Gray Image",myImage_gray);
			imshow(disp,compressedImage);
			// Save the image to disk
			compressedImage.convertTo(compressedImage, CV_8UC3, 255.0);
			imwrite(pDisp,compressedImage);
			// waiting for the key press
			waitKey(-1);
			// Destroying the window
			destroyWindow("Original Image");
			destroyWindow("Input Gray Image");
			destroyWindow(disp);
		}
		else
		{
			cout << "Failed to convert!!!" << endl;
			return 0;
		}
	}
	return 0;
}
