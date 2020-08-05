# Ubuntu下忘记MySql密码解决方案

1. 在终端输入

   ```
   sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf
   ```

2. 在文件内搜索找到`skip-external-locking`，在其下面添加一行：

   ```
   skip-grant-tables
   ```

3. 重启mysql

   ```
   sudo service mysql restart
   ```

4. 在终端输入命令<font color=red>mysql</font>，进入mysql

5. 使用命令<font color=red>use mysql</font>；切换到mysql数据库

6. 修改root密码

   ```
   UPDATE mysql.user SET authentication_string=password('你想设置的密码') WHERE User='root' AND Host ='localhost';
   ```

7. 修改字段plugin(Plufin字段标识可以用于验证用户身份的插件，如果字段为空，服务器使用内建授权验证机制验证用户身份)

   ```
   UPDATE user SET plugin="mysql_native_password";
   ```

8. 刷新权限，输入命令

   ```
   flush privileges;
   ```

9. 退出mysql

   ```
   quit;
   ```

10. 回到设置的文件内 <font color=red>(/etc/mysql/mysql.conf.d/mysqld.cnf)</font>内把添加的命令注释掉 