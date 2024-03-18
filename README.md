# Qt C++ / GrowingAnalytics Library
GrowingAnalytics 提供Qt 5/6上采集数据到GrowingIO平台的能力

本地测试版本：5.15.2/6.2.4

### 功能
发送自定义事件

发送登录用户属性事件

### 文件夹说明
- Example demo所在目录
    - GrowingAnalytics library所在目录

### 如何使用该库
通过git clone获取本仓库

在项目中通过include(./GrowingAnalytics/GrowingAnalytics.pri)方式包含GrowingAnalytics.pri

在需要使用的地方通过#include "growinganalytics.h"包含头文件

### 使用方式
#### 初始化
|方法名称|类型|是否必填|说明|
| --- | --- | --- | --- |
|set_debug|bool|否|是否debug模式，debug仅打印日志，实际不产生网络请求|
|set_server_url|QString|是|设置采集服务地址|
|set_project_id|QString|是|设置项目id|
|set_domain|QString|是|设置项目domain|

** 代码示例 **

```C++
GROWING_NAMESPACE::GrowingOptions options;
options.set_debug(false);
options.set_server_url("https://api.growingio.com");
options.set_project_id("0a1b4**********************bdb96");
options.set_domain("76e**********6b2");
analytics_ = new GROWING_NAMESPACE::GrowingAnalytics(this, options);
```

#### 发送自定义埋点
|方法名称|类型|是否必填|说明|
| --- | --- | --- | --- |
|set_event_name|QString|是|埋点事件的名称|
|set_attributes|QHash<QString, QString>|是|埋点事件的属性|

** 代码示例 **

```C++
GROWING_NAMESPACE::CustomEvent event;
event.set_attributes({{"qt_sdk_var", "qt_sdk_var_value"}});
event.set_event_name("qt_sdk_event");
analytics_->SendEvent(event);
```

#### 设置登录用户
设置登录用户id

** 代码示例 **

```C++
analytics_->SetUserId("187****7841");
```

#### 设置登录用户
清除登录用户id

** 代码示例 **

```C++
analytics_->ClearUser();
```

#### 发送访问用户属性
|方法名称|类型|是否必填|说明|
| --- | --- | --- | --- |
|set_attributes|QHash<QString, QString>|是|访问用户的属性|

** 代码示例 **

```C++
GROWING_NAMESPACE::VistorAttributesEvent event;
event.set_attributes({{"qt_sdk_vstr_attr", "qt_sdk_vstr_attr_value"}});
analytics_->SendEvent(event);
```

#### 发送登录用户属性
|方法名称|类型|是否必填|说明|
| --- | --- | --- | --- |
|set_attributes|QHash<QString, QString>|是|登录用户的属性|

** 代码示例 **

```C++
GROWING_NAMESPACE::LoginUserAttributesEvent event;
event.set_attributes({{"qt_sdk_ppl_attr", "qt_sdk_ppl_attr_value"}});
analytics_->SendEvent(event);
```
