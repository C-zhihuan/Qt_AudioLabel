#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include <QTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     ui(new Ui::MainWindow),
     player(new QMediaPlayer(this)),
     model(new QFileSystemModel(this))

{
    ui->setupUi(this);


    // 播放器连接
   connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::onPositionChanged);
   connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::onDurationChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete model;
}

//*******************************************************播放设置*****************************************

void MainWindow::on_Btn_play_clicked()
{
    if (currentFile.isEmpty()) {
            QMessageBox::warning(this, "错误", "没有选择音频文件！");
            return;
        }

        player->setMedia(QUrl::fromLocalFile(currentFile));
        player->setVolume(ui->volumeSlider->value());
        player->play();
}

void MainWindow::on_Btn_Stop_clicked()
{
    player->stop();
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    player->setVolume(value);
}

void MainWindow::on_Btn_Pause_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState) {
            // 如果当前正在播放，则暂停
            player->pause();
            ui->Btn_Pause->setText("继续播放");  // 可以更改按钮文本
        } else if (player->state() == QMediaPlayer::PausedState) {
            // 如果当前是暂停状态，则继续播放
            player->play();
            ui->Btn_Pause->setText("暂停");  // 可以更改按钮文本
        }
}

void MainWindow::onPositionChanged(qint64 position)
{
    qint64 duration = player->duration();

        // 防止除零错误
        if (duration <= 0) {
            return;  // 如果没有有效的音频时长，则退出
        }

        // 如果当前播放器的位置超过了音频时长，设置为最大值
        if (position > duration) {
            position = duration;
        }

        // 更新进度条
        if (!userInteractingWithSlider) {
            // 将音频的position映射到进度条范围（0到1000）
            int sliderValue = static_cast<int>((position * 1000) / duration);  // 映射到0到1000的范围
            ui->progressSlider->setValue(sliderValue);
        }

        // 更新进度显示标签
        updateProgressLabel(position, duration);
}



void MainWindow::on_progressSlider_valueChanged(int value)
{
    if (userInteractingWithSlider && player->duration() > 0) {
            qint64 duration = player->duration();

            // 防止无效的duration
            if (duration <= 0) {
                return;
            }

            // 将进度条的值映射回音频的时长
            qint64 newPosition = (value * duration) / 1000;

            // 确保新位置在有效范围内
            if (newPosition < 0) {
                newPosition = 0;
            } else if (newPosition > duration) {
                newPosition = duration;
            }

            player->setPosition(newPosition);
        }
}

void MainWindow::onDurationChanged(qint64 duration)
{
    // 防止无效的duration
        if (duration <= 0) {
            return;
        }

        // 使用音频的时长来设置进度条的最大值
        ui->progressSlider->setRange(0, 1000);

        // 更新进度显示标签
        updateProgressLabel(player->position(), duration);
}


void MainWindow::updateProgressLabel(qint64 currentPosition, qint64 duration)
{
    // 将毫秒转换为时:分:秒格式
        QTime currentTime(0, 0);
        currentTime = currentTime.addMSecs(currentPosition);
        QTime totalTime(0, 0);
        totalTime = totalTime.addMSecs(duration);
        // 更新进度显示标签
        ui->label_time->setText(currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss"));
}



void MainWindow::on_progressSlider_sliderPressed()
{
    userInteractingWithSlider = true; // 用户开始滑动进度条
}

void MainWindow::on_progressSlider_sliderReleased()
{
    userInteractingWithSlider = false; // 用户停止滑动进度条
}




//***************************************文件选择与修改*************************************************

void MainWindow::on_comboBox_Sort_currentIndexChanged(int index)
{
    onSortChanged(index);
}

void MainWindow::onSortChanged(int index)
{
    QString currentDir = ui->Edit_FilePath->text();  // 获取当前目录路径

        switch (index) {
            case 0:  // 按文件名排序

                model->sort(0, Qt::AscendingOrder);   // 按文件名升序排序
                break;
            case 1:  // 按修改时间排序

                model->sort(0, Qt::DescendingOrder);  // 按修改时间降序排序
                break;
            case 2:  // 按文件类型排序

                model->sort(0, Qt::AscendingOrder);   // 按文件路径升序排序
                break;
            default:
                break;
        }

        // 重新设置排序后的路径到树视图
        ui->treeView->setRootIndex(model->index(currentDir));
}

void MainWindow::on_Btn_Openfile_clicked()
{
    FilePath=MyFile->openDirectoryDialog();
    if (FilePath.isEmpty())return;

    ui->Edit_FilePath->setText(FilePath);
    model->setRootPath(FilePath);
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(FilePath));
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString filePath = model->filePath(index);
        if (filePath.endsWith(".mp3") || filePath.endsWith(".wav")) {
            currentFile = filePath;
            QString oldBaseName = QFileInfo(currentFile).baseName();  // 获取文件的基本名字（不含扩展名）
            ui->lineEdit_label_Rename->setText(oldBaseName);
            on_Btn_play_clicked();
        } else {
            QMessageBox::warning(this, "错误", "请选择音频文件！");
        }
}



void MainWindow::on_Btn_Rename_clicked()
{
    if (currentFile.isEmpty()) {
            QMessageBox::warning(this, "错误", "没有选择音频文件！");
            return;
        }

    QString basePath = QFileInfo(currentFile).absolutePath(); // 获取文件所在目录
    QString oldName = QFileInfo(currentFile).fileName();
    QString oldBaseName = QFileInfo(currentFile).baseName();  // 获取文件的基本名字（不含扩展名）
    QString extension = QFileInfo(currentFile).suffix();     // 获取文件的扩展名


    QString newBaseName = ui->lineEdit_label_Rename->text();

    if (!newBaseName.isEmpty() && newBaseName != oldBaseName) {
            QString newFileName = newBaseName + "." + extension; // 保持原扩展名
            QString newFilePath = basePath + "/" + newFileName;

            // 停止播放文件，避免占用
            player->stop();

            if (QFile::rename(currentFile, newFilePath)) {
                QMessageBox::information(this, "成功", "文件已重命名为: " + newFileName);
                currentFile = newFilePath;

                // 刷新列表视图
                model->setRootPath(basePath);
                ui->treeView->setRootIndex(model->index(basePath));
            } else {
                QMessageBox::critical(this, "失败", "文件重命名失败，请检查权限或文件是否被占用！");
            }
        }
}

void MainWindow::AddNameFile(QString addname)
{
    QString oldName=ui->lineEdit_label_Rename->text();
    QString Rename=addname+oldName;
    ui->lineEdit_label_Rename->setText(Rename);
}


//*****************************************************标签按键**********************************************
void MainWindow::on_Btn_label_1_clicked()
{
    QString newName=ui->lineEdit_label_1->text();
    AddNameFile(newName);
}

void MainWindow::on_Btn_label_2_clicked()
{
    QString newName=ui->lineEdit_label_2->text();
    AddNameFile(newName);
}

void MainWindow::on_Btn_label_3_clicked()
{
    QString newName=ui->lineEdit_label_3->text();
    AddNameFile(newName);
}

void MainWindow::on_Btn_label_4_clicked()
{
    QString newName=ui->lineEdit_label_4->text();
    AddNameFile(newName);
}

void MainWindow::on_Btn_label_5_clicked()
{
    QString newName=ui->lineEdit_label_5->text();
    AddNameFile(newName);
}

void MainWindow::on_Btn_label_6_clicked()
{
    QString newName=ui->lineEdit_label_6->text();
    AddNameFile(newName);
}

void MainWindow::on_Btn_label_7_clicked()
{
    QString newName=ui->lineEdit_label_7->text();
    AddNameFile(newName);
}

void MainWindow::on_Btn_label_8_clicked()
{
    QString newName=ui->lineEdit_label_8->text();
    AddNameFile(newName);
}










