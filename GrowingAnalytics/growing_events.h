#ifndef GROWING_EVENTS_H
#define GROWING_EVENTS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QTimeZone>
#include "growing_macros.h"
#include "growing_device_info.h"

GROWING_BEGIN_NAMESPACE

class Event
{
public:
    Event() = default;
    virtual ~Event() = default;
    QString ToJson() {
        QJsonObject json;
        Write(json);
        return QJsonDocument(json).toJson(QJsonDocument::Compact);
    }
    virtual void Write(QJsonObject& json) const {
        // 获取系统信息
        json["platform"] = GrowingDeviceInfo::instance().platform();
        // json["platform"] = "QT";

        // 获取系统版本
        json["operatingSystem"] = GrowingDeviceInfo::instance().operating_system();
        json["platformVersion"] = GrowingDeviceInfo::instance().platform_version();
        // operatingSystemVersion废弃，json不允许发送该字段
        // json["operatingSystemVersion"] = GrowingDeviceInfo::instance().operating_system_version();

        // 获取屏幕分辨率
        int width = GrowingDeviceInfo::instance().width();
        int height = GrowingDeviceInfo::instance().height();
        if (width > 0) {
            json["screenWidth"] = width;
        }
        if (height > 0) {
            json["screenHeight"] = height;
        }

        json["appName"] = GrowingDeviceInfo::instance().app_name();
        json["appVersion"] = GrowingDeviceInfo::instance().app_version();

        // 获取语言
        json["language"] = GrowingDeviceInfo::instance().language();

        // 获取设备标识
        json["deviceId"] = GrowingDeviceInfo::instance().device_id();

        // 获取用户信息
        QString user_id = GrowingDeviceInfo::instance().user_id();
        QString user_key = GrowingDeviceInfo::instance().user_key();
        if (!user_id.isEmpty()) {
            json["userId"] = user_id;
        }
        if (!user_key.isEmpty()) {
            json["userKey"] = user_key;
        }

        // 获取时间戳
        json["timestamp"] = QDateTime::currentMSecsSinceEpoch();
        // json["timezoneOffset"] = - (QTimeZone::systemTimeZone().offsetFromUtc(QDateTime::currentDateTime()) / 60);

        // 获取事件类型
        json["eventType"] = GetEventType();

        json["dataSourceId"] = data_source_id();

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
    QString data_source_id_;
    QHash<QString, QString> attributes_;
public:
    QString data_source_id() const {
        return data_source_id_;
    }
    void set_data_source_id(QString data_source_id) {
        if (data_source_id_ != data_source_id) {
            data_source_id_ = data_source_id;
        }
    }
    QHash<QString, QString> attributes() const {
        return attributes_;
    }
    // QT容器类实现了隐式内存共享，可以不使用智能指针
    void set_attributes(QHash<QString, QString> attributes) {
        if (attributes_ != attributes) {
            attributes_ = attributes;
        }
    }
};

class CustomEvent: public Event {
public:
    CustomEvent(): Event() {}
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
public:
    QString event_name() const {
        return event_name_;
    }
    void set_event_name(QString event_name) {
        if (event_name_ != event_name) {
            event_name_ = event_name;
        }
    }
};

class LoginUserAttributesEvent: public Event {
public:
    LoginUserAttributesEvent(): Event() {}
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

#endif // GROWING_EVENTS_H
