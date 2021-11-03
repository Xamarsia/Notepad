#include "ui_aboutnotepaddialog.h"

#include "aboutnotepaddialog.h"
#include "constsdefine.h"

AboutNotepadDialog::AboutNotepadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutNotepadDialog)
{
    ui->setupUi(this);
    connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
    setWindowTitle(notepadConstants::aboutNotepadDialogTitle);
}

AboutNotepadDialog::~AboutNotepadDialog()
{
    delete ui;
}

void AboutNotepadDialog::onOkPushButtonClicked()
{
    close();
}

