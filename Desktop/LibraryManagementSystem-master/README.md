# LibraryManagementSystem

## 简介

本工程是一个图书管理系统，是 OOP 课程的大作业。

## 环境与依赖

* Ubuntu

* 依赖`mongo-cxx-driver`（[git 仓库在这里](https://github.com/mongodb/mongo-cxx-driver)）

## Build

在根目录下执行
```
make
```
即可

## 架构

本工程大体分为 5 部分。分别是：

* 核心类

* 数据库（`Database`）：负责直接与数据库交互，提供数据库接口

* 服务器（`Server`）：所有需要在服务器跑的代码

* 控制器（`Controller`）：所有在用户本地需要计算、与`Server`交互的流程

* 客户端（`Client`）：负责直接与用户交互

整个架构使用代理/委托模式，`Server`包装`Database`的接口，`Controller`包装`Server`的接口，`Client`包装`Controller`的接口，并最终呈现给用户。

程序的运行流程即是上面的顺序（`Client`，`Controller`，`Server`，`Database`）

本文档将分这 5 部分来进行介绍。

## 核心类

这部分包含了贯穿其他所有部分的核心类，包括：

* `Field`：“字段”类，是一个与`std::pair<string,string>`作用类似的类。

  * 重载了`=`运算符，两个对象键相同即判定为相等。（每个对象每个键最多对应一个值）

* `AbstractObject<Container>`（及其派生类）：使用模板实现，用容器`Container`存放所有`Field`。派生出`Object`类和`Search`类。

  * `Object`类继承自`AbstractObject<set>`，将派生出`User`、`Book`、`PracticalBook`、`Record`四个类。分别为用户类、书本信息类（相同书本的集合，存储相同书本的信息，如ISBN、出版社等）、实体书本类（存储每一本具体书籍，与借阅记录对应）、记录类（包含借、还、浏览、预览记录）

    * 对于每一个类，我们都设置了成员函数`explicitKey()`、`implicityKey()`、`uniqueKey()`（使用虚函数实现），分别返回显示给用户看的键、隐藏的键（如`Status`等）、可以用于确定唯一对象的类（如用户的用户名）

  * `Search`类继承自`AbstractObject<multiset>`（考虑到可能对同一字段有多个检索要求），将派生出`CompleteMatchingSearch`（完全匹配检索）类、`ReSearch`（正则表达式检索）类。

    * 这两个派生类本身没有任何实现上的区别，在检索信息时，会根据不同的传入类型（通过虚函数判断）执行对应的检索策略。

* `Password`：封装密码的类（用户密码使用 MD5 加密进行存储，保证安全性）。

* `Content`：封装预览内容的**基类**。派生出`TxtContent`用于预览`txt`格式存储的文件。（这么设计的目的是为了方便添加对新文件类型的支持）

  * 预览使用虚函数实现。

## Database

这是包装数据库的类（数据库使用 MongoDB），包装了`mongocxx`（MongoDB 的 c++ 驱动）。**负责直接与数据库交互**

这个类包含如下接口（以下所有“对象”均包括`Object`的 4 个派生类。使用虚函数判断派生类类型）：

* `userExist`：判断用户是否存在**并检查权限**。（检查权限的目的是为了防止用户在前端做骚操作）

* `isAdmin`：判断用户是否为管理员。

* `isRoot`：判断用户是否为 Root。

* `findOne`：利用 uniqueKey 精确查找唯一的对象（如根据用户名查找对应用户）。

* `objectExist`：判断对象是否存在。

（上面的所有函数都会判断对象是否被冻结）

* `newRecordId`：在新建一条记录时会调用，返回新记录的编号。

* `modifyPassword`：修改用户密码。

（下面4个函数都使用模板实现）

* `add`：添加对象。

* `update`：更新对象。

* `remove`：删除对象。

* `search`：检索对象。（传入`Search`类对象，会根据虚函数判断其类型，并执行对应检索策略）

此外，为了方便将一个`AbstractObject`的派生类转换为`mongocxx`需要的`document`类，我们设置了`toDocument`以及一些类似的私有成员函数，来支持`Object`、`Search`类的转换。

## Server

包装`Database`类，主要负责**权限判断**（防止用户在前端乱搞）和**一些其他运算、处理**。

**下面将对接口做冗长的列举，这些接口与需求有着较大联系，本质上是使用代理/委托模式对数据库接口的扩充：**

* `search`：检索对象。

* `add`：添加对象。

* `update`：更新对象。

* `remove`：删除对象（与需求中的要求不同的是，我们认为这个操作是十分危险的，因此这个接口只对 Root 开放。作为替代品，我们提供了“冻结”用户/书本的接口，在下面可以看到）

（以上接口均使用模板实现）

* `userLogin`：用户登录（查找用户、检查密码、返回用户信息）

* `modifyPassword`：修改密码

* `borrowBook`：提出借阅申请（同时生成对应的 Record）

* `returnBook`：提出归还申请（同时生成对应的 Record）

* `acceptRequest`：接受申请（包括了借阅申请和归还申请）（同时更新对应记录的状态）

* `rejectRequest`：接受申请（包括了借阅申请和归还申请）（同时更新对应记录的状态）

（以下两个冻结/解冻的接口使用模板实现，并只针对`User`、`Book`、`PracticalBook`类，不支持冻结一个`Record`）

* `freeze`：冻结对象。

* `unfreeze`：解冻对象。

* `browseBook`：浏览一本书籍的信息，用于提示数据库生成对应浏览记录。

* `previewBookContent`：预览一本书籍。（同时生成对应的 Record）


## 分工

王聿中：核心类、Database、Server

沈诣博：Controller、Client（命令行界面）

俞宸：Client（图形界面）

同时，所有人各自负责自己部分的文档。

[这是我们的 git 仓库](https://github.com/wangyurzee7/LibraryManagementSystem)，工作量可以在这里体现。

## 致谢

感谢刘知远老师、黄民烈老师在课堂上的精彩授课。

感谢本课程的所有助教的课后指导。

感谢徐良钦同学小组、邢健开同学小组的git仓库给我们的一些启发与帮助。

感谢所有与我们讨论过本课程内容或大作业相关问题的同学。
