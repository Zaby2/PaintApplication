#include "scribblearea.h"
#include "qevent.h"
#include "ui_scribblearea.h"
#include "linkedlist.h"

#include "QPrintDialog"
#include "QPainter"
#include "QPrinter"
#include <QRandomGenerator>
#include <QLineEdit>
#include <QInputDialog>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsEffect>
#include <QLabel>
#include <QTransform>



ScribbleArea::ScribbleArea(QWidget *parent) :
    QWidget(parent)
{

    //setAttribute(Qt::WA_StaticContents);
   modified = false;
   drawing = false;
   myPenWidth = 5;
   myPenColor = Qt::blue;
   //setStyle(0);
   QRect screenGeometry = screen()->geometry();
   resizeImage(&image, QSize(screenGeometry.width(), screenGeometry.height()));
   image.fill(qRgb(255,255,255));
   head = new LinkedList();
   head->prev = nullptr;
   head->next = nullptr;
   head->image = image;
   tail = head;
   QGraphicsOpacityEffect* opacityEffect_ = new QGraphicsOpacityEffect;
   opacityEffect_->setOpacity(0.9);
   image = applyEffectToPics(opacityEffect_, 0);
   update();

   //connect(saveAct, SIGNAL(update()), )
}
QImage ScribbleArea::applyEffectToPics(QGraphicsEffect *effect, int extent)
{
   QGraphicsScene scene;
   QGraphicsPixmapItem item;
   item.setPixmap(QPixmap::fromImage(image));
   item.setGraphicsEffect(effect);
   scene.addItem(&item);
   QImage res(image.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
   res.fill(Qt::transparent);
   QPainter ptr(&res);
   scene.render(&ptr, QRectF(), QRectF( -extent, -extent, image.width()+extent*2, image.height()+extent*2 ) );
   return res;
}





void  ScribbleArea::getCurStyle(int num) {
   int height = image.height();
   int width = image.width();
   switch(num) {
   case 1:
   {
       QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect;
       image = applyEffectToPics(blurEffect , 0);
       update();
       break;
   }
   case 2:
   {

       image.mirror(true, false);

       update();
        break;
   }
   case 3:
   {
      QGraphicsColorizeEffect* colorizeEffect = new QGraphicsColorizeEffect;
        if(myPenColor != QColor("white")) {
          colorizeEffect->setColor(myPenColor);
      }
      colorizeEffect->setStrength(1);
      image = applyEffectToPics(colorizeEffect, 0);

       update();
        break;
   }
   case 4:
   {
      QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect;
        double a = QInputDialog::getDouble(this, tr("Введите степень прозрачности от 0 до 1"),tr("Прозрачность") , QLineEdit::Normal);
      if(a > 1) {
            a = 1;
      } else if(a <= 0) {
            a = 0.1;
      }
      opacityEffect->setOpacity(a);
      image = applyEffectToPics(opacityEffect, 0);
       update();
        break;
   }

   case 5:
   {
        image = image.convertToFormat(QImage::Format_Grayscale8);
        update();
        break;
   }
   case 6:
   {
        image.mirror();
        update();
        break;
   }
   case 7:
   {
        image = image.transformed(QTransform().rotate(90.0));
        image = image.scaled(width, height);
        update();
        break;
   }
   case 8:
   {
        image = image.transformed(QTransform().rotate(-90.0));
        image = image.scaled(width, height);
        update();
        break;
   }
   case 9:
   {
        image.fill(myPenColor);
        update();
        break;
   }
   }
   saveAct();

}

void ScribbleArea::setBlur(int x, int y) {
   int startX = x;
   int startY = y;
   int iter = 0;
   int redAcc = 0;
   int greenAcc = 0;
   int blueAcc = 0;
   QRgb cur;
   QColor color;
   int curX = 0;
   int _curY = 0;
   if(9 + x > image.width()) {
        curX = image.width() - x + 9 - 1;

   } else {
        curX = 18;
   }
   if(9 + y > image.height()) {
        _curY = image.height() - y + 9-1;
   } else {
        _curY = 18;
   }
   while (x > 0 && iter < 9) {
        x--;
        iter++;
   }
   iter = 0;
   while(y > 0 && iter < 9) {
        y--;
        iter++;
   }
   iter = 0;
   for(int i = 0; i < curX; i++) {
        for(int j = 0; j < _curY; j++) {
            cur = image.pixel(x, y);
            color = cur;
            redAcc += color.red();
            greenAcc += color.green();
            blueAcc += color.blue();
            iter++;
            y++;
        }
        y = startY;
        x++;
   }
   if(iter == 0) {
        iter = 1;
   }
   //qDebug() << startX << startY;
    QColor newColor = QColor(redAcc/iter, greenAcc/iter, blueAcc/iter);
    image.setPixelColor(startX, startY, newColor);

}

void ScribbleArea::saveAct() {
   if(head->next != nullptr) {
       LinkedList *cur = head->next;
       while(cur->next) {
           LinkedList *bCur = cur;
           cur = cur->next;
           cur->prev = nullptr;
           delete bCur;
           listLen--;
       }
       cur = nullptr;
       delete cur;
       head->next = nullptr;
       listLen--;
   }
   head->next = new LinkedList();
   head->next->image = image;
   head->next->prev = head;
   head = head->next;
   listLen++;
   if(listLen > 5) {
       LinkedList *bTail = tail;
       tail = tail->next;
       tail->prev = nullptr;
       delete bTail;
       //qDebug() << "deleted";
       listLen--;
   }
   oldImage_ = image;
}



void ScribbleArea::setStyle(int num) {
   switch(num) {
   case 0:
       pen.setStyle(Qt::SolidLine);
       break;
   case 1:
       pen.setStyle(Qt::DashLine);
       break;
   case 2:
       pen.setStyle(Qt::DashDotLine);
       break;
   case 3:
       pen.setStyle(Qt::DashDotDotLine);
       break;
   }
   //qDebug() << pen.style();
}

void ScribbleArea::drawStr(int num) {
   draw = num;
   if(draw == 4) {
       lastPoint.setX(0);
       lastPoint.setY(0);
   }
   if(draw == 5) {
       text = QInputDialog::getText(this, tr("Введите текст"), tr("Текст: "), QLineEdit::Normal);
   }
   oldImage_ = image;

}
int ScribbleArea::curDrawing() {
   return draw;
}

void ScribbleArea::reDoAction() {
  if (head->next != nullptr) {
      QImage oldImage = head->next->image;
      head = head->next;
      image = oldImage;
      oldImage_ = image;
      update();
  }
}

void ScribbleArea::unDoAction() {
    if (head->prev != nullptr) {
        QImage oldImage = head->prev->image;
        head = head->prev;
        image = oldImage;
        oldImage_ = image;
        lastPoint.setX(0);
        lastPoint.setY(0);
        update();
    }
}

bool ScribbleArea::openImageconst(const QString &fileName) {
    QImage newImage;
    if(!newImage.load(fileName)) {
        return false;
    }
    QRect screenGeometry = screen()->geometry();
    qDebug() << screenGeometry.height();
    //QLabel *label = new QLabel(this);
   //label->setPixmap(QPixmap::fromImage(newImage));
    newImage = newImage.scaled(image.width(), image.height());
   // QSize newSize = newImage.size().expandedTo(size());
    //resizeImage(&newImage, newSize);

    //newImage.scaled(screenGeometry.width(), screenGeometry.height(), Qt::KeepAspectRatio);
    image = newImage;
    modified = false;
    saveAct();
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat) {
    QImage ourImage = image;

    if (ourImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
void ScribbleArea::setPenColor(const QColor &newColor) {
    myPenColor = newColor;
    if(image.format()==QImage::Format_Grayscale8) {
        image = image.convertToFormat(QImage::Format_ARGB32);
        update();
    }

}
void ScribbleArea::setPenWidth(const int newWidth) {
    myPenWidth = newWidth;

}
void ScribbleArea::clearImage() {
    image.fill(qRgb(255,255,255));
    modified = true;
    update();
    oldImage_ = image;
    saveAct();
}
void ScribbleArea::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(!lastPoint.isNull() && curDrawing() == 4) {
        } else {
           lastPoint = event->pos();
        }
         drawing = true;

    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event) {
    if((event->buttons() & Qt::LeftButton) && drawing) {
         if(curDrawing() != 6) {
           drawLineTo(event->pos());
         }

    }

}
void ScribbleArea::mouseReleaseEvent(QMouseEvent *event) {


    if(event->button() == Qt::LeftButton && drawing) {

        drawLineTo(event->pos());
        if(curDrawing() == 4) {
           lastPoint = event->pos();
        }
        drawing = false;
    }
     saveAct();

}
void ScribbleArea::paintEvent(QPaintEvent *event) {
    QPainter paint(this);
   QRect rect = event->rect();
   paint.drawImage(rect, image, rect);

}
void ScribbleArea::resizeEvent(QResizeEvent *event) {
    if(width() > image.width() || height() > image.height()) {
        int nWidth = qMax(width() + 128, image.width());
        int nHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(nWidth, nHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &end) {
 if (curDrawing() != 0) {
       image = oldImage_;
        update();
    }
    int rad = (myPenWidth/2)+2;
    QRect boundingRect = QRect(lastPoint, end).normalized().adjusted(-rad, -rad, + rad, +rad);
    QPainter painter (&image);
    pen.setColor(myPenColor);
    pen.setWidth(myPenWidth);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    if (curDrawing() == 0 || curDrawing() == 1 || curDrawing() == 4) {
        painter.drawLine(lastPoint, end);
    } else if(curDrawing() == 2) {
        painter.drawRect(lastPoint.x(), lastPoint.y(), (end.x() - lastPoint.x()), (end.y()-lastPoint.y()));
    } else if(curDrawing() == 3) {
        painter.drawEllipse(lastPoint.x(), lastPoint.y(), (end.x() - lastPoint.x()), (end.y()-lastPoint.y()));
    } else if(curDrawing() == 5) {
        int thickness = painter.pen().width();
        QChar ch(QRandomGenerator::global()->bounded('A', 'Z' + 1));

        QFont biggerFont = painter.font();
        biggerFont.setBold(true);
        biggerFont.setPointSize(biggerFont.pointSize() + thickness);
        painter.setFont(biggerFont);
        if(text.isEmpty()) {
            painter.drawText(end, QString(ch));
        } else {
            painter.drawText(end, text);
        }


       QFontMetrics metrics(painter.font());
       boundingRect = metrics.boundingRect(ch);
       boundingRect.translate(end);
       boundingRect.adjust(-10, -10, +10, +10);
    } else if(curDrawing() == 6) {

       int x = end.x();
       int y = end.y();
       //qDebug()<<  "!!" << image.width();
       int curX = 0;
       int _curY = 0;
       if(myPenWidth + x > image.width()) {
            curX = image.width() - x + myPenWidth - 1;

       } else {
            curX = myPenWidth*2;
       }
       if(myPenWidth + y > image.height()) {
            _curY = image.height() - y + myPenWidth-1;
       } else {
            _curY = myPenWidth*2;
       }
       int iter = 0;
       while (x > 0 && iter < myPenWidth) {
            x--;
            iter++;
       }
       iter = 0;
       while(y > 0 && iter < myPenWidth) {
            y--;
            iter++;
       }
       int curY = y;
      //qDebug()<<  "!" << curX << _curY;
       for(int i = 0; i < curX - 1; i++) {
            for(int j = 0; j < _curY - 1; j++) {
                setBlur(x, y);
                y++;
                //qDebug() << iter << x << y;
            }
            y = curY;
            x++;
       }
       update();
    }

   // painter.drawRect(lastPoint.x(), end.y(),lastPoint.x(),lastPoint.x() );
    modified = true;
    if(curDrawing() == 5) {
       update(boundingRect);
    } else {
       update(QRect(lastPoint, end).normalized().adjusted(-rad, -rad, + rad, +rad));
    }
    //
    if (curDrawing() == 0 || curDrawing() == 5) {
         lastPoint = end;
    }

}
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize) {
    if(image->size() == newSize) {
        return;
    }
    QImage img(newSize, QImage::Format_RGB32);
    img.fill(qRgb(255,255,255));
    QPainter paint(&img);
    paint.drawImage(QPoint(0, 0), *image);
    *image = img;

}

void ScribbleArea::print() {
    //QPrinter print(QPrinter::HighResolution);
   // QPrintDialog printDialog(&print, this);
   // if(printDialog.exec() == QDialog::Accepted) {
   //     QPainter paint(&print);
   //     QRect rect = paint.viewport();
   //     QSize size = image.size();
    //    size.scale(rect.size(), Qt::KeepAspectRatio);
    //    paint.setViewport(rect.x(), rect.y(), size.width(), size.height());
    //    paint.setWindow(image.rect());
    //    paint.drawImage(0,0, image);
   // }

}


ScribbleArea::~ScribbleArea()
{
    //delete ui;
}
