#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QDir>
#include <QComboBox>
#include <QPainter>
#include <unistd.h>
#include <QApplication>
#include <QTimer>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
    void on_go_clicked();
    void on_play_clicked();
    void on_stop_clicked();

    private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
