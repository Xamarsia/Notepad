#pragma once

#include <QWidget>
#include <QDialog>
#include <QTextDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class FindOrReplaceDialog; }
QT_END_NAMESPACE

class NotepadWindow;

class FindOrReplaceDialog : public QDialog
{
    Q_OBJECT

public:

enum class DialogueType {FIND, REPLACE};

    FindOrReplaceDialog(const DialogueType& dialogType, QWidget *parent = nullptr);
    ~FindOrReplaceDialog();

    void setDialogType(const DialogueType& dialogType);
    void setInitialState();
    void initConnects();
    QString getTextForFind();
    QString getTextForReplace();
    void setTextForFind(const QString &string);
    void setTextForReplace(const QString &string);

    QTextDocument::FindFlags getFlag();

    bool isWrapAround();
    bool isUpDirection();

private slots:
    void onLineEditTextChanged(const QString &arg1);
    void onReplaceButtonClicked();
    void onFindNextButtonClicked();

signals:
    void findNextButtonClicked(const QString&, QTextDocument::FindFlags, bool, bool);
    void replaceButtonClicked(const QString&, QTextDocument::FindFlags, const QString&, bool, bool);
    void replaceAllButtonClicked();

private:
    Ui::FindOrReplaceDialog *ui;

    bool mWrapAround = false;
    NotepadWindow *mNotepadWindow;
    DialogueType mDialogueType;
};
