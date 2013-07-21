#include "upload.h"

#include <QStringList>
#include <QFile>
#include <QNetworkAccessManager>
#include <QCoreApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QDebug>

Upload::Upload(const QStringList & args, QObject * parent) : QObject(parent){
  if(args.count() == 5){
    m_url = QUrl(args[1]);
    m_file = new QFile(args[2], this);
    m_nam = new QNetworkAccessManager(this);

    m_offset = (args[3]).toInt();
    m_length = (args[4]).toInt();

    QTimer::singleShot(250, this, SLOT(exec()));
  }
  else{
    qDebug() << "Error arguments \n$ ./upload url file offset length\ne.g.\n./upload \"http://posttestserver.com/post.php?dump\" test.txt 3 5";
    QTimer::singleShot(250, qApp, SLOT(quit()));
  }
}

void Upload::exec(){

  if(m_file->open(QIODevice::ReadOnly)){

    qDebug() << "starting...";

    m_file->seek(m_offset);

    QNetworkRequest request;
    request.setUrl(m_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setHeader(QNetworkRequest::ContentLengthHeader, m_length);

    QNetworkReply * reply = m_nam->post(request, m_file);

    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64))); 
  }else{
    qDebug() << "failed";
  } 
}

void Upload::onFinished(){
  QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
  qDebug() << "finished: " << reply->readAll(); 
  QTimer::singleShot(250, qApp, SLOT(quit())); 
}

void Upload::onUploadProgress(qint64 bytes, qint64 bytesTotal){
  Q_UNUSED(bytesTotal)

  qDebug() << bytes << " - " << m_length;
}

void Upload::onNetworkError(QNetworkReply::NetworkError err){
  qDebug() << err;
  QTimer::singleShot(250, qApp, SLOT(quit()));
}

Upload::~Upload(){

}