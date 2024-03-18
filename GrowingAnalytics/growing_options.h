#ifndef GROWING_OPTIONS_H
#define GROWING_OPTIONS_H

#include <QSharedData>
#include <QSharedDataPointer>
#include <QObject>
#include "growing_macros.h"

GROWING_BEGIN_NAMESPACE

class GrowingOptionsData: public QSharedData {
public:
    GrowingOptionsData(): debug_(true), server_url_("https://api.growingio.com") {}
    GrowingOptionsData(const GrowingOptionsData& obj)
        : QSharedData(obj), debug_(obj.debug_), server_url_(obj.server_url_), project_id_(obj.project_id_), domain_(obj.domain_) {}
    ~GrowingOptionsData() {}
private:
    bool debug_;
    QString server_url_;
    QString project_id_;
    QString domain_;
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
    QString project_id() const {
        return d->project_id_;
    }
    void set_project_id(QString project_id) {
        d->project_id_ = project_id;
    }
    QString domain() const {
        return d->domain_;
    }
    void set_domain(QString domain) {
        d->domain_ = domain;
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

#endif // GROWING_OPTIONS_H
