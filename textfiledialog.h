#pragma once
#include <QFileDialog>
#include <QStringConverter>
#include "fileinfo.h"

class QComboBox;

class TextFileDialog : public QFileDialog
{
    Q_OBJECT

public:
    static OpenFileInfo getOpenFile(QWidget *parent);

    static FileInfo getSaveFile(QWidget *parent, const FileInfo &defaultFileInfo);

private:
    enum Encoding {AutoDetect, System, UTF16LE, UTF16BE, UTF8, UTF8WithBOM};
    enum class DialogType {OPEN, SAVE};

    QComboBox* encodingComboBox;
    DialogType dialogType;

    TextFileDialog(QWidget *parent = nullptr);

    TextFileDialog(const TextFileDialog&) = delete;
    TextFileDialog& operator=(const TextFileDialog&)= delete;

    void setDialogType(const DialogType& dialogType);
    void setCurrentEncoding(Encoding encoding);
    void appendFileExtensionIfNotExist(QString& filePath);

    void initializeEncodingComboBox();
    void initializeItemsToEncodingComboBox();

    Encoding getEncoding() const;
    FileInfo getFileInfo() const;
    static Encoding getEncodingFromFileInfo(const FileInfo& fileInfo);
};

