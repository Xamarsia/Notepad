#include "notepadmanager.h"
#include "notepadwindow.h"

NotepadManager::NotepadManager(QWidget *parent) :  QObject(parent){}

NotepadManager& NotepadManager::instance()
{
    static NotepadManager notepadManager;
    return notepadManager;
}

void NotepadManager::createNewWindow(){
    NotepadWindow *window = new NotepadWindow();
    window->setWindowIcon(QIcon(":/icon/NotepadIcon/document.png"));
    window->show();
}
