#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Projekt_IM");
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    patternPreview = new QLabel();
    patternPreview->setScaledContents(true);
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(setSteeringEnabled()));
    connect(ui->stopButton,SIGNAL(clicked()),this,SLOT(setSteeringDisabled()));

    //podpiecie okna konfiguracji
    cfgWindow = new ConfigWindow(0);
    connect(ui->confWindowButton,SIGNAL(pressed()),cfgWindow,SLOT(show()));
    connect(cfgWindow,SIGNAL(minHueChanged(int)),this,SLOT(setHueMin(int)));
    connect(cfgWindow,SIGNAL(maxHueChanged(int)),this,SLOT(setHueMax(int)));
    connect(cfgWindow,SIGNAL(minValChanged(int)),this,SLOT(setValMin(int)));
    connect(cfgWindow,SIGNAL(maxValChanged(int)),this,SLOT(setValMax(int)));
    connect(cfgWindow,SIGNAL(minSatChanged(int)),this,SLOT(setSatMin(int)));
    connect(cfgWindow,SIGNAL(maxSatChanged(int)),this,SLOT(setSatMax(int)));
    connect(cfgWindow,SIGNAL(minObjSizeChanged(int)),this,SLOT(setMinObjectSize(int)));
    connect(cfgWindow,SIGNAL(dblClickDelayChanged(int)),this,SLOT(setDblClickDelay(int)));
    connect(cfgWindow,SIGNAL(maxPercDiffChanged(int)),this,SLOT(setMaxPercDiff(int)));

    connect(cfgWindow,SIGNAL(upPatternSaved()),this,SLOT(saveUpPattern()));
    connect(cfgWindow,SIGNAL(downPatternSaved()),this,SLOT(saveDownPattern()));
    connect(cfgWindow,SIGNAL(rightPatternSaved()),this,SLOT(saveRightPattern()));
    connect(cfgWindow,SIGNAL(leftPatternSaved()),this,SLOT(saveLeftPattern()));
    connect(cfgWindow,SIGNAL(clickPatternSaved()),this,SLOT(saveClickPattern()));
    connect(cfgWindow,SIGNAL(neutralPatternSaved()),this,SLOT(saveNeutralPattern()));
    //-----------------------------------------------
    //inicjalizacja parametrow przetwarzania
    hueMin=145;
    hueMax=180;
    satMin=80;
    satMax=255;
    valMin=75;
    valMax=255;
    minObjectSize=150;
    maxPercDiff = 0.15;
    dblClickDelay = 3;
    steeringEnabled = false;
    //-----------------------------------------------

    //uruchomienie kamery i wczytanie kaskady haar'a
    videoCapture = new VideoCapture(-1);

    QString cascadePath = qApp->applicationDirPath() +
                            "/haarcascade_frontalface_alt2.xml";
    faceCC = new CascadeClassifier(cascadePath.toStdString());

    if(videoCapture->isOpened()){
        Mat testFrame;
        *videoCapture >> testFrame;
        if(!testFrame.empty()){
            //this->setFixedHeight(testFrame.rows+70);
            //this->setFixedWidth(testFrame.cols);
            QTimer::singleShot(FRAME_INTERVAL, this,  SLOT(updateImages()));
        }
    }
    else{
        ui->allFrameView->setText("Nie wykryto kamery!");
        ui->mouthView->setText("Nie wykryto kamery!");
    }

    if(faceCC->empty()){
        qDebug() << "Error: Could not load one of cascade classifiers.";
    }
    //-----------------------------------------------------
}


/**
 * @brief Pobiera obraz z kamery i wykonuje na nim wszystkie potrzebne
 *        operacje, a nastepnie wywoluje akcje zwiazana z aktualnym stanem
 *        ust. Jest wywolywana asynchronicznie co FRAME_INTERVAL ms;
 */
void MainWindow::updateImages(){

    *videoCapture >> frame;

    if(!frame.empty()){

        if(!patternCLICK.empty() && !patternDOWN.empty()
                && !patternLEFT.empty() && !patternNEUTRAL.empty()
                && !patternRIGHT.empty() && !patternUP.empty()){
            ui->startButton->setEnabled(true);
            ui->stopButton->setEnabled(true);
        }

        //wyostrzanie
        Mat tmp;
        GaussianBlur(frame, tmp, cv::Size(0, 0), 3);
        addWeighted(frame, 1.5,tmp , -0.5, 0, frame);
        //---

        int facesCount = detectFace(frame, &faceRect, faceCC);

        if(facesCount > 0){

            calcMouthArea(&mouthRect,faceRect);

            Mat mouth;
            mouth = frame(mouthRect).clone(); //wyciecie kawalka z ustami

            Mat mouthHSV;
            cvtColor(mouth, mouthHSV, CV_BGR2HSV); //przeksztalcenie do HSV

            //wykrycie ust po barwie w HSV
            Mat hueInRange;
            inRange(mouthHSV,Scalar(hueMin,satMin,valMin),Scalar(hueMax,satMax,valMax),hueInRange);

            //zalewanie otworow
            Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size( 3,3 ));
            morphologyEx(hueInRange,hueInRange,MORPH_CLOSE,kernel);

            //wyciecie malych obiektow ktore nie naleza do ust
            removeSmallObjects(&hueInRange,minObjectSize);

            finalMouth = hueInRange.clone();

            QImage mouthImage = convertMatToQImage(finalMouth.clone());
            QImage scaledMouth = mouthImage.scaled( mouthImage.width()*4,
                                                    mouthImage.height()*4);
            ui->mouthView->setPixmap(QPixmap::fromImage(scaledMouth));

            //odczytanie stanu ust i wykonanie akcji
            //nacisniecie CTRL odblokowuje kursor
            if(steeringEnabled &&
                    QApplication::queryKeyboardModifiers() != Qt::ControlModifier){
                MouthState mouthState = detectMouthState(finalMouth,patternUP,
                                                        patternDOWN, patternRIGHT,
                                                        patternLEFT,patternCLICK,
                                                        patternCLICK,maxPercDiff);
                QPoint cursorPos = QCursor::pos();
                if(mouthState == MOUTH_LEFT){cursorPos.setX( cursorPos.x() - MOUSE_MOVE );}
                else if(mouthState == MOUTH_RIGHT){cursorPos.setX( cursorPos.x() + MOUSE_MOVE );}
                else if(mouthState == MOUTH_UP){cursorPos.setY( cursorPos.y() - MOUSE_MOVE );}
                else if(mouthState == MOUTH_DOWN){cursorPos.setY( cursorPos.y() + MOUSE_MOVE );}
                QCursor::setPos(cursorPos);

                QString state = (mouthState==MOUTH_UNDEFINED) ? "MOUTH_UNDEFINED" : "";
                state = (mouthState==MOUTH_LEFT) ? "MOUTH_LEFT" : state;
                state = (mouthState==MOUTH_RIGHT) ? "MOUTH_RIGHT" : state;
                state = (mouthState==MOUTH_UP) ? "MOUTH_UP" : state;
                state = (mouthState==MOUTH_DOWN) ? "MOUTH_DOWN" : state;
                state = (mouthState==MOUTH_CLICK) ? "MOUTH_CLICK" : state;
                state = (mouthState==MOUTH_NEUTRAL) ? "MOUTH_NEUTRAL" : state;
                qDebug() << state;
            }
            //-------------------------------

        }

        drawRect(frame, faceRect, BLUE);
        drawRect(frame, mouthRect, RED);
        QImage faceImage = convertMatToQImage(frame);

        ui->allFrameView->setPixmap(QPixmap::fromImage(faceImage));
    }
    //zakolejkowanie ponownego wywołania
    QTimer::singleShot(FRAME_INTERVAL, this,SLOT(updateImages()));
}

/**
 * @brief Konwertuje typ cv::Mat do typu QImage
 * @param mat obraz o jednym lub trzech kanalach
 * @return przekonertowany obraz QImage
 */
QImage MainWindow::convertMatToQImage(const Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void MainWindow::setHueMin(int value){
    hueMin = value;
}

void MainWindow::setHueMax(int value){
    hueMax = value;
}

void MainWindow::setSatMin(int value){
    satMin = value;
}

void MainWindow::setSatMax(int value){
    satMax = value;
}

void MainWindow::setValMin(int value){
    valMin = value;
}

void MainWindow::setValMax(int value){
    valMax = value;
}

void MainWindow::setMinObjectSize(int value){
    minObjectSize = value;
}

void MainWindow::setMaxPercDiff(int value){
    maxPercDiff = (((double)value) / 100);
}

void MainWindow::setDblClickDelay(int value){
    dblClickDelay = value;
}

void MainWindow::saveUpPattern(){
    this->patternUP = preparePattern(finalMouth);
    showPatternPreview(this->patternUP, "Gest: do góry");

}
void MainWindow::saveDownPattern(){
    this->patternDOWN = preparePattern(finalMouth);
    showPatternPreview(this->patternDOWN, "Gest: w dół");
}
void MainWindow::saveLeftPattern(){
    this->patternLEFT = preparePattern(finalMouth);
    showPatternPreview(this->patternLEFT, "Gest: w lewo");
}
void MainWindow::saveRightPattern(){
    this->patternRIGHT = preparePattern(finalMouth);
    showPatternPreview(this->patternRIGHT, "Gest: w prawo");
}
void MainWindow::saveClickPattern(){
    this->patternCLICK = preparePattern(finalMouth);
    showPatternPreview(this->patternCLICK, "Gest: kliknięcie");
}
void MainWindow::saveNeutralPattern(){
    this->patternNEUTRAL = preparePattern(finalMouth);
    showPatternPreview(this->patternNEUTRAL, "Gest: neutralny");
}

void MainWindow::showPatternPreview(Mat img,QString windowTitle){
    QImage image = convertMatToQImage(img);

    patternPreview->clear();
    patternPreview->setWindowTitle(windowTitle);
    patternPreview->setFixedHeight(image.height()*4);
    patternPreview->setFixedWidth(image.width()*4);
    patternPreview->setPixmap(QPixmap::fromImage(image.scaled( image.width()*4,
                                                               image.height()*4)));
    patternPreview->show();
}

void MainWindow::closeEvent(QCloseEvent *event){
    QMainWindow::closeEvent(event);
    patternPreview->close();
    cfgWindow->close();
}

MainWindow::~MainWindow()
{
    delete cfgWindow;
    delete faceCC;
    delete videoCapture;
    delete ui;
    delete patternPreview;
}
