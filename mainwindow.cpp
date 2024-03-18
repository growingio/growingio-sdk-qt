#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "growing_analytics.h"
#include "worker.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    GROWING_NAMESPACE::GrowingOptions options;
    options.set_debug(true);
    options.set_server_url("https://api.growingio.com");
    options.set_project_id("0a1b4**********************bdb96");
    options.set_domain("76e**********6b2");
    analytics_ = new GROWING_NAMESPACE::GrowingAnalytics(this, options);

    ui->setupUi(this);

    QObject::connect(ui->btn_set_user, &QPushButton::clicked, this, [=]() {
        analytics_->SetUserId("187****7841");
    });
    QObject::connect(ui->btn_clear_user, &QPushButton::clicked, this, [=]() {
        analytics_->ClearUser();
    });

    QObject::connect(ui->btn_cstm, &QPushButton::clicked, this, [=]() {
        GROWING_NAMESPACE::CustomEvent event;
        event.set_attributes({{"qt_sdk_var", "qt_sdk_var_value"}});
        event.set_event_name("qt_sdk_event");
        analytics_->SendEvent(event);
    });

    QObject::connect(ui->btn_ppl, &QPushButton::clicked, this, [=]() {
        GROWING_NAMESPACE::LoginUserAttributesEvent event;
        event.set_attributes({{"qt_sdk_ppl_attr", "qt_sdk_ppl_attr_value"}});
        analytics_->SendEvent(event);
    });

    QObject::connect(ui->btn_vstr, &QPushButton::clicked, this, [=]() {
        GROWING_NAMESPACE::VistorAttributesEvent event;
        event.set_attributes({{"qt_sdk_vstr_attr", "qt_sdk_vstr_attr_value"}});
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
