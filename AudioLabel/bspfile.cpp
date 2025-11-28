#include "bspfile.h"

BspFile::BspFile(QObject *parent) : QObject(parent)
{

}

QString BspFile::openDirectoryDialog()
{
    // 打开文件夹选择对话框，返回选择的目录路径
       QString dir = QFileDialog::getExistingDirectory(nullptr, "选择目录", QDir::homePath());

       // 检查用户是否选择了目录
       if (dir.isEmpty()) {
           qWarning() << "没有选择目录";
       } else {
           qDebug() << "选择的目录是：" << dir;
       }
       return dir;
}
