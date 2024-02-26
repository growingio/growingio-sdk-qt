#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "growinganalytics.h"
#include "worker.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    GROWING_NAMESPACE::GrowingOptions options;
    options.set_debug(true);
    options.set_data_source_id("a67a308c0ba3077c");
    options.set_account_id("fdf3a1ab7d0b2eea");
    options.set_server_url("https://napi.growingio.com");
    analytics_ = new GROWING_NAMESPACE::GrowingAnalytics(this, options);

    ui->setupUi(this);
    QObject::connect(ui->btn_cstm, &QPushButton::clicked, this, [=]() {
        GROWING_NAMESPACE::CustomEvent event;
        event.set_device_id("device_id_18101010202");
        event.set_user_id("user_id_18101010202");
        event.set_attributes({{"key_1", "value_1"}});
        event.set_event_name("web_11");
        analytics_->SendEvent(event);
    });

    QObject::connect(ui->btn_ppl, &QPushButton::clicked, this, [=]() {
        GROWING_NAMESPACE::LoginUserAttributesEvent event;
        event.set_device_id("device_id_18101010202");
        event.set_user_id("user_id_18101010202");
        event.set_attributes({{"cpp_user_key", "value_1"}});
        analytics_->SendEvent(event);
    });

    QObject::connect(ui->btn_multi_thread, &QPushButton::clicked, this, [=]() {
        QThread* thread = new QThread();
        Worker* worker = new Worker(analytics_);
        worker->moveToThread(thread);
        connect( thread, &QThread::started, worker, &Worker::process);
        connect( worker, &Worker::finished, thread, &QThread::quit);
        connect( worker, &Worker::finished, worker, &Worker::deleteLater);
        connect( thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    });

    QObject::connect(analytics_, &GROWING_NAMESPACE::GrowingAnalytics::SendSuccess, this, [=](QString data) {
        ui->edit_info->appendPlainText(data);
    });
    QObject::connect(analytics_, &GROWING_NAMESPACE::GrowingAnalytics::SendFailure, this, [=](int err_code, QString err_msg) {
        ui->edit_info->appendPlainText(QString::number(err_code) + ", " + err_msg);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
