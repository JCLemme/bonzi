#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadAnimations("../Bonzi.json");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_go_clicked()
{

}

void MainWindow::on_play_clicked()
{
    if(!isPlaying)
    {
        currentAnimation = animations[ui->animationBox->currentText()];
        currentFrame = 0;
        shouldPlay = true;
        isExiting = false;
        isPlaying = true;
        advanceAnimation();
    }
}

void MainWindow::on_stop_clicked()
{
    if(isPlaying)
        shouldPlay = false;
}
