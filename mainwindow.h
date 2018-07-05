#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unistd.h>
#include <stdlib.h>

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>

#include "common.h"
#include "handclassifier.h"
#include "videocap.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    HandClassifier handCls;
    VideoCap videoCap;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // img processor
    void readImg(string imgDir, bool flag);
    void updateImgLabel(bool flag);
    void updateWebcamLabel();

    // random number generator
    int randGen();

    // gesture classifier
    void capWebcamImgs();
    int getLabel(int*, int);
    void cleanVote();

    // webcam video
    void updateWebcamVideoLabel();

    // score
    void updateScore(int, int);
    void updateScoreboard();

private slots:
    void on_startButt_clicked();

    void countdownTimeHandler();
    void videoTimeHandler();
    void gestTimeHandler();

private:
    Ui::MainWindow *ui;

    // counter
    int counter;
    QTimer *countdownTimer;
    QTimer *videoTimer;
    QTimer *gestTimer;

    // label img
    vector<string> imgDirs;
    Mat computerImg;
    Mat playerImg;

    // classifier
    int gestVote[4] = {0, 0, 0, 0};
    Mat webcamVideoFrame;
    vector<Mat> webcamImgs;
    vector<Mat> webcamLabelImgs;

    // score
    int gest_machine;
    int gest_man;
    int score_machine;
    int score_man;


    // shortcut
    QShortcut *key_begin;
};

#endif // MAINWINDOW_H
