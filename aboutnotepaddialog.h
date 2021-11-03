#pragma once

#include <QDialog>

namespace Ui {
class AboutNotepadDialog;
}

class AboutNotepadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutNotepadDialog(QWidget *parent = nullptr);
    ~AboutNotepadDialog();

private slots:

    void onOkPushButtonClicked();

private:
    Ui::AboutNotepadDialog *ui;
};

