void  findSIFTMatching(vector<SiftDescriptor> descriptors1, vector<SiftDescriptor> descriptors2, vector<double> &matchedIndices)
{
       
	//to store matching points of two images
    vector< vector<double> > points1, points2; 
	double ratio,first,second;
	
	double result =0;
	int point1_idx=0, point2_idx;
	
	for(int i=0; i<descriptors1.size(); i++)
	{
	    first = second = std::numeric_limits<int>::max(); 
	    point2_idx=0;
		for(int j=0; j<descriptors2.size()-1; j++)
		{
			double dist =0;
			
			for(int l=0; l<128; l++)
			{
				   
		      dist +=  pow( (descriptors1[i].descriptor[l] - descriptors2[j].descriptor[l]), 2) ; 
		       
		    }	    
			result = sqrt(dist);
			if (result < first)
            {
	            second = first;
	            first = result;
	            point2_idx = j;  // storing index of 2nd image descriptor with lowest distance
            }
            else if (result < second && result != first)
            	second = result;		    
		}

		if(second != 0){
	    	ratio = first/second; 
		}else{
			ratio = 1;
		}

		matchedIndices.push_back(-1);
		if(ratio < 0.85 )  // for two different images
		{
				matchedIndices[i] = point2_idx;		
		}
	}
	
	
}

vector<SiftDescriptor> getSIFTDistriptors(CImg<double> input_image){


  // convert image to grayscale
  CImg<double> gray = input_image.get_RGBtoHSI().get_channel(2);
  vector<SiftDescriptor> descriptors = Sift::compute_sift(gray);

  return descriptors;
}

void getCrossHair(CImg<double> &img,vector<SiftDescriptor> descriptors){

		for(int i=0; i<descriptors.size(); i++)
		{
			// creating crosshairs
			for(int j=0; j<5; j++)
				for(int k=0; k<5; k++)
				if(j==2 || k==2)
					for(int p=0; p<3; p++)
					if((descriptors[i].col+k < img.width() ) && (descriptors[i].row+j <img.height() ) )
			img(descriptors[i].col+k,descriptors[i].row+j, 0, p)=0;

		}

}

void drawLine(CImg<double> input_image1,CImg<double> input_image2,vector<SiftDescriptor> descriptors1,vector<SiftDescriptor> descriptors2,vector<double> matchedIndices,string imgName){
		const unsigned char color[] = { 255,128,64 }; 

			CImg<double> tmp1 = input_image1;
			CImg<double> tmp2 = input_image2;


			CImg<double> merged = tmp1.append(tmp2,'x',0.0f);

			int xShift = input_image1.width();



		for (int i = 0; i < descriptors1.size(); i++ )
		 {
		 	if(matchedIndices[i] == -1){
		 		continue;
		 	}

		    merged.draw_line((int)descriptors1[i].col,(int)descriptors1[i].row,(int)(xShift + descriptors2[matchedIndices[i]].col),(int)descriptors2[matchedIndices[i]].row,color );
		    
		 }

		 merged.get_normalize(0,255).save(imgName.c_str()); 
}
