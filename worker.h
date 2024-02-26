#ifndef WORKER_H
#define WORKER_H

#include "growinganalytics.h"

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
            event.set_device_id("device_id_18101010202");
            event.set_user_id("user_id_18101010202");
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
