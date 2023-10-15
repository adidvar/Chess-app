#ifndef AIWIDGET_H
#define AIWIDGET_H

#include "computer.hpp"
#include <QWidget>
#include <QListWidgetItem>
#include <bitboard.hpp>
#include <memory>
#include <computer.hpp>

namespace Ui {
class AIWidget;
}

class AIWidget : public QWidget
{
    Q_OBJECT
    BitBoard home;
    BitBoard current;
    Color color_;

    Evaluate a;
    Evaluate b;

   public:
    explicit AIWidget(QWidget *parent = nullptr);
    ~AIWidget();

private slots:
    void on_set_clicked();
    void setBoard(BitBoard board);

    void on_turns_list_itemClicked(QListWidgetItem *item);

    void on_white_radio_clicked();

    void on_black_radio_clicked();

    void on_a_sliderMoved(int position);

    void on_b_sliderMoved(int position);

private:
    Ui::AIWidget *ui;
    //alignas(16) char array[256];
    //alignas(16) char array1[256];
};

#endif // AIWIDGET_H
