
int getRndIndices(int maxVal,int rndNumbers[8]){

  int size = 8;

  for(int i=0; i<size; i++){
      rndNumbers[i] = (rand()%maxVal);
  }

}

void dot(CImg<double> A,CImg<double> B,CImg<double> &H){
	//cout << "Width of H: "  << H.width() << endl;
	//cout << "Height of H: "  << H.height() << endl;

	for (int x = 0; x < H.width(); x++)
    {
        for (int y = 0; y < H.height(); y++)
        {
        	// This is actually row * column but cimg stores valuesin c(col,row) format
            H(x, y) = A.get_column(x).dot(B.get_row(y));

            //cout << "Dot product "<< x << " " << y << endl;
            //A.get_column(x).print();
            //B.get_row(y).print();
        }
    }
}

void initializeCimg(CImg<double> &A){

	for (int i = 0; i < A.width(); i++) 
	{
		for (int j = 0; j < A.height(); j++) 
		{
			A(i,j) = 0;
		}
	}
}

CImg<double> reshape(CImg<double> img){

	CImg<double> A(3,3);

	initializeCimg(A);

	A(0,0) = img(0,0);
	A(0,1) = img(1,0);
	A(0,2) = img(2,0);
	A(1,0) = img(3,0);
	A(1,1) = img(4,0);
	A(1,2) = img(5,0);
	A(2,0) = img(6,0);
	A(2,1) = img(7,0);
	A(2,2) = 1;

	return A;
}

void drawLine_part2(CImg<double> input_image1,CImg<double> input_image2,vector< vector<double> > descriptors1,vector< vector<double> > descriptors2,string imgName){
		const unsigned char color[] = { 255,128,64 }; 

		CImg<double> tmp1 = input_image1;
		CImg<double> tmp2 = input_image2;


		CImg<double> merged = tmp1.append(tmp2,'x',0.0f);

		merged.get_normalize(0,255).save("merged.png"); 

		int xShift = input_image1.width();

		for (int i = 0; i < descriptors1.size(); i++ )
		 {
		    merged.draw_line((int)descriptors1[i][0],(int)descriptors1[i][1],
		    	xShift + (int)descriptors2[i][0], (int)descriptors2[i][1],color);
		    
		 }

		 merged.get_normalize(0,255).save(imgName.c_str()); 
}


CImg<double> ransac(vector< vector<double> > siftCorr1,vector< vector<double> > siftCorr2,vector< vector<double> > &bestInliers1,vector< vector<double> > &bestInliers2,int totalIter,int errThreshold){

    double bestOutRatio = 1;
    int bestIndex = -1;

    const int sampleSize = 4;
    CImg<double> bestHmat(3,3);


    vector<double> bestErrData;
    vector<double> bestPick;

    for(int iterNum = 0; iterNum < totalIter;iterNum++)
    {

		double localErr = 0;
		vector<double> localErrData;
		vector<double> localPick;
		double localOutRatio = 0;
		vector< vector<double> > localInliers1;
      	vector< vector<double> > localInliers2;

	     //     c1 	c2 	c3 	c4 c5
	    // r1
	    // r2
	    // r3
	    // r4
	    // r5	

	    //width,height
	    // column, row
	    CImg<double> A(8,8);
		CImg<double> B(8,1);
		CImg<double> H(8,1);

	    for (int i = 0; i < A.width(); i++) 
	    {
			for (int j = 0; j < A.height(); j++) 
			{
				A(i,j) = 0;
			}
	    }

	    for (int i = 0; i < B.width(); i++) 
	    {	
	    	B(i,0) = 0;
	    	H(i,0) = 0;		
	    }


		for(int i =0; i < 2*sampleSize;i=i+2){
			double rndNumber = rand() % siftCorr1.size();
			localPick.push_back(rndNumber);

			B(i,0) = siftCorr2[rndNumber][0];
			B(i+1,0) = siftCorr2[rndNumber][1];

			A(i,0) = siftCorr1[rndNumber][0];
			A(i,1) = siftCorr1[rndNumber][1];
			A(i,2) = 1;
			A(i,3) = 0;
			A(i,4) = 0;
			A(i,5) = 0;
			A(i,6) = -A(i,0) * B(i,0);
			A(i,7) = -A(i,1) * B(i,0);

			A(i+1,0) = 0;
			A(i+1,1) = 0;
			A(i+1,2) = 0;
			A(i+1,3) = siftCorr1[rndNumber][0];
			A(i+1,4) = siftCorr1[rndNumber][1];
			A(i+1,5) = 1;
			A(i+1,6) = -A(i+1,3) * B(i+1,0);
			A(i+1,7) = -A(i+1,4) * B(i+1,0);

		}

		A.invert();
		dot(A,B,H);
		
		CImg<double> Hmat(3,3);
		Hmat = reshape(H);

		for(int i =0; i< siftCorr1.size(); i++)
		{
			CImg<double> lA(3,1);
			CImg<double> lB(3,1);
			CImg<double> lAH(3,1);

			initializeCimg(lA);
			initializeCimg(lB);
			initializeCimg(lAH);


			lA(0,0) = siftCorr1[i][0];
			lA(1,0) = siftCorr1[i][1];
			lA(2,0) = 1;

			lB(0,0) = siftCorr2[i][0];
			lB(1,0) = siftCorr2[i][1];
			lB(2,0) = 1;


			dot(Hmat,lA,lAH);

			localErr = (abs(lB - lAH)).sum();
			localErrData.push_back(localErr);

			if(localErr < errThreshold){
            
                vector<double> linlier1;
                linlier1.push_back(lA(0,0));
                linlier1.push_back(lA(1,0));
                localInliers1.push_back(linlier1);

                vector<double> linlier2;
                linlier2.push_back(lB(0,0));
                linlier2.push_back(lB(1,0));
                localInliers2.push_back(linlier2);
          }
    	}

    	localOutRatio = double((siftCorr1.size() - localInliers1.size()))/siftCorr1.size();

	      if(bestOutRatio > localOutRatio){
	          bestOutRatio = localOutRatio;
	          bestInliers1 = localInliers1;
	          bestInliers2 = localInliers2;
	          bestErrData = localErrData;
	          bestPick = localPick;
	          bestHmat = Hmat;

	          //cout << " iterNum: " << iterNum << "  bestOutRatio: " << bestOutRatio << endl;
	      }

	}

	return bestHmat;
}

vector< vector<double> > findSummaryVec(vector< vector<double> > siftPoints,int k,int w){

	vector< vector<double> > siftSummaryPoints;
	for(int i =0; i< siftPoints.size(); i++){
    	
    	
    	vector<double> v = siftPoints[i];
    	vector<double> summaryVec;

    	for(int kIndex=0;kIndex<k;kIndex++){

    		vector<double> xi;
    		double rnd= ((double) rand() / (INT_MAX));

			for(int j =0; j< 128; j++){
				xi.push_back(rnd);
			}

			double num = 0;
			for(int j =0; j< 128; j++){
				num += xi[j] * v[j];
			}
			summaryVec.push_back(round(num/w));
    	}    	

    	siftSummaryPoints.push_back(summaryVec);
	}

	return siftSummaryPoints;

}

void findSummaryMatches(vector< vector<double> > summaryVec1,vector< vector<double> > summaryVec2,vector< vector<double> > &summaryMatches){

	for(int i =0;i < summaryVec1.size();i++){

		if(summaryMatches.size() < summaryVec1.size())
        	summaryMatches.push_back(vector<double> ());

		for(int j =0;j < summaryVec2.size();j++){
			bool isMatch = true;

			for(int k =0; k < summaryVec2[j].size();k++){

				if(summaryVec1[i][k] != summaryVec2[j][k]){
					isMatch = false;
					break;
				}
			}

			if(isMatch){
				summaryMatches[i].push_back(j);
			}
		}
	}
}

vector< double > summaryQuantization(vector< vector<double> > descriptors1,vector< vector<double> > descriptors2,int w,int k,int numTrials){
	
   	vector< vector<double> > bestMatches;
	vector< vector<double> > summaryMatches;
	vector< vector<double> > allDistances;
	vector< double > finalMatches;


	// Find best summary vectors matches
    for(int iTrial=0;iTrial<numTrials;iTrial++){

	   	vector< vector<double> > siftSummaryVec1;
	    vector< vector<double> > siftSummaryVec2;

	   	// Find summary vectors
		siftSummaryVec1 = findSummaryVec(descriptors1,k,w);
		siftSummaryVec2 = findSummaryVec(descriptors2,k,w);

		// Find identical summary vectors among different images
		findSummaryMatches(siftSummaryVec1,siftSummaryVec2,summaryMatches);

    }

    // Find all unique summary matches
    for(int i =0; i< summaryMatches.size(); i++){

		set<double> s( summaryMatches[i].begin(), summaryMatches[i].end() );
		summaryMatches[i].assign( s.begin(), s.end() );
		bestMatches.push_back(summaryMatches[i]);
    }

    // Find euclidean distance between matches in 128D space
    for(int i =0; i< bestMatches.size(); i++){

    	allDistances.push_back(vector<double> ());

        for(int j =0; j< bestMatches[i].size(); j++){

        	double dist = 0;
        	int index = bestMatches[i][j];

        	for(int k =0;k<descriptors1[i].size();k++){
        		dist += pow(descriptors1[i][k] - descriptors2[index][k],2);
        	}

        	allDistances[i].push_back(sqrt(dist));
        }	
    }

    // Find point which has smallest euclidean distance and delare them as a match
	for(int i =0; i< allDistances.size(); i++){
		double iMin = INT_MAX;
		double bestIndex = -1;
	    for(int j =0; j< allDistances[i].size(); j++){
	    	if(iMin > allDistances[i][j]){
	    		iMin = allDistances[i][j];
	    		bestIndex = bestMatches[i][j];
	    	}
	    }

	    //cout << "for index " << i << "distance: " << iMin <<endl;
	    finalMatches.push_back(bestIndex);	
	}

	return finalMatches;
}