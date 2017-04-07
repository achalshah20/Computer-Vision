// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.
#define cimg_use_png
#define cimg_use_jpeg



//Link to the header file
#include "CImg.h"

#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <limits>
#include <set>


//Use the cimg namespace to access the functions easily
using namespace cimg_library;
using namespace std;

#include "part1.h"
#include "part2.h"
#include "part3.h"

void siftToDouble(vector<SiftDescriptor> siftVec,vector< vector<double> > &dVec){

	for(int i =0 ;i <siftVec.size();i++){
		dVec.push_back(vector<double> ());
		for(int j=0;j<128;j++){
			dVec[i].push_back(double(siftVec[i].descriptor[j]));
		}
	}
}

int main(int argc, char **argv)
{
  try {

	if(argc < 2)
	{
		cout << "Insufficent number of arguments; correct usage:" << endl;
		cout << "    a2 part_id ..." << endl;
		//return -1;
	}

    string part = argv[1];
    cout << argc << endl;

    if(part == "part1")
      {
      	    string queryImgName = argv[2];
			vector<string> inputFiles; 
			for (int n = 3; n < argc; n++)
			{
				inputFiles.push_back(argv[n]);
			}

  		
  		vector<pair<int,string> > featureMatchingPair;
      	CImg<double> queryImage(queryImgName.c_str());

      	for(int i =0; i< inputFiles.size();i++)
      	{
			
			CImg<double> input_image2(inputFiles[i].c_str());
			vector<double> matchedIndices;


			vector<SiftDescriptor> descriptors1 = getSIFTDistriptors(queryImage);
			vector<SiftDescriptor> descriptors2 = getSIFTDistriptors(input_image2);

			findSIFTMatching(descriptors1,descriptors2,matchedIndices);

			getCrossHair(queryImage,descriptors1);
			getCrossHair(input_image2,descriptors2);

			string targetFileName(inputFiles[i].substr(inputFiles[i].rfind("/") + 1));
			string queryImgFileName(queryImgName.substr(queryImgName.rfind("/") + 1));
			
			string filename = queryImgFileName + "_"+ targetFileName +"_Siftmatches.png";

			drawLine(queryImage,input_image2,descriptors1,descriptors2,matchedIndices,filename.c_str());
           	int matchesCount = 0;
			for(int t=0;t<matchedIndices.size();t++){
				if(matchedIndices[t] != -1)
					matchesCount++;
			}

           featureMatchingPair.push_back(make_pair(matchesCount,inputFiles[i]));
      	}

      	sort(featureMatchingPair.begin(), featureMatchingPair.end());


      	cout << "For query image: " << queryImgName << endl; 
      	cout << endl;
      	int j =0;

		for (int i=featureMatchingPair.size()-1; i>=0 ; --i)
		{
			if(j++ >= 10){
				break;
			}
			cout << featureMatchingPair[i].first<<" matches for: " << featureMatchingPair[i].second << endl;
		}

	        
      }
    else if(part == "part2_1")
      {
		int totalIter = atoi(argv[2]);
      	int errThreshold = atoi(argv[3]);
    	string queryImgName = argv[4];


		vector<string> inputFiles; 
		for (int n = 5; n < argc; n++)
		{
			if(queryImgName == argv[n]){
			continue;
		}
			inputFiles.push_back(argv[n]);
		}

  		vector<pair<int,string> > featureMatchingPair;
        CImg<double> queryImage(queryImgName.c_str());

        for(int inIndex =0; inIndex< inputFiles.size();inIndex++)
	     {
	        CImg<double> input_image2(inputFiles[inIndex].c_str());
	        vector<double> matchedIndices;


	        vector<SiftDescriptor> descriptors1 = getSIFTDistriptors(queryImage);
	        vector<SiftDescriptor> descriptors2 = getSIFTDistriptors(input_image2);

	        findSIFTMatching(descriptors1,descriptors2,matchedIndices);

			vector< vector<double> > siftCorr1;
			vector< vector<double> > siftCorr2;
			vector<double> siftMachedIndices;
			int k = 0;
			for(int i =0; i< matchedIndices.size(); i++){

				if(matchedIndices[i] == -1){
					continue;
				}

				siftMachedIndices.push_back(i);

				siftCorr1.push_back(vector<double> ());
				siftCorr1[k].push_back(descriptors1[i].col);
				siftCorr1[k].push_back(descriptors1[i].row);

				siftCorr2.push_back(vector<double> ());
				siftCorr2[k].push_back(descriptors2[matchedIndices[i]].col);
				siftCorr2[k].push_back(descriptors2[matchedIndices[i]].row);
				k++;
			}

			vector< vector<double> > ransacPoints1;
			vector< vector<double> > ransacPoints2;

			ransac(siftCorr1,siftCorr2,ransacPoints1,ransacPoints2,totalIter,errThreshold);
			
			getCrossHair(queryImage,descriptors1);
	        getCrossHair(input_image2,descriptors2);
			string targetFileName(inputFiles[inIndex].substr(inputFiles[inIndex].rfind("/") + 1));
			string queryImgFileName(queryImgName.substr(queryImgName.rfind("/") + 1));

			string filename = queryImgFileName + "_"+ targetFileName +"_ransac.png";



		  	drawLine_part2(queryImage,input_image2,ransacPoints1,ransacPoints2,filename.c_str());
		  	featureMatchingPair.push_back(make_pair(ransacPoints1.size(),inputFiles[inIndex]));
			}

			sort(featureMatchingPair.begin(), featureMatchingPair.end());
			

			cout << "For query image: " << queryImgName << endl; 
			cout << endl;
			int j =0;
			for (int i=featureMatchingPair.size()-1; i>=0 ; --i)
			{
				if(j++ >= 10){
					break;
				}

				cout << featureMatchingPair[i].first<<" matches for: " << featureMatchingPair[i].second << endl;
			}

      }
      else if(part == "part2_2")
      {
      	int w = atoi(argv[2]);
      	int k = atoi(argv[3]);
      	int numTrials = atoi(argv[4]);
    	string queryImgName = argv[5];

  		vector<string> inputFiles; 
		for (int n = 6; n < argc; n++)
		{
			if(queryImgName == argv[n]){
			continue;
		}
			inputFiles.push_back(argv[n]);
		}

  		vector<pair<int,string> > featureMatchingPair;
      	CImg<double> queryImage(queryImgName.c_str());

        for(int inIndex =0; inIndex< inputFiles.size();inIndex++)
	    {
	        CImg<double> input_image2(inputFiles[inIndex].c_str());
	        vector<double> matchedIndices;


	        vector<SiftDescriptor> descriptors1 = getSIFTDistriptors(queryImage);
	        vector<SiftDescriptor> descriptors2 = getSIFTDistriptors(input_image2);

	        vector< vector<double> > d1;
	        vector< vector<double> > d2;

	        siftToDouble(descriptors1,d1);
	        siftToDouble(descriptors2,d2);


			vector< double > finalMatches = summaryQuantization(d1,d2,w,k,numTrials);

			vector< vector<double> > image1Matches;
			vector< vector<double> > image2Matches;
			int matchIndex = 0;
			for(int i =0; i< finalMatches.size(); i++){

				if(finalMatches[i] == -1){
					continue;
				}

				image1Matches.push_back(vector<double> ());
				image1Matches[matchIndex].push_back(descriptors1[i].col);
				image1Matches[matchIndex].push_back(descriptors1[i].row);

				image2Matches.push_back(vector<double> ());
				image2Matches[matchIndex].push_back(descriptors2[finalMatches[i]].col);
				image2Matches[matchIndex].push_back(descriptors2[finalMatches[i]].row);
				matchIndex++;
			}
		  

		string targetFileName(inputFiles[inIndex].substr(inputFiles[inIndex].rfind("/") + 1));
		string queryImgFileName(queryImgName.substr(queryImgName.rfind("/") + 1));

		string filename = queryImgFileName + "_"+ targetFileName +"_summaryMatches.png";

		getCrossHair(queryImage,descriptors1);
	    getCrossHair(input_image2,descriptors2);

		drawLine_part2(queryImage,input_image2,image1Matches,image2Matches,filename.c_str());
		featureMatchingPair.push_back(make_pair(image1Matches.size(),inputFiles[inIndex]));
		}
		
		sort(featureMatchingPair.begin(), featureMatchingPair.end());
		

		cout << "For query image: " << queryImgName << endl; 
		cout << endl;
		int j =0;
		for (int i=featureMatchingPair.size()-1; i>=0 ; --i)
		{
			if(j++ >= 10){
				break;
			}

			cout << featureMatchingPair[i].first<<" matches for: " << featureMatchingPair[i].second << endl;
		}
      }
    else if(part == "part3")
      {
		if(argc < 4)
        {
        	string queryImgName = argv[2];
          
	        CImg<double> queryImage(queryImgName.c_str());
			CImg<double> targetImage(queryImage,"xyzc",0);
		          
          	CImg<double> transform(3,3);

           transform(0,0) = 0.907;
           transform(0,1) = 0.258;
           transform(0,2) = -182;
           
           transform(1,0) = -0.153;
           transform(1,1) = 1.44;
           transform(1,2) = 58;
           
           transform(2,0) = -0.000306;
           transform(2,1) = 0.000731;
           transform(2,2) = 1;
            

           transform = transform.get_invert();


           warp_image(queryImage,transform,targetImage);
          
           targetImage.save("lincon_output.png");
		          
		           
        }
        else 
        {
        	string queryImgName = argv[2];
			int totalIter = 100;
			int errThreshold = 200;

        	vector<string> inputFiles; 
			for (int n = 3; n < argc; n++)
			{
				if(queryImgName == argv[n]){
				continue;
				}
				inputFiles.push_back(argv[n]);
			}
          
			CImg<double> transform(3,3);
			CImg<double> queryImage(queryImgName.c_str());
			CImg<double> outImage(queryImage,"xyzc",0);

			string img_str;

			for(int inIndex=0;inIndex<inputFiles.size();inIndex++)
			{
				CImg<double> targetImage(inputFiles[inIndex].c_str());

				vector<double> matchedIndices;

				vector<SiftDescriptor> descriptors1 = getSIFTDistriptors(queryImage);
				vector<SiftDescriptor> descriptors2 = getSIFTDistriptors(targetImage);

				findSIFTMatching(descriptors1,descriptors2,matchedIndices);

				vector< vector<double> > siftCorr1;
				vector< vector<double> > siftCorr2;
				vector<double> siftMachedIndices;
			
				int k = 0;
				for(int i =0; i< matchedIndices.size(); i++){

					if(matchedIndices[i] == -1){
						continue;
					}

					siftMachedIndices.push_back(i);

					siftCorr1.push_back(vector<double> ());
					siftCorr1[k].push_back(descriptors1[i].col);
					siftCorr1[k].push_back(descriptors1[i].row);

					siftCorr2.push_back(vector<double> ());
					siftCorr2[k].push_back(descriptors2[matchedIndices[i]].col);
					siftCorr2[k].push_back(descriptors2[matchedIndices[i]].row);
					k++;
				}

				vector< vector<double> > ransacPoints1;
				vector< vector<double> > ransacPoints2;
	          	
	          	CImg<double> transform(3,3);

				transform = ransac(siftCorr1,siftCorr2,ransacPoints1,ransacPoints2,totalIter,errThreshold);
				transform = transform.get_invert();

	           	for(int i=0;i<3;i++){
	           		for(int j=0;j<3;j++){
	           			cout << transform(i,j) << " ";
	           		}

	           		cout << endl;
	           	}
	           	string targetFileName(inputFiles[inIndex].substr(inputFiles[inIndex].rfind("/") + 1));

	            int lastindex = targetFileName.find_last_of("."); 
	            string rawname = targetFileName.substr(0, lastindex); 
	            
	            string new_str = rawname + "-warped" + ".png";  
	            
	            // call to part2 function for getting homography co-ordinates
	            //CImg TrfMatrix = function(RefImage,temp);

	            warp_image(targetImage,transform,outImage);
	            
	            outImage.get_normalize(0,255).save(new_str.c_str());   
	        }
        }
      }
    else
      throw std::string("unknown part!");

    // feel free to add more conditions for other parts (e.g. more specific)
    //  parts, for debugging, etc.
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}
