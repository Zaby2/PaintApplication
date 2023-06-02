
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QByteArray>
#include <QColorDialog>
#include <QInputDialog>
#include <QImageWriter>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    sc = new ScribbleArea();
    QCursor cur = QCursor(QPixmap(":resources/resources/brush1.png").scaled(20,20));
    this->setCursor(cur);
    setCentralWidget(sc);
    createActions();
    createMenus();
    setWindowTitle(tr("Paint"));
    QRect screenGeometry = screen()->geometry();
    lastColor = QColor("blue");
    resize(screenGeometry.width(), screenGeometry.height());
    qDebug() << QDir::currentPath();

}

void MainWindow::setStyleSolid() {
    sc->setStyle(0);
}
void MainWindow::setStyleDash() {
    sc->setStyle(1);
}
void MainWindow::setStyleDot() {
    sc->setStyle(2);
}
void MainWindow::setStyleDashDot() {
    sc->setStyle(3);
}
void MainWindow::_blurEffect() {
    sc->getCurStyle(1);

}
void MainWindow::_dropShadowEffect() {
    sc->getCurStyle(2);
}
void MainWindow::_colorizeEffect() {
    sc->getCurStyle(3);

}
void MainWindow::_opacityEffect() {
    sc->getCurStyle(4);

}
void MainWindow::_setBackgroundColor() {
    sc->getCurStyle(9);

}
void MainWindow::_makeBlackWhite() {
    sc->getCurStyle(5);

}
void MainWindow::_mirrorEffect() {
    sc->getCurStyle(6);
}
void MainWindow::_turnRight() {
    sc->getCurStyle(7);

}
void MainWindow::_turnLeft() {
    sc->getCurStyle(8);

}


void MainWindow::drawStr() {
    sc->setPenColor(lastColor);
    sc->drawStr(1);
    lastColor = sc->penColor();
    QCursor cur = QCursor(QPixmap(":resources/resources/line.png").scaled(16,16));
    this->setCursor(cur);
}
void MainWindow::drawRect() {
    sc->setPenColor(lastColor);
    sc->drawStr(2);
    lastColor = sc->penColor();
    QCursor cur = QCursor(QPixmap(":resources/resources/line.png").scaled(16,16));
    this->setCursor(cur);
}
void MainWindow::drawEllip() {
    sc->setPenColor(lastColor);
    sc->drawStr(3);
    lastColor = sc->penColor();
    QCursor cur = QCursor(QPixmap(":resources/resources/line.png").scaled(16,16));
    this->setCursor(cur);
}
void MainWindow::drawNrect() {
     sc->setPenColor(lastColor);
    sc->drawStr(4);
    lastColor = sc->penColor();
    QCursor cur = QCursor(QPixmap(":resources/resources/line.png").scaled(16,16));
    this->setCursor(cur);
}

void MainWindow::drawAirBrush() {
    sc->setPenColor(lastColor);
    sc->drawStr(5);
    lastColor = sc->penColor();
    QCursor cur = QCursor(QPixmap(":resources/resources/text.png").scaled(16,16));
    this->setCursor(cur);

}
void MainWindow::_setBlurBrush() {
    sc->setPenColor(lastColor);
    sc->drawStr(6);
    QCursor cur = QCursor(QPixmap(":resources/resources/blur.png").scaled(16,16));
    this->setCursor(cur);

}



void MainWindow::setBrush() {
    sc->drawStr(0);
    sc->setPenColor(lastColor);
    erasing = false;
    QCursor cur = QCursor(QPixmap(":resources/resources/brush1.png").scaled(20,20));
    this->setCursor(cur);

}

void MainWindow::setEraser() {
     sc->drawStr(0);
    erasing = true;
    QColor eraser = QColor("white");
    lastColor = sc->penColor();
    //qDebug() << lastColor;
    sc->setPenColor(eraser);
    QCursor cur = QCursor(QPixmap(":resources/resources/eraser.png").scaled(16,16));
    this->setCursor(cur);
}


void MainWindow::closeEvent(QCloseEvent *event) {
    if (mbSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
void MainWindow::open() {
    if(mbSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QDir::currentPath());
if(!fileName.isEmpty()) {
            sc->openImageconst(fileName);
    }
}
}
void MainWindow::save() {
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFor = action->data().toByteArray();
    saveFile(fileFor);

}

void MainWindow::penColor() {
    QCursor cur = QCursor(QPixmap(":resources/resources/brush1.png").scaled(20,20));
    this->setCursor(cur);
   // if(erasing) {
    sc->setPenColor(lastColor);
    erasing = false;
  //  }
    QColor newColor = QColorDialog::getColor(sc->penColor());
    if(newColor.isValid()) {
    sc->setPenColor(newColor);
    lastColor = sc->penColor();
    }
}


void MainWindow::penWidth() {
    bool btn;
    int nWidth = QInputDialog::getInt(this, tr("Paint"), tr("Толщина кисти: "),sc->penWidth(), 1,40, 1, &btn);
    if(btn) {
    sc->setPenWidth(nWidth);
    }
}

void MainWindow::createActions() {
    openAct = new QAction(tr("Открыть файл"), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    foreach(QByteArray form, QImageWriter::supportedImageFormats()){
        QString txt = tr("%1...").arg(QString(form).toUpper());
        QAction *action = new QAction(txt, this);
        action->setData(form);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }
    exitAct = new QAction(tr("Выход"), this);
    exitAct->setShortcut(QKeySequence::Close);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    penColorAct = new QAction(tr("Цвет"), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
    penWidthAct = new QAction(tr("Толщина"), this);
    connect( penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));
    clearScreenAct = new QAction(tr("Очистить экран"), this);
    connect( clearScreenAct, SIGNAL(triggered()), sc, SLOT(clearImage()));//&&&

    eraserAct = new QAction(tr("Стиратель"), this);
    connect( eraserAct, SIGNAL(triggered()), this, SLOT(setEraser()));

    brushAct = new QAction(tr("Кисть"), this);
    blurBrushAct = new QAction(tr("Кисть размытия"), this);
    connect( brushAct, SIGNAL(triggered()), this, SLOT(setBrush()));
    connect(  blurBrushAct, SIGNAL(triggered()), this, SLOT(_setBlurBrush()));


    unDoAct = new QAction();
    unDoAct->setIcon(QIcon(":resources/resources/arrow.png"));
    connect( unDoAct, SIGNAL(triggered()), sc, SLOT(unDoAction()));

    reDoAct = new  QAction();
    reDoAct->setIcon(QIcon(":resources/resources/redo.png"));
    connect( reDoAct, SIGNAL(triggered()), sc, SLOT(reDoAction()));


    straightLine = new QAction();
    straightLine->setIcon(QIcon(":resources/resources/line.png"));
    connect( straightLine, SIGNAL(triggered()), this, SLOT(drawStr()));


    rectangle = new QAction();
    rectangle->setIcon(QIcon(":resources/resources/rect.png"));
    connect( rectangle, SIGNAL(triggered()), this, SLOT(drawRect()));


    ellipse = new QAction();
    ellipse->setIcon(QIcon(":resources/resources/ellipse.png"));
    connect( ellipse, SIGNAL(triggered()), this, SLOT(drawEllip()));

    nRectAct = new QAction();
    nRectAct->setIcon(QIcon(":resources/resources/hexagon.png"));
    connect( nRectAct, SIGNAL(triggered()), this, SLOT(drawNrect()));


    airBrush = new QAction(tr("Текст"));
    //airBrush->setIcon(QIcon(":resources/resources/text.png"));
    connect( airBrush, SIGNAL(triggered()), this, SLOT(drawAirBrush()));




    solidLine_ = new QAction(tr("Сплошная линия"));
    dashLine_ = new QAction(tr("Пунктирная линия"));
    dashDotLine_ = new QAction(tr("Штрихпунктриная линия"));
    dashDotDotLine_ = new QAction(tr("Штрихпунктирная линия (два штриха)"));
    connect(solidLine_, SIGNAL(triggered()), this, SLOT(setStyleSolid()));
    connect(dashLine_, SIGNAL(triggered()), this, SLOT(setStyleDash()));
    connect(dashDotLine_, SIGNAL(triggered()), this, SLOT(setStyleDot()));
    connect(dashDotDotLine_, SIGNAL(triggered()), this, SLOT(setStyleDashDot()));




    blurEffect = new QAction(tr("Размыть фон изображения"));

    colorizeEffect = new QAction(tr("Наложить цвет  изображение"));
    opacityEffect = new QAction(tr("Установить прозрачность изображения"));
    makeBlackWhite = new QAction(tr("Сделать изображение черно-белым"));
    mirrorEffect = new QAction(tr("Отразить изображение по горизонтали"));
    dropShadowEffect = new QAction(tr("Отразить по вертекали"));
    turnLeft = new QAction(tr("Повернуть изображение влево"));
    turnRight = new QAction(tr("Повернуть изображение вправо"));
    setBackgroundColor = new QAction(tr("Залить изображение"));

    connect(blurEffect, SIGNAL(triggered()), this, SLOT(_blurEffect()));
    connect(dropShadowEffect, SIGNAL(triggered()), this, SLOT(_dropShadowEffect()));
    connect(colorizeEffect, SIGNAL(triggered()), this, SLOT(_colorizeEffect()));
    connect(opacityEffect, SIGNAL(triggered()), this, SLOT(_opacityEffect()));
    connect(makeBlackWhite, SIGNAL(triggered()), this, SLOT(_makeBlackWhite()));
    connect(mirrorEffect, SIGNAL(triggered()), this, SLOT(_mirrorEffect()));
    connect(turnLeft, SIGNAL(triggered()), this, SLOT(_turnLeft()));
    connect(turnRight, SIGNAL(triggered()), this, SLOT(_turnRight()));
    connect(setBackgroundColor, SIGNAL(triggered()), this, SLOT(_setBackgroundColor()));}

void MainWindow::createMenus() {
    saveAsMenu = new QMenu(tr("Сохранить как"), this);
    foreach(QAction *act, saveAsActs) {
        saveAsMenu->addAction(act);

    }
    fileMenu = new QMenu(this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    fileMenu->setIcon(QIcon(":resources/resources/file.png"));

    //qDebug() << QCoreApplication::applicationDirPath();
    optionMenu = new QMenu(this);
    optionMenu->setIcon(QIcon(":resources/resources/paint.png"));
    optionMenu->addAction(brushAct);
    optionMenu->addAction(blurBrushAct);
    optionMenu->addAction(airBrush);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addAction(clearScreenAct);


    eraserMenu = new QMenu(this);
    eraserMenu->setIcon(QIcon(":resources/resources/eraser.png"));
    eraserMenu->addAction(eraserAct);
    eraserMenu->addAction(penWidthAct);


    figuresMenu = new QMenu(this);
    figuresMenu->setIcon(QIcon(":resources/resources/line.png"));
    figuresMenu->addAction(straightLine);
    figuresMenu->addAction(rectangle);
    figuresMenu->addAction(ellipse);
    figuresMenu->addAction(nRectAct);


    styleMenu = new QMenu(tr("Стили"), this);
    styleMenu->addAction(solidLine_);
    styleMenu->addAction(dashLine_);
    styleMenu->addAction(dashDotLine_);
    styleMenu->addAction(dashDotDotLine_);
    figuresMenu->addMenu(styleMenu);


    imageEffects = new QMenu(tr("Стили картинки"), this);
    imageEffects->addAction(blurEffect);  
    imageEffects->addAction(colorizeEffect);
    imageEffects->addAction(setBackgroundColor);
    imageEffects->addAction(opacityEffect);
    imageEffects->addAction(makeBlackWhite);
    imageEffects->addAction(mirrorEffect);
    imageEffects->addAction(dropShadowEffect);
    imageEffects->addAction(turnLeft);
    imageEffects->addAction(turnRight);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(eraserMenu);
    menuBar()->addMenu(figuresMenu);
    menuBar()->addMenu(imageEffects);
    menuBar()->addAction(unDoAct);
    menuBar()->addAction(reDoAct);



}
bool MainWindow::mbSave() {
    if(sc->isModified()) {
        QMessageBox saving;
        saving.setText("Вы хотите сохранить изменения?");
        saving.setInformativeText("Картинка была изменена");
        saving.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        saving.setDefaultButton(QMessageBox::Save);
        int res = saving.exec();
        switch(res) {
        case QMessageBox::Save:
            saveFile("png");
           // int i = 0;

            break;
        case QMessageBox::Cancel:
            return true;

        case QMessageBox::Discard:
        return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat) {
    QString path = QDir::currentPath() + "/untilted." + fileFormat;
    QString fl = QFileDialog::getSaveFileName(this, tr("Сохранить как") ,path); // Формат
    fl += "." + fileFormat;

    if(fl.isEmpty()) {
        return false;
    }
    else {
        return sc->saveImage(fl, fileFormat.constData());

    }
}



MainWindow::~MainWindow()
{
    delete ui;
}


