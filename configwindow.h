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
    void minObjSizeChanged(int val);
    void maxPercDiffChanged(int val);
    void dblClickDelayChanged(int val);
    void upPatternSaved();
    void downPatternSaved();
    void leftPatternSaved();
    void rightPatternSaved();
    void clickPatternSaved();
    void neutralPatternSaved();
    void sharpenCheckboxClicked(bool checked);
    void mouseSpeedValueChanged(int val);
    void colorSpaceChanged(bool checked);

public slots:

    void emitSharpenCheckboxClicked(bool checked);
    void emitMinObjSizeChanged(int val);
    void emitMaxPercDiffChanged(int val);
    void emitDblClickDelayChanged(int val);
    void emitMouseSpeedValueChanged(int value);
    void emitColorSpaceChanged(bool checked){emit colorSpaceChanged(checked);}

    void emitUpPatternSaved(){emit upPatternSaved();}
    void emitDownPatternSaved(){emit downPatternSaved();}
    void emitLeftPatternSaved(){emit leftPatternSaved();}
    void emitRightPatternSaved(){emit rightPatternSaved();}
    void emitClickPatternSaved(){emit clickPatternSaved();}
    void emitNeutralPatternSaved(){emit neutralPatternSaved();}

};

#endif // CONFIGWINDOW_H
