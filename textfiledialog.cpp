#include "textfiledialog.h"
#include "constsdefine.h"

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSettings>

TextFileDialog::TextFileDialog(QWidget *parent)
    : QFileDialog(parent)
{
    setOption(DontUseNativeDialog);
    initializeEncodingComboBox();

    const QStringList filters({notepadConstants::textFileDialogDocumentsFilter, notepadConstants::allFilesDialogFilter});
    TextFileDialog::setNameFilters(filters);
}

void TextFileDialog::initializeEncodingComboBox()
{
    QGridLayout* mainLayout = qobject_cast<QGridLayout *>(layout());

    mainLayout->addWidget(mainLayout->itemAtPosition(3, 0)->widget(), 4, 0);
    mainLayout->addWidget(mainLayout->itemAtPosition(3, 1)->widget(), 4, 1);
    mainLayout->addWidget(mainLayout->itemAtPosition(3, 2)->widget(), 3, 2, 2, 1);

    QLabel* encodingLabel = new QLabel(notepadConstants::encodindLabel, this);
    mainLayout->addWidget(encodingLabel, 3, 0);

    encodingComboBox = new QComboBox(this);
    mainLayout->addWidget(encodingComboBox, 3, 1);

    initializeItemsToEncodingComboBox();
}

void TextFileDialog::initializeItemsToEncodingComboBox()
{
    encodingComboBox->addItem(notepadConstants::autoDetectEncoding, AutoDetect);
    encodingComboBox->addItem(notepadConstants::systemEncoding, System);
    encodingComboBox->addItem(notepadConstants::UTF16LEEncoding, UTF16LE);
    encodingComboBox->addItem(notepadConstants::UTF16BEEncoding, UTF16BE);
    encodingComboBox->addItem(notepadConstants::UTF8Encoding, UTF8);
    encodingComboBox->addItem(notepadConstants::UTF8BOMEncoding, UTF8WithBOM);
}

void TextFileDialog::setDialogType(const DialogType& dialogType)
{
    this->dialogType = dialogType;

    int autoDetectIndex = encodingComboBox->findText(notepadConstants::autoDetectEncoding);
    if(dialogType == DialogType::OPEN && autoDetectIndex == -1)
    {
        encodingComboBox->insertItem(0, notepadConstants::autoDetectEncoding, AutoDetect);
    }
    else if(dialogType == DialogType::SAVE && autoDetectIndex != -1)
    {
        encodingComboBox->removeItem(autoDetectIndex);
    }
}

void TextFileDialog::setCurrentEncoding(Encoding encoding)
{
    int index = encodingComboBox->findData(encoding);
    encodingComboBox->setCurrentIndex(index);
}

TextFileDialog::Encoding TextFileDialog::getEncoding() const
{
    Encoding encoding = static_cast<Encoding>(encodingComboBox->currentData().toInt());
    return encoding;
}

FileInfo TextFileDialog::getFileInfo() const
{
    Encoding encoding = getEncoding();
    FileInfo fileInfo{};
    fileInfo.filePath = selectedFiles()[0];
    switch(encoding)
    {
        case Encoding::AutoDetect:
            break;
        case Encoding::System:
            fileInfo.encoding = QStringConverter::Encoding::System;
            fileInfo.flag = QStringConverter::Flag::Default;
            break;
        case Encoding::UTF16LE:
            fileInfo.encoding = QStringConverter::Encoding::Utf16LE;
            fileInfo.flag = QStringConverter::Flag::WriteBom;
            break;
        case Encoding::UTF16BE:
            fileInfo.encoding = QStringConverter::Encoding::Utf16BE;
            fileInfo.flag = QStringConverter::Flag::WriteBom;
            break;
        case Encoding::UTF8:
            fileInfo.encoding = QStringConverter::Encoding::Utf8;
            fileInfo.flag = QStringConverter::Flag::Default;
            break;
        case Encoding::UTF8WithBOM:
            fileInfo.encoding = QStringConverter::Encoding::Utf8;
            fileInfo.flag = QStringConverter::Flag::WriteBom;
            break;
    }
    return fileInfo;
}

TextFileDialog::Encoding TextFileDialog::getEncodingFromFileInfo(const FileInfo& fileInfo)
{
    Encoding encoding = Encoding::System;
    if(fileInfo.encoding == QStringConverter::Encoding::Utf8)
    {
        encoding = Encoding::UTF8;
        if(fileInfo.flag == QStringConverter::Flag::WriteBom)
        {
            encoding = Encoding::UTF8WithBOM;
        }
    }
    else if(fileInfo.encoding == QStringConverter::Encoding::Utf16LE)
    {
        encoding = Encoding::UTF16LE;
    }
    else if(fileInfo.encoding == QStringConverter::Encoding::Utf16BE)
    {
        encoding = Encoding::UTF16BE;
    }
    return encoding;
}

void setWorkDirectory(QDir directory)
{
    QSettings settings(notepadConstants::organizationName, notepadConstants::programName);
    settings.setValue(notepadConstants::directoryPathKey, directory.absolutePath());
}

QDir getLastWorkDirectory()
{
    QSettings settings(notepadConstants::organizationName, notepadConstants::programName);
    if(settings.value(notepadConstants::directoryPathKey).isValid())
    {
        return QDir(settings.value(notepadConstants::directoryPathKey).toString());
    }
    return QDir::home();
}

FileInfo TextFileDialog::getSaveFile(QWidget *parent, const FileInfo& defaultFileInfo)
{
    TextFileDialog instance(parent);

    Encoding encoding = getEncodingFromFileInfo(defaultFileInfo);
    instance.setDirectory(getLastWorkDirectory());
    instance.setDialogType(DialogType::SAVE);
    instance.setCurrentEncoding(encoding);

    instance.setWindowTitle(notepadConstants::saveAsDialogTitle);
    instance.setAcceptMode(QFileDialog::AcceptSave);
    if(!defaultFileInfo.filePath.isEmpty())
    {
        instance.selectFile(defaultFileInfo.filePath);
    }


    if(instance.exec() == DialogCode::Rejected)
    {
        return FileInfo{};
    }

    setWorkDirectory(instance.directory());

    FileInfo fileInfo = instance.getFileInfo();
    instance.appendFileExtensionIfNotExist(fileInfo.filePath);

    return fileInfo;
}

OpenFileInfo TextFileDialog::getOpenFile(QWidget *parent)
{
    TextFileDialog instance(parent);

    instance.setDialogType(DialogType::OPEN);
    instance.setDirectory(getLastWorkDirectory());
    instance.setCurrentEncoding(AutoDetect);

    instance.setWindowTitle(notepadConstants::openFileDialogTitle);
    instance.setAcceptMode(QFileDialog::AcceptOpen);
    instance.setFileMode(FileMode::ExistingFile);

    if(instance.exec() == DialogCode::Rejected)
    {
        return OpenFileInfo{};
    }

    setWorkDirectory(instance.directory());

    FileInfo fileInfo = instance.getFileInfo();
    bool autoDetect = instance.getEncoding() == Encoding::AutoDetect;

    return OpenFileInfo{fileInfo, autoDetect};
}

void TextFileDialog::appendFileExtensionIfNotExist(QString& filePath)
{
    QString selectedfilter = selectedNameFilter();

    if(selectedfilter != notepadConstants::textFileDialogDocumentsFilter)
    {
        return;
    }

    QString targetFileExtention = notepadConstants::txtFileExtention;
    if(filePath.lastIndexOf(targetFileExtention) != (filePath.size() - targetFileExtention.size()))
    {
        filePath.append(notepadConstants::txtFileExtention);
    }
}



