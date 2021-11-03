#include "ui_notepadwindow.h"

#include "filehelper.h"
#include "constsdefine.h"
#include "notepadwindow.h"
#include "notepadmanager.h"
#include "aboutnotepaddialog.h"
#include "findorreplacedialog.h"

#include <QLabel>
#include <QMessageBox>
#include <QPrinter>
#include <QString>
#include <QPrintDialog>
#include <QTextCursor>
#include <QAction>
#include <QFontDialog>
#include <QPageSetupDialog>

NotepadWindow::NotepadWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NotepadWindow)
{
    ui->setupUi(this);

    mFileHelper = new FileHelper;

    mScaleLabel = new QLabel(ui->statusbar);
    mBlankLabel  = new QLabel(ui->statusbar);
    mEncodingLabel = new QLabel(ui->statusbar);
    mCursorPositionLabel = new QLabel(ui->statusbar);
    mLineTerminatorLabel = new QLabel(ui->statusbar);

    mPageSetupDialog = new QPageSetupDialog(this);
    mAboutNotepadDialog = new AboutNotepadDialog(this);
    mPrintDialog = new QPrintDialog(mPageSetupDialog->printer(), this);
    mFindDialog = new FindOrReplaceDialog(FindOrReplaceDialog::DialogueType::FIND, this);
    mReplaceDialog = new FindOrReplaceDialog(FindOrReplaceDialog::DialogueType::REPLACE, this);

    initConnects();
    setInitialState();
    onTextEditCursorPositionChanged();
    setWindowTitle(notepadConstants::untitled + " - " + notepadConstants::programName);
}

void NotepadWindow::setInitialState()
{
    ui->actionUndo->setEnabled(false);
    ui->statusbar->setVisible(true);
    onTextEditSelectedText(false);
    ui->actionStatusBar->setChecked(true);

    statusBar()->insertPermanentWidget(0, mBlankLabel, 4);
    statusBar()->insertPermanentWidget(1, mCursorPositionLabel, 2);
    statusBar()->insertPermanentWidget(2, mScaleLabel, 1);
    statusBar()->insertPermanentWidget(3, mLineTerminatorLabel, 1);
    statusBar()->insertPermanentWidget(4, mEncodingLabel, 2);

    mEncodingLabel->setText(QStringConverter::nameForEncoding(QStringConverter::Encoding::Utf8));
    mScaleLabel->setText(QString(" %1 %").arg(mZoomCounter));

    mFileHelper->clear();
    mLineTerminatorLabel->setText(mFileHelper->getLineTerminatorText());
}

void NotepadWindow::initConnects()
{
    connect(mFindDialog, &FindOrReplaceDialog::findNextButtonClicked, this, &NotepadWindow::selectText);
    connect(mReplaceDialog, &FindOrReplaceDialog::findNextButtonClicked, this, &NotepadWindow::selectText);
    connect(mReplaceDialog, &FindOrReplaceDialog::replaceButtonClicked, this, &NotepadWindow::replaceTriggered);
    connect(mReplaceDialog, &FindOrReplaceDialog::replaceAllButtonClicked, this, &NotepadWindow::onActionReplaceAllTriggered);

    connect(ui->actionReplace, &QAction::triggered, this, &NotepadWindow::onActionReplaceTriggered);
    connect(ui->actionFind, &QAction::triggered, this, &NotepadWindow::onActionFindTriggered);

    connect(ui->actionCopy, SIGNAL(triggered()), ui->textEdit, SLOT(copy()));
    connect(ui->actionSelectAll, SIGNAL(triggered()), ui->textEdit, SLOT(selectAll()));
    connect(ui->actionCut, SIGNAL(triggered()), ui->textEdit, SLOT(cut()));
    connect(ui->actionPaste, SIGNAL(triggered()), ui->textEdit, SLOT(paste()));
    connect(ui->actionUndo, SIGNAL(triggered()), ui->textEdit, SLOT(undo()));
    connect(ui->actionNewWindow, SIGNAL(triggered()), this, SLOT(onActionNewWindowTriggered()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onActionOpenTriggered()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onActionExitTriggered()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(onActionDeleteTriggered()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), this, SLOT(onActionZoomInTriggered()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(onActionZoomOutTriggered()));
    connect(ui->actionTimeDate, SIGNAL(triggered()), this, SLOT(onActionTimeDateTriggered()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(onActionNewTriggered()));
    connect(ui->actionPageSetup, SIGNAL(triggered()), this, SLOT(onActionPageSetupTriggered()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(onActionPrintTriggered()));
    connect(ui->actionFont, SIGNAL(triggered()), this, SLOT(onActionFontTriggered()));
    connect(ui->actionStatusBar, SIGNAL(triggered(bool)), this, SLOT(onActionStatusBarTriggered(bool)));
    connect(ui->actionRestoreDefaultZoom, SIGNAL(triggered()), this, SLOT(onActionRestoreDefaultZoomTriggered()));
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onTextEditCursorPositionChanged()));
    connect(ui->actionWordWrap, SIGNAL(triggered(bool)), this, SLOT(onActionWordWrapTriggered(bool)));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), this, SLOT(onTextEditSelectedText(bool)));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(onTextEditTextChanged()));
    connect(ui->actionFindNext, SIGNAL(triggered()), this, SLOT(onActionFindNextTriggered()));
    connect(ui->actionFindPrevious, SIGNAL(triggered()), this, SLOT(onActionFindPreviousTriggered()));
    connect(ui->actionAboutNotepud, SIGNAL(triggered()), this, SLOT(onActionAboutNotepudTriggered()));
    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailableTriggered(bool)));
    connect(ui->actionSaveAs,  SIGNAL(triggered()), this, SLOT(onActionSaveAsTriggered()));
    connect(ui->actionSave,  SIGNAL(triggered()), this, SLOT(onActionSaveTriggered()));
}

NotepadWindow::~NotepadWindow()
{
    delete ui;
}

void NotepadWindow::onActionExitTriggered()
{
    if(requestClose())
    {
        QApplication::quit();
    }
}

void NotepadWindow::onActionDeleteTriggered()
{
    ui->textEdit->textCursor().removeSelectedText();
}

void NotepadWindow::onActionZoomInTriggered()
{
    if(mZoomCounter != notepadConstants::maxZoom)
    {
        ui->textEdit->zoomIn();
        mZoomCounter += notepadConstants::stepSizeForZoom;
        mScaleLabel->setText(QString(" %1 %").arg(mZoomCounter));
    }
}

void NotepadWindow::onActionZoomOutTriggered()
{
    if(mZoomCounter != notepadConstants::minZoom)
    {
        ui->textEdit->zoomOut();
        mZoomCounter -= notepadConstants::minZoom;
        mScaleLabel->setText(QString(" %1 %").arg(mZoomCounter));
    }
}

void NotepadWindow::onUndoAvailableTriggered(bool available)
{
     ui->actionUndo->setEnabled(available);
}

void NotepadWindow::onTextEditTextChanged()
{
    setTextChanged(true);
}

void NotepadWindow::setTextChanged(bool fileChanged)
{
    QString title = windowTitle();
    if(fileChanged && title[0] != '*')
    {
       setWindowTitle('*' + title);
    }
    else if(!fileChanged && title[0] == '*')
    {
        setWindowTitle(title.remove(0, 1));
    }
    mFileChanged = fileChanged;
}

void NotepadWindow::onActionWordWrapTriggered(bool checked)
{
    if(checked)
    {
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    }
    else
    {
        ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
    }
}

void NotepadWindow::onActionTimeDateTriggered()
{
    QString dateTime = QDateTime::currentDateTime().toString("hh:mm AP MM/dd/yyyy");
    ui->textEdit->insertPlainText(dateTime);
}

void NotepadWindow::onActionSaveTriggered()
{
    if(mFileHelper->filePathIsEmpty())
    {
        saveInNewFile(mFileHelper->getFileInfo());
    }
    else
    {
        saveInCurrentFile();
    }
}

void NotepadWindow::saveInCurrentFile(const FileInfo& fileInfo)
{
    mFileHelper->writeToFile(ui->textEdit->toPlainText(), fileInfo);
    setInfoFromFileHelper();
    setTextChanged(false);
}

void NotepadWindow::saveInCurrentFile()
{
    mFileHelper->writeToFile(ui->textEdit->toPlainText());
    setInfoFromFileHelper();
    setTextChanged(false);
}

void NotepadWindow::saveInNewFile(const FileInfo& fileInfo)
{
    FileInfo newFileInfo = TextFileDialog::getSaveFile(this, fileInfo);

    if(!newFileInfo.filePath.isEmpty())
    {
        saveInCurrentFile(newFileInfo);
    }
}

void NotepadWindow::onActionOpenTriggered()
{
    if(!requestClose())
    {
        return;
    }

    OpenFileInfo mFileData  = TextFileDialog::getOpenFile(this);
    if(mFileData.fileInfo.filePath.isEmpty())
    {
        return;
    }

    QString text = mFileHelper->readFromFile(mFileData);
    ui->textEdit->setPlainText(text);

    setInfoFromFileHelper();

    setTextChanged(false);
}


void NotepadWindow::setInfoFromFileHelper()
{
    mEncodingLabel->setText(mFileHelper->getEncodingText());
    mLineTerminatorLabel->setText(mFileHelper->getLineTerminatorText());
    setWindowTitle(mFileHelper->getFileName() + " - " + notepadConstants::programName);
}

void NotepadWindow::onActionSaveAsTriggered()
{
    saveInNewFile(mFileHelper->getFileInfo());
}

void NotepadWindow::onActionNewTriggered()
{
    if(requestClose())
    {
        setWindowTitle(notepadConstants::untitled + " - " + notepadConstants::programName);
        mFileHelper->clear();
        ui->textEdit->clear();
        setTextChanged(false);
    }
}

void NotepadWindow::onActionRestoreDefaultZoomTriggered()
{
    while(mZoomCounter != notepadConstants::defaultZoom)
    {
        if(mZoomCounter > notepadConstants::defaultZoom)
        {
            onActionZoomOutTriggered();
        }
        else
        {
            onActionZoomInTriggered();
        }
    }
}

void NotepadWindow::onActionStatusBarTriggered(bool checked)
{
    if(checked)
    {
        mScaleLabel->setText(QString(" %1 %").arg(mZoomCounter));
        ui->statusbar->setVisible(true);
    }
    else
    {
        ui->statusbar->setVisible(false);
    }
}

void NotepadWindow::onTextEditCursorPositionChanged()
{
   QTextCursor textCursor = ui->textEdit->textCursor();
   int blockNumber = textCursor.blockNumber() + 1;
   int columnNumber = textCursor.columnNumber() + 1;
   QString cursor = QString("Ln %1, Col %2").arg(blockNumber).arg(columnNumber);
   mCursorPositionLabel->setText(cursor);
}

QMessageBox::StandardButton documentHasBeenModifiedWarningDialog(QWidget* parent)
{
    QMessageBox::StandardButton buttonClicked = QMessageBox::warning(parent, notepadConstants::programName,
               notepadConstants::documentModifiedMessage,
               QMessageBox::Save | QMessageBox::Discard
               | QMessageBox::Cancel,
               QMessageBox::Save);
    return buttonClicked;
}

bool NotepadWindow::requestClose()
{
    if(!mFileChanged)
    {
        return true;
    }

    QMessageBox::StandardButton buttonClicked = documentHasBeenModifiedWarningDialog(this);
    if (buttonClicked == QMessageBox::Save)
    {
        onActionSaveTriggered();
        setTextChanged(false);
        return true;
    }
    else if(buttonClicked == QMessageBox::Discard)
    {
        setTextChanged(false);
        return true;
    }
    return false;
}

void NotepadWindow::closeEvent(QCloseEvent *event)
{
    if(!requestClose())
    {
        event->ignore();
        return;
    }
    event->accept();
}

void NotepadWindow::onActionPageSetupTriggered()
{
    mPageSetupDialog->open();
}

void NotepadWindow::onActionPrintTriggered()
{
   if(mPrintDialog->exec() == QDialog::Accepted)
   {
      ui->textEdit->print(mPageSetupDialog->printer());
   }
}

void NotepadWindow::onActionNewWindowTriggered()
{
    NotepadManager::instance().createNewWindow();
}

void NotepadWindow::onActionFindTriggered()
{
    if(mReplaceDialog->isVisible())
    {
        mReplaceDialog->close();
    }

    if(!ui->textEdit->textCursor().selectedText().isEmpty())
    {
        mFindDialog->setTextForFind(ui->textEdit->textCursor().selectedText());
    }
    mFindDialog->show();
}

void NotepadWindow::onActionReplaceTriggered()
{
    if(mFindDialog->isVisible())
    {
        mFindDialog->close();
    }

    if(!ui->textEdit->textCursor().selectedText().isEmpty())
    {
        mReplaceDialog->setTextForFind(ui->textEdit->textCursor().selectedText());
    }
    mReplaceDialog->show();
}

void NotepadWindow::onActionFindNextTriggered()
{
    bool isTextFound = ui->textEdit->find(ui->textEdit->textCursor().selectedText());
    if(!isTextFound)
    {
        cannotFindWarningDialog();
    }
}

void NotepadWindow::cannotFindWarningDialog()
{
    QMessageBox::warning(this, notepadConstants::programName, notepadConstants::cannotFind, QMessageBox::Ok);
}

void NotepadWindow::onActionFindPreviousTriggered()
{
    bool isTextFound = ui->textEdit->find(ui->textEdit->textCursor().selectedText(), QTextDocument::FindBackward);
    if(!isTextFound)
    {
        cannotFindWarningDialog();
    }
}

void NotepadWindow::onTextEditSelectedText(bool yes)
{
    ui->actionCopy->setEnabled(yes);
    ui->actionCut->setEnabled(yes);
    ui->actionDelete->setEnabled(yes);
}

bool NotepadWindow::selectText(const QString& textForFind, QTextDocument::FindFlags flag,
                                        bool isWrapAround, bool isUpDirection)
{
    bool isTextFound = ui->textEdit->find(textForFind, flag);
    if(isTextFound)
    {
        return true;
    }
    if(isWrapAround)
    {
        QTextCursor::MoveOperation moveOperation = QTextCursor::Start;
        if(isUpDirection)
        {
            moveOperation = QTextCursor::End;
        }
        ui->textEdit->moveCursor(moveOperation);
        isTextFound = ui->textEdit->find(textForFind, flag);
    }
    if(!isTextFound)
    {
        cannotFindWarningDialog();
        return false;
    }
    return true;
}

void NotepadWindow::replaceTriggered(const QString& textForFind, QTextDocument::FindFlags flag,
                                             const QString& textForReplace, bool isWrapAround, bool isUpDirection)
{
    bool isTextFound = selectText(textForFind, flag, isWrapAround, isUpDirection);
    if(isTextFound)
    {
        ui->textEdit->textCursor().insertText(textForReplace);
    }
}

void NotepadWindow::onActionReplaceAllTriggered()
{
    ui->textEdit->moveCursor(QTextCursor::Start);
    QString findString = mReplaceDialog->getTextForFind();
    QString replaceString = mReplaceDialog->getTextForReplace();

    while(ui->textEdit->find(findString))
    {
        ui->textEdit->textCursor().insertText(replaceString);
    }
}

void NotepadWindow::onActionFontTriggered()
{
    ui->textEdit->setFont(QFontDialog::getFont(0, ui->textEdit->font()));
}

void NotepadWindow::onActionAboutNotepudTriggered()
{
    if(!mAboutNotepadDialog->isVisible())
    {
        mAboutNotepadDialog->exec();
    }
}
