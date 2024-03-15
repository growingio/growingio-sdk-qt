#ifndef GROWING_DEVICE_INFO_H
#define GROWING_DEVICE_INFO_H

#include <QSettings>
#include "growing_macros.h"

#ifdef QT_GUI_LIB
#include <QGuiApplication>
#include <QScreen>
#endif

GROWING_BEGIN_NAMESPACE

template <typename T, typename D = T>
class Singleton
{
    friend D;
    static_assert(std::is_base_of_v<T, D>, "T should be a base type for D");

public:
    static T& instance();

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton( const Singleton& ) = delete;
    Singleton& operator=( const Singleton& ) = delete;
};

template <typename T, typename D>
T& Singleton<T, D>::instance()
{
    static D inst;
    return inst;
}

class GrowingDeviceInfo: public Singleton<GrowingDeviceInfo> {
public:
    GrowingDeviceInfo(): settings_("growing_cache", QSettings::NativeFormat) {
        // 测试各个平台时, 存储文件查找路径, 推荐直接输出fileName找到对应路径
        // https://doc.qt.io/qt-5/qsettings.html#locations-where-application-settings-are-stored
        qDebug() << settings_.fileName();
        device_id_ = settings_.value("device_id").toString();
        if (device_id_.isEmpty()) {
            // since 5.11 https://bugreports.qt.io/browse/QTBUG-885?focusedCommentId=356049&page=com.atlassian.jira.plugin.system.issuetabpanels%3Acomment-tabpanel
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
            device_id_ = QUuid::createUuid().toString(QUuid::WithoutBraces);
#elif
            device_id_ = QUuid::createUuid().toString().mid(1, 36);
#endif
            settings_.setValue("device_id", device_id_);
        }

// 获取屏幕信息
#ifdef QT_GUI_LIB
        QScreen *primaryScreen = QGuiApplication::primaryScreen();
        QSize size = primaryScreen->size();
        width_ = size.width();
        height_ = size.height();
#endif

        app_name_ = QCoreApplication::instance()->applicationName();
        app_version_ = QCoreApplication::instance()->applicationVersion();

        language_ = QLocale::system().name();

        user_id_ = settings_.value("user_id").toString();
        user_key_ = settings_.value("user_key").toString();

        operating_system_ = QSysInfo::prettyProductName() + " " + QSysInfo::currentCpuArchitecture();
        operating_system_version_ = QSysInfo::productVersion();
        platform_version_ = QSysInfo::productVersion();

        // 获取系统信息
#if defined(Q_OS_MAC)
        platform_ = "Mac";
#elif defined(Q_OS_WIN)
        platform_ = "Windows";
#elif defined(Q_OS_LINUX)
        platform_ = "Linux";
#else
        platform_ = "Unknown";
#endif
    }

public:
    QString device_id() {
        return device_id_;
    }
    int width() {
        return width_;
    }
    int height() {
        return height_;
    }
    QString app_name() {
        return app_name_;
    }
    QString app_version() {
        return app_version_;
    }
    QString language() {
        return language_;
    }
    void set_user_id(QString user_id) {
        settings_.setValue("user_id", user_id);
        user_id_ = user_id;
    }
    QString user_id() {
        return user_id_;
    }
    void set_user_key(QString user_key) {
        settings_.setValue("user_key", user_key);
        user_key_ = user_key;
    }
    QString user_key() {
        return user_key_;
    }
    QString operating_system() {
        return operating_system_;
    }
    QString operating_system_version() {
        return operating_system_version_;
    }
    QString platform_version() {
        return platform_version_;
    }
    QString platform() {
        return platform_;
    }

public:
    void ClearUser() {
        settings_.remove("user_id");
        settings_.remove("user_key");
    }

private:
    QSettings settings_;
    QString device_id_;

    int width_;
    int height_;

    QString app_name_;
    QString app_version_;

    QString language_;

    QString user_id_;
    QString user_key_;

    QString operating_system_;
    QString operating_system_version_;
    QString platform_version_;

    QString platform_;
};

GROWING_END_NAMESPACE

#endif // GROWING_DEVICE_INFO_H
