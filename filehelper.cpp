#include "filehelper.h"
#include "constsdefine.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QOperatingSystemVersion>

bool UTF8hasBOM(const QByteArray& byteArray)
{
    if(!byteArray.isEmpty() && byteArray.size() > 2)
    {
        if(byteArray[0] == char(0xef) && byteArray[1] == char(0xbb) && byteArray[2] == char(0xbf))
        {
            return true;
        }
    }
    return false;
}

QByteArray FileHelper::readFromFile()
{
    QFile file(mFileInfo.filePath);
    QByteArray byteArray{};
    if(file.open(QFile::ReadOnly))
    {
        byteArray = file.readAll();
    }
    file.close();
    return byteArray;
}

QString FileHelper::getTextFromByteArray(const QByteArray& byteArray, bool autoDetect)
{
    QString text;
    if (autoDetect)
    {
        text = getTextWithAutoDetectEncoding(byteArray);
    }
    else
    {
        text = getTextWithSelectedEncoding(byteArray);
    }
    return text;
}

void FileHelper::autoDetectFileFlag(const QByteArray& byteArray)
{
    if(mFileInfo.encoding == QStringConverter::Encoding::Utf8 && !UTF8hasBOM(byteArray))
    {
        mFileInfo.flag = QStringConverter::Flag::Default;
    }
    else
    {
        mFileInfo.flag = QStringConverter::Flag::WriteBom;
    }
}

QString FileHelper::getTextWithAutoDetectEncoding(const QByteArray& byteArray)
{
    QTextStream textStream(byteArray);
    QString text = textStream.readAll();
    mFileInfo.encoding = textStream.encoding();
    autoDetectFileFlag(byteArray);

    return text;
}

QString FileHelper::getTextWithSelectedEncoding(const QByteArray& byteArray)
{
    QTextStream textStream(byteArray);
    textStream.setAutoDetectUnicode(false);

    textStream.setEncoding(mFileInfo.encoding);
    mFileInfo.encoding = textStream.encoding();

    return textStream.readAll();
}


void FileHelper::setLineTerminatorLabel(const QString& text)
{
    if(text.contains(QRegularExpression{R"-((\r\n))-"}))
    {
        mLineTerminatorText = notepadConstants::crlfLineEnding;
    }
    else if(text.contains(QRegularExpression{R"-((\n))-"}))
    {
        mLineTerminatorText = notepadConstants::lfLineEnding;
    }
    else if(text.contains(QRegularExpression{R"-((\r))-"}))
    {
        mLineTerminatorText = notepadConstants::crLineEnding;
    }
    else
    {
       mLineTerminatorText = getLineTerminatorFromOS();
    }
}

QString FileHelper::getEncodingText() const
{
    QString encodingText = QStringConverter::nameForEncoding(mFileInfo.encoding);

    if(mFileInfo.encoding == QStringConverter::Encoding::Utf8
            && mFileInfo.flag == QStringConverter::Flag::WriteBom)
    {
        encodingText = notepadConstants::UTF8BOMEncoding;
    }
    return encodingText;
}

const QString& FileHelper::getLineTerminatorText() const
{
    return mLineTerminatorText;
}

const FileInfo& FileHelper::getFileInfo() const
{
    return mFileInfo;
}

bool FileHelper::filePathIsEmpty() const
{
    return mFileInfo.filePath.isEmpty();
}

void FileHelper::clear()
{
    mFileInfo = FileInfo{};
    setLineTerminatorLabel();
}

QString FileHelper::getFileName() const
{
    QString fileName{};
    int lastIndex = mFileInfo.filePath.lastIndexOf(QChar('/'));
    if(lastIndex != -1)
    {
        int nameSize = (mFileInfo.filePath.size() - (lastIndex + 1));
        fileName = mFileInfo.filePath.last(nameSize);
    }
    return fileName;
}

QString FileHelper::getLineTerminatorFromOS() const
{
    QString lineTerminator{};
    auto currentOperatingSystem = QOperatingSystemVersion::current();
    if(currentOperatingSystem.type() == QOperatingSystemVersion::Windows)
    {
        lineTerminator = notepadConstants::crlfLineEnding;
    }
    else if(currentOperatingSystem.type() == QOperatingSystemVersion::MacOS)
    {
        lineTerminator = notepadConstants::crLineEnding;
    }
    else
    {
        lineTerminator = notepadConstants::lfLineEnding;
    }
    return lineTerminator;
}

QString FileHelper::readFromFile(const OpenFileInfo& fileData)
{
    mFileInfo = fileData.fileInfo;

    QByteArray byteArray = readFromFile();
    QString text = getTextFromByteArray(byteArray, fileData.autoDetect);
    setLineTerminatorLabel(text);
    return text;
}

bool FileHelper::writeToFile(const QString& text, const FileInfo& fileInfo)
{
    mFileInfo = fileInfo;
    return writeToFile(text);
}

bool FileHelper::writeToFile(const QString& text)
{
    if(filePathIsEmpty())
    {
        return false;
    }
    QFile file(mFileInfo.filePath);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        return false;
    }

    QTextStream textStream(&file);

    textStream.setEncoding(mFileInfo.encoding);

    if(mFileInfo.flag == QStringConverter::Flag::WriteBom)
    {
        textStream.setGenerateByteOrderMark(true);
    }
    textStream << text;

    file.flush();
    file.close();

    return true;
}

