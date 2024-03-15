# Qt C++ / GrowingAnalytics Library
GrowingAnalytics 提供Qt 5上采集数据到GrowingIO平台的能力

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
#### 发送自定义埋点
|方法名称|类型|是否必填|说明|
| --- | --- | --- | --- |
|set_event_name|QString|是|埋点事件的名称|
|set_attributes|QHash<QString, QString>|是|埋点事件的属性|

** 代码示例 **

```C++
GROWING_NAMESPACE::CustomEvent event;
event.set_attributes({{"key_1", "value_1"}});
event.set_event_name("web_11");
analytics_->SendEvent(event);

```

#### 设置登录用户
设置登录用户id

** 代码示例 **

```C++
analytics_->SetUserId("187****7841");
```

#### 发送登录用户属性
|方法名称|类型|是否必填|说明|
| --- | --- | --- | --- |
|set_attributes|QHash<QString, QString>|是|埋点事件的属性|

** 代码示例 **

```C++
GROWING_NAMESPACE::LoginUserAttributesEvent event;
event.set_attributes({{"cpp_user_key", "value_1"}});
analytics_->SendEvent(event);
```
