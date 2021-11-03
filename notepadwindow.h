#pragma once

#include "textfiledialog.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QTextDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class NotepadWindow; }
QT_END_NAMESPACE

class FindOrReplaceDialog;
class NotepadManager;
class QPrintDialog;
class QPageSetupDialog;
class AboutNotepadDialog;
class QLabel;
class TextFileDialog;
class FileHelper;


class NotepadWindow : public QMainWindow
{
    Q_OBJECT

public:
    NotepadWindow(QWidget *parent = nullptr);
    ~NotepadWindow();

private slots:

    void onActionExitTriggered();

    void onActionTimeDateTriggered();

    void onActionDeleteTriggered();

    void onActionZoomInTriggered();

    void onActionZoomOutTriggered();

    void onActionRestoreDefaultZoomTriggered();

    void onActionWordWrapTriggered(bool checked);

    void onActionStatusBarTriggered(bool checked);

    void onUndoAvailableTriggered(bool available);

    void onTextEditSelectedText(bool yes);

    void onActionNewTriggered();

    void onActionOpenTriggered();

    void onActionSaveTriggered();

    void onActionSaveAsTriggered();

    void onTextEditTextChanged();

    void onActionPrintTriggered();

    void onActionFindTriggered();

    void onActionReplaceTriggered();

    void onActionPageSetupTriggered();

    void onActionNewWindowTriggered();

    void onActionFindNextTriggered();

    void onActionFindPreviousTriggered();

    void onActionReplaceAllTriggered();

    void onActionFontTriggered();

    void onActionAboutNotepudTriggered();

    void onTextEditCursorPositionChanged();

private:

    void initConnects();

    bool requestClose();

    void cannotFindWarningDialog();

    void setInfoFromFileHelper();

    void saveInNewFile(const FileInfo &fileInfo);

    void saveInCurrentFile();

    void setInitialState();

    void setTextChanged(bool fileChanged);

    void closeEvent(QCloseEvent *event) override;

    void saveInCurrentFile(const FileInfo& fileInfo);

    bool selectText(const QString& textForFind, QTextDocument::FindFlags flag,
                            bool isWrapAround, bool isUpDirection);

    void replaceTriggered(const QString& textForFind, QTextDocument::FindFlags flag,
                          const QString& textForReplace, bool isWrapAround, bool isUpDirection);

private:

    Ui::NotepadWindow *ui;

    QPrintDialog* mPrintDialog;
    FindOrReplaceDialog *mFindDialog;
    FindOrReplaceDialog *mReplaceDialog;
    QPageSetupDialog* mPageSetupDialog; 
    AboutNotepadDialog* mAboutNotepadDialog;

    FileHelper* mFileHelper;

    QLabel  *mScaleLabel;
    QLabel  *mLineTerminatorLabel;
    QLabel  *mBlankLabel;
    QLabel  *mEncodingLabel;
    QLabel  *mCursorPositionLabel;

    int mZoomCounter = 100;
    bool mFileChanged = false;
};

