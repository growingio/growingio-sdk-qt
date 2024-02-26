#ifndef GROWINGOPTIONS_H
#define GROWINGOPTIONS_H

#include <QSharedData>
#include <QSharedDataPointer>
#include <QObject>
#include "growingmacros.h"

GROWING_BEGIN_NAMESPACE

class GrowingOptionsData: public QSharedData {
public:
    GrowingOptionsData(): debug_(true), server_url_("https://napi.growingio.com") {}
    GrowingOptionsData(const GrowingOptionsData& obj)
        : QSharedData(obj), debug_(obj.debug_), data_source_id_(obj.data_source_id_), server_url_(obj.server_url_) {}
    ~GrowingOptionsData() {}
private:
    bool debug_;
    QString data_source_id_;
    QString account_id_;
    QString server_url_;
    friend class GrowingOptions;
};

class GrowingOptions {
public:
    GrowingOptions() {
        d = new GrowingOptionsData();
    }
    GrowingOptions(const GrowingOptions& obj): d(obj.d) {}
    GrowingOptions& operator=(const GrowingOptions& obj) {
        if (this == &obj) {
            return *this;
        }
        d = obj.d;
        return *this;
    }
    bool debug() const {
        return d->debug_;
    }
    void set_debug(bool debug) {
        d->debug_ = debug;
    }
    QString data_source_id() const {
        return d->data_source_id_;
    }
    void set_data_source_id(QString data_source_id) {
        d->data_source_id_ = data_source_id;
    }
    QString account_id() const {
        return d->account_id_;
    }
    void set_account_id(QString account_id) {
        d->account_id_ = account_id;
    }
    QString server_url() const {
        return d->server_url_;
    }
    void set_server_url(QString server_url) {
        d->server_url_ = server_url;
    }
private:
    QSharedDataPointer<GrowingOptionsData> d;
};

GROWING_END_NAMESPACE

#endif // GROWINGOPTIONS_H
