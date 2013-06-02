#ifndef MOUTHSTATEDET_H_INCLUDED
#define MOUTHSTATEDET_H_INCLUDED

#include "opencv2\core\core.hpp"
#include "opencv2\opencv.hpp"

/**
 * @file mouthStateDet.h
 * @brief Zawiera funkcje przetwarzajace obraz i analizujace stan ust.
 */

enum MouthState{

    MOUTH_UP,
    MOUTH_DOWN,
    MOUTH_RIGHT,
    MOUTH_LEFT,
    MOUTH_CLICK,
    MOUTH_DOUBLE_CLICK,
    MOUTH_UNDEFINED,
    MOUTH_NEUTRAL
};

enum RectColor{
    RED,
    GREEN,
    BLUE
};

/**
 * @brief Wyznacza na podstawie parametrow aktualny stan ust.
 * @param mouth Binarny obraz ust.
 * @param patternUp Binarny wzor gestu "do gory".
 * @param patternDown Binarny wzor gestu "w dol".
 * @param patternRight Binarny wzor gestu "w prawo".
 * @param patternLeft Binarny wzor gestu "w lewo".
 * @param patternClick Binarny wzor gestu "klikniecie".
 * @param patternNautral Binarny wzor gestu "neutralny".
 * @param limit Procentowy limit uznania gestu za poprawny.
 * @return Stan ust.
 */
MouthState detectMouthState(cv::Mat mouth, cv::Mat patternUp, cv::Mat patternDown,
                                cv::Mat patternRight, cv::Mat patternLeft,
                                    cv::Mat patternClick, cv::Mat patternNeutral, double limit);
/**
 * @brief Wykrywa za pomoca kaskad Haar'a obszar twarzy na podanym obrazie.
 * @param frame Ramka kolorowego obrazu.
 * @param faceRect cv::Rect na wyznaczony obszar twarzy.
 * @param faceCC Wskaźnik do zainicjowanego klasyfikatora kaskadowego.
 * @return Ilosc wykrytych na obrazie twarzy.
 */
int detectFace(const cv::Mat& frame, cv::Rect *faceRect, cv::CascadeClassifier *faceCC);

/**
 * @brief Wykrywa usta na podanym obszarze twarzy. Obecnie nieuzywana poniewaz
 *          niepoprawnie wykrywa usta gdy sa otwarte. Zastapione przez calcMouthArea()
 * @param frame
 * @param faceArea
 * @param mouthRect
 * @param mouthCC
 * @param mouthInFaceRect
 * @return
 */
int detectMouth(const cv::Mat& frame, cv::Rect faceArea, cv::Rect *mouthRect,cv::CascadeClassifier *mouthCC, cv::Rect *mouthInFaceRect = NULL);

/**
 * @brief Rysuje na podanej ramce prostokaty.
 * @param frame Ramka obrazu.
 * @param rect Prostokat do narysowania.
 * @param color Kolor prostokata.
 */
void drawRect(cv::Mat &frame,cv::Rect rect, RectColor color);

/**
 * @brief Usuwa obiekty mniejsze niz podany rozmiar.
 * @param binImage Obraz w formacie binarnym.
 * @param size Minimalny rozmiar obiektu (ilosc pikseli).
 */
void removeSmallObjects(cv::Mat *binImage,int size);

/**
 * @brief Umozliwia absdiff na obrazkach o roznych wymiarach. Ma sens w przypadku
 *        malych roznic w wymiarach. Przycina do mniejszego wymiaru.
 * @param prevMat Jeden z obrazow w formacie binarnym.
 * @param currMat Drugi z obrazow w formacie binarnym.
 * @param dest Obraz w ktorym zapisywany jest wynik. Przyjmuje rozmiar mniejszego z obrazow.
 * @return TRUE - jesli nie bylo bledow, FALSE - w przeciwnym przypadku.
 */
bool myAbsDiff(cv::Mat prevMat, cv::Mat currMat, cv::Mat *dest);

/**
 * @brief Wyznacza skrajne wspolrzedne na obrazie binarnym.
 * @param img Obraz binarny.
 * @param maxLeft Zmienna do zapisania skrajnego lewego polozenia.
 * @param maxRight Zmienna do zapisania skrajnego prawego polozenia.
 * @param maxTop Zmienna do zapisania skrajnego gornego polozenia.
 * @param maxBottom Zmienna do zapisania skrajnego dolnego polozenia.
 * @return TRUE - jesli nie bylo bledow, FALSE - w przeciwnym przypadku.
 */
bool extremeCoordinates(cv::Mat img, int *maxLeft, int *maxRight, int *maxTop, int *maxBottom);

/**
 * @brief Docina gest do jego wymiarów.
 * @param image Binary obraz ust.
 * @return Przycięty gest gotowy do zapisania.
 */
cv::Mat preparePattern(cv::Mat image);

/**
 * @brief Wyznacza na podstawie faceRect obszar ust i wpisuje go do mouthRect
 * @param mouthRect Obszar ust.
 * @param faceRect Obszar twarzy.
 */
void calcMouthArea(cv::Rect *mouthRect, cv::Rect faceRect);

/**
 * @brief Zwraca z wektora cv::Rect o najwiekszej powierzchni.
 * @param rects Wektor cv::Rect.
 * @return cv::Rect o najwiekszej powierzchni.
 */
int getBiggestRect(const vector<cv::Rect> rects);

#endif // MOUTHSTATEDET_H_INCLUDED
