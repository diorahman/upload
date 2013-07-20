#include <QtCore/QCoreApplication>
#include <QStringList>

#include "upload.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  Upload upload(QCoreApplication::arguments());
  return app.exec();
}
