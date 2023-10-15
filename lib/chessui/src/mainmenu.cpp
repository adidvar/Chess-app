#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "singleplayer.h"
#include "aiwidget.h"

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_singleplayer_clicked()
{
    auto window = new SinglePlayer();
    window->show();
}


void MainMenu::on_analysis_clicked()
{
    auto window = new AIWidget();
    window->show();
}
