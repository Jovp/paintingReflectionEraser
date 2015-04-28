//
//  cornerDetector.cpp
//  ProjetSI343
//
//  Created by Julien Philip on 28/04/2015.
//  Copyright (c) 2015 Julien Philip. All rights reserved.
//

#include "cornerDetector.h"

using namespace cv;
using namespace std;

float Squarelength(Vec3b pix){
    return (pow(pix[0],2)+pow(pix[1],2)+pow(pix[2],2));
}

float Squarelength(Vec2i& pix){
    return (pow(pix[0],2)+pow(pix[1],2));
}

float length(Vec2i pix){
    return sqrt(Squarelength(pix));
}

float distancePoint(Vec2i vec1, Vec2i vec2){
    return sqrt ((vec1[0]-vec2[0])*(vec1[0]-vec2[0])+(vec1[1]-vec2[1])*(vec1[1]-vec2[1]));
}

Vec3b minusV(Vec3b& pix1,Vec3b& pix2){
    return Vec3b(abs(pix1[0]-pix2[0]),abs(pix1[1]-pix2[1]),abs(pix1[2]-pix2[2]));
}

Vec2b minusV2(Vec2b vec1,Vec2b vec2){
    std::cout << "coucou" << std::endl;
    Vec2b answer=Vec2b(vec1[0]-vec2[0],vec1[1]-vec2[1]);
    std::cout << "coucou2" << std::endl;
    return answer;
}

float variance(Vec3b pix1,Vec3b pix2,Vec3b pix3,Vec3b pix4){
    Vec3b moyenne=Vec3b((pix1[0]+pix2[0]+pix3[0]+pix4[0])/4,(pix1[1]+pix2[1]+pix3[1]+pix4[1])/4,(pix1[2]+pix2[2]+pix3[2]+pix4[2])/4);
    return (Squarelength(minusV(pix1,moyenne))+Squarelength(minusV(pix2,moyenne))+Squarelength(minusV(pix3,moyenne))+Squarelength(minusV(pix4,moyenne)))/4;
}

float varianceColor(const std::vector<Vec2i>& quad,Mat& img){
    int x1=quad[0][1];
    int y1=quad[0][0];
    int x2=quad[1][1];
    int y2=quad[1][0];
    int x3=quad[2][1];
    int y3=quad[2][0];
    int x4=quad[3][1];
    int y4=quad[3][0];
    
    /*
     std::cout << img.at<Vec3b>(x3,y3) << std::endl;
     std::cout << img.at<Vec3b>(x2,y2) << std::endl;
     std::cout << img.at<Vec3b>(x1,y1) << std::endl;
     std::cout << img.at<Vec3b>(x4,y4) << std::endl;
     */
    return variance(img.at<Vec3b>(x1,y1), img.at<Vec3b>(x2,y2), img.at<Vec3b>(x3,y3), img.at<Vec3b>(x4,y4));
}

float area(Vec2i pix1,Vec2i pix2,Vec2i pix3,Vec2i pix4){
    return abs(pix1[0]*pix2[1]-pix2[0]*pix1[1]+pix2[0]*pix3[1]-pix3[0]*pix2[1]+pix3[0]*pix4[1]-pix4[0]*pix3[1]+pix4[0]*pix1[1]-pix1[0]*pix4[0]);
}

float area(const std::vector<Vec2i>& quad){
    return area(quad[0],quad[1],quad[2],quad[3]);
}

void drawQuad(const std::vector<Vec2i>& quad,Mat& img){
    line(img, quad[0], quad[1], Scalar(0,0,255), 1, 8, 0);
    line(img, quad[1], quad[2], Scalar(0,0,255), 1, 8, 0);
    line(img, quad[2], quad[3], Scalar(0,0,255), 1, 8, 0);
    line(img, quad[3], quad[0], Scalar(0,0,255), 1, 8, 0);
}

void drawQuadMax(const std::vector<Vec2i>& quad,Mat& img){
    for (int i =0;i<quad.size()-1;i++)
        line(img, quad[i], quad[i+1], Scalar(255,0,255), 4, 8, 0);
    line(img, quad[quad.size()-1], quad[0], Scalar(255,0,255), 4, 8, 0);
}


// On modifie quad_maxArea, c'est dans cette variable que sont stoqués les 4 coins. Chemin est le chemin vers l'image.
void detectCorner(std::vector<Vec2i>& quad_maxArea,const string& chemin )
{
    
    
    //Objet nécessaires
    
    std::vector<Vec2i> listCorner;
    std::vector<std::vector<Vec2i>> listPolygon;
    
    //Ouverture de l'image
    
    Mat image;
    int ddepth = CV_16S;
    int scale = 1;
    int delta = 0;
    const float pi=3.14159;
    image = imread(chemin);
    int imHeight, imWidth;
    imHeight=image.rows;
    imWidth=image.cols;
    // Read the file
    
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return;
    }
    
    //Affichage de l'image
    
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.
    
    //Passage en niveau de gris
    Mat img_gray;
    cvtColor( image, img_gray, cv::COLOR_RGB2GRAY );
    
    
    // Gradient
    
    Mat gradS(image.rows,image.cols,CV_8UC3,Scalar::all(0));
    Mat imageP(image.rows,image.cols,CV_8UC3,Scalar::all(0));
    Mat gradSP(image.rows,image.cols,CV_8UC3,Scalar::all(0));
    //Mat_<float> gradSP=(Mat_<float>) gradSP1;
    //Mat gradSP;
    resize(img_gray,imageP ,Size(1000, int(1000*double(image.rows)/double(image.cols))),0, 0, INTER_AREA);
    medianBlur(imageP, imageP, 11);
    
    //Méthode avec Canny(imageP,gradSP,100,200);
    
    
    //gradient horizontal et vertical
    Mat grad, grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    
    Sobel( imageP, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    Sobel( imageP, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    
    cout <<  "test1" << std::endl ;
    cout << gradSP.rows << "  " << gradSP.cols << std::endl;
    gradSP.at<float>(2257,4607)=1;
    cout << gradSP.at<float>(2257,4609) << std::endl;
    
    //sotcke la somme pondérée
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradSP );
    
    threshold( gradSP, gradSP, 25, 255,0 );
    
    namedWindow( "Display resize", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display resize", gradSP );
    resize(gradSP,gradS ,Size(image.cols, image.rows),0, 0, INTER_LINEAR);
    
    
    //Afficher le gradient
    imshow( "Display window", gradS );
    
    // Harris
    Mat HarrisRes;
    cornerHarris( img_gray, HarrisRes, 20, 5, 0.05, BORDER_DEFAULT );
    
    //Tranformée de hough :
    Mat houghTrans(image.rows,image.cols,CV_8UC3,Scalar::all(0));
    Mat houghLine(image.rows,image.cols,CV_8UC3,Scalar::all(0));
    
    /*
     st: Output of the edge detector. It should be a grayscale image (although in fact it is a binary one)
     lines: A vector that will store the parameters (x_{start}, y_{start}, x_{end}, y_{end}) of the detected lines
     rho : The resolution of the parameter r in pixels. We use 1 pixel.
     theta: The resolution of the parameter \theta in radians. We use 1 degree (CV_PI/180)
     threshold: The minimum number of intersections to “detect” a line
     minLinLength: The minimum number of points that can form a line. Lines with less than this number of points are disregarded.
     maxLineGap: The maximum gap between two points to be considered in the same line.
     */
    vector<Vec4i> lines;
    HoughLinesP(gradS, lines, 1, CV_PI/180, 130, 500, 3 );
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( houghTrans, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 1, LINE_AA);
        
        line( houghLine, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,255), 2, LINE_AA);
    }
    
    cout << lines.size() << std::endl;
    
    // Comparer les droites deux à deux et trouver l'intersection
    for (int i=0; i<lines.size(); i++) {
        for (int j=i+1; j<lines.size(); j++) {

            // Tester si l'angle entre les droites est assez grand
            if (fmod (abs(lines[i][1]-lines[j][1]),2*pi)>0.05 /*environ 50 deg */) {
                
                
                Vec4i l1 = lines[i];
                Vec4i l2 = lines[j];
                Point pt1=Point(l1[0], l1[1]);
                Point pt2=Point(l1[2], l1[3]);
                Point pt3=Point(l2[0], l2[1]);
                Point pt4=Point(l2[2], l2[3]);
                
                // On calcule le point d'intersection
                double m1, m2, b0, b1;
                int x=0, y;
                
                bool m1vert = abs(pt1.x-pt2.x) < 1;
                bool m2vert = abs(pt3.x-pt4.x) < 1;
                if (! m1vert){
                    m1 = double(pt2.y-pt1.y)/double(pt2.x-pt1.x);
                    
                    b1 = pt1.y-m1*pt1.x;
                    /*cout<< "m1 : "<< m1<< std::endl;
                     cout<< "b1 : "<< b1 << std::endl;*/
                    
                }
                if (! m2vert){
                    m2 = double(pt4.y-pt3.y)/double(pt4.x-pt3.x);
                    
                    b0 = pt3.y-m2*pt3.x;
                    /*cout<< "m2 : "<< m2<< std::endl;
                     cout<< "b0 : "<< b0 << std::endl;*/
                    
                }else if(m1vert){
                    x = pt1.x;
                    y = m2*x + b0;
                }else if (m2vert){
                    x = pt3.x;
                    y = m1*x + b1;
                    
                }
                if(x==0){
                    x = int(floor(double(b1-b0)/double(m2-m1)));
                    y = int(floor(m1*x +b1));
                }
                
                //cout << x << "et " << y << std::endl;
                
                
                int sizeWindowResearch=25;
                
                if(x>sizeWindowResearch && x<imWidth-sizeWindowResearch && y>sizeWindowResearch && y<imHeight-sizeWindowResearch){
                    // on teste si le point fait parti du contour :
                    if(houghLine.at<Vec3b>(y,x)[0]==255){
                        int iMaxCorner, jMaxCorner;
                        float cornerLevel=0.0;
                        int isCornerHarris=0;
                        for (int i2=-sizeWindowResearch; i2<sizeWindowResearch; i2++) {
                            for (int j2=-sizeWindowResearch; j2<sizeWindowResearch; j2++) {
                                if(HarrisRes.at<float>(y+j2,x+i2)>0.00001){
                                    isCornerHarris=1;
                                    
                                    if(HarrisRes.at<float>(y+j2,x+i2)>cornerLevel)
                                    {
                                        cornerLevel=HarrisRes.at<float>(y+j2,x+i2);
                                        iMaxCorner=x+i2;
                                        jMaxCorner=y+j2;
                                    }
                                    
                                }
                            }
                            
                        }
                        if (isCornerHarris==1) {
                            //rectangle(image, Point(iMaxCorner-1,jMaxCorner-1), Point(iMaxCorner+1,jMaxCorner+1), Scalar(0,255,0), 1, LINE_AA, 0);// change la couleur du pixel
                            
                            if(!(std::find(listCorner.begin(), listCorner.end(), Vec2i(iMaxCorner,jMaxCorner)) != listCorner.end())){
                                listCorner.push_back(Vec2i(iMaxCorner,jMaxCorner));
                            }
                            
                        }
                        
                    }
                }
                
                
                
                
            }
        }
    }
    
    
    std::cout << "nombre coin : " << listCorner.size() << std::endl;
    
    for (int i=0;i<listCorner.size();i++){
        std::cout << "position coin : "<< listCorner[i] << std::endl;
    }
    
    
    
    Mat imageBlur;
    blur(image, imageBlur, Size(100,100));
    Mat imageBlurPetite;
    blur(image, imageBlurPetite, Size(10,10));
    
    // Calcul de l'enveloppe convexe et dessin
    std::vector<Vec2i> quad;
    std::vector<Vec2i> convexEnv;
    convexHull(listCorner, convexEnv,true,true );
    std::cout<< "variance: " << varianceColor(convexEnv, imageBlur) << std::endl;
    //drawQuadMax(convexEnv, image);
    Mat image3;
    resize(image,image3 ,Size(image.cols/7, image.rows/7),0, 0, INTER_AREA);
    
    imshow( "Display Corner", image3 );
    
    std::cout<< "Points proches" << std::endl;
    // On ajoute les points proches
    int init_size=convexEnv.size();
    int sizeWindowResearch2=100;
    
    for (int i =0; i<init_size; i++) {
        for (int i2=-sizeWindowResearch2; i2<sizeWindowResearch2; i2++) {
            for (int j2=-sizeWindowResearch2; j2<sizeWindowResearch2; j2++) {
                if (std::find(listCorner.begin(), listCorner.end(), Vec2i(convexEnv[i][0]+i2,convexEnv[i][1]+j2)) != listCorner.end())
                {
                    convexEnv.push_back(Vec2i(convexEnv[i][0]+i2,convexEnv[i][1]+j2));
                }
            }
        }}
    
    std::cout<< "Nombre de point : " << convexEnv.size() << std::endl;
    std::cout<< "Minimisation énergie" << std::endl;
    // On cherche les 4 points de l'enveloppe convexe qui minimisent la variance en ayant une aire grande
    
    float max_en=0;
    for(int i1=0;i1<convexEnv.size()-3;i1++){
        std::cout<< i1 << std::endl;
        for(int i2=i1+1;i2<convexEnv.size()-2;i2++)
            if (distancePoint(convexEnv[i1], convexEnv[i2])>100.0)
                for(int i3=i2+1;i3<convexEnv.size()-1;i3++)
                    if (distancePoint(convexEnv[i1], convexEnv[i3])>100.0 && distancePoint(convexEnv[i3], convexEnv[i2])>100.0)
                        for(int i4=i3+1;i4<convexEnv.size();i4++){
                            if (distancePoint(convexEnv[i3], convexEnv[i4])>100.0 && distancePoint(convexEnv[i1], convexEnv[i4])>100.0 && distancePoint(convexEnv[i4], convexEnv[i2])>100.0){
                                // check if the corner ar far from each others (15 of STD)
                                std::vector<Vec2i> quad;
                                std::vector<Vec2i> quadOrder;
                                quad.push_back(convexEnv[i1]);
                                quad.push_back(convexEnv[i2]);
                                quad.push_back(convexEnv[i3]);
                                quad.push_back(convexEnv[i4]);
                                convexHull(quad, quadOrder,true,true );
                                
                                // On vérifie qu'on a un quadrilatère de grande aire avec une faible variance en couleur
                                if (quadOrder.size()==4 && sqrt(area(quadOrder))/pow(varianceColor(quadOrder, imageBlur),2)>max_en){
                                    // On teste si cela ne ressemble pas à un triangle
                                    
                                    
                                    quad_maxArea=quadOrder;
                                    max_en=sqrt(area(quadOrder))/pow(varianceColor(quadOrder, imageBlur),2);
                                    //varianceColor(quadOrder, imageBlurPetite);
                                    
                                    
                                }
                            }
                            
                            
                        }
    }
    
    //drawQuadMax(quad_maxArea, image);
    std::cout << "variance solution : "<<varianceColor(quad_maxArea, imageBlur) << std::endl;
    
    //std::cout << "nombre polygon : "<< numberOfPolygon << std::endl;
    //Affichage
    /*
    Mat image2, gradS2;
    namedWindow( "Display Line", WINDOW_AUTOSIZE );// Create a window for display.
    resize(houghTrans,houghTrans ,Size(image.cols/7, image.rows/7),0, 0, INTER_AREA);
    imshow( "Display Line", houghTrans );
    namedWindow( "Display Corner", WINDOW_AUTOSIZE );// Create a window for display.
    resize(image,image2 ,Size(image.cols/5, image.rows/5),0, 0, INTER_AREA);
    imshow( "Display Corner", image2 );
     */
    
    return;
}
