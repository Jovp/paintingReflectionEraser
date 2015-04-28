//
//  cornerDetector.h
//  ProjetSI343
//
//  Created by Julien Philip on 28/04/2015.
//  Copyright (c) 2015 Julien Philip. All rights reserved.
//

#ifndef __ProjetSI343__cornerDetector__
#define __ProjetSI343__cornerDetector__

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
using namespace cv;
using namespace std;

class cornerDetector {
    public :
    
    void detectCorner(std::vector<Vec2i>& quad_maxArea, string& chemin );
    
    
    float Squarelength(Vec3b pix);
    
    float Squarelength(Vec2i& pix);
    
    float length(Vec2i pix);
    
    float distancePoint(Vec2i vec1, Vec2i vec2);
    
    Vec3b minusV(Vec3b& pix1,Vec3b& pix2);
    
    Vec2b minusV2(Vec2b vec1,Vec2b vec2);
    
    float variance(Vec3b pix1,Vec3b pix2,Vec3b pix3,Vec3b pix4);
    
    float varianceColor(const std::vector<Vec2i>& quad,Mat& img);
    
    float area(Vec2i pix1,Vec2i pix2,Vec2i pix3,Vec2i pix4);
    
    float area(const std::vector<Vec2i>& quad);
    
    void drawQuad(const std::vector<Vec2i>& quad,Mat& img);
    
    void drawQuadMax(const std::vector<Vec2i>& quad,Mat& img);
    
};

#endif /* defined(__ProjetSI343__cornerDetector__) */
