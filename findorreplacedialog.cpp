#include "ui_findorreplacedialog.h"

#include "constsdefine.h"
#include "findorreplacedialog.h"

FindOrReplaceDialog::FindOrReplaceDialog(const DialogueType& dialogType, QWidget *parent) :
    QDialog(parent), ui(new Ui::FindOrReplaceDialog), mDialogueType(dialogType)
{
    ui->setupUi(this);

    setDialogType(dialogType);
    setInitialState();
    initConnects();
}

void FindOrReplaceDialog::initConnects()
{
    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(onFindNextButtonClicked()));
    connect(ui->replaceButton, SIGNAL(clicked()), this, SLOT(onReplaceButtonClicked()));
    connect(ui->replaceAllButton, SIGNAL(clicked()), this, SIGNAL(replaceAllButtonClicked()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onLineEditTextChanged(QString)));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void FindOrReplaceDialog::onFindNextButtonClicked()
{
    emit findNextButtonClicked(getTextForFind(), getFlag(), isWrapAround(), isUpDirection());
}

void FindOrReplaceDialog::onReplaceButtonClicked()
{
    emit replaceButtonClicked(getTextForFind(), getFlag(), getTextForReplace(), isWrapAround(), isUpDirection());
}

void FindOrReplaceDialog::setInitialState()
{
    ui->findButton->setEnabled(false);
    ui->replaceButton->setEnabled(false);
    ui->replaceAllButton->setEnabled(false);
    ui->downRadioButton->setChecked(true);
}

void FindOrReplaceDialog::setDialogType(const DialogueType& dialogType)
{
    if(dialogType == FindOrReplaceDialog::DialogueType::FIND)
    {
        ui->replaceAllButton->setVisible(false);
        ui->replaceButton->setVisible(false);
        ui->replaceLineEdit->setVisible(false);
        ui->replaceLabel->setVisible(false);
        this->setWindowTitle(notepadConstants::findDialogTitle);
    }
    else
    {
        ui->directionGroupBox->setVisible(false);
        this->setWindowTitle(notepadConstants::replaceDialogTitle);
    }
}

FindOrReplaceDialog::~FindOrReplaceDialog()
{
    delete ui;
}

void FindOrReplaceDialog::onLineEditTextChanged(const QString &arg1)
{
    ui->findButton->setEnabled(!arg1.isEmpty());
    ui->replaceButton->setEnabled(!arg1.isEmpty());
    ui->replaceAllButton->setEnabled(!arg1.isEmpty());
}

QString FindOrReplaceDialog::getTextForFind()
{
    return ui->lineEdit->text();
}

QString FindOrReplaceDialog::getTextForReplace()
{
    return ui->replaceLineEdit->text();
}

void FindOrReplaceDialog::setTextForFind(const QString &string)
{
    ui->lineEdit->setText(string);
}

void FindOrReplaceDialog::setTextForReplace(const QString &string)
{
    ui->replaceLineEdit->setText(string);
}

bool FindOrReplaceDialog::isWrapAround()
{
    return ui->wrapAround->isChecked();
}

bool FindOrReplaceDialog::isUpDirection()
{
    return ui->upRadioButton->isChecked();
}

QTextDocument::FindFlags FindOrReplaceDialog::getFlag()
{
    QTextDocument::FindFlags flags;
    if(ui->upRadioButton->isChecked())
    {
        flags |= QTextDocument::FindBackward;
    }
    if(ui->matchCase->isChecked())
    {
        flags |= QTextDocument::FindCaseSensitively;
    }
    return flags;
}

