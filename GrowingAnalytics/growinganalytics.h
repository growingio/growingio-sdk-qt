#ifndef GROWINGANALYTICS_H
#define GROWINGANALYTICS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include "growingevents.h"
#include "growingoptions.h"
#include "growingmacros.h"

#ifdef QT_GUI_LIB
#include <QGuiApplication>
#include <QScreen>
#endif

GROWING_BEGIN_NAMESPACE

// 参考Google开源项目命名风格 https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/
class GrowingAnalytics : public QObject
{
    Q_OBJECT
public:
    // 构造时可以指定QApplication为parent，析构时自动释放
    GrowingAnalytics(QObject* parent = nullptr, GrowingOptions options = GrowingOptions()): QObject(parent), network_access_manager_(this), options_(options) {

        // 将多线程消息转到当前对象所属线程处理
        connect(this, &GrowingAnalytics::SendMessage, this, &GrowingAnalytics::PostMessage, Qt::QueuedConnection);
#ifdef QT_GUI_LIB
        // 处理退出信号，发送close事件
        // QObject::connect(QGuiApplication::instance(), &QGuiApplication::aboutToQuit, [](){
        //     qDebug() << "close event";
        // });

        // 处理请求完成信号，日志化
        // QObject::connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ReplyFinished(QNetworkReply*)));
        // QObject::connect(&_networkAccessManager, &QNetworkAccessManager::finished, this, [](QNetworkReply* reply){
        //     qDebug() << reply->errorString();
        // });
#endif
    }
    virtual ~GrowingAnalytics() = default;
    void SendEvent(Event& event) {
        event.set_data_source_id(options_.data_source_id());

        QJsonArray json_array;
        QJsonObject json_obj;
        event.Write(json_obj);
        json_array.push_back(json_obj);

        QJsonDocument doc(json_array);
        QByteArray data = doc.toJson(QJsonDocument::Compact);

        if (options_.debug()) {
            qDebug() << "send data";
        }
        emit SendMessage(data);
    }
// private slots:
//     void ReplyFinished(QNetworkReply* reply) {
//         qDebug() << reply->errorString();
//     }
private:
    QNetworkAccessManager network_access_manager_;
    GrowingOptions options_;

// 发送信息信号
signals:
    void SendSuccess(QString data);
    void SendFailure(int err_code, QString err_msg);
    void SendMessage(QByteArray data);

private:
    // body中实现编译器也会默认为inline（取决于编译器）
    inline QString GetCollectServerUrl() {
        return options_.server_url() + QLatin1String("/v3/projects/")
                + options_.account_id() + QLatin1String("/collect");
    }
    inline QString GetUserAgent() {
        // Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) Qt/4.7.4 Mobile Safari/534.3
        return "Mozilla/5.0 (" + QSysInfo::prettyProductName() + "; " + QLocale::system().name() + ") "
            "GrowingAnalytics/" GROWING_VERSION " Qt/" QT_VERSION_STR;
    }
private slots:
    void PostMessage(QByteArray data) {
        const QUrl url(GetCollectServerUrl());
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setHeader(QNetworkRequest::UserAgentHeader, GetUserAgent());

        if (options_.debug()) {
            qDebug() << "post data: " << data;
        }

        if (!options_.debug()) {
            QNetworkReply *reply = network_access_manager_.post(request, data);
            // 忽略ssl异常
            QObject::connect(reply, SIGNAL(sslErrors(QList<QSslError>)), reply,
                             SLOT(ignoreSslErrors()));
            // 请求日志
            QObject::connect(reply, &QNetworkReply::finished, this, [=](){
                if (reply->error() == QNetworkReply::NoError) {
                    // QString contents = QString::fromUtf8(reply->readAll());
                    qDebug() << "send success: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                    emit SendSuccess(data);
                } else {
                    int err_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                    QString err_msg = reply->errorString();
                    qDebug() << err_code << ", " << err_msg;
                    emit SendFailure(err_code, err_msg);
                }
                reply->deleteLater();
            });
        }

        if (options_.debug()) {
            qDebug() << "post finish";
        }
    }
};

GROWING_END_NAMESPACE

#endif // GROWINGANALYTICS_H
