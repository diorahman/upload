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
  if(args.count() == 3){
    m_url = QUrl(args[1]);
    m_file = new QFile(args[2], this);
    m_nam = new QNetworkAccessManager(this);

    QTimer::singleShot(250, this, SLOT(exec()));
  }
  else{
    qDebug() << "Error";
    QTimer::singleShot(250, qApp, SLOT(quit()));
  }
}

void Upload::exec(){

  if(m_file->open(QIODevice::ReadOnly)){

    qDebug() << "starting...";

    QNetworkRequest request;
    request.setUrl(m_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

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
}

void Upload::onUploadProgress(qint64 bytes, qint64 bytesTotal){
  qDebug() << bytes << " - " << bytesTotal;
}

void Upload::onNetworkError(QNetworkReply::NetworkError err){
  qDebug() << err;

  QCoreApplication::exit();
}

Upload::~Upload(){

}