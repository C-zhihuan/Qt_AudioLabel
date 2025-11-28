#ifndef BSPFILE_H
#define BSPFILE_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

class BspFile : public QObject
{
    Q_OBJECT
public:
    explicit BspFile(QObject *parent = nullptr);

    QString openDirectoryDialog();

signals:

};

#endif // BSPFILE_H
