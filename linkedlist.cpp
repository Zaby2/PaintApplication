#include "linkedlist.h"
#include "ui_linkedlist.h"

LinkedList::LinkedList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkedList)
{
    ui->setupUi(this);
}

LinkedList::~LinkedList()
{
    delete ui;
}
