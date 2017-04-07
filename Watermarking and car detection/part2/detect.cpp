//
// detect.cpp : Detect cars in satellite images.
//
// Based on skeleton code by D. Crandall, Spring 2017
//
// PUT YOUR NAMES HERE
//
//

#include <SImage.h>
#include <SImageIO.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// The simple image class is called SDoublePlane, with each pixel represented as
// a double (floating point) type. This means that an SDoublePlane can represent
// values outside the range 0-255, and thus can represent squared gradient magnitudes,
// harris corner scores, etc.
//
// The SImageIO class supports reading and writing PNG files. It will read in
// a color PNG file, convert it to grayscale, and then return it to you in
// an SDoublePlane. The values in this SDoublePlane will be in the range [0,255].
//
// To write out an image, call write_png_file(). It takes three separate planes,
// one for each primary color (red, green, blue). To write a grayscale image,
// just pass the same SDoublePlane for all 3 planes. In order to get sensible
// results, the values in the SDoublePlane should be in the range [0,255].
//

// Below is a helper functions that overlays rectangles
// on an image plane for visualization purpose.

// Draws a rectangle on an image plane, using the specified gray level value and line width.
//
void overlay_rectangle(SDoublePlane &input, int _top, int _left, int _bottom, int _right, double graylevel, int width)
{
  for(int w=-width/2; w<=width/2; w++) {
    int top = _top+w, left = _left+w, right=_right+w, bottom=_bottom+w;

    // if any of the coordinates are out-of-bounds, truncate them
    top = min( max( top, 0 ), input.rows()-1);
    bottom = min( max( bottom, 0 ), input.rows()-1);
    left = min( max( left, 0 ), input.cols()-1);
    right = min( max( right, 0 ), input.cols()-1);

    // draw top and bottom lines
    for(int j=left; j<=right; j++)
	  input[top][j] = input[bottom][j] = graylevel;
    // draw left and right lines
    for(int i=top; i<=bottom; i++)
	  input[i][left] = input[i][right] = graylevel;
  }
}

// DetectedBox class may be helpful!
//  Feel free to modify.
//
class DetectedBox {
public:
  int row, col, width, height;
  double confidence;
};

// Function that outputs the ascii detection output file
void  write_detection_txt(const string &filename, const vector<DetectedBox> &cars)
{
  ofstream ofs(filename.c_str());

  for(vector<DetectedBox>::const_iterator s=cars.begin(); s != cars.end(); ++s)
    ofs << s->row << " " << s->col << " " << s->width << " " << s->height << " " << s->confidence << endl;
}

// Function that outputs a visualization of detected boxes
void  write_detection_image(const string &filename, const vector<DetectedBox> &cars, const SDoublePlane &input)
{
  SDoublePlane output_planes[3];

  for(int p=0; p<3; p++)
    {
      output_planes[p] = input;
      for(vector<DetectedBox>::const_iterator s=cars.begin(); s != cars.end(); ++s)
	overlay_rectangle(output_planes[p], s->row, s->col, s->row+s->height-1, s->col+s->width-1, p==2?255:0, 2);
    }

  SImageIO::write_png_file(filename.c_str(), output_planes[0], output_planes[1], output_planes[2]);
}



// The rest of these functions are incomplete. These are just suggestions to
// get you started -- feel free to add extra functions, change function
// parameters, etc.

// Convolve an image with a separable convolution kernel
//
int getBorder(int width, int p)
{
    //If pixel value is less than 0 which means filter is outside image(left side) so use same pixel value as border of image
    // Reflect approach
    if(p < 0)
    {
        return -p - 1;
    }

     //If pixel value is less than image width which means filter is outside image(right side) so use same pixel value as border of image
    // Reflect approach
    if(p >= width)
    {
        return 2*width - p - 1;
    }
   return p;
}

// Convolve an image with a  convolution kernel
//
SDoublePlane convolve_general(const SDoublePlane &input, const SDoublePlane &filter)
{
  SDoublePlane output(input.rows(), input.cols());

  // Convol value
  double conValue;
  int x1,y1;

  cout << "rows: " << input.rows() << endl;
    cout << "cols: " << input.cols() << endl;

    for(int rowId = 0; rowId < input.rows(); rowId++){
        for(int colId = 0; colId < input.cols(); colId++){
            conValue = 0.0;

            for(int x = -1;x <= 1; x++){
                for(int y = -1;y <= 1; y++ ){

                    // Get border if filter is outside image
                    x1 = getBorder(input.cols(), colId - y);
                    y1 = getBorder(input.rows(), rowId - x);

                    double *filter_ptr = filter[x+1];
                    double *input_ptr = input[y1];

                    // Convol image with filter
                    conValue += filter_ptr[y+1] * input_ptr[x1];
                }
            }
            output[rowId][colId] = conValue;
        }
    }


  return output;
}

SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter)
{
  SDoublePlane output(input.rows(), input.cols());
SDoublePlane intermediate(input.rows(), input.cols());

    cout << "Row filter dimensions-> rows: " << row_filter.rows() << "cols: "<<row_filter.cols()  << endl;
    cout << "Column filter dimensions-> rows: " << col_filter.rows() << "cols: "<<col_filter.cols() << endl;

    for(int i =0 ; i< row_filter.rows();i++){
        for(int j=0;j<row_filter.cols();j++){
            double *ptr = row_filter[i];

            cout << ptr[j] << " ";
        }

        cout << endl;
    }

  // Convol value
  double rowFilterVal;
  int x1,y1;

    for(int rowId = 0; rowId < input.rows(); rowId++){
        for(int colId = 0; colId < input.cols(); colId++){
            rowFilterVal = 0.0;

            for(int x = -1;x <= -2+col_filter.rows(); x++){
                for(int y = -1;y <= -2+col_filter.cols(); y++ ){

                    // Get border if filter is outside image
                    x1 = getBorder(input.cols(), colId - y);
                    y1 = getBorder(input.rows(), rowId - x);

                    double *filter_ptr = col_filter[x+1];
                    double *input_ptr = input[y1];


                    // Convol image with filter
                    rowFilterVal += filter_ptr[y+1] * input_ptr[x1];
                }
            }


            //cout << rowFilterVal << endl;
            intermediate[rowId][colId] = rowFilterVal;
        }
    }

  // Convol value
  double colFilterVal;

    for(int rowId = 0; rowId < intermediate.rows(); rowId++){
        for(int colId = 0; colId < intermediate.cols(); colId++){
            colFilterVal = 0.0;

            for(int x = -1;x <= -2+row_filter.rows(); x++){
                for(int y = -1;y <= -2+row_filter.cols(); y++ ){

                    // Get border if filter is outside image
                    x1 = getBorder(intermediate.cols(), colId - y);
                    y1 = getBorder(intermediate.rows(), rowId - x);

                    double *filter_ptr = row_filter[x+1];
                    double *inter_ptr = intermediate[y1];

                    //cout << filter_ptr[y+1] << endl;


                    // Convol image with filter
                    colFilterVal += filter_ptr[y+1] * inter_ptr[x1];
                }
            }
            //cout << colFilterVal << endl;
            output[rowId][colId] = colFilterVal;
        }
    }



  // Convolution code here

  return output;
}

SDoublePlane binary_image_method(const SDoublePlane &input)
{
  SDoublePlane binary_image(input.rows(), input.cols());
  SDoublePlane intermediate_result(input.rows(), input.cols());

  // create binary image
  for(int i=0;i<binary_image.rows();i++){
    for(int j=0;j<binary_image.cols();j++){
      if (input[i][j] < 75 ) {
        binary_image[i][j] = 0;
      } else {
        binary_image[i][j] = 255;
      }

      intermediate_result[i][j] = 0;
    }
  }

  // fill in the shapes
  // to make every shape as a convex polygon
  int change = 1;
  while (change != 0) {
    change = 0;
    for(int i=1;i<binary_image.rows()-1;i++){
      for(int j=1;j<binary_image.cols()-1;j++){

        if (binary_image[i][j] == 255) {
          if (binary_image[i+1][j-1] +
          binary_image[i+1][j] +
          binary_image[i+1][j+1] +
          binary_image[i][j-1] +
          binary_image[i][j+1] +
          binary_image[i-1][j-1] +
          binary_image[i-1][j] +
          binary_image[i-1][j+1] < 1275
          )
          {
            binary_image[i][j] = 0;
            change++;
          }
        }
      }
    }
  }

  //Open the file to write the results.
  ofstream detect_file ("detect.txt");
  if (detect_file.is_open()) {

    // trying to find rectangular shape dimensions similar to car
    int up=0, down=0, right=0, left = 0, x=0, y=0;
    for(int i=1;i<binary_image.rows()-1;i++){
      for(int j=1;j<binary_image.cols()-1;j++){
        if(binary_image[i][j] == 0) {
          up = i;
          while (up>=0 && binary_image[up][j] == 0) {
            up--;
          }
          down = i;
          while(down<binary_image.rows() && binary_image[down][j] == 0) {
            down++;
          }
          left=j;
          while(left>=0 && binary_image[i][left] == 0) {
            left--;
          }
          right=j;
          while(right<binary_image.cols() && binary_image[i][right] == 0) {
            right++;
          }

          // If found make changes in image for visualization and write the results into file.
          if((left+right)/2 == j && (up+down)/2 == i) {
            if ((((right-left > 15 && right-left < 30) && (float)(down-up)/(float)(right-left) >= 1.15 && (float)(down-up)/(float)(right-left) <= 2.6)) ||
               (((down-up > 15 && down-up < 30) && (float)(right-left)/(float)(down-up) >= 1.3 && (float)(right-left)/(float)(down-up) <= 2.6))) {
              up++;
              down--;
              right--;
              left++;
              detect_file<<up<<" "<<left<<" "<<(down-up+1)<<" "<<(right-left+1)<<" 1\n";
              for(int k=up; k<=down;k++) {
                for(int l=left; l<=right; l++) {
                  intermediate_result[k][l] = 255;
                }
              }
            }
            // For testin purposes
            // else {
            //   cout<<(down-up)<<" "<<(right-left)<<" "<<(float)(down-up)/(float)(right-left)<<"\n";
            // }
          }

        }
      }
    }
    //Close output file
    detect_file.close();
  }

  //Make changes for visualization
  for(int i=0;i<binary_image.rows();i++){
    for(int j=0;j<binary_image.cols();j++){
      if (intermediate_result[i][j] == 255) {
        input[i][j] = 255;
      }
    }
  }


  SImageIO::write_png_file("detected.png",input,intermediate_result,intermediate_result);
  SImageIO::write_png_file("binary_image.png",binary_image,binary_image,binary_image);
  return binary_image;
}

// Apply a sobel operator to an image, returns the result
//
SDoublePlane sobel_gradient_filter(const SDoublePlane &input, double threshold)
{
  SDoublePlane output(input.rows(), input.cols());
  SDoublePlane gradients(input.rows(), input.cols());
  SDoublePlane directions(input.rows(), input.cols());


  // Implement a sobel gradient estimation filter with 1-d filters
 SDoublePlane gx(3,3);
 gx[0][0] = -1;
 gx[0][1] = 0;
 gx[0][2] = 1;
 gx[1][0] = -2;
 gx[1][1] = 0;
 gx[1][2] = 2;
 gx[2][0] = -1;
 gx[2][1] = 0;
 gx[2][2] = 1;

  SDoublePlane gy(3,3);
 gy[0][0] = -1;
 gy[0][1] = -2;
 gy[0][2] = -1;
 gy[1][0] = 0;
 gy[1][1] = 0;
 gy[1][2] = 0;
 gy[2][0] = 1;
 gy[2][1] = 2;
 gy[2][2] = 1;

 SDoublePlane gx_convolved = convolve_general(input,gx);
 SDoublePlane gy_convolved = convolve_general(input,gy);
    double pi = 3.1452;
    double thresholdH = 0.2;
    double thresholdL = 0.15;
double maxim = 0;

for(int i=0;i<output.rows();i++){
    for(int j=0;j<output.cols();j++){
        output[i][j] = sqrt(pow(gx_convolved[i][j],2) + pow(gy_convolved[i][j],2));
  }
}

for(int i=0;i<output.rows();i++){
  for(int j=0;j<output.cols();j++){
    if (output[i][j] > maxim)
    {
      maxim = output[i][j];
    }
  }
}


 for(int i=0;i<output.rows();i++){
    for(int j=0;j<output.cols();j++){
        // output[i][j] = sqrt(pow(gx_convolved[i][j],2) + pow(gy_convolved[i][j],2));

        //cout <<maxim<<","<<output[i][j]<<","<<maxim * threshold<<"\n";

        if(output[i][j] > maxim * thresholdH){
          output[i][j]  = 255;
        } else if(output[i][j] > maxim * thresholdL){
          output[i][j] = 100;
        } else {
          output[i][j] = 0;
        }
 }
 }

 for(int i=1;i<output.rows()-1;i++){
    for(int j=1;j<output.cols()-1;j++){

        if (
          output[i+1][j-1] > 0 &&
          output[i+1][j] > 0 &&
          output[i+1][j+1] > 0 &&
          output[i][j-1] > 0 &&
          output[i][j] > 0 &&
          output[i][j+1] > 0 &&
          output[i-1][j-1] > 0 &&
          output[i-1][j] > 0 &&
          output[i-1][j+1] > 0
          )
        {
          output[i][j] = 100;
        }
 }
 }

 for(int i=1;i<output.rows()-1;i++){
    for(int j=1;j<output.cols()-1;j++){

        if (output[i][j] == 100)
        {
          output[i][j] = 0;
        }
 }
 }
//  for(int i=0;i<directions.rows();i++){
//    for(int j=0;j<directions.cols();j++){
//        directions[i][j] = atan2(gy_convolved[i][j],gx_convolved[i][j]);
//        //cout << directions[i][j] << " ";
//    }
// }



  return output;
}

// Apply an edge detector to an image, returns the binary edge map
//
SDoublePlane find_edges(const SDoublePlane &input, double thresh=0)
{
  SDoublePlane output(input.rows(), input.cols());

  // Implement an edge detector of your choice, e.g.
  // use your sobel gradient operator to compute the gradient magnitude and threshold

  return output;
}


//
// This main file just outputs a few test images. You'll want to change it to do
//  something more interesting!
//
int main(int argc, char *argv[])
{
  if(!(argc == 2))
    {
      cerr << "usage: " << argv[0] << " input_image" << endl;
      return 1;
    }

  string input_filename(argv[1]);
  //cout << input_filename;
  SDoublePlane input_image= SImageIO::read_png_file(input_filename.c_str());
//  SDoublePlane input_image(3,3);
//  for(int i=0; i<3; i++)
//    for(int j=0; j<3; j++)
//      input_image[i][j] = 16;
//    input_image[2][2] = 32;
  // test step 2 by applying mean filters to the input image
  SDoublePlane mean_filter(3,3);
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mean_filter[i][j] = 1/9.0;

  SDoublePlane output_image = convolve_general(input_image, mean_filter);



  SImageIO::write_png_file("convoled_general.png",output_image,output_image,output_image);


  SDoublePlane col_filter(1,3);
  for(int i=0; i<=0; i++)
    for(int j=0; j<3; j++)
      col_filter[i][j] = 1/3.0;


  SDoublePlane row_filter(3,1);
  for(int i=0; i<3; i++)
    for(int j=0; j<=0; j++)
      row_filter[i][j] = 1/3.0;

    SDoublePlane output_image_sep = convolve_separable(input_image, row_filter,col_filter);

    SImageIO::write_png_file("convoled_sep.png",output_image_sep,output_image_sep,output_image_sep);

    SDoublePlane output_image_sobel = sobel_gradient_filter(input_image, 0.6);
    SImageIO::write_png_file("edges.png",output_image_sobel,output_image_sobel,output_image_sobel);

    SDoublePlane sachin_hack = binary_image_method(input_image);



  // randomly generate some detected cars -- you'll want to replace this
  //  with your car detection code obviously!
  // vector<DetectedBox> cars;
  // for(int i=0; i<10; i++)
  //   {
  //     DetectedBox s;
  //     s.row = rand() % output_image_sobel.rows();
  //     s.col = rand() % output_image_sobel.cols();
  //     s.width = 20;
  //     s.height = 20;
  //     s.confidence = rand();
  //     cars.push_back(s);
  //   }

  // write_detection_txt("detected.txt", cars);
  // write_detection_image("detected.png", cars, output_image_sobel);
}
