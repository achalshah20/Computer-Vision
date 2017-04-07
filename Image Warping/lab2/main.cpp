#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "CImg.h"

using namespace cimg_library;

using namespace std;


int main()
{
    cout << "Hello world!" << endl;

    std::ifstream file("../b657-wars.txt");


    vector<float> x;
    vector<float> y;
    vector<float> z;

    vector<float> u ;
    vector<float> v ;
    vector<float> constVec;

    vector<float> u1;
    vector<float> v1;
    vector<float> constVec1;
    int number;
    int i = 0;
    std::string   line;

    while(std::getline(file, line))
    {
        std::vector<int>   lineData;
        std::stringstream  lineStream(line);


        int value;
        if(lineStream >> value)
        {
            x.push_back(value);
        }
        cout << endl;

        if(lineStream >> value)
        {
            y.push_back(value);
        }

        
        z.push_back(0.0);
        
 
    }



    file.close();

  

    cout << "parsing done" << endl;

    CImg<unsigned char>  img_og(3000,3000,1,3,1);


    for (int i=0; i<x.size(); i++)
      {
 
	        img_og(x[i],y[i],1) = 255; 
	        img_og(x[i],y[i],2) = 255; 
	        img_og(x[i],y[i],3) = 255; 

      }

      //theImage.display();
 	img_og.save("img_og.bmp");



    cout << "og image done " << endl;

    for(i=0;i<x.size();i++){
        constVec.push_back(float(z[i]) + 2.0);
        u.push_back(x[i]/constVec[i]);
        v.push_back(y[i]/constVec[i]);
        constVec[i] = 1;
        
        cout << u[i] << " " << v[i] << " " <<  constVec[i] << " ";
        cout << endl;
       

    }

    CImg<unsigned char>  img_tr(3000,3000,1,3,1);


    for (int i=0; i<u.size(); i++)
      {

	        img_tr(u[i],v[i],1) = 128; 
	        img_tr(u[i],v[i],2) = 1; 
	        img_tr(u[i],v[i],3) = 1; 

      }

 	img_tr.save("img_tr.bmp");


    for(i=0;i<z.size();i++){
        constVec1.push_back((1/2)*y[i] + (1/2)*z[i] + 2.0);
        u1.push_back(float(x[i]/constVec1[i]));
        v1.push_back(float(((1/2)*x[i] + (1/2) * z[i]) /constVec1[i]));
        constVec1[i] = 1;

      

    }

        CImg<unsigned char>  img_rotate(3000,3000,1,3,1);


    for (int i=0; i<u1.size(); i++)
      {

	        img_rotate(u1[i],v1[i],1) = 255; 
	        img_rotate(u1[i],v1[i],2) = 255; 
	        img_rotate(u1[i],v1[i],3) = 255; 


      }

 	img_rotate.save("img_rotate.bmp");





    return 0;
}





