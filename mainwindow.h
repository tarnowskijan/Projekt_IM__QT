#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <QPoint>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2\opencv.hpp"
#include "mouthStateDet.h"
#include "configwindow.h"

#define FRAME_INTERVAL 30
#define MOUSE_MOVE 5

using namespace cv;

namespace Ui {
class MainWindow;
}

/**
 * @brief Klasa glownego okna programu. Tutaj jest wywolywana cala logika przetwarzania obrazu.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    ConfigWindow *cfgWindow;
    QLabel *patternPreview; //okno do wyswietlania zapisanego gestu

    VideoCapture *videoCapture; //obiekt przechwytujacy wideo
    CascadeClassifier *faceCC; //obiekt poszukujacy twarzy za pomoca kaskad Haar'a

    //zmienne przechowujace zapisane gesty
    Mat patternDOWN;
    Mat patternUP;
    Mat patternLEFT;
    Mat patternRIGHT;
    Mat patternCLICK;
    Mat patternNEUTRAL;
    //------------------------------------
    //przechowuja obszary
    Mat frame;
    Mat finalMouth;
    Rect faceRect;
    Rect mouthRect;
    Rect mouthInFaceRect;
    //------------------------------------

    //wartosci HSV dla ust, dobrane empirycznie
    int hueMin;
    int hueMax;
    int satMin; //zalezne od oswietlenia sceny
    int satMax;
    int valMin;
    int valMax;
    //----------------------------------------
    int minObjectSize; //minimalny rozmiar obiektu
    double maxPercDiff; //maksymalna roznica miedzy wzorcem gestu a aktualnym obrazem
                        //zeby zostal uznany za poprawnie sklasyfikowany
    int dblClickDelay; //ilosc sek. jaka trzeba utrzymywac gest klikniecia aby
                        //zostal uznany za podwojne klikniecie
    int steeringEnabled; //wlaczone sterowanie myszka

    /**
     * @brief Konwertuje typ cv::Mat do typu QImage
     * @param mat obraz o jednym lub trzech kanalach
     * @return przekonertowany obraz QImage
     */
    QImage convertMatToQImage(const Mat& mat);

    /**
     * @brief Wyswietla powiekszony obraz zapisanego gestu.
     * @param img Obraz gestu.
     * @param windowTitle Tytul okna.
     */
    void showPatternPreview(Mat img, QString windowTitle);

public slots:
    /**
     * @brief Pobiera obraz z kamery i wykonuje na nim wszystkie potrzebne
     *        operacje, a nastepnie wywoluje akcje zwiazana z aktualnym stanem
     *        ust. Jest wywolywana asynchronicznie co FRAME_INTERVAL ms;
     */
    void updateImages();

    void setHueMin(int value);
    void setHueMax(int value);
    void setSatMin(int value);
    void setSatMax(int value);
    void setValMin(int value);
    void setValMax(int value);

    void setMinObjectSize(int value);
    void setMaxPercDiff(int value);
    void setDblClickDelay(int value);

    void saveUpPattern();
    void saveDownPattern();
    void saveLeftPattern();
    void saveRightPattern();
    void saveClickPattern();
    void saveNeutralPattern();

    void setSteeringEnabled(){steeringEnabled = true;}
    void setSteeringDisabled(){steeringEnabled = false;}

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
