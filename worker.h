#ifndef WORKER_H
#define WORKER_H

#include "growing_analytics.h"

class Worker : public QObject {
    Q_OBJECT
public:
    Worker(GROWING_NAMESPACE::GrowingAnalytics *analytics): analytics_(analytics) {
    }
    ~Worker() {}
public slots:
    void process() {
        if (analytics_) {
            GROWING_NAMESPACE::CustomEvent event;
            event.set_attributes({{"key_1", "value_1"}});
            event.set_event_name("web_12");
            analytics_->SendEvent(event);
        }
        qDebug() << "worker process finish";
        emit finished();
    }
signals:
    void finished();
private:
    GROWING_NAMESPACE::GrowingAnalytics *analytics_;
};

#endif // WORKER_H
