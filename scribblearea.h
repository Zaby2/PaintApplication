#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include "linkedlist.h"
#include <QPen>
#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPoint>

namespace Ui {
class ScribbleArea;
}

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget *parent = nullptr);
    bool openImageconst (const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const {return modified;}
    QColor penColor() const {return myPenColor;}
    int penWidth() const{return myPenWidth;}
    ~ScribbleArea();
public slots:
    void clearImage();
    void print();
    void unDoAction();
    void reDoAction();
    void drawStr(int num);
    int curDrawing();
    void setStyle(int num);
    void getCurStyle(int num);


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void saveAct();
    QImage applyEffectToPics(QGraphicsEffect *effect, int extent = 0);
    void setBlur(int x, int y);

private:
    Ui::ScribbleArea *ui;


    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    bool modified;
    bool drawing;
    QColor myPenColor;
    QImage image;
    QImage oldImage_;
    QPoint lastPoint;
    QPen pen;
    int draw = 0;
    int style = 0;
    int myPenWidth;
    int listLen = 0;
    QString text;
public:
    LinkedList *head = nullptr;
    LinkedList *tail = nullptr;
};

#endif // SCRIBBLEAREA_H
