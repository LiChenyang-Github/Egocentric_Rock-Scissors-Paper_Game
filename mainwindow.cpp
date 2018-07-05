#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    counter = 0;
    score_machine = 0;
    score_man = 0;
    countdownTimer = new QTimer(this);
    videoTimer = new QTimer(this);
    gestTimer = new QTimer(this);
    key_begin = new QShortcut(QKeySequence(Qt::Key_B), this);
    connect(key_begin, SIGNAL(activated()), this, SLOT(on_startButt_clicked()));
    connect(countdownTimer, SIGNAL(timeout()), this, SLOT(countdownTimeHandler()));
    connect(videoTimer, SIGNAL(timeout()), this, SLOT(videoTimeHandler()));
    connect(gestTimer, SIGNAL(timeout()), this, SLOT(gestTimeHandler()));

    videoTimer->start(30);

    imgDirs.push_back("/home/ubuntu/lichenyang/caffe-for-cudnn-v2.5.48-master/ui_mine_tk1/gesture_classifier/img/rock.png");
    imgDirs.push_back("/home/ubuntu/lichenyang/caffe-for-cudnn-v2.5.48-master/ui_mine_tk1/gesture_classifier/img/scissors.png");
    imgDirs.push_back("/home/ubuntu/lichenyang/caffe-for-cudnn-v2.5.48-master/ui_mine_tk1/gesture_classifier/img/paper.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readImg(string imgDir, bool flag)
{
    if(flag)    // read img for the computer label
    {
        computerImg = imread(imgDir);
        cv::resize(computerImg, computerImg, cv::Size(ui->imgLabel_1->height(), ui->imgLabel_1->width()));
    }
    else    // read img for the player label
    {
        playerImg = imread(imgDir);
        cv::resize(playerImg, playerImg, cv::Size(ui->imgLabel_2->height(), ui->imgLabel_2->width()));
    }
}

void MainWindow::updateImgLabel(bool flag)
{
    Mat rgb;
    QImage img;
    if (flag)    // update img for the computer label
    {
        if (computerImg.channels() == 3)
        {
            cvtColor(computerImg, rgb, CV_BGR2RGB);
            img = QImage((const unsigned char*)(rgb.data),
                         rgb.cols, rgb.rows,
                         rgb.cols*rgb.channels(),
                         QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(computerImg.data),
                         computerImg.cols, computerImg.rows,
                         computerImg.cols*computerImg.channels(),
                         QImage::Format_RGB888);
        }
        ui->imgLabel_1->setPixmap(QPixmap::fromImage(img));
    }
    else    // update img for the player label
    {
        if (playerImg.channels() == 3)
        {
            cvtColor(playerImg, rgb, CV_BGR2RGB);
            img = QImage((const unsigned char*)(rgb.data),
                         rgb.cols, rgb.rows,
                         rgb.cols*rgb.channels(),
                         QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(playerImg.data),
                         playerImg.cols, playerImg.rows,
                         playerImg.cols*playerImg.channels(),
                         QImage::Format_RGB888);
        }
        ui->imgLabel_2->setPixmap(QPixmap::fromImage(img));
    }
}

void MainWindow::updateWebcamLabel()
{
    vector<Mat> rgbs(webcamLabelImgs.size());
    vector<QImage> imgs;
    for (int i =0; i < webcamLabelImgs.size(); i++)
    {
//        imshow("test", webcamImgs[i]);
//        waitKey();
        if (webcamLabelImgs[i].channels() == 3)
        {
            cvtColor(webcamLabelImgs[i], rgbs[i], CV_BGR2RGB);
            imgs.push_back(QImage((const unsigned char*)(rgbs[i].data),
                         rgbs[i].cols, rgbs[i].rows,
                         rgbs[i].cols*rgbs[i].channels(),
                         QImage::Format_RGB888));
        }
        else
        {
            imgs.push_back(QImage((const unsigned char*)(webcamLabelImgs[i].data),
                         webcamLabelImgs[i].cols, webcamLabelImgs[i].rows,
                         webcamLabelImgs[i].cols*webcamLabelImgs[i].channels(),
                         QImage::Format_RGB888));
        }

    }
    ui->webcamLabel_1->setPixmap(QPixmap::fromImage(imgs[0]));
    ui->webcamLabel_2->setPixmap(QPixmap::fromImage(imgs[1]));
    ui->webcamLabel_3->setPixmap(QPixmap::fromImage(imgs[2]));
}

void MainWindow::updateWebcamVideoLabel()
{
    Mat rgb;
    QImage img;
    if (webcamVideoFrame.channels() == 3)
    {
        cvtColor(webcamVideoFrame, rgb, CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols, rgb.rows,
                     rgb.cols*rgb.channels(),
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(webcamVideoFrame.data),
                     webcamVideoFrame.cols, webcamVideoFrame.rows,
                     webcamVideoFrame.cols*webcamVideoFrame.channels(),
                     QImage::Format_RGB888);
    }
    ui->webcamLabel_0->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::updateScore(int machine, int man)
{
    if (machine == man)
    {
        QMessageBox::information(NULL, tr("Message"), tr("Draw!"));
        return;
    }
    if (machine == 0)   // rock
    {
        if (man == 1)
        {
            score_machine++;
            QMessageBox::information(NULL, tr("Message"), tr("Lost!"));
            return;
        }
        else
        {
            score_man++;
            QMessageBox::information(NULL, tr("Message"), tr("Win!"));
            return;
        }
    }
    if (machine == 1)   // scissors
    {
        if (man == 0)
        {
            score_man++;
            QMessageBox::information(NULL, tr("Message"), tr("Win!"));
            return;
        }
        else
        {
            score_machine++;
            QMessageBox::information(NULL, tr("Message"), tr("Lost!"));
            return;
        }
    }
    if (machine == 2)   // paper
    {
        if (man == 0)
        {
            score_machine++;
            QMessageBox::information(NULL, tr("Message"), tr("Lost!"));
            return;
        }
        else
        {
            score_man++;
            QMessageBox::information(NULL, tr("Message"), tr("Win!"));
            return;
        }
    }

}

void MainWindow::updateScoreboard()
{
    ui->scoreLabel_machine->setText(QString::number(score_machine));
    ui->scoreLabel_man->setText(QString::number(score_man));
}

int MainWindow::randGen()
{
    return rand() % 3;
}

void MainWindow::capWebcamImgs()
{
    Mat img;
    webcamImgs.clear();
    webcamLabelImgs.clear();
    for (int i = 0; i < 3; i++)
    {
        img = videoCap.getNextFrame();
        Mat img_resize;
        cv::resize(img, img_resize, cv::Size(ui->webcamLabel_1->height(), ui->webcamLabel_1->width()));
        webcamImgs.push_back(img);
        webcamLabelImgs.push_back(img_resize);
    }

}

int MainWindow::getLabel(int *vote, int gestNum)
{
    int vote_max = 0;
    int idx = 0;
    for(int k = 0; k < gestNum; k++)
    {
        if (vote[k]>vote_max)
        {
            vote_max = vote[k];
            idx = k;
        }
    }
    if (idx == gestNum-1)
    {
        QMessageBox::information(NULL, tr("Message"), tr("Please use the correct gesture!"));
    }
    return idx;
}

void MainWindow::cleanVote()
{
    int len = sizeof(gestVote) / sizeof(gestVote[0]);
    for (int i = 0; i < len; i++)
    {
        gestVote[i] = 0;
    }
}

void MainWindow::on_startButt_clicked()
{
    countdownTimer->start(1000);
    gestTimer->start(30);
}

void MainWindow::countdownTimeHandler()
{

    ui->countDownLabel->setText(QString::number(3-counter));
    if(counter==3)
    {
        counter = 0;
        ui->countDownLabel->setText(QString("Action!"));

        gest_machine = randGen();
        readImg(imgDirs[gest_machine], true);
        capWebcamImgs();

        for (int i = 0; i < webcamImgs.size(); i++)
        {
//            qDebug() << "hahahhaha:" << i <<endl;
            gestVote[handCls.handClassify(webcamImgs[i])]++;
        }
//        cout << gestVote[0] << gestVote[1] << gestVote[2] << gestVote[3] << endl;

        countdownTimer->stop();
        gestTimer->stop();

        updateWebcamLabel();
        gest_man = getLabel(gestVote, 4);
        if (gest_man == 3)
        {
            return;
        }

        readImg(imgDirs[gest_man], false);
        updateImgLabel(true);
        updateImgLabel(false);
        updateScore(gest_machine, gest_man);
        updateScoreboard();
        cleanVote();
//        cout << gestVote[0] << gestVote[1] << gestVote[2] << gestVote[3] << endl;
        return;
    }
    counter++;

}

void MainWindow::videoTimeHandler()
{
//    qDebug() << "hahahahhahahahahahahahah" << endl;
    webcamVideoFrame = videoCap.getNextFrame();
//    qDebug() << "hahahahhahahahahahahahah" << endl;
    cv::resize(webcamVideoFrame, webcamVideoFrame, cv::Size(ui->webcamLabel_0->height(), ui->webcamLabel_0->width()));
//    qDebug() << "hahahahhahahahahahahahah" << endl;
    updateWebcamVideoLabel();
}

void MainWindow::gestTimeHandler()
{
    readImg(imgDirs[randGen()], true);
    updateImgLabel(true);
}
