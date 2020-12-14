# Shop

>其实也不是很大工程，就分为三个部分:增删商品/查商品/数据库保存/指令系统

* 整个程序的入口以一个 for 循环阻塞接受命令。

**插入商品**

1.插入一个商品时，先查找商品是否在数据库，再插入

```c
// 通过名字检索
> Add '大米' 2,'花生油' 3;

// 新增商品
> New '青菜' 5;
```

**删除商品**

1.商品--

```c
// 通过名字检索
Del '大米' 5;
```

2.从链表扣除,这个打算做成定时任务

**查找商品**

1.查找商品是否在仓库，后返回

```c
>Select '大米';
```

2.排序功能

```c
>Search all desc;

// ...

>Search all asc;
```

3.列出商品

```c
>Search all;
```

**数据库保存**

1.提供 save 功能，也有定时任务。

```c
>Save;
```

**指令系统**

```c
// 增/删/查/改
add 'case' nums,'case' nums;
new 'case' nums,'case' nums;
del 'case' nums,'case' nums;
select 'case','case' [desc/asc];
search all [desc/asc];

// 内存-文件交换
save;    // 更新到文件中，有定时任务
read;    // 文件读取刷新内存数据库

// 用户
sudo; // 获得管理员权限
exit;  // 退出管理员模式/退出程序
```