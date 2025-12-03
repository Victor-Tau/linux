# Linux 编程

本仓库包含若干使用 C 语言编写的 Linux 编程示例，主要涵盖：

- **文件 I/O 与性能对比**
- **多线程与信号量同步**
- **共享内存与进程间通信（IPC）**
- **TCP / UDP 套接字网络编程**

适合作为 Linux 环境下 C 语言系统编程与网络编程入门/课程实验示例。

---

## 环境要求

- 操作系统：Linux
- 编译器：`gcc`（任意较新版本均可）
- 已安装常见开发头文件与库（glibc、pthread 等）

编译命令示例均以 `gcc` 为例，如有需要可自行加入 `-g` 等调试选项。

---

## 文件与功能说明

### 1. 文件拷贝与性能对比

#### `filecopy.c`

- 功能：使用 **1 字节缓冲区** 拷贝文件，并统计耗时。
- 特点：通过极小缓冲区进行读写，便于与后续示例进行性能对比。
- 行为：
  - 从当前目录下固定读取 `poster.jpg`，拷贝为 `test.jpg`；
  - 使用 `gettimeofday` 计算总耗时并打印。

#### `filecopy_1024.c`

- 功能：使用 **1024 字节缓冲区** 拷贝文件，并统计耗时。
- 用法：
  - `./filecopy_1024 <srcfile> <dstfile>`
- 行为：
  - 循环按 1024 字节 `read` + `write`，结束后打印时间。

#### `filecopy_thread.c`

- 功能：使用 **多线程 + POSIX 信号量** 实现“生产者-消费者”模型的文件拷贝，并统计耗时。
- 用法：
  - `./filecopy_thread <srcfile> <dstfile>`
- 结构：
  - 线程 1：负责从源文件读取数据到缓冲区；
  - 线程 2：负责将缓冲区数据写入目标文件；
  - 使用两个信号量（`sem1`、`sem2`）在读写线程之间进行同步。

---

### 2. 共享内存 + System V 信号量（进程间通信）

#### `filecopy_share_memory_write.c`

- 角色：**写进程 / 生产者**。
- 功能：
  - 通过共享内存向另一个进程发送递增计数（0~99），最后发送字符串 `"end"` 作为结束标记；
  - 使用两个 System V 信号量进行进程间同步，避免竞争条件。

#### `filecopy_share_memory_read.c`

- 角色：**读进程 / 消费者**。
- 功能：
  - 从共享内存读取写进程发送的计数值并打印；
  - 读到 `"end"` 后删除共享内存并退出。

> 两个程序需要配合运行，可在两个终端分别启动写进程和读进程，演示共享内存 + 信号量的基本使用。

---

### 3. TCP 套接字网络编程

#### `tcp_server.c`

- 功能：简单的 **TCP 回显服务器（Echo Server）**。
- 特点：
  - 监听 `0.0.0.0:8000`；
  - 接受一个客户端连接后，发送欢迎信息；
  - 持续接收客户端发送的字符串并原样回发（回显）。

#### `tcp_client.c`

- 功能：TCP 客户端，连接本机 `127.0.0.1:8000`，与服务器交互。
- 行为：
  - 连接成功后接收并打印服务器欢迎信息；
  - 循环从标准输入读取字符串发送给服务器，并打印服务器返回的数据；
  - 输入 `"quit"` 时退出。

> 适合作为 TCP 客户端/服务器编程最小示例，用于理解 `socket` / `bind` / `listen` / `accept` / `connect` / `send` / `recv` 等调用流程。

---

### 4. UDP 套接字网络编程

#### `udp_server.c`

- 功能：UDP 服务器，接收并打印来自客户端的数据报，收到 `"stop"` 后退出。
- 特点：
  - 监听端口 `8000`；
  - 使用 `recvfrom` 接收数据，无需建立连接。

#### `udp_client.c`

- 功能：UDP 客户端，向服务器连续发送若干“数据包”字符串，最后发送 `"stop"` 结束。
- 行为：
  - 目标地址为 `127.0.0.1:8000`；
  - 通过 `sendto` 周期性发送 `"data packet with ID <n>"`，最后发送 `"stop"`。

> 适合作为对比 TCP/UDP 使用方式差异的示例。

---

## 编译与运行示例

以下以单文件直接编译为例，你也可以根据需要编写 `Makefile` 统一管理。

### 1. 文件拷贝示例

```bash
# 1 字节缓冲拷贝（需要当前目录存在 poster.jpg）
gcc -o filecopy filecopy.c
./filecopy

# 1024 字节缓冲拷贝
gcc -o filecopy_1024 filecopy_1024.c
./filecopy_1024 src.bin dst.bin

# 多线程文件拷贝
gcc -o filecopy_thread filecopy_thread.c -lpthread
./filecopy_thread src.bin dst.bin
```

### 2. 共享内存读写

```bash
# 写进程
gcc -o shm_write filecopy_share_memory_write.c

# 读进程
gcc -o shm_read filecopy_share_memory_read.c

# 分别在两个终端运行
./shm_write
./shm_read
```

### 3. TCP 通信示例

```bash
# 编译
gcc -o tcp_server tcp_server.c
gcc -o tcp_client tcp_client.c

# 先启动服务器
./tcp_server

# 再启动客户端（可在另一个终端）
./tcp_client
```

### 4. UDP 通信示例

```bash
# 编译
gcc -o udp_server udp_server.c
gcc -o udp_client udp_client.c

# 先启动服务器
./udp_server

# 再启动客户端
./udp_client
```

---

## 参考与扩展

- 你可以在此基础上：
  - 为 TCP 服务器增加多客户端支持（如 `fork` / 多线程）；
  - 扩展 UDP 示例，增加更复杂的数据格式和超时处理；
  - 编写统一的 `Makefile` 一键编译全部示例；
  - 比较不同文件拷贝方式在大文件上的性能差异。

本仓库适合用于课程实验、个人练习或作为更大项目的示例基础。
