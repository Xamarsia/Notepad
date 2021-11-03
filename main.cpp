#include "notepadmanager.h"
#include "textfiledialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   NotepadManager::instance().createNewWindow();

   return app.exec();
}
