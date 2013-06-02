#include "mouthStateDet.h"
#include <QDebug>

using namespace std;
using namespace cv;

int getBiggestRect(const vector<Rect> rects){
    int biggestIdx = 0;

    if (rects.size()<=1){return 0;}

    for(unsigned int i = 0; i < rects.size(); i++){
        Rect current = rects.at(i);
        Rect biggest = rects.at(biggestIdx);

        if( (current.width * current.height) > (biggest.width * biggest.height)){
            biggestIdx = i;
        }
        else if((current.width * current.height) == (biggest.width * biggest.height)
                && current.width > biggest.width){
            biggestIdx = i;
        }
    }

    return biggestIdx;
}

void drawRect(Mat &frame,Rect rect, RectColor color){

    Scalar col;

    switch(color){
    case RED:
        col = Scalar(0,0,255);
        break;
    case GREEN:
        col = Scalar(0,255,0);
        break;
    case BLUE:
        col = Scalar(255,0,0);
        break;
    default:
        col = Scalar(0,0,255);
    }

    rectangle(frame, rect, col,1);
}

int detectFace(const Mat& frame, Rect *faceRect,CascadeClassifier *faceCC){

    if(faceRect==NULL || faceCC == NULL){return -1;}

    vector<Rect> faceRects;

    double scaleFactor = 1.1; //okresla o ile procent zwieksza sie pole detekcji w kazdej iteracji
    int minNeighbours = 4; //jak wiele sasiadow musi miec obiekt aby go pozostawic
    int flags = 0; //przestarzale, ale musi byc
    Size minSize(0.25 * frame.rows,0.25 * frame.cols); //tak aby twarz zajmowala co najmniej 1/4 ekranu
    Size maxSize = Size();

    faceCC->detectMultiScale(frame, faceRects, scaleFactor,
                             minNeighbours, flags, minSize,maxSize);

    double areaMult = 0.0; //o ile znaleziony obszar zostanie powiekszony
                            //wprowadzone w celu lepszego poszukiwania ust

    if(faceRects.size() > 0){
        Rect face = faceRects.at( getBiggestRect(faceRects) );

        //powiekszenie obszaru w dol
        int h = areaMult * face.height;
        face.height = face.height + h;

        if((face.y + face.height) > frame.rows){face.height = frame.rows - face.y;}

        faceRect->x = face.x;
        faceRect->y = face.y;
        faceRect->width = face.width;
        faceRect->height = face.height;
    }

    return faceRects.size();
}

int detectMouth2(const Mat& frame, Rect faceArea,Rect *mouthRect,CascadeClassifier *mouthCC, cv::Rect *mouthInFaceRect){

    if(mouthRect==NULL || mouthCC == NULL) {return -1;}
    vector<Rect> mouthRects;


    double scaleFactor = 1.1; //okresla o ile procent zwieksza sie pole detekcji w kazdej iteracji
    int minNeighbours = 10; //jak wiele sasiadow musi miec obiekt aby go pozostawic
    int flags = 0; //przestarzale, ale musi byc
    Size minSize(50,35);
    Size maxSize = Size();
    Rect probMouthArea(faceArea.x,
                       faceArea.y + (faceArea.height*1/2), //*2/3),
                       faceArea.width,
                       faceArea.height/2); // /3);

    mouthCC->detectMultiScale(frame(probMouthArea), mouthRects, scaleFactor,
                              minNeighbours, flags, minSize,maxSize);

    double areaMult = 0.07; //o ile znaleziony obszar zostanie powiekszony
                            //wprowadzone zeby obszar obejmowal wiekszy obszar ust

    //przeskalowanie + powiekszenie
    if(mouthRects.size() > 0){
       Rect r = mouthRects.at( getBiggestRect( mouthRects ) );

       if(mouthInFaceRect != NULL){
            mouthInFaceRect->x = r.x;
            mouthInFaceRect->y = r.y;
            mouthInFaceRect->width = r.width;
            mouthInFaceRect->height = r.height;
       }

        r.x += probMouthArea.x;
        r.y += probMouthArea.y;

        int w = areaMult * r.width;
        int h = areaMult * r.height;
        r.x = r.x - w;
        r.y = r.y - h;
        r.width = r.width + 2*w;
        r.height = r.height + h;

        mouthRect->x = r.x;
        mouthRect->y = r.y;
        mouthRect->width = r.width;
        mouthRect->height = r.height;
    }

    return mouthRects.size();
}

void removeSmallObjects(Mat *binImage,int size){

    if(size == 0) {return;}

    Scalar white(255,255,255);
    Scalar black(0,0,0);
    Mat tempImage = binImage->clone();
    vector<vector<Point> > contours;
    double area;

    findContours(tempImage,contours,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

    for(unsigned int i=0; i < contours.size(); i++){
        area = contourArea(contours.at(i));

        if(area <= size){
            drawContours(tempImage,contours,i,black,CV_FILLED);
        }
        else{
            drawContours(tempImage,contours,i,white,CV_FILLED);
        }
    }

    (*binImage) = tempImage.clone();
}

bool myAbsDiff(Mat prevMat, Mat currMat, Mat *dest){
    if(prevMat.channels() > 1 || currMat.channels() > 1){return false;}

    int heigth = ( prevMat.rows > currMat.rows) ? currMat.rows : prevMat.rows;
    int width = ( prevMat.cols > currMat.cols) ? currMat.cols : prevMat.cols;

    Mat tempPrevMat;
    Mat tempCurrMat;

    int heigthDiff = abs(prevMat.rows - heigth);
    int widthDiff = abs(prevMat.cols - width);

    tempPrevMat = prevMat( Rect( widthDiff/2,
                                    heigthDiff/2,
                                        width,
                                            heigth ) ).clone();

    heigthDiff = abs(currMat.rows - heigth);
    widthDiff = abs(currMat.cols - width);

    tempCurrMat = currMat( Rect( widthDiff/2,
                                    heigthDiff/2,
                                        width,
                                            heigth ) ).clone();

    absdiff(tempPrevMat, tempCurrMat, *dest);

    return true;
}

bool extremeCoordinates(Mat img, int *maxLeft, int *maxRight, int *maxTop, int *maxBottom){
    if(img.channels() > 1){return false;}

    vector< vector<Point> > contours;

    findContours(img, contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    int mLeft = img.cols;
    int mRight = 0;
    int mTop = img.rows;
    int mBottom = 0;

    if(contours.size() > 0){


        vector<Point> currContour;

        for(unsigned int j = 0; j < contours.size(); j++){
            currContour = contours.at(j);

            for(unsigned int i = 0; i < currContour.size(); i++){
                Point p = currContour.at(i);

                if(p.x > mRight){mRight = p.x;}
                if(p.x < mLeft){mLeft = p.x;}
                if(p.y > mBottom){mBottom = p.y;}
                if(p.y < mTop){mTop = p.y;}

            }
        }

    }
    else{
        return false;
    }

    *maxLeft = mLeft;
    *maxRight = mRight;
    *maxTop = mTop;
    *maxBottom = mBottom;

    return true;
}

Mat preparePattern(Mat image){
    int mLeft;
    int mRight;
    int mBottom;
    int mTop;

    if(extremeCoordinates(image.clone(),&mLeft,&mRight,&mTop,&mBottom)){
        Rect r(mLeft, mTop, mRight - mLeft, mBottom - mTop);
        return image(r).clone();
    }

    return Mat();
}

void calcMouthArea(Rect *mouthRect, Rect faceRect){
    //obszar ust wziety z icts_2010[...]
    mouthRect->x = faceRect.x + faceRect.width/4;
    mouthRect->y = faceRect.y + (2*faceRect.height/3);
    mouthRect->width = faceRect.width/2;
    mouthRect->height = faceRect.height/3;
}

MouthState detectMouthState(Mat mouth, Mat patternUp, Mat patternDown,
                                Mat patternRight, Mat patternLeft,
                                    Mat patternClick, Mat patternNeutral,
                                    double limit){
    Mat tmp;
    //wartosci SAD dla poszczegolnych gestow
    double sadDown;
    double sadUp;
    double sadLeft;
    double sadRight;
    double sadClick;
    double sadNeutral;
    //--------------------------------------

    //przyciecie obrazu ust
    int mLeft;
    int mRight;
    int mBottom;
    int mTop;

    extremeCoordinates(mouth.clone(),&mLeft,&mRight,&mTop,&mBottom);
    Rect r(mLeft, mTop, mRight - mLeft, mBottom - mTop);
    //----------------------------------------

    //omowienie magii w wyrazeniu: sadDown = sum(tmp)[0] / (255 * mouth.rows * mouth.cols);
    //najpierw obliczylismy absdiff wzorca i aktualnego stanu,
    //nastepnie sumujemy (sum()) wszystkie wartosci w tmp w celu policzenia SAD,
    //nastepnie dzielimy przez 255 bo takie wart max sa w podanych obrazach binarnych (0 lub 255)
    //i potem jeszcze dzielimy przez rozmiar podanego stanu ust
    //co daje nam procent rozniacych sie pikseli

    myAbsDiff(patternDown,mouth(r),&tmp);
    sadDown = sum(tmp)[0] / (255 * mouth(r).rows * mouth(r).cols);

    myAbsDiff(patternUp,mouth(r),&tmp);
    sadUp = sum(tmp)[0] / (255 * mouth(r).rows * mouth(r).cols);

    myAbsDiff(patternLeft,mouth(r),&tmp);
    sadLeft = sum(tmp)[0] / (255 * mouth(r).rows * mouth(r).cols);

    myAbsDiff(patternRight,mouth(r),&tmp);
    sadRight = sum(tmp)[0] / (255 * mouth(r).rows * mouth(r).cols);

    myAbsDiff(patternClick,mouth(r),&tmp);
    sadClick = sum(tmp)[0] / (255 * mouth(r).rows * mouth(r).cols);

    myAbsDiff(patternNeutral,mouth(r),&tmp);
    sadNeutral = sum(tmp)[0] / (255 * mouth(r).rows * mouth(r).cols);

    double minimal1;
    double minimal2;
    double minimal3;
    double minimal;

    //wyznaczenie minimalnej wartosci SAD
    minimal1 = (sadDown < sadUp) ? sadDown : sadUp;
    minimal2 = (sadRight < sadLeft) ? sadRight : sadLeft;
    minimal3 = (sadClick < sadNeutral) ? sadClick : sadNeutral;
    minimal = ((minimal1 < minimal2) ? ((minimal3 < minimal1) ? minimal3 : minimal1) : ((minimal3 < minimal2) ? minimal3 : minimal2));
    //-----------------------------------

    MouthState mouthState;
    //jesli jakikolwiek wzorzec rozni sie mniej niz limit to zwracamy odpowiedni stan
    if(minimal < limit)
        mouthState = ((minimal == sadDown) ? MOUTH_DOWN : ((minimal == sadUp) ? MOUTH_UP : ((minimal == sadLeft) ? MOUTH_LEFT : ((minimal == sadRight) ? MOUTH_RIGHT :  ((minimal == sadClick) ? MOUTH_CLICK : ((minimal == sadNeutral) ? MOUTH_NEUTRAL : MOUTH_UNDEFINED))))));
    else{//jesli nie to stan jest niezidentyfikowany
       mouthState = MOUTH_UNDEFINED;
    }

    return mouthState;
}

void detectMouth(const Mat mouth, Mat *dest){

    //zrodlo algorytmu:
    //"LIP DETECTION BASED-ON NORMALIZED RGB CHROMATICITY DIAGRAM"
    //Aryuanto Soetedjo, Koichi Yamada, F. Yudi Limpraptono

    if(mouth.channels()!=3){return;}

    Mat R(mouth.rows, mouth.cols,CV_8UC1);
    Mat G(mouth.rows, mouth.cols,CV_8UC1);
    Mat B(mouth.rows, mouth.cols,CV_8UC1);

    //Wyciagniecie osobnych kanalow z obrazu BGR
    Mat bgr[] = {B,G,R};
    split(mouth,bgr);
    //-----------------------------------------

    double r[mouth.rows][mouth.cols];
    double g[mouth.rows][mouth.cols];
    Mat igr(mouth.rows,mouth.cols,CV_32FC1);

    //Obliczenie chrominancji RG oraz wyrazenia Igr ze zrodla.
    for(int row = 0; row < mouth.rows; row++){
        for(int col = 0; col < mouth.cols; col++){

            int RGB = R.at<unsigned char>(row,col) +
                        G.at<unsigned char>(row,col) +
                           B.at<unsigned char>(row,col);
            //chrominancja RG
            r[row][col] = (double)R.at<unsigned char>(row,col)/(double)RGB;
            g[row][col] = (double)G.at<unsigned char>(row,col)/(double)RGB;

            //wyrazenie Igr
            double temp = ((100 + (100*g[row][col]) - (100*r[row][col]))/200);
            igr.at<float>(row,col) = (float)temp;

        }
    }
    //------------------------------------------------------

    normalize(igr,igr,0,255,NORM_MINMAX,CV_8UC1); //normalizacja

    threshold(igr,igr,0,255,CV_THRESH_OTSU); //binaryzacja alg. Otsu
    igr = 255 - igr; //odwrocenie barw

    //tu powinno byc jeszcze porownanie r-g<TR, jak w zrodle
    //ale z niewiadomych przyczyn zawsze jest prawdziwe wiec zostalo
    //pominiete i zastosowano sama binaryzacje

    *dest = igr.clone();
}

