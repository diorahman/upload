#ifndef UPLOAD_H
#define UPLOAD_H

#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QUrl>


class QNetworkAccessManager;
class QFile;

class Upload : public QObject{

  Q_OBJECT

public: 
  Upload(const QStringList & args, QObject * parent = 0);
  ~Upload();

private slots:
  void exec();
  void onFinished();
  void onUploadProgress(qint64 bytes, qint64 bytesTotal);
  void onNetworkError(QNetworkReply::NetworkError err);

private:
  QUrl m_url;

  QNetworkAccessManager * m_nam;
  QFile * m_file;

  int m_offset;
  int m_length;

};

#endif