# FTPServer
**项目概述**

基于LibEvent的高并发FTP服务器

I/O模型：Reactor事件驱动模式



## 依赖

- C++17
- Libevent库
- Json for modern C++



## 使用

```
mkdir build
cd build
cmake ..
make
```



## 实现思路

- evconnlistene监听连接
- 当有新连接，CloneFactory构造一个FTPServerCmd对象，将他分配给线程池中的一个线程
- 激活一个线程，将新创建的对象加入事件监听（每个线程各自维护自己的Libevent库）
- 当有新命令到达，取出命令执行，并返回结果



所有工作类都继承自task基类：

![image-20240121205242600](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\image-20240121205242600.png)



工厂模式来创建FTPserverCMD对象，当有新的连接请求时，factory产生一个FTPserverCMD对象，FTPserverCMD对象中包含一个map，映射string命令和FTPtask的命令处理对象。即根据字符串，来判断是什么命令，并交给相应的对象处理。



#### 线程池部分：

- 每个线程都有各自的事件循环
- 使用管道pipe，并添加到事件循环，当有新连接时，线程池通过向pipe写入数据，激活线程的回调函数，将新连接添加到线程的事件循环



#### FTPTask类：

- FTPServerCMD类，以及所有处理命令的类，都继承自此类，用多态实现命令处理



### 性能测试：

持续更新....



问题：
