# FTPServer
基于LibEvent的FTP服务器



所有工作类都继承自task基类：
我们用工厂模式来创建FTPserverCMD对象。
当有新的连接请求时，factory产生一个FTPserverCMD对象，FTPserverCMD对象中包含一个map，映射string和FTPtask的命令处理对象。即根据字符串，来判断是什么命令，并交给相应的对象处理。（factory创建FTPserverCMD命令时，初始化map）

类的继承关系：

![image-20240121205242600](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\image-20240121205242600.png)
