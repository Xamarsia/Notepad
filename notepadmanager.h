#pragma once

#include <QObject>

class NotepadManager : public QObject
{
public:
    static NotepadManager& instance();

public slots:
    void createNewWindow();

private:
    NotepadManager(QWidget *parent = nullptr);
    ~NotepadManager(){}

    NotepadManager(const NotepadManager&) = delete;
    NotepadManager& operator=(const NotepadManager&)= delete;
};

