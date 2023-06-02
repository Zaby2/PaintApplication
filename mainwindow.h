
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scribblearea.h"
#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) override;


private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void setEraser();
    void setBrush();
    void drawStr();
    void drawRect();
    void drawEllip();
    void drawNrect();
    void drawAirBrush();
    void setStyleSolid();
    void setStyleDash();
    void setStyleDot();
    void setStyleDashDot();
    void _blurEffect();
    void _dropShadowEffect();
    void _colorizeEffect();
    void _opacityEffect();
    void _mirrorEffect();
    void _turnLeft();
    void _turnRight();
    void _makeBlackWhite();
    void _setBlurBrush();
    void _setBackgroundColor();

private:
    Ui::MainWindow *ui;
    ScribbleArea *sc;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *eraserMenu;
    QMenu *figuresMenu;
    QMenu *styleMenu;
    QMenu *imageEffects;
    QAction *openAct;
    QAction *exitAct;
    QList<QAction*> saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *eraserAct;
    QAction *straightLine;
    QAction *rectangle;
    QAction *ellipse;
    QAction *unDoAct;
    QAction *reDoAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *brushAct;
    QAction *blurBrushAct;
    QAction *nRectAct;
    QAction *solidLine_;
    QAction *dashLine_;
    QAction *dashDotLine_;
    QAction *dashDotDotLine_;
    QAction *airBrush;
    QAction *blurEffect;
    QAction *dropShadowEffect;
    QAction *colorizeEffect;
    QAction *opacityEffect;
    QAction *mirrorEffect;
    QAction *turnLeft;
    QAction *turnRight;
    QAction *makeBlackWhite;
    QAction *setBackgroundColor;
    QColor lastColor;
    void createActions();
    void createMenus();
    bool mbSave();
    bool saveFile(const QByteArray &fileFormat);
    bool erasing;

    int listLen;

};

#endif // MAINWINDOW_H
