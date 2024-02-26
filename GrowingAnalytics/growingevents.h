#ifndef GROWINGEVENTS_H
#define GROWINGEVENTS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimeZone>
#include "growingmacros.h"

#ifdef QT_GUI_LIB
#include <QGuiApplication>
#include <QScreen>
#endif

GROWING_BEGIN_NAMESPACE

class Event: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString device_id_ READ device_id WRITE set_device_id NOTIFY DeviceIdChanged)
    Q_PROPERTY(QString data_source_id_ READ data_source_id WRITE set_data_source_id NOTIFY DataSourceIdChanged)
    Q_PROPERTY(QString user_id_ READ user_id WRITE set_user_id NOTIFY UserIdChanged)
    Q_PROPERTY(QString user_key_ READ user_key WRITE set_user_key NOTIFY UserKeyChanged)
    Q_PROPERTY(QHash<QString, QString> attributes_ READ attributes WRITE set_attributes NOTIFY AttributesChanged)
public:
    Event(QObject* parent = nullptr): QObject(parent) {}
    virtual ~Event() = default;
    QString ToJson() {
        QJsonObject json;
        Write(json);
        return QJsonDocument(json).toJson(QJsonDocument::Compact);
    }
    virtual void Write(QJsonObject& json) const {
        // 获取系统信息
                json["platform"] = "Other";
        #ifdef Q_OS_MAC
                json["platform"] = "Mac";
        #endif
        #ifdef Q_OS_WIN
                json["platform"] = "Windows";
        #endif
        #ifdef Q_OS_LINUX
                json["platform"] = "Linux";
        #endif

        // json["platform"] = "QT";

        // 获取系统版本
        json["operatingSystem"] = QSysInfo::prettyProductName() + " " + QSysInfo::currentCpuArchitecture();
        json["platformVersion"] = QSysInfo::productVersion();
        // operatingSystemVersion废弃，json不允许发送该字段
        // json["operatingSystemVersion"] = QSysInfo::productVersion();

// 获取应用信息
#ifdef QT_GUI_LIB
        QScreen *primaryScreen = QGuiApplication::primaryScreen();
        QSize size = primaryScreen->size();
        json["screenWidth"] = size.width();
        json["screenHeight"] = size.height();

        json["appName"] = QGuiApplication::instance()->applicationName();
        json["appVersion"] = QGuiApplication::instance()->applicationVersion();
#endif

        // 获取时间戳
        json["timestamp"] = QDateTime::currentMSecsSinceEpoch();
        // json["timezoneOffset"] = - (QTimeZone::systemTimeZone().offsetFromUtc(QDateTime::currentDateTime()) / 60);

        // 获取语言
        json["language"] = QLocale::system().name();

        // 获取事件类型
        json["eventType"] = GetEventType();

        json["dataSourceId"] = data_source_id();
        if (!device_id().isEmpty()) {
            json["deviceId"] = device_id();
        }
        if (!user_id().isEmpty()) {
            json["userId"] = user_id();
        }
        if (!user_key().isEmpty()) {
            json["userKey"] = user_key();
        }
        if (!attributes_.isEmpty()) {
            QJsonObject attributes_json;
            QHash<QString, QString>::const_iterator iter = attributes_.constBegin();
            while (iter != attributes_.constEnd()) {
                if (!iter.key().isEmpty()) {
                    attributes_json[iter.key()] = iter.value();
                }
                iter++;
            }
            json["attributes"] = attributes_json;
        }
    }
protected:
    virtual QString GetEventType() const = 0;

protected:
    QString device_id_;
    QString data_source_id_;
    QString user_id_;
    QString user_key_;
    QHash<QString, QString> attributes_;
signals:
    void DeviceIdChanged(QString);
    void DataSourceIdChanged(QString);
    void UserIdChanged(QString);
    void UserKeyChanged(QString);
    void AttributesChanged(QHash<QString, QString>);
public:
    QString device_id() const {
        return device_id_;
    }
    void set_device_id(QString device_id) {
        if (device_id_ != device_id) {
            device_id_ = device_id;
            emit DeviceIdChanged(device_id_);
        }
    }
    QString data_source_id() const {
        return data_source_id_;
    }
    void set_data_source_id(QString data_source_id) {
        if (data_source_id_ != data_source_id) {
            data_source_id_ = data_source_id;
            emit DataSourceIdChanged(data_source_id_);
        }
    }
    QString user_id() const {
        return user_id_;
    }
    void set_user_id(QString user_id) {
        if (user_id_ != user_id) {
            user_id_ = user_id;
            emit UserIdChanged(user_id_);
        }
    }
    QString user_key() const {
        return user_key_;
    }
    void set_user_key(QString user_key) {
        if (user_key_ != user_key) {
            user_key_ = user_key;
            emit UserKeyChanged(user_key_);
        }
    }
    QHash<QString, QString> attributes() const {
        return attributes_;
    }
    // QT容器类实现了隐式内存共享，可以不使用智能指针
    void set_attributes(QHash<QString, QString> attributes) {
        if (attributes_ != attributes) {
            attributes_ = attributes;
            emit AttributesChanged(attributes_);
        }
    }
};

class CustomEvent: public Event {
    Q_OBJECT
    Q_PROPERTY(QString event_name_ READ event_name WRITE set_event_name NOTIFY EventNameChanged)
public:
    CustomEvent(QObject* parent = nullptr): Event(parent) {}
    virtual ~CustomEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        Event::Write(json);
        json["eventName"] = event_name();
    }
protected:
    virtual QString GetEventType() const override {
        return "CUSTOM";
    }
protected:
    QString event_name_;
signals:
    void EventNameChanged(QString);
public:
    QString event_name() const {
        return event_name_;
    }
    void set_event_name(QString event_name) {
        if (event_name_ != event_name) {
            event_name_ = event_name;
            emit EventNameChanged(event_name_);
        }
    }
};

class LoginUserAttributesEvent: public Event {
    Q_OBJECT
public:
    LoginUserAttributesEvent(QObject* parent = nullptr): Event(parent) {}
    virtual ~LoginUserAttributesEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        Event::Write(json);
    }
protected:
    virtual QString GetEventType() const override {
        return "LOGIN_USER_ATTRIBUTES";
    }
};

GROWING_END_NAMESPACE

#endif // GROWINGEVENTS_H
