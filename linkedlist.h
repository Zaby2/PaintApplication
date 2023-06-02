#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <QWidget>
#include <QImage>

namespace Ui {
class LinkedList;
}

class LinkedList : public QWidget
{
    Q_OBJECT

public:
    explicit LinkedList(QWidget *parent = nullptr);
    ~LinkedList();

private:
    Ui::LinkedList *ui;
public:
    LinkedList *next = nullptr;
    LinkedList *prev = nullptr;
    QImage image;

};

#endif // LINKEDLIST_H
