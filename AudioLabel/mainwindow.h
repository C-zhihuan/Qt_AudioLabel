#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bspfile.h>
#include <QFileSystemModel>
#include <QMediaPlayer>
#include <QMediaContent>
#include <QSortFilterProxyModel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Btn_Openfile_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_Btn_play_clicked();

    void on_Btn_Rename_clicked();

    void on_Btn_Stop_clicked();

    void on_Btn_label_1_clicked();

    void on_Btn_label_2_clicked();

    void on_Btn_label_3_clicked();

    void on_Btn_label_4_clicked();

    void on_Btn_label_5_clicked();

    void on_Btn_label_6_clicked();

    void on_Btn_label_7_clicked();

    void on_Btn_label_8_clicked();

    void on_volumeSlider_valueChanged(int value);

    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);

    void on_progressSlider_sliderPressed();

    void on_progressSlider_sliderReleased();

    void on_progressSlider_valueChanged(int value);

    void on_Btn_Pause_clicked();

    void on_comboBox_Sort_currentIndexChanged(int index);

private:
    void AddNameFile(QString addname);

    bool userInteractingWithSlider = false;  // 标记用户是否正在拖动进度条
    void updateProgressLabel(qint64 currentPosition, qint64 duration);

    void onSortChanged(int index);

private:
    Ui::MainWindow *ui;
    BspFile *MyFile;
    QMediaPlayer *player;

    QString FilePath;
    QFileSystemModel *model;
    QSortFilterProxyModel *proxyModel;

    QString currentFile;


};
#endif // MAINWINDOW_H
