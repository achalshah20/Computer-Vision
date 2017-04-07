//
// Watermark.cpp : Add watermark to an image, or inspect if a watermark is present.
//
// Based on skeleton code by D. Crandall, Spring 2017
//
// PUT YOUR NAMES HERE
//
//

//Link to the header file
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <SImage.h>
#include <SImageIO.h>
#include <fft.h>
#include <time.h>
#include <vector>
#include <math.h>

using namespace std;

// This code requires that input be a *square* image, and that each dimension
//  is a power of 2; i.e. that input.width() == input.height() == 2^k, where k
//  is an integer. You'll need to pad out your image (with 0's) first if it's
//  not a square image to begin with. (Padding with 0's has no effect on the FT!)
//
// Forward FFT transform: take input image, and return real and imaginary parts.
//
void fft(const SDoublePlane &input, SDoublePlane &fft_real, SDoublePlane &fft_imag)
{
  fft_real = input;
  fft_imag = SDoublePlane(input.rows(), input.cols());

  FFT_2D(1, fft_real, fft_imag);
}

// Inverse FFT transform: take real and imaginary parts of fourier transform, and return
//  real-valued image.
//
void ifft(const SDoublePlane &input_real, const SDoublePlane &input_imag, SDoublePlane &output_real)
{
  output_real = input_real;
  SDoublePlane output_imag = input_imag;

  FFT_2D(0, output_real, output_imag);
}

// Find correlation between vectors
double find_corr(vector<double> c,vector<int> v){

    int n = c.size();
    double sum_x =0,sum_y=0,sum_xy=0,sum_x2=0,sum_y2 = 0;
    for(int i = 0; i < c.size(); i++){
            sum_x += c[i];
            sum_x2 += pow(c[i],2);
    }

   for(int i = 0; i < v.size(); i++){
            sum_y += v[i];
            sum_y2 += pow(v[i],2);
    }

    for(int i = 0; i< n; i++){
        sum_xy += c[i] * v[i];
    }

    double coeff = (n * sum_xy - (sum_x*sum_y))/sqrt((n * sum_x2 - pow(sum_x,2)) * (n * sum_y2 - pow(sum_y,2)));

    return coeff;
}

// Write this in Part 1.1
SDoublePlane fft_magnitude(const SDoublePlane &fft_real, const SDoublePlane &fft_imag)
{
    cout << "FFT real size" << fft_real.rows() << endl;
    int N = fft_real.rows();
    SDoublePlane spect(N,N);

    for(int i=0;i<N;i++){
        double *spect_ptr = spect[i];
        double *fft_r_ptr = fft_real[i];
        double *fft_i_ptr = fft_imag[i];

        for(int j=0;j<N;j++){


            spect_ptr[j] =  log(sqrt(pow(fft_r_ptr[j],2)  + pow(fft_i_ptr[j],2)));

        }

    }


    return spect;
}

// Scale SDoublePlane between 0-255 pixels
SDoublePlane convertToImage(const SDoublePlane &input){

int N = input.cols();
    for(int i=0;i<N;i++){
        double *ptr = input[i];

        double min_old = ptr[0];
        double max_old = ptr[0];

         for(int j=0;j<N;j++){


            if(ptr[j] < min_old){
                min_old = ptr[j];
            }

            if(ptr[j] > max_old){
                max_old = ptr[j];
            }
        }

        for(int j=0;j<N;j++){

                //
                ptr[j] = (((ptr[j] - max_old) * 255.0) / (max_old - min_old)) + 255.0;
                //cout << ptr[j] << " ";
        }
    }

    return input;

}

// Write this in Part 1.2
SDoublePlane remove_interference(const SDoublePlane &input);

// Write this in Part 1.3 -- add watermark N to image
SDoublePlane mark_image(const SDoublePlane &input, int N);

// Write this in Part 1.3 -- check if watermark N is in image
SDoublePlane check_image(const SDoublePlane &input, int N);


int main(int argc, char **argv)
{
  try {

    if(argc < 4)
      {
	cout << "Insufficent number of arguments; correct usage:" << endl;
	cout << "    p2 problemID inputfile outputfile" << endl;
	return -1;
      }

    string part = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];
    cout << "In: " << inputFile <<"  Out: " << outputFile << endl;

    SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str());

    SDoublePlane fft_real,fft_imag;

    // Take a fft of image, it will return 2 parts (real & imaginary)
    fft(input_image,fft_real,fft_imag);

    //SDoublePlane fft_real_image = convertToImage(fft_real);
    //SDoublePlane fft_imag_image = convertToImage(fft_imag);

    //SImageIO::write_png_file("fft_real_image.png",fft_real_image,fft_real_image,fft_real_image);
    //SImageIO::write_png_file("fft_imag_image.png",fft_imag_image,fft_imag_image,fft_imag_image);

        int N = fft_imag.cols();
        double PI = 3.14;

        // Number of bins should be same as N
        int bins = 0;
        for(double r = 100.0;r>0.0;r=r-0.5){

                 for(double theta=0;theta<=2 * PI;theta+=0.01){


            // Find points on circle
            int x = int(N/2 + r * cos(theta));
            int y = int(N/2 + r * sin(theta));

//            // Find same points on other side as it is symmetry
//            int x_rev = int(N/2 + r*cos(theta + PI));
//            int y_rev =int(N/2 + r*sin(theta + PI));

            double* ptr_real = fft_real[x];
            double* ptr_img = fft_imag[x];


            //cout << ptr[y] << " ";

            cout << y << " ";
            //Replace some of the points with new value
            ptr_real[y] = 0.0;
//            ptr_real[y_rev] = 0.0;

            ptr_img[y] = 0.0;
//            ptr_img[y_rev] = 0.0;

        }

        }

         SDoublePlane new_imag;
    ifft(fft_real,fft_imag,new_imag);

    SImageIO::write_png_file("low_a2.png",new_imag,new_imag,new_imag);

        //Calculate the log magnitude of real & imaginary parts
        SDoublePlane spect = fft_magnitude(fft_real,fft_imag);

        //cout << spect[0][0] << endl;
        // Convert log magnitude values to image pixels
        SDoublePlane spect_img = convertToImage(spect);

        // Save spectrogram of image
        SImageIO::write_png_file("spect_low.png",spect_img,spect_img,spect_img);


        SDoublePlane input_image_h = SImageIO::read_png_file(inputFile.c_str());

        SDoublePlane fft_real_h,fft_imag_h;

        // Take a fft of image, it will return 2 parts (real & imaginary)
        fft(input_image_h,fft_real_h,fft_imag_h);


        // Number of bins should be same as N
        for(double r = 511.0;r>256.0;r=r-0.5){

            for(double theta=0;theta<=2 * PI;theta+=0.01){


            // Find points on circle
            int x = int(N/2 + r * cos(theta));
            int y = int(N/2 + r * sin(theta));

            // Find same points on other side as it is symmetry
            int x_rev = int(N/2 + r*cos(theta + PI));
            int y_rev =int(N/2 + r*sin(theta + PI));

            double* ptr_real_h = fft_real_h[x];
            double* ptr_img_h = fft_imag_h[x];


            //cout << ptr[y] << " ";

            cout << y << " ";
            //Replace some of the points with new value
            ptr_real_h[y] = 0.0;
            //ptr_real_h[y_rev] = 0.0;

            ptr_img_h[y] = 0.0;
            //ptr_img_h[y_rev] = 0.0;


        }

        }

    SDoublePlane new_imag_h;
    ifft(fft_real_h,fft_imag_h,new_imag_h);

    SImageIO::write_png_file("high_a2.png",new_imag_h,new_imag_h,new_imag_h);

    SDoublePlane spect_h = fft_magnitude(fft_real_h,fft_imag_h);

        //cout << spect[0][0] << endl;
        // Convert log magnitude values to image pixels
        SDoublePlane spect_img_h= convertToImage(spect_h);

        // Save spectrogram of image
        SImageIO::write_png_file("spect_high.png",spect_img_h,spect_img_h,spect_img_h);


//        // Calculate the log magnitude of real & imaginary parts
//        SDoublePlane spect = fft_magnitude(fft_real,fft_imag);
//
//        //cout << spect[0][0] << endl;
//        // Convert log magnitude values to image pixels
//        SDoublePlane spect_img = convertToImage(spect);
//
//        // Save spectrogram of image
//        SImageIO::write_png_file("spect.png",spect_img,spect_img,spect_img);


	//Morphing
	SDoublePlane low_a1 = SImageIO::read_png_file("low_a1.png");
	SDoublePlane low_a2 = SImageIO::read_png_file("low_a2.png");
	SDoublePlane high_a1 = SImageIO::read_png_file("high_a1.png");
	SDoublePlane high_a2 = SImageIO::read_png_file("high_a2.png");
	SDoublePlane morphedimg(1024,1024);
	for(int i=0;i<1024;i++){
		double* ptr_final = morphedimg[i];
		double* ptr1 = low_a1[i];
		double* ptr2 = high_a2[i];

			for(int j=0;j<1024;j++){

				ptr_final[j] = (ptr1[j] + ptr2[j])/2;
			}

	}

	SImageIO::write_png_file("morphedimg.png",morphedimg,morphedimg,morphedimg);






//    for(int i=0;i<512;i++){
//
//             for(int j=0;j<512;j++){
//
//                fft_real[i][j] =
//
//    }
//
//    }


    //for(int j =0;j < 512 ; j++){


//        // Calculate the log magnitude of real & imaginary parts
//        SDoublePlane spect = fft_magnitude(fft_real,fft_imag);
//
//        //cout << spect[0][0] << endl;
//        // Convert log magnitude values to image pixels
//        SDoublePlane spect_img = convertToImage(spect);
//
//        // Save spectrogram of image
//        SImageIO::write_png_file("spect.png",spect_img,spect_img,spect_img);



  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}








