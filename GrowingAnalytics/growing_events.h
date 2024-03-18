#ifndef GROWING_EVENTS_H
#define GROWING_EVENTS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QTimeZone>
#include "growing_macros.h"
#include "growing_device_info.h"

GROWING_BEGIN_NAMESPACE

class Event {
public:
    Event(): tm_(QDateTime::currentMSecsSinceEpoch()) {}
    virtual ~Event() = default;
    QString ToJson() {
        QJsonObject json;
        Write(json);
        return QJsonDocument(json).toJson(QJsonDocument::Compact);
    }
    virtual void Write(QJsonObject& json) const {
        json["d"] = domain();

        json["s"] = GrowingDeviceInfo::instance().session_id();
        json["u"] = GrowingDeviceInfo::instance().device_id();

        json["t"] = GetEventType();
        json["tm"] = tm_;

        QString user_id = GrowingDeviceInfo::instance().user_id();
        if (!user_id.isEmpty()) {
            json["cs1"] = user_id;
        }

    }
protected:
    virtual QString GetEventType() const = 0;
public:
    QString domain() const {
        return domain_;
    }
    void set_domain(QString domain) {
        domain_ = domain;
    }
    qint64 tm() {
        return tm_;
    }
protected:
    QString domain_;
    qint64 tm_;
};

class VisitEvent: public Event {
public:
    VisitEvent() = default;
    virtual ~VisitEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        Event::Write(json);
        // json["b"] = GrowingDeviceInfo::instance().platform();
        // 考虑到saas的校验逻辑，不做平台判断
        json["b"] = "Windows";

        int width = GrowingDeviceInfo::instance().width();
        int height = GrowingDeviceInfo::instance().height();
        if (width > 0) {
            json["sw"] = width;
        }
        if (height > 0) {
            json["sh"] = height;
        }

        json["os"] = GrowingDeviceInfo::instance().operating_system();
        json["osv"] = GrowingDeviceInfo::instance().operating_system_version();

        json["cv"] = GrowingDeviceInfo::instance().app_version();
        json["sn"] = GrowingDeviceInfo::instance().app_name();

        json["l"] = GrowingDeviceInfo::instance().language();

        json["av"] = GROWING_VERSION;
    }
protected:
    virtual QString GetEventType() const override {
        return "vst";
    }
};

class PageEvent: public Event {
public:
    PageEvent() = default;
    virtual ~PageEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        Event::Write(json);
        json["p"] = "MainPage";
    }
protected:
    virtual QString GetEventType() const override {
        return "page";
    }
};

class BaseAttributesEvent: public Event
{
public:
    BaseAttributesEvent() = default;
    virtual ~BaseAttributesEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        Event::Write(json);
        if (!attributes_.isEmpty()) {
            QJsonObject attributes_json;
            QHash<QString, QString>::const_iterator iter = attributes_.constBegin();
            while (iter != attributes_.constEnd()) {
                if (!iter.key().isEmpty()) {
                    attributes_json[iter.key()] = iter.value();
                }
                iter++;
            }
            json["var"] = attributes_json;
        }
    }
protected:
    QHash<QString, QString> attributes_;
public:
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

class CustomEvent: public BaseAttributesEvent {
public:
    CustomEvent() = default;
    virtual ~CustomEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        BaseAttributesEvent::Write(json);
        json["n"] = event_name();
        json["ptm"] = ptm_;
    }
protected:
    virtual QString GetEventType() const override {
        return "cstm";
    }
protected:
    QString event_name_;
    qint64 ptm_;
public:
    QString event_name() const {
        return event_name_;
    }
    void set_event_name(QString event_name) {
        if (event_name_ != event_name) {
            event_name_ = event_name;
        }
    }
    qint64 ptm() const {
        return ptm_;
    }
    void set_ptm(qint64 ptm) {
        if (ptm_ != ptm) {
            ptm_ = ptm;
        }
    }
};

class LoginUserAttributesEvent: public BaseAttributesEvent {
public:
    LoginUserAttributesEvent() = default;
    virtual ~LoginUserAttributesEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        BaseAttributesEvent::Write(json);
    }
protected:
    virtual QString GetEventType() const override {
        return "ppl";
    }
};

class VistorAttributesEvent: public BaseAttributesEvent {
public:
    VistorAttributesEvent() = default;
    virtual ~VistorAttributesEvent() override = default;
    virtual void Write(QJsonObject& json) const override {
        BaseAttributesEvent::Write(json);
    }
protected:
    virtual QString GetEventType() const override {
        return "vstr";
    }
};

GROWING_END_NAMESPACE

#endif // GROWING_EVENTS_H
