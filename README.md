# APUE
A repository for learning APUE

# 文件


## 函数

### 文件I/O

- open

	- opentat

- creat

- close

- lseek

- read

- write

- dup

	- dup2

- sync

	- fsync

	- fdatasync

- fnctl

- ioctl

### 文件操作

- stat

	- fstat

	- fstatat

	- lstat

- access

	- accessat

- umask

- chmod

	- fchmod

	- fchmodat

- chown

	- fchown

	- fchownat

	- lchown

- truncate

	- ftruncate

- 链接

	- link

		- linkat

	- unlink

		- unlinkat

	- remove

- rename

	- renameat

- symlink

	- symlinkat

- readlink

	- readlinkat

- 时间

	- futimens

		- utimensat

	- utimes

- 目录

	- mkdir

		- mkdirat

	- rmdir

	- readdir

	- opendir

		- fdopendir

	- closedir

	- telldir

	- seekdir

	- chdir

		- fchdir

	- getcwd

## 文件共享

### 进程表项

- fd标志

- 文件指针

### 文件表项

- 文件状态标志

	- 只读打开

	- 只写打开

	- 读写打开

	- 只执行打开

	- 只搜索打开目录

	- 追加写

	- 非阻塞模式

	- 等待写完成（数据和属性）

	- 等待写完成（仅数据）

	- 同步读和写

	- 等待写完成

	- 异步I/O

- 当前文件偏移量

- v节点指针

### v节点表项

- v节点信息

- v_data

### i节点

- i节点信息

	- 

- 当前文件长度

- i_vnode

## 文件缓存

## 文件结构体stat

### st_mode

- 文件类型

	- 普通文件

	- 目录文件

	- 块特殊文件

	- 字特殊文件

	- FIFO

	- socket

	- 符号链接

- 文件权限

	- S_ISUID执行时设置用户ID

	- S_ISGID执行时设置用户组

	- S_ISVTX粘着位（保存正文位）

		- 删除或移动限制

			- 拥有此文件

			- 拥有此目录

			- 超级用户

	- 用户读、写、执行

	- 组读、写、执行

	- 其他读、写、执行

### st_ino

### st_dev

### st_rdev

### st_nlink

### st_uid

### st_gid

### st_size

- 文件长度

	- 普通文件：16或512的整倍数

		- 文件空洞

	- 符号链接：文件名中的实际字节数

	- 目录文件

### st_atime

- 文件数据最后访问时间

### st_mtime

- 文件数据最后修改时间

### st_ctime

- i节点状态最后更改时间

### st_blksize

- I/O适合的长度块

### st_blocks

- 512字节快（扇区）数量

## 与进程相关ID

### 实际用户ID

### 实际组ID

### 有效用户ID

### 有效组ID

### 附属组ID

### 保存的设置用户ID

### 保存的设置组ID

## 文件系统

### i节点编号

- i节点

	- i节点自身信息

		- 链接数

	- 数据块

	- 目录块

### 文件名

