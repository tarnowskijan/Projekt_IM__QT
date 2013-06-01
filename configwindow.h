#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>

namespace Ui {
class ConfigWindow;
}

/**
 * @brief Okno konfiguracyjne. Jedynie przekazuje akcje użytkownika poprzez sygnały.
 */
class ConfigWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::ConfigWindow *ui;
    
public:
    explicit ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();

signals:
    void minHueChanged(int val);
    void maxHueChanged(int val);
    void minSatChanged(int val);
    void maxSatChanged(int val);
    void minValChanged(int val);
    void maxValChanged(int val);
    void minObjSizeChanged(int val);
    void maxPercDiffChanged(int val);
    void dblClickDelayChanged(int val);
    void upPatternSaved();
    void downPatternSaved();
    void leftPatternSaved();
    void rightPatternSaved();
    void clickPatternSaved();
    void neutralPatternSaved();

public slots:
    void emitMinHueChanged(int val);
    void emitMaxHueChanged(int val);
    void emitMinSatChanged(int val);
    void emitMaxSatChanged(int val);
    void emitMinValChanged(int val);
    void emitMaxValChanged(int val);
    void emitMinObjSizeChanged(int val);
    void emitMaxPercDiffChanged(int val);
    void emitDblClickDelayChanged(int val);

    void emitUpPatternSaved(){emit upPatternSaved();}
    void emitDownPatternSaved(){emit downPatternSaved();}
    void emitLeftPatternSaved(){emit leftPatternSaved();}
    void emitRightPatternSaved(){emit rightPatternSaved();}
    void emitClickPatternSaved(){emit clickPatternSaved();}
    void emitNeutralPatternSaved(){emit neutralPatternSaved();}

};

#endif // CONFIGWINDOW_H
