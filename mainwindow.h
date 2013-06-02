#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <QPoint>
#include <QFile>
#include <QMessageBox>
#include <windows.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2\opencv.hpp"
#include "mouthStateDet.h"
#include "configwindow.h"

#define FRAME_INTERVAL 30

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
    QString patternPath;

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
    bool colorSpaceBGR; //true - kolory BGR, false - kolory RGB
    bool sharpen;   //czy wyostrzac obraz
    int minObjectSize; //minimalny rozmiar obiektu
    double maxPercDiff; //maksymalna roznica miedzy wzorcem gestu a aktualnym obrazem
                        //zeby zostal uznany za poprawnie sklasyfikowany
    int dblClickDelay; //ilosc sek. jaka trzeba utrzymywac gest klikniecia aby
                        //zostal uznany za podwojne klikniecie
    int steeringEnabled; //wlaczone sterowanie myszka
    int mouseSpeed;

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

    /**
     * @brief Wywoluje klikniecie lewym przyciskiem. Korzysta z WinAPI.
     */
    void leftClick();

private slots:
    /**
     * @brief Zapisuje zaprogramowane gesty. Aby zapisac wszystkie
     *          gesty musza byc wprowadzone.
     */
    void savePatterns();
    /**
     * @brief Wczytuje gesty. Aby wczytac, wszystkie 6 gestow
     *          musi byc dostepne w katalogu programu w folderze "patterns".
     */
    void loadPatterns();

public slots:
    /**
     * @brief Pobiera obraz z kamery i wykonuje na nim wszystkie potrzebne
     *        operacje, a nastepnie wywoluje akcje zwiazana z aktualnym stanem
     *        ust. Jest wywolywana asynchronicznie co FRAME_INTERVAL ms;
     */
    void updateImages();

    void setMinObjectSize(int value);
    void setMaxPercDiff(int value);
    void setDblClickDelay(int value);
    void setSharpen(bool value);
    void setMouseSpeed(int val);

    void saveUpPattern();
    void saveDownPattern();
    void saveLeftPattern();
    void saveRightPattern();
    void saveClickPattern();
    void saveNeutralPattern();

    void setSteeringEnabled(){steeringEnabled = true;}
    void setSteeringDisabled(){steeringEnabled = false;}
    void setColorSpace(bool value){colorSpaceBGR = value;}

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
