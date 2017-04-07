
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
double find_corr(vector<int> v,vector<double> c){

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

    return fabs(coeff);
}

// Write this in Part 1.1
SDoublePlane fft_magnitude(const SDoublePlane &fft_real, const SDoublePlane &fft_imag)
{
    //cout << "FFT real size" << fft_real.rows() << endl;
    SDoublePlane spect(512,512);

    for(int i=0;i<512;i++){
        double *spect_ptr = spect[i];
        double *fft_r_ptr = fft_real[i];
        double *fft_i_ptr = fft_imag[i];

        for(int j=0;j<512;j++){


            spect_ptr[j] =  log(sqrt(pow(fft_r_ptr[j],2)  + pow(fft_i_ptr[j],2)));

        }

    }


    return spect;
}

// Scale SDoublePlane between 0-255 pixels
SDoublePlane convertToImage(const SDoublePlane &input){

    for(int i=0;i<512;i++){
        double *ptr = input[i];

        double min_old = ptr[0];
        double max_old = ptr[0];

         for(int j=0;j<512;j++){


            if(ptr[j] < min_old){
                min_old = ptr[j];
            }

            if(ptr[j] > max_old){
                max_old = ptr[j];
            }
        }

        for(int j=0;j<512;j++){
                //
                //ptr[j] = (((ptr[j] - max_old) * 255.0) / (max_old - min_old)) + 255.0;
                ptr[j] = (((ptr[j] - min_old) * 255.0) / (max_old - min_old));
                //cout << ptr[j] << " ";
        }
    }

    return input;

}

// Write this in Part 1.2
SDoublePlane remove_interference(const SDoublePlane &input){

//    for(int i=0;i<512;i++){
//        double *input_ptr = input[i];
//
//        for(int j=0;j<512;j++){
//
//            double t = input_ptr[j];
//
//            if(fabs(t) > 1.44){
//                input_ptr[j] = 0.0;
//            }
//
//        }
//
//    }
//

    // Remove HI and IH which is appearing in spectrogram
    // Check top left and bottom right portion of fft components and remove them by replacing frequency value in previous row
    for(int i=0;i<220;i++){
        double *input_ptr = input[i];

        for(int j=0;j<220;j++){


            if(abs(input_ptr[j]) > 0.1){

                    input_ptr[j] = input_ptr[j-1];

//                    cout << i << " " << j << "";
//                    cout << endl;
            }

        }

    }

     for(int i=300;i<512;i++){
        double *input_ptr = input[i];

        for(int j=300;j<512;j++){


            if(abs(input_ptr[j]) > 0.1){
                    input_ptr[j] = input_ptr[j-1];

//                    cout << i << " " << j << "";
//                    cout << endl;
            }

        }

    }

    // Matlab analysis to check if pixel locations we found here is correct or not
    // HI
    // j = 157,160,163
    // i = 157:161

    // i = 159
    // j = 157:163

    //IH
    // j = 351,354,357
    // i = 353:357

    //i = 355
    // j = 351:357

    return input;


}

// Write this in Part 1.3 -- add watermark N to image
SDoublePlane mark_image(const SDoublePlane &input, int N)
{

    // Lets take 50 values
    int l = 360;
    int r = 128;
    double alpha = 5;
    double PI = 180;

    // Setting seed of random number generator
    srand(N);

    vector<int> v;


    int size_img = input.cols();



    for(int i = 0;i<l;i++){
        v.push_back(rand()%2);
        //cout << v[i] << " ";
    }


    // Extract real and imaginary part of image
    SDoublePlane fft_real,fft_imag;
    fft(input,fft_real,fft_imag);

    int nrow = fft_real.rows();
    int ncol = fft_real.cols();

    // Number of bins should be same as N
    int bins = 0;


    for(double theta=0;theta<=2*PI;theta+=1){

        //cout << bins << " ";

        // Find points on circle
        int x = size_img / 2 + r*cos(theta);
        int y = size_img /2 + r*sin(theta);

        // Find same points on other side as it is symmetry
        int x_rev = size_img/2 + r*cos(theta + PI);
        int y_rev = size_img/2 + r*sin(theta + PI);

        double *ptr_real = fft_real[x];
        double *ptr_img = fft_imag[x];

        //cout << alpha * fabs(ptr_real[y]) * v[bins];

        // Replace some of the points with new value
        ptr_real[y] = ptr_real[y] + alpha * fabs(ptr_real[y]) * v[bins];
        ptr_img[y] = ptr_img[y] + alpha * fabs(ptr_img[y]) * v[bins];

        // Find points until we reach max limit
        if(v.size()-1 == bins){
            break;
        }

        // Incrementing bin
        bins += 1;
    }

    // Number of bins should be same as N
    // Inverse FFT to store image
    SDoublePlane new_img;
    ifft(fft_real,fft_imag,new_img);

    return(new_img);

}

// Write this in Part 1.3 -- check if watermark N is in image
double check_image(const SDoublePlane &input, int N){

    // Lets take 50 values
    int l = 360;
    int r = 128;
    double alpha = 5;
    double PI = 180;

    // Setting seed of random number generator
    srand(N);


    int size_img = input.cols();


    // Extract real and imaginary part of image
    SDoublePlane fft_real,fft_imag;
    fft(input,fft_real,fft_imag);


    vector<int> v;

    for(int i = 0;i<l;i++){
            v.push_back(rand()%2);
    }

    vector<double> c;
    int bins = 0;



    for(double theta=0;theta<=2*PI;theta+=1){

        // Find points on circle
        int x = size_img / 2 + r*cos(theta);
        int y = size_img /2 + r*sin(theta);

        double *ptr_w = fft_real[x];

        c.push_back(ptr_w[y]);

        // Find points until we reach max limit
        if(v.size()-1 == bins){
            break;
        }

        // Incrementing bin
        bins += 1;
    }

    // Find correlation coefficient
    double coeff = find_corr(v,c);
    //cout << "Correlation:    " << coeff <<endl;

    return(coeff);


}


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
    //cout << "In: " << inputFile <<"  Out: " << outputFile << endl;




    if(part == "1.1")
      {
        SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str());

        SDoublePlane fft_real,fft_imag;

        // Take a fft of image, it will return 2 parts (real & imaginary)
        fft(input_image,fft_real,fft_imag);

        // Calculate the log magnitude of real & imaginary parts
        SDoublePlane spect = fft_magnitude(fft_real,fft_imag);

        //cout << spect[0][0] << endl;
        // Convert log magnitude values to image pixels
        SDoublePlane spect_img = convertToImage(spect);

        // Save spectrogram of image
        SImageIO::write_png_file(outputFile.c_str(),spect_img,spect_img,spect_img);
      }
    else if(part == "1.2")
      {
          // TODO: Remove noise from image

        SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str());

        SDoublePlane fft_real,fft_imag;
        int N = input_image.cols();
        //cout << N << endl;
        double PI = 3.142;

        // Take a fft of image, it will return 2 parts (real & imaginary)
        fft(input_image,fft_real,fft_imag);

        // Remove interference from image
        fft_real = remove_interference(fft_real);

        // Remove interference from image
        fft_imag = remove_interference(fft_imag);

        SDoublePlane new_imag;
        // Inverse FFT
        ifft(fft_real,fft_imag,new_imag);

        // Save image
        SImageIO::write_png_file(outputFile.c_str(),new_imag,new_imag,new_imag);

        // Reading cleaned image
        input_image = SImageIO::read_png_file(outputFile.c_str());

        fft_real,fft_imag;

        // Take a fft of image, it will return 2 parts (real & imaginary)
        fft(input_image,fft_real,fft_imag);

        // Calculate the log magnitude of real & imaginary parts
        SDoublePlane spect = fft_magnitude(fft_real,fft_imag);

        // Convert log magnitude values to image pixels
        SDoublePlane spect_img = convertToImage(spect);

        // Save spectrogram of image
        SImageIO::write_png_file("output/spect_after_noise_removal.png",spect_img,spect_img,spect_img);


      }
    else if(part == "1.3")
      {
            if(argc < 6)
              {
                cout << "Need 6 parameters for watermark part:" << endl;
                cout << "    p2 1.3 inputfile outputfile operation N" << endl;
                return -1;
              }
            string op(argv[4]);
            int N = atoi(argv[5]);

            if(op == "add")
              {
                SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str());

                SDoublePlane watermarked_img = mark_image(input_image,N);


                SImageIO::write_png_file(outputFile.c_str(),watermarked_img,watermarked_img,watermarked_img);


                    // Reading cleaned image
                input_image = SImageIO::read_png_file(outputFile.c_str());

                SDoublePlane fft_real,fft_imag;

                // Take a fft of image, it will return 2 parts (real & imaginary)
                fft(input_image,fft_real,fft_imag);

                // Calculate the log magnitude of real & imaginary parts
                SDoublePlane spect = fft_magnitude(fft_real,fft_imag);

                // Convert log magnitude values to image pixels
                SDoublePlane spect_img = convertToImage(spect);

                // Save spectrogram of image
                string spectName = "output/spect_"+ std::string(argv[5]) +".png";
                SImageIO::write_png_file(spectName.c_str(),spect_img,spect_img,spect_img);

              }
            else if(op == "check")
              {

                    SDoublePlane input_image = SImageIO::read_png_file(outputFile.c_str());

                    double corr = check_image(input_image,N);
                    //cout << "Correlation: " << corr << endl;
                    cout << corr;

                    if(corr > 0.20){
                        //cout << "Watermark present!! for N: " << N << endl;
                    }else{
                        //cout << "Watermark not present!! for N: " << N << endl;
                    }


              }

            else
              throw string("Bad operation!");

        }
            else
              throw string("Bad part!");

  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}








