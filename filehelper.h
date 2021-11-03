#pragma once

#include "fileinfo.h"

#include <QTextStream>
#include <QString>
#include <QStringConverter>

class FileHelper
{

public:
    QString readFromFile(const OpenFileInfo& fileData);
    bool writeToFile(const QString& text, const FileInfo& fileData);
    bool writeToFile(const QString& text);

    QString getEncodingText() const;
    const QString& getLineTerminatorText() const;
    const FileInfo& getFileInfo() const;

    bool filePathIsEmpty() const;
    void clear();

    QString getFileName() const;
private:
    FileInfo mFileInfo;
    QString mLineTerminatorText;

private:
    QByteArray readFromFile();

    QString getTextWithSelectedEncoding(const QByteArray& byteArray);
    QString getTextWithAutoDetectEncoding(const QByteArray& byteArray);
    QString getTextFromByteArray(const QByteArray& byteArray, bool autoDetect);

    void autoDetectFileFlag(const QByteArray& byteArray);

    void setLineTerminatorLabel(const QString& text = "");
    QString getLineTerminatorFromOS() const;
};
