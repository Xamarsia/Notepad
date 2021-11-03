#pragma once

#include <QString>
#include <QStringConverter>

struct FileInfo
{
    QString filePath{};
    QStringConverter::Encoding encoding{};
    QStringConverter::Flag flag{};
};

struct OpenFileInfo
{
    FileInfo fileInfo;
    bool autoDetect;
};


