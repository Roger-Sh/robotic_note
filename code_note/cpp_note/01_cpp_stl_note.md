# CPP_STL_Note



## STL 基本概念

- STL - Standard Template Library
- STL 分为六大组件
  - 容器, 算法, 迭代器, 仿函数, 适配器, 空间配置器
  - 容器，container
    - 序列式容器
      - 强调排序， 每个元素有固定位置
    - 关联式容器
      - 强调关系， 没有严格的顺序
    - 常用数据结构
      - vector 向量
      - list 列表
      - tree 树
      - stack 栈
      - deque 双端数组
      - queue 队列
      - set 集合
      - map 
      - ...
  - 算法，algorithm
    - 质变算法
      - 运算中会更改元素内容， 拷贝， 替换， 删除等
    - 非质变算法
      - 查找， 计数，遍历，寻找极值等
    - 常见算法
      - sort
      - find
      - copy
      - for_each
  - 迭代器，iterator
    - 容器和算法之间的胶合器
    - 依序寻访某个容器所含的各个元素，而又无需暴露该容器的内部表示方式
    - 迭代器种类
        - 输入迭代器：对数据的只读访问
        - 输出迭代器：对数据的只写访问
        - 前向迭代器：读写操作，并能向前推进迭代器
        - 双向迭代器：读写操作，并能向前和向后操作
        - 随机访问迭代器：读写操作，可以以跳跃的方式访问任意数据，功能最强的迭代器
  - 仿函数
    - 行为类似函数， 算法的策略
  - 适配器
    - 修饰容器或仿函数或迭代器接口的东西
  - 空间配置器
    - 空间的配置与管理




## STL 容器

### String 字符串

#### string 构造函数

```c++
// 创建空的字符串
std::string str;	

// 使用字符串s初始化
const char* str = "hello world";
std::string s2(str); 

// 拷贝初始化
std::string s3(s2); 

// 使用n个字符c初始化
std::string s4(10, 'a');

```

#### string 赋值 =

```c++
// operator =
string& operator=(const char* s);		  // char* 字符串
string& operator=(const string &s);		// string 字符串
string& operator=(char c);						// char 字符

// method assign()
string& assign(const char *s);				// char* 字符串
string& assign(const char *s, int n);  // 字符串s前n个字符
string& assign(const string &s);	// 字符串s
string& assign(int n, char c);	// n个字符c
```

#### string 拼接 +=

```c++
// operator +=
string& operator+=(const char* str);
string& operator+=(const char c);
string& operator+=(const string& str);

// method append()
string& append(const char *s);
string& append(const char *s, int n);
string& append(const string &s);
string& append(const string &s, int pos, int n);
```

#### stirng 查找与替换 find replace

```c++
// method find() 查找 *****************************
//查找str第一次出现位置,从pos开始查找
int find(const string& str, int pos = 0) const;
 //查找s第一次出现位置,从pos开始查找
int find(const char* s, int pos = 0) const;
//从pos位置查找s的前n个字符第一次位置
int find(const char* s, int pos, int n) const;
 //查找字符c第一次出现位置
int find(const char c, int pos = 0) const; 

// method rfind()  反向查找****************************
 //查找str最后一次位置,从pos开始查找
int rfind(const string& str, int pos = npos) const;
//查找s最后一次出现位置,从pos开始查找
int rfind(const char* s, int pos = npos) const; 
//从pos查找s的前n个字符最后一次位置
int rfind(const char* s, int pos, int n) const;     
//查找字符c最后一次出现位置
int rfind(const char c, int pos = 0) const;     

// method replace() 替换 ******************************
//替换从pos开始n个字符为字符串str
string& replace(int pos, int n, const string& str); 
//替换从pos开始的n个字符为字符串s
string& replace(int pos, int n,const char* s);             

```

#### string 比较 compare

```c++
// method compare()
// return 等于0，大于1， 小于-1
int compare(const string &s) const;      //与字符串s比较
int compare(const char *s) const;     //与字符串s比较
```

#### string 字符存取 [] at

```c++
char& operator[](int n);     //通过[]方式取字符
char& at(int n);                     //通过at方法获取字符
```

#### string 插入和删除 insert erase

```c++
string& insert(int pos, const char* s);                 //插入字符串
string& insert(int pos, const string& str);         //插入字符串
string& insert(int pos, int n, char c);                //在指定位置插入n个字符c
string& erase(int pos, int n = npos);                    //删除从Pos开始的n个字符 
```

#### string 子串 substr

```c++
string substr(int pos = 0, int n = npos) const;   //返回由pos开始的n个字符组成的字符串
```



### Vector 向量, 动态数组

#### Vector 构造

```c++
vector<T> v;                		     //采用模板实现类实现，默认构造函数
vector(v.begin(), v.end());       //将v[begin(), end())区间中的元素拷贝给本身。
vector(n, elem);                            //构造函数将n个elem拷贝给本身。
vector(const vector &vec);         //拷贝构造函数。
```

#### Vector 赋值 = , (beg, end), (n, elem)

```c++
vector& operator=(const vector &vec);	//重载等号操作符
assign(beg, end);       //将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);        //将n个elem拷贝赋值给本身。
```

#### Vector 容量与大小 empty, capacity, size, resize

```c++
//判断容器是否为空
empty();                      

//容器的容量
capacity();                   

 //返回容器中元素的个数
size();                             

//重新指定容器的长度为num，若容器变长，则以默认值填充新位置。
//如果容器变短，则末尾超出容器长度的元素被删除。
resize(int num);             

//重新指定容器的长度为num，若容器变长，则以elem值填充新位置。
 //如果容器变短，则末尾超出容器长度的元素被删除
resize(int num, elem);`  
```

#### Vector 插入与删除 push_back, pop_back, insert, erase, clear

```c++
push_back(ele);                                         //尾部插入元素ele
pop_back();                                                //删除最后一个元素
insert(const_iterator pos, ele);        //迭代器指向位置pos插入元素ele
insert(const_iterator pos, int count,ele);//迭代器指向位置pos插入count个元素ele
erase(const_iterator pos);                     //删除迭代器指向的元素
erase(const_iterator start, const_iterator end);//删除迭代器从start到end之间的元素
clear();                                                        //删除容器中所有元素
```

#### Vector 数据获取 at, [], front, back

```c++
at(int idx);     //返回索引idx所指的数据
operator[];       //返回索引idx所指的数据
front();            //返回容器中第一个数据元素
back();              //返回容器中最后一个数据元素
```

#### Vector 互换容器 swap

```c++
swap(vec);  // 将vec与本身的元素互换
```

#### Vector 预留空间 reserve

```c++
reserve(int len); //容器预留len个元素长度，预留位置不初始化，元素不可访问。
```



### Deque 双端数组

#### Deque 特点

- 双端数组，可以对头端进行插入删除操作
- Deque 与 Vector 对比
  - vector对于头部的插入删除效率低，数据量越大，效率越低
  - deque相对而言，对头部的插入删除速度回比vector快
  - vector访问元素时的速度会比deque快,这和两者内部实现有关
  - deque容器的迭代器也是支持随机访问的
- Deque 原理
  - deque内部有个**中控器**，维护每段缓冲区中的内容，缓冲区中存放真实数据，中控器维护的是每个缓冲区的地址，使得使用deque时像一片连续的内存空间

#### Deque 构造

```c++
deque<T> deqT;                      //默认构造形式
deque(beg, end);                  //构造函数将[beg, end)区间中的元素拷贝给本身。
deque(n, elem);                    //构造函数将n个elem拷贝给本身。
deque(const deque &deq);   //拷贝构造函数
```

#### Deque 赋值 =, (beg, end), (n. elem)

```c++
deque& operator=(const deque &deq);          //重载等号操作符
assign(beg, end);                                           //将[beg, end)区间中的数据拷贝赋值给本身。
assign(n, elem);                                             //将n个elem拷贝赋值给本身。
```

#### Deque 大小 empty, size, resize

```c++
//判断容器是否为空
deque.empty();                     

//返回容器中元素的个数
deque.size();                        

//重新指定容器的长度为num,若容器变长，则以默认值填充新位置。
//如果容器变短，则末尾超出容器长度的元素被删除。
deque.resize(num);               

//重新指定容器的长度为num,若容器变长，则以elem值填充新位置。
//如果容器变短，则末尾超出容器长度的元素被删除。
deque.resize(num, elem);     
```

#### Deque 插入与删除 push_back, push_front, pop_back, pop_front

- 两端插入操作

  ```c++
  push_back(elem);          //在容器尾部添加一个数据
  push_front(elem);        //在容器头部插入一个数据
  pop_back();                   //删除容器最后一个数据
  pop_front();                 //删除容器第一个数据
  ```

- 指定位置操作

  ```c++
  //在pos位置插入一个elem元素的拷贝，返回新数据的位置。
  insert(pos,elem);         
  //在pos位置插入n个elem数据，无返回值。
  insert(pos,n,elem);     
   //在pos位置插入[beg,end)区间的数据，无返回值。
  insert(pos,beg,end);   
   //清空容器的所有数据
  clear();                          
   //删除[beg,end)区间的数据，返回下一个数据的位置。
  erase(beg,end);            
   //删除pos位置的数据，返回下一个数据的位置。
  erase(pos);                   
  ```


#### Deque 数据获取 at, [], front, back

```c++
at(int idx);     	//返回索引idx所指的数据
operator[];      	//返回索引idx所指的数据
front();    		//返回容器中第一个数据元素
back();        		//返回容器中最后一个数据元素
```

#### Deque 排序 sort

```c++
sort(iterator beg, iterator end)  //对beg和end区间内元素进行排序
```



### Stack 栈 deque, vector

#### Stack 特点

-   stack是一种**先进后出**(First In Last Out,FILO)的数据结构，它只有一个出口


-   栈中只有顶端的元素才可以被外界使用，因此栈不允许有遍历行为


-   栈中进入数据称为  --- **入栈**  `push`


- 栈中弹出数据称为  --- **出栈**  `pop`

- 默认情况下, `std::stack` 使用 `std::deque` 作为底层容器来实现。但也可以选择使用 `std::vector` 或 `std::list` 作为底层容器。

  ```cpp
  std::stack<int> s;  // 使用 std::deque 作为底层容器
  std::stack<int, std::vector<int>> s2;  // 使用 std::vector 作为底层容器
  ```

  


#### Stack 构造

```c++
// 默认构造函数
stack<T> stk;

// 拷贝构造函数
stack(const stack &stk);
```

#### Stack 赋值 = 

```c++
stack& operator=(const stack &stk);
```

#### Stack 存取 push, pop, top

```c++
// 栈顶添加元素
stk.push(element);

// 栈顶移除元素
stk.pop();

// 返回栈顶元素
stk.top();
```

#### Stack 大小 empty size

```c++
stk.empty();
stk.size();
```

 

### Queue 队列 deque, list

#### Queue 特点

-   Queue是一种**先进先出**(First In First Out,FIFO)的数据结构，它有两个出口


-   队列容器允许从一端新增元素，从另一端移除元素


-   队列中只有队头和队尾才可以被外界使用，因此队列不允许有遍历行为


-   队列中进数据称为 --- **入队**    `push`


- 队列中出数据称为 --- **出队**    `pop`

- 默认情况下, `std::queue` 使用 `std::deque` 作为底层容器来实现。但也可以选择使用 `std::list` 作为底层容器。

  ```cpp
  std::queue<int> q;  // 使用 std::deque 作为底层容器
  std::queue<int, std::list<int>> q2;  // 使用 std::list 作为底层容器
  ```

  


#### Queue 构造

```c++
// 默认构造
queue<T> que;

// 拷贝构造
queue(const queue &que);
```

#### Queue 赋值 = 

```c++
queue& operator=(const queue &que);
```

#### Queue 存取 push, pop, back, front

```c++
// 队尾添加元素
que.push(elem);

// 队头移除元素
que.pop();

// 获取队尾元素
que.back();

// 获取队头元素
que.front();
```

#### Queue 大小 empty, size

```c++
que.empty();
que.size();
```



### List 链表

#### List 特点

-   **功能：**将数据进行链式存储

-   **链表**（list）是一种物理存储单元上非连续的存储结构，数据元素的逻辑顺序是通过链表中的指针链接实现的
    -   链表的组成：链表由一系列**结点**组成
    
-   结点的组成：一个是存储数据元素的**数据域**，另一个是存储下一个结点地址的**指针域**

-   STL中的链表是一个**双向循环链表**

-   List有一个重要的性质，插入操作和删除操作都不会造成原有list迭代器的失效，这在vector是不成立的。

-   list的优点：

    * 采用动态存储分配，不会造成内存浪费和溢出

    * 链表执行插入和删除操作十分方便，修改指针即可，不需要移动大量元素


-   list的缺点：

    * 链表灵活，但是空间(指针域) 和 时间（遍历）额外耗费较大

#### List 构造

```c++
// list采用采用模板类实现,对象的默认构造形式
list<T> lst;

// 构造函数将[beg, end)区间中的元素拷贝给本身
list(beg,end);

// 构造函数将n个elem拷贝给本身
list(n,elem);

// 拷贝构造函数。
list(const list &lst);
```

#### List 赋值和交换 assign, =, swap

```c++
// 将[beg, end)区间中的数据拷贝赋值给本身。
assign(beg, end);

// 将n个elem拷贝赋值给本身。
assign(n, elem);

// 重载等号操作符
list& operator=(const list &lst);

// 将lst与本身的元素互换。
swap(lst);
```

#### List 大小 size, empty, resize

```c++
// 返回容器中元素的个数
size();

// 判断容器是否为空
empty();

// 重新指定容器的长度为num，若容器变长，则以默认值填充新位置。
// 如果容器变短，则末尾超出容器长度的元素被删除。
resize(num);

// 重新指定容器的长度为num，若容器变长，则以elem值填充新位置。
// 如果容器变短，则末尾超出容器长度的元素被删除。
resize(num, elem);

```

#### List 插入和删除 push, pop, insert, erase, clear, remove

```c++
// 在容器尾部加入一个元素
push_back(elem);

// 删除容器中最后一个元素
pop_back();

// 在容器开头插入一个元素
push_front(elem);

// 从容器开头移除第一个元素
pop_front();

// 在pos位置插elem元素的拷贝，返回新数据的位置。
insert(pos,elem);

// 在pos位置插入n个elem数据，无返回值。
insert(pos,n,elem);

// 在pos位置插入[beg,end)区间的数据，无返回值。
insert(pos,beg,end);

// 移除容器的所有数据
clear();

// 删除[beg,end)区间的数据，返回下一个数据的位置。
erase(beg,end);

// 删除pos位置的数据，返回下一个数据的位置。
erase(pos);

// 删除容器中所有与elem值匹配的元素。
remove(elem);
```

#### List 数据存取 front, back

```c++
// 获取第一个元素
front();

// 获取最后一个元素
back();
```

#### List 反转和排序 reverse, sort

-   反转

```c++
// 反转链表
reverse();
```

-   排序

```c++
// 排序, 默认从小到达
// 可以指定排序规则
// 自定义数据类型必须指定排序规则
sort();
```

-   自定义排序

```c++
/**
自定义排序
**/
#include <list>
#include <string>
class Person 
{
public:
	Person(string name, int age , int height) 
    {
		m_Name = name;
		m_Age = age;
		m_Height = height;
	}

public:
	string m_Name;  //姓名
	int m_Age;      //年龄
	int m_Height;   //身高
};


bool ComparePerson(Person& p1, Person& p2) 
{
	if (p1.m_Age == p2.m_Age) 
    {
		return p1.m_Height  > p2.m_Height;
	}
	else
	{
		return  p1.m_Age < p2.m_Age;
	}

}

void test01() {

	list<Person> L;

	Person p1("刘备", 35 , 175);
	Person p2("曹操", 45 , 180);
	Person p3("孙权", 40 , 170);
	Person p4("赵云", 25 , 190);
	Person p5("张飞", 35 , 160);
	Person p6("关羽", 35 , 200);

	L.push_back(p1);
	L.push_back(p2);
	L.push_back(p3);
	L.push_back(p4);
	L.push_back(p5);
	L.push_back(p6);

	for (list<Person>::iterator it = L.begin(); it != L.end(); it++) 
    {
		cout << "姓名： " << it->m_Name << " 年龄： " << it->m_Age 
              << " 身高： " << it->m_Height << endl;
	}

	cout << "---------------------------------" << endl;
	L.sort(ComparePerson); //排序

	for (list<Person>::iterator it = L.begin(); it != L.end(); it++) 
    {
		cout << "姓名： " << it->m_Name << " 年龄： " << it->m_Age 
              << " 身高： " << it->m_Height << endl;
	}
}

int main() 
{
	test01();

	system("pause");

	return 0;
}
```



### Set/Multiset 有序集合 BiTree

#### Set 特点

-   set/multiset属于**关联式容器**，底层结构是用**二叉树**实现。

-   所有元素都会在插入时自动被排序

-   **set和multiset区别**：
* set不允许容器中有重复的元素
    
* multiset允许容器中有重复的元素


#### Set 构造与赋值

```c++
// 默认构造函数：
set<T> st;

// 拷贝构造函数
set(const set &st);

// 重载等号操作符
set& operator=(const set &st);
```

#### Set 大小与交换

```c++
// 返回容器中元素的数目
size();

// 判断容器是否为空
empty();

// 交换两个集合容器
swap(st);
```

#### Set 插入与删除

```c++
// 在容器中插入元素。
insert(elem);

// 清除所有元素
clear();

// 删除pos迭代器所指的元素，返回下一个元素的迭代器。
erase(pos);

// 删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
erase(beg, end);

// 删除容器中值为elem的元素。
erase(elem);
```

#### Set 查找与统计

```c++
// 查找key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
find(key);

// 统计key的元素个数
count(key);
```

#### Set 与Multiset 区别

* set不可以插入重复数据，而multiset可以
* set插入数据的同时会返回插入结果，表示插入是否成功
* multiset不会检测数据，因此可以插入重复数据

#### Set 排序 

-   set容器默认排序规则为从小到大
-   利用仿函数，可以改变排序规则

```c++
/**
set使用自定义排序规则
**/

#include <set>

class MyCompare 
{
public:
	bool operator()(int v1, int v2) 
    {
		return v1 > v2;
	}
};

void test01() 
{    
	set<int> s1;
	s1.insert(10);
	s1.insert(40);
	s1.insert(20);
	s1.insert(30);
	s1.insert(50);

	//默认从小到大
	for (set<int>::iterator it = s1.begin(); it != s1.end(); it++) 
    {
		cout << *it << " ";
	}
	cout << endl;

	//指定排序规则
	set<int,MyCompare> s2;
	s2.insert(10);
	s2.insert(40);
	s2.insert(20);
	s2.insert(30);
	s2.insert(50);

	for (set<int, MyCompare>::iterator it = s2.begin(); it != s2.end(); it++) 
    {
		cout << *it << " ";
	}
	cout << endl;
}

int main() 
{

	test01();

	system("pause");

	return 0;
}
```



```c++
/**
set使用自定义数据类型和自定义排序规则
**/

#include <set>
#include <string>

class Person
{
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}

	string m_Name;
	int m_Age;

};

class comparePerson
{
public:
	bool operator()(const Person& p1, const Person &p2)
	{
		//按照年龄进行排序  降序
		return p1.m_Age > p2.m_Age;
	}
};

void test01()
{
	set<Person, comparePerson> s;

	Person p1("刘备", 23);
	Person p2("关羽", 27);
	Person p3("张飞", 25);
	Person p4("赵云", 21);

	s.insert(p1);
	s.insert(p2);
	s.insert(p3);
	s.insert(p4);

	for (set<Person, comparePerson>::iterator it = s.begin(); it != s.end(); it++)
	{
		cout << "姓名： " << it->m_Name << " 年龄： " << it->m_Age << endl;
	}
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



### Unordered_set 无序集合 BiTree

#### Unordered_set 特点

-   unordered_set 容器，可直译为“无序 set 容器”。即 unordered_set 容器和 set 容器很像，唯一的区别就在于 set 容器会自行对存储的数据进行排序，而 unordered_set 容器不会。

-   不再以键值对的形式存储数据，而是直接存储数据的值 ；
-   容器内部存储的各个元素的值都互不相等，且不能被修改；
-   不会对内部存储的数据进行排序

#### Unordered_set 构造

```c++
// 创建空的set
unordered_set<int> set1;

// 拷贝构造
unordered_set<int> set2(set1);

// 使用迭代器构造
unordered_set<int> set3(set1.begin(), set1.end());

// 使用数组作为其初值进行构造
unordered_set<int> set4(arr,arr+5);

// 移动构造
unordered_set<int> set5(move(set2));

// 使用初始列表进行构造
unordered_set<int> set6 {1,2,10,10};
```

#### Unordered_set 判空, 查找, 计数, 清空

```c++
// 判断空
set1.empty();

// find()函数——查找
//查找2，找到返回迭代器，失败返回end()
set1.find(2);

// count()函数——出现次数
//返回指2出现的次数，0或1
set1.count(2);

// clear()函数——清空
set1.clear();
```

#### Unordered_set insert()/emplace()相关

```c++
// insert()函数——插入元素
//插入元素，返回pair<unordered_set<int>::iterator, bool>
set1.insert(3);
//使用initializer_list插入元素
set1.insert({1,2,3});
//指定插入位置，如果位置正确会减少插入时间，返回指向插入元素的迭代器
set1.insert(set1.end(), 4);
//使用范围迭代器插入
set1.insert(set2.begin(), set2.end());

// insert() 传入单个元素
// 会返回一个 pair 对象
// 这个 pair 对象包含一个迭代器，以及一个附加的布尔值用来说明插入是否成功
// 如果元素被插入，返回的迭代器会指向新元素
// 如果没有被插入，迭代器指向阻止插入的元素
auto pr = words.insert("ninety"); // Returns a pair - an iterator & a bool value

// insert()传入两个参数（迭代器+待插入元素）
// 可以用一个迭代器作为insert()的第一个参数，它指定了元素被插入的位置
// 在这种情况下，只会返回一个迭代器
auto iter = words.insert (pr.first, "nine"); // 1st arg is a hint. Returns an iterator

// insert()传入初始化列表
// 插入初始化表中的元素
// 在这种情况下，什么都没有返回
words.insert({"ten", "seven", "six"});  // Inserting an initializer list

// emplace()函数——插入元素(转移构造)
// 使用转移构造函数添加新元素3，比insert效率高
set1.emplace(3);
```

#### Unordered_set erase() 相关

```c++
// erase()函数——删除元素
// 删除操作，成功返回1，失败返回0
set1.erase(1);
// 删除操作，成功返回下一个pair的迭代器
set1.erase(set1.find(1));
// 删除set1的所有元素，返回指向end的迭代器
set1.erase(set1.begin(), set1.end());
```

#### Unordered_set bucket 相关

```c++
// 返回容器中的篮子总数
set1.bucket_count();

// 返回1号篮子中的元素数
set1.bucket_size(1);

// 元素1在哪一个篮子
set1.bucket(1);

// 负载因子，返回每个篮子元素的平均数，即size/float(bucket_count);
set1.load_factor();

// 设置篮子的数量为20，并且重新rehash
set1.rehash(20);
```



### Pair 对

#### Pair 特点

pair是将2个数据组合成一组数据，当需要这样的需求时就可以使用pair，如stl中的map就是将key和value放在一起来保存。另一个应用是，当一个函数需要返回2个数据的时候，可以选择pair。 pair的实现是一个结构体，主要的两个成员变量是first second 因为是使用struct不是class，所以可以直接使用pair的成员变量。

#### Pair 构造与初始化

```c++
//创建一个空的pair对象（使用默认构造），它的两个元素分别是T1和T2类型，采用值初始化。
pair<T1, T2> p1;

//创建一个pair对象，它的两个元素分别是T1和T2类型，其中first成员初始化为v1，second成员初始化为v2。
pair<T1, T2> p1(v1, v2);

// 以v1和v2的值创建一个新的pair对象，其元素类型分别是v1和v2的类型。
make_pair(v1, v2);

// pair的两个类型可以不一样
// 创建一个空对象anon，两个元素类型都是string
pair<string, string> anon;
// 创建一个空对象 word_count, 两个元素类型分别是string和int类型
pair<string, int> word_count;
// 创建一个空对象line，两个元素类型分别是string和vector类型
pair<string, vector<int> > line;

// 定义时进行成员初始化：
pair<string, string> author("James","Joy");。
pair<string, int> name_age("Tom", 18);
pair<string, int> name_age2(name_age);

// pair类型的使用相当的繁琐，如果定义多个相同的pair类型对象，可以使用typedef简化声明：
typedef pair<string,string> Author;
Author proust("March","Proust");
Author Joy("James","Joy");

// 变量间赋值
pair<int, double> p1(1, 1.2);
pair<int, double> p2 = p1;     
pair<int, double> p3；
p3 = p1;
```

#### Pair 操作

```c++
// 两个pair对象间的小于运算，其定义遵循字典次序：如 p1.first < p2.first 或者 !(p2.first < p1.first) && (p1.second < p2.second) 则返回true。
p1 < p2;

// 如果两个对象的first和second依次相等，则这两个对象相等；该运算使用元素的==操作符。
p1 == p2;

// 返回对象p1中名为first的公有数据成员
p1.first;

// 返回对象p1中名为second的公有数据成员
p1.second;
```

#### Pair 与 make_pair

```c++
pair<int, double> p1;
p1 = make_pair(1, 1.2);
```

#### 通过tie获取pair元素值

```c++
/**
在某些清况函数会以pair对象作为返回值时，可以直接通过std::tie进行接收
**/

std::pair<std::string, int> getPreson() 
{
    return std::make_pair("Sven", 25);
}
 
int main(int argc, char **argv) 
{
    std::string name;
    int ages;
 
    std::tie(name, ages) = getPreson();
 
    std::cout << "name: " << name << ", ages: " << ages << std::endl;
 
    return 0;
}
```



### Map/Multimap 有序映射 红黑树实现

#### Map 特点

-   map中所有元素都是pair
    -   map中所有元素都是成对出现，插入数据时候要使用对组
-   pair中第一个元素为key（键值），起到索引作用，第二个元素为value（实值）
    -   可以根据key值快速找到value值
-   所有元素都会根据元素的键值自动排序
    -   unordered_map 没有自动排序
-   map/multimap属于**关联式容器**，底层结构是用二叉树实现。
    -   map不允许容器中有重复key值元素
    -   multimap允许容器中有重复key值元素

#### Map 构造与赋值

```c++
// map默认构造函数
map<T1, T2> mp; 

// 拷贝构造函数
map(const map &mp);

// 重载等号操作符
map& operator=(const map &mp);
```

#### Map 大小与交换

```c++
// 返回容器中元素的数目
map.size();

// 判断容器是否为空
map.empty();

// 交换两个集合容器
map.swap(st);
```

#### Map 插入与删除

```c++
// 在容器中插入元素。
//第一种插入方式
m.insert(pair<int, int>(1, 10));
//第二种插入方式
m.insert(make_pair(2, 20));
//第三种插入方式
m.insert(map<int, int>::value_type(3, 30));
//第四种插入方式
m[4] = 40; 

// 清除所有元素
map.clear();

// 删除pos迭代器所指的元素，返回下一个元素的迭代器。
map.erase(pos);

// 删除区间[beg,end)的所有元素 ，返回下一个元素的迭代器。
map.erase(beg, end);

// 删除容器中值为key的元素。
map.erase(key);
```

#### Map 查找与统计

```c++
// 查找key是否存在,若存在，返回该键的元素的迭代器；若不存在，返回set.end();
find(key);
map<int, int>::iterator pos = m.find(3);
if (pos != m.end())
    cout << "找到了元素 key = " << (*pos).first << " value = " << (*pos).second << endl;
else
    cout << "未找到元素" << endl;
// 统计key的元素个数
count(key);
```

#### Map 排序

-   map容器默认排序规则为 按照key值进行 从小到大排序
-   利用仿函数，可以改变排序规则

```c++
#include <map>

class MyCompare {
public:
	bool operator()(int v1, int v2) {
		return v1 > v2;
	}
};

void test01() 
{
	//默认从小到大排序
	//利用仿函数实现从大到小排序
	map<int, int, MyCompare> m;

	m.insert(make_pair(1, 10));
	m.insert(make_pair(2, 20));
	m.insert(make_pair(3, 30));
	m.insert(make_pair(4, 40));
	m.insert(make_pair(5, 50));

	for (map<int, int, MyCompare>::iterator it = m.begin(); it != m.end(); it++) {
		cout << "key:" << it->first << " value:" << it->second << endl;
	}
}
int main() {

	test01();

	system("pause");

	return 0;
}
```



### Unordered_map 无序映射 哈希表

#### Unordered_map 特点

-   unordered_map是一种关联容器，存储基于键值和映射组成的元素，即key-value。允许基于键快速查找元素。在unordered_map中，键值唯一标识元素，映射的值是一个与该对象关联的内容的对象。
-   Unordered_map 与 map 的对比
    -   unordered_map的无序体现在内部存储结构为**哈希表**，以便通过键值快速访问元素。
    -   与之对应的有序的关联容器为map，map的有序体现在内部存储结构为**红黑树**，存储时元素自动按照从小到大的顺序排列。
-   查找的效率
    -   unordered_map查找效率更高，可以达到`O(1)`，但是对于元素子集的范围迭代效率较低。
    -   对于map，按照中序遍历的遍历次序，能够方便迭代得出从小到大的元素
-   属性
    -   关联性
        -   关联容器中的元素由他们的键引用，而不是由他们在容器中的绝对位置引用。
    -   无序性
        -   无序容器使用**散列表**来组织它们的元素，散列表允许通过它们的键快速访问元素。
    -   Map映射
        -   每个元素将一个键key与一个映射值value相关联：键意味着标识其主要内容是映射值的元素。
    -   key的唯一性
        -   在容器中没有两个元素有相同的key
    -   allocator-aware
        -   分配器对象来动态地处理它的存储需求

#### Unordered_map 构造与初始化

```c++
// 默认构造函数
unordered_map<int, string> umap;

// 使用初始化列表初始化
// 显式调用C++的构造函数
unordered_map<int, string> umap = unordered_map<int, string>({{1,"a"},{2,"b"}});  
// 隐式调用构造函数，更简洁
unordered_map<int, string> umap2({{3,"c"},{4,"d"}});	
unordered_map<string, string> umap{
    {"淘宝","https://www.taobao.com/"},
    {"京东","https://www.jd.com/"},
    {"天猫商城","https://jx.tmall.com/"} };

// 拷贝构造函数 
unordered_map<int, string> umap4(umap3);

// 通过迭代器构造初始化
unordered_map<int, string> umap5(umap1.begin(), umap1.end());

// 拷贝初始化
typedef std::unordered_map<std::string,std::string> stringmap;
stringmap first = {{"AAPL","Apple"},{"MSFT","Microsoft"}};  // init list
stringmap second = {{"GOOG","Google"},{"ORCL","Oracle"}};   // init list
third = merge(first,second);                      // move
first = third;                                    // copy
```

#### Unordered_map 大小与判空

```C++
// 判空
umap.empty();

// 大小
umap.size();
```

#### Unordered_map 访问

```c++
// operator[]
first["GOOG"] = "Google";		// new element inserted
string brand1 = first["GOOG"];	// read

// at
unrdered_map<string,int> mymap = {
    {"Mars", 3000},
    {"Saturn", 60000},
    {"Jupiter", 70000}};
mymap.at("Mars") = 3396;
mymap.at("Saturn") += 127;
mymap.at("Jupiter") = mymap.at("Saturn") + 9638;
```

#### Unordered_map find 与 count

-   find

```c++
// find
iterator find ( const key_type& k );
const_iterator find ( const key_type& k ) const;

// example
std::unordered_map<std::string,double> mymap1 = {
	    {"mom",5.4},
	    {"dad",6.1},
	    {"bro",5.9} };
string person = "dad";
// iterator
// unordered_map<string,double>::const_iterator
unordered_map<std::string,double>::const_iterator got = mymap1.find(person);
if(got == mymap1.end()){
    cout << "not found" << endl;
}else{
    cout << got->first << " is " << got->second << endl; 
}

```

-   count

```c++
// count
size_type count ( const key_type& k ) const;
```

#### Unordered_map 修改

-   插入
    -   operator[]
    -   emplace()
    -   insert()

```c++
// 通过operator[]插入
map_name [key_name] = value
    
// 通过emplace()插入
std::unordered_map<std::string,std::string> mymap;
mymap.emplace ("NCC-1701", "J.T. Kirk");
mymap.emplace ("NCC-1701-D", "J.L. Picard");
mymap.emplace ("NCC-74656", "K. Janeway");

// 通过insert()插入
unordered_map<string,double> myrecipe, mypantry = {{"milk",2.0},{"flour",1.5}};;					
pair<string,double> myshopping ("baking powder",0.3);
// copy insertion
myrecipe.insert(myshopping);
// range inseration
myrecipe.insert(mypantry.begin(), mypantry.end());
// initializer list inseration
myrecipe.insert({{"sugar",0.8},{"salt",0.1},{"sugar",0.9}});		
```

-   删除 erase()
    -   by key
    -   by position
    -   by range

```c++
// create map
std::unordered_map<std::string,std::string> mymap3;
mymap3["U.S."] = "Washington";
mymap3["U.K."] = "London";
mymap3["France"] = "Paris";
mymap3["Russia"] = "Moscow";
mymap3["China"] = "Beijing";
mymap3["Germany"] = "Berlin";
mymap3["Japan"] = "Tokyo";

// 根据位置删除
mymap3.erase(mymap3.begin());
// 根据键值删除
mymap3.erase("France");
// 根据范围删除
mymap3.erase(mymap3.find("Germany"), mymap3.end());
```



## STL 迭代器

### for 循环 vs for_each

| for 循环                                          | for_each       |
| ------------------------------------------------- | -------------- |
| 循环内可以修改迭代器的值，导致迭代器可能跳过end() | 没有这个问题   |
| 无法并行计算                                      | 可以并行计算   |
| 推荐配合索引+size()                               | 推荐配合迭代器 |



### 迭代 vector

### 迭代 List

### 迭代 Stack

### 迭代 Queue

### 迭代 Set

### 迭代 Unordered_set

### 迭代 map

### 迭代 unordered_map

## STL 函数对象(仿函数)

### 函数对象概念

-   重载**函数调用操作符**的类，其对象常称为**函数对象**
-   **函数对象**使用重载的()时，行为类似函数调用，也叫**仿函数**
    -   函数对象(仿函数)是一个**类**，不是一个函数
-   函数对象特点
    -   函数对象在使用时，可以像普通函数那样调用, 可以有参数，可以有返回值
    -   函数对象超出普通函数的概念，函数对象可以有自己的状态
    -   函数对象可以作为参数传递

```c++
#include <string>

//1、函数对象在使用时，可以像普通函数那样调用, 可以有参数，可以有返回值
class MyAdd
{
public :
	int operator()(int v1,int v2)
	{
		return v1 + v2;
	}
};

void test01()
{
	MyAdd myAdd;
	cout << myAdd(10, 10) << endl;
}

//2、函数对象可以有自己的状态
class MyPrint
{
public:
	MyPrint()
	{
		count = 0;
	}
	void operator()(string test)
	{
		cout << test << endl;
		count++; //统计使用次数
	}

	int count; //内部自己的状态
};
void test02()
{
	MyPrint myPrint;
	myPrint("hello world");
	myPrint("hello world");
	myPrint("hello world");
	cout << "myPrint调用次数为： " << myPrint.count << endl;
}

//3、函数对象可以作为参数传递
void doPrint(MyPrint &mp , string test)
{
	mp(test);
}

void test03()
{
	MyPrint myPrint;
	doPrint(myPrint, "Hello C++");
}

int main() {

	//test01();
	//test02();
	test03();

	system("pause");

	return 0;
}
```



### 函数对象-谓词

* 返回bool类型的仿函数称为**谓词**

* 如果operator()接受一个参数，那么叫做**一元谓词**

    * ```c++
        #include <vector>
        #include <algorithm>
        
        //1.一元谓词
        struct GreaterFive{
        	bool operator()(int val) {
        		return val > 5;
        	}
        };
        
        void test01() {
        
        	vector<int> v;
        	for (int i = 0; i < 10; i++)
        	{
        		v.push_back(i);
        	}
        
        	vector<int>::iterator it = find_if(v.begin(), v.end(), GreaterFive());
        	if (it == v.end()) {
        		cout << "没找到!" << endl;
        	}
        	else {
        		cout << "找到:" << *it << endl;
        	}
        
        }
        
        int main() {
        
        	test01();
        
        	system("pause");
        
        	return 0;
        }
        ```

        

* 如果operator()接受两个参数，那么叫做**二元谓词**

    * ```c++
        #include <vector>
        #include <algorithm>
        //二元谓词
        class MyCompare
        {
        public:
        	bool operator()(int num1, int num2)
        	{
        		return num1 > num2;
        	}
        };
        
        void test01()
        {
        	vector<int> v;
        	v.push_back(10);
        	v.push_back(40);
        	v.push_back(20);
        	v.push_back(30);
        	v.push_back(50);
        
        	//默认从小到大
        	sort(v.begin(), v.end());
        	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
        	{
        		cout << *it << " ";
        	}
        	cout << endl;
        	cout << "----------------------------" << endl;
        
        	//使用函数对象改变算法策略，排序从大到小
        	sort(v.begin(), v.end(), MyCompare());
        	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
        	{
        		cout << *it << " ";
        	}
        	cout << endl;
        }
        
        int main() {
        
        	test01();
        
        	system("pause");
        
        	return 0;
        }
        ```

        

### STL 内建函数对象

-   STL内建了一些函数对象,这些仿函数所产生的对象，用法和一般函数完全相同
    -   算术仿函数
    -   关系仿函数

    -   逻辑仿函数
-   使用内建函数对象，需要引入头文件 `#include<functional>`

-   算术仿函数

    -   `template<class T> T plus<T>`                //加法仿函数

    -   `template<class T> T minus<T>`              //减法仿函数

    -   `template<class T> T multiplies<T>`    //乘法仿函数

    -   `template<class T> T divides<T>`         //除法仿函数

    -   `template<class T> T modulus<T>`         //取模仿函数

    -   `template<class T> T negate<T>`           //取反仿函数

    -   ```c++
        /**
        example
        **/
            
        #include <functional>
        //negate
        void test01()
        {
        	negate<int> n;
        	cout << n(50) << endl;
        }
        
        //plus
        void test02()
        {
        	plus<int> p;
        	cout << p(10, 20) << endl;
        }
        
        int main() {
        
        	test01();
        	test02();
        
        	system("pause");
        
        	return 0;
        }
        ```

        

-   关系仿函数

    -   `template<class T> bool equal_to<T>`                    //等于

    * `template<class T> bool not_equal_to<T>`            //不等于
    * `template<class T> bool greater<T>`                      //大于
    * `template<class T> bool greater_equal<T>`          //大于等于
    * `template<class T> bool less<T>`                           //小于
    -   `template<class T> bool less_equal<T>`               //小于等于

    -   ```c++
        #include <functional>
        #include <vector>
        #include <algorithm>
        
        class MyCompare
        {
        public:
        	bool operator()(int v1,int v2)
        	{
        		return v1 > v2;
        	}
        };
        void test01()
        {
        	vector<int> v;
        
        	v.push_back(10);
        	v.push_back(30);
        	v.push_back(50);
        	v.push_back(40);
        	v.push_back(20);
        
        	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        		cout << *it << " ";
        	}
        	cout << endl;
        
        	//自己实现仿函数
        	//sort(v.begin(), v.end(), MyCompare());
        	//STL内建仿函数  大于仿函数
        	sort(v.begin(), v.end(), greater<int>());
        
        	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        		cout << *it << " ";
        	}
        	cout << endl;
        }
        
        int main() {
        
        	test01();
        
        	system("pause");
        
        	return 0;
        }
        ```

        

-   逻辑仿函数

    -   `template<class T> bool logical_and<T>`              //逻辑与

    -   `template<class T> bool logical_or<T>`                //逻辑或

    -   `template<class T> bool logical_not<T>`              //逻辑非

    -   ```c++
        #include <vector>
        #include <functional>
        #include <algorithm>
        void test01()
        {
        	vector<bool> v;
        	v.push_back(true);
        	v.push_back(false);
        	v.push_back(true);
        	v.push_back(false);
        
        	for (vector<bool>::iterator it = v.begin();it!= v.end();it++)
        	{
        		cout << *it << " ";
        	}
        	cout << endl;
        
        	//逻辑非  将v容器搬运到v2中，并执行逻辑非运算
        	vector<bool> v2;
        	v2.resize(v.size());
        	transform(v.begin(), v.end(),  v2.begin(), logical_not<bool>());
        	for (vector<bool>::iterator it = v2.begin(); it != v2.end(); it++)
        	{
        		cout << *it << " ";
        	}
        	cout << endl;
        }
        
        int main() {
        
        	test01();
        
        	system("pause");
        
        	return 0;
        }
        ```

        

## STL 常用算法

* 算法主要是由头文件`<algorithm>` `<functional>` `<numeric>`组成。

* `<algorithm>`是所有STL头文件中最大的一个，范围涉及到比较、 交换、查找、遍历操作、复制、修改等等
* `<numeric>`体积很小，只包括几个在序列上面进行简单数学运算的模板函数
* `<functional>`定义了一些模板类,用以声明函数对象。





### STL 常用遍历算法

#### `for_each`     遍历容器

* 实现遍历容器
* for_each(iterator beg, iterator end, _func);
    * // 遍历算法 遍历容器元素
    * // beg 开始迭代器
    * // end 结束迭代器
    * // _func 函数或者函数对象

```c++
#include <algorithm>
#include <vector>

//普通函数
void print01(int val) 
{
	cout << val << " ";
}
//函数对象
class print02 
{
 public:
	void operator()(int val) 
	{
		cout << val << " ";
	}
};

//for_each算法基本用法
void test01() {

	vector<int> v;
	for (int i = 0; i < 10; i++) 
	{
		v.push_back(i);
	}

	//遍历算法
	for_each(v.begin(), v.end(), print01);
	cout << endl;

	for_each(v.begin(), v.end(), print02());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### `transform`   搬运容器到另一个容器中

* 搬运容器到另一个容器中
* `transform(iterator beg1, iterator end1, iterator beg2, _func);`
    * //beg1 源容器开始迭代器
    * //end1 源容器结束迭代器
    * //beg2 目标容器开始迭代器
    * //_func 函数或者函数对象

```c++
#include<vector>
#include<algorithm>

//常用遍历算法  搬运 transform

class TransForm
{
public:
	int operator()(int val)
	{
		return val;
	}

};

class MyPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int>v;
	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}

	vector<int>vTarget; //目标容器

	vTarget.resize(v.size()); // 目标容器需要提前开辟空间

	transform(v.begin(), v.end(), vTarget.begin(), TransForm());

	for_each(vTarget.begin(), vTarget.end(), MyPrint());
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



### STL 常用查找算法

-   常用查找算法
    -   `find`                     //查找元素
    -   `find_if`               //按条件查找元素
    -   `adjacent_find`    //查找相邻重复元素
    -   `binary_search`    //二分查找法
    -   `count`                   //统计元素个数
    -   `count_if`             //按条件统计元素个数



#### find

-   查找指定元素，找到返回指定元素的迭代器，找不到返回结束迭代器end()

-   `find(iterator beg, iterator end, value);  `

    -   // 按值查找元素，找到返回指定位置迭代器，找不到返回结束迭代器位置
    -   // beg 开始迭代器
    -   // end 结束迭代器
    -   // value 查找的元素

-   ```c++
    #include <algorithm>
    #include <vector>
    #include <string>
    void test01() {
    
    	vector<int> v;
    	for (int i = 0; i < 10; i++) {
    		v.push_back(i + 1);
    	}
    	//查找容器中是否有 5 这个元素
    	vector<int>::iterator it = find(v.begin(), v.end(), 5);
    	if (it == v.end()) 
    	{
    		cout << "没有找到!" << endl;
    	}
    	else 
    	{
    		cout << "找到:" << *it << endl;
    	}
    }
    
    class Person {
    public:
    	Person(string name, int age) 
    	{
    		this->m_Name = name;
    		this->m_Age = age;
    	}
    	//重载==
    	bool operator==(const Person& p) 
    	{
    		if (this->m_Name == p.m_Name && this->m_Age == p.m_Age) 
    		{
    			return true;
    		}
    		return false;
    	}
    
    public:
    	string m_Name;
    	int m_Age;
    };
    
    void test02() {
    
    	vector<Person> v;
    
    	//创建数据
    	Person p1("aaa", 10);
    	Person p2("bbb", 20);
    	Person p3("ccc", 30);
    	Person p4("ddd", 40);
    
    	v.push_back(p1);
    	v.push_back(p2);
    	v.push_back(p3);
    	v.push_back(p4);
    
    	vector<Person>::iterator it = find(v.begin(), v.end(), p2);
    	if (it == v.end()) 
    	{
    		cout << "没有找到!" << endl;
    	}
    	else 
    	{
    		cout << "找到姓名:" << it->m_Name << " 年龄: " << it->m_Age << endl;
    	}
    }
    ```

    

#### find_if

-   按条件查找元素
-   `find_if(iterator beg, iterator end, _Pred);  `
    -   // 按值查找元素，找到返回指定位置迭代器，找不到返回结束迭代器位置
    -   // beg 开始迭代器
    -   // end 结束迭代器
    -   // _Pred 函数或者谓词（返回bool类型的仿函数）

```c++
#include <algorithm>
#include <vector>
#include <string>

//内置数据类型
class GreaterFive
{
public:
	bool operator()(int val)
	{
		return val > 5;
	}
};

void test01() {

	vector<int> v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i + 1);
	}

	vector<int>::iterator it = find_if(v.begin(), v.end(), GreaterFive());
	if (it == v.end()) {
		cout << "没有找到!" << endl;
	}
	else {
		cout << "找到大于5的数字:" << *it << endl;
	}
}

//自定义数据类型
class Person {
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}
public:
	string m_Name;
	int m_Age;
};

class Greater20
{
public:
	bool operator()(Person &p)
	{
		return p.m_Age > 20;
	}

};

void test02() {

	vector<Person> v;

	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);

	vector<Person>::iterator it = find_if(v.begin(), v.end(), Greater20());
	if (it == v.end())
	{
		cout << "没有找到!" << endl;
	}
	else
	{
		cout << "找到姓名:" << it->m_Name << " 年龄: " << it->m_Age << endl;
	}
}

int main() {

	//test01();

	test02();

	system("pause");

	return 0;
}
```



#### adjacent_find

-   查找相邻重复元素
-   `adjacent_find(iterator beg, iterator end);  `
    -   // 查找相邻重复元素,返回相邻元素的第一个位置的迭代器
    -   // beg 开始迭代器
    -   // end 结束迭代器

```c++
#include <algorithm>
#include <vector>

void test01()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(5);
	v.push_back(2);
	v.push_back(4);
	v.push_back(4);
	v.push_back(3);

	//查找相邻重复元素
	vector<int>::iterator it = adjacent_find(v.begin(), v.end());
	if (it == v.end()) {
		cout << "找不到!" << endl;
	}
	else {
		cout << "找到相邻重复元素为:" << *it << endl;
	}
}
```



#### binary_search

-   查找指定元素是否存在
-   `bool binary_search(iterator beg, iterator end, value);  `
    -   // 查找指定的元素，查到 返回true  否则false
    -   // 注意: 在**无序序列中不可用**
    -   // beg 开始迭代器
    -   // end 结束迭代器
    -   // value 查找的元素

```c++
#include <algorithm>
#include <vector>

void test01()
{
	vector<int>v;

	for (int i = 0; i < 10; i++)
	{
		v.push_back(i);
	}
	//二分查找
	bool ret = binary_search(v.begin(), v.end(),2);
	if (ret)
	{
		cout << "找到了" << endl;
	}
	else
	{
		cout << "未找到" << endl;
	}
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### count

-   统计元素个数
-   `count(iterator beg, iterator end, value);  `
    -   // 统计元素出现次数
    -   // beg 开始迭代器
    -   // end 结束迭代器
    -   // value 统计的元素

```c++
#include <algorithm>
#include <vector>

//内置数据类型
void test01()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(4);
	v.push_back(5);
	v.push_back(3);
	v.push_back(4);
	v.push_back(4);

	int num = count(v.begin(), v.end(), 4);

	cout << "4的个数为： " << num << endl;
}

//自定义数据类型
class Person
{
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}
	bool operator==(const Person & p)
	{
		if (this->m_Age == p.m_Age)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	string m_Name;
	int m_Age;
};

void test02()
{
	vector<Person> v;

	Person p1("刘备", 35);
	Person p2("关羽", 35);
	Person p3("张飞", 35);
	Person p4("赵云", 30);
	Person p5("曹操", 25);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);
    
    Person p("诸葛亮",35);

	int num = count(v.begin(), v.end(), p);
	cout << "num = " << num << endl;
}
int main() {

	//test01();

	test02();

	system("pause");

	return 0;
}
```



#### count_if

-   按条件统计元素个数
-   `count_if(iterator beg, iterator end, _Pred);  `
    -   // 按条件统计元素出现次数
    -   // beg 开始迭代器
    -   // end 结束迭代器
    -   // _Pred 谓词

```c++
#include <algorithm>
#include <vector>

class Greater4
{
public:
	bool operator()(int val)
	{
		return val >= 4;
	}
};

//内置数据类型
void test01()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(4);
	v.push_back(5);
	v.push_back(3);
	v.push_back(4);
	v.push_back(4);

	int num = count_if(v.begin(), v.end(), Greater4());

	cout << "大于4的个数为： " << num << endl;
}

//自定义数据类型
class Person
{
public:
	Person(string name, int age)
	{
		this->m_Name = name;
		this->m_Age = age;
	}

	string m_Name;
	int m_Age;
};

class AgeLess35
{
public:
	bool operator()(const Person &p)
	{
		return p.m_Age < 35;
	}
};
void test02()
{
	vector<Person> v;

	Person p1("刘备", 35);
	Person p2("关羽", 35);
	Person p3("张飞", 35);
	Person p4("赵云", 30);
	Person p5("曹操", 25);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	v.push_back(p5);

	int num = count_if(v.begin(), v.end(), AgeLess35());
	cout << "小于35岁的个数：" << num << endl;
}


int main() {

	//test01();

	test02();

	system("pause");

	return 0;
}
```



### STL 常用排序算法

#### `sort` 

-   对容器内元素进行排序

-   `sort(iterator beg, iterator end, _Pred);  `

    -   // 按值查找元素，找到返回指定位置迭代器，找不到返回结束迭代器位置
    
    -   //  beg    开始迭代器	
    
    -   //  end    结束迭代器	
    
    
    -   // _Pred  谓词

```c++
#include <algorithm>
#include <vector>

void myPrint(int val)
{
	cout << val << " ";
}

void test01() {
	vector<int> v;
	v.push_back(10);
	v.push_back(30);
	v.push_back(50);
	v.push_back(20);
	v.push_back(40);

	//sort默认从小到大排序
	sort(v.begin(), v.end());
	for_each(v.begin(), v.end(), myPrint);
	cout << endl;

	//从大到小排序
	sort(v.begin(), v.end(), greater<int>());
	for_each(v.begin(), v.end(), myPrint);
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### `random_shuffle` 

* 洗牌   指定范围内的元素随机调整次序

- `random_shuffle(iterator beg, iterator end);  `

    -   // 指定范围内的元素随机调整次序
    
    -   // beg 开始迭代器
    
    -   // end 结束迭代器



```c++
#include <algorithm>
#include <vector>
#include <ctime>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	srand((unsigned int)time(NULL));
	vector<int> v;
	for(int i = 0 ; i < 10;i++)
	{
		v.push_back(i);
	}
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;

	//打乱顺序
	random_shuffle(v.begin(), v.end());
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### `merge`

* 两个容器元素合并，并存储到另一容器中

- `merge(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);  `

    -   // 容器元素合并，并存储到另一容器中
    
    -   // 注意: 两个容器必须是**有序的**
    
    -   // beg1   容器1开始迭代器
    
    -   // end1   容器1结束迭代器
    
    -   // beg2   容器2开始迭代器
    
    -   // end2   容器2结束迭代器
    
    -   // dest    目标容器开始迭代器

```c++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10 ; i++) 
    {
		v1.push_back(i);
		v2.push_back(i + 1);
	}

	vector<int> vtarget;
	//目标容器需要提前开辟空间
	vtarget.resize(v1.size() + v2.size());
	//合并  需要两个有序序列
	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), vtarget.begin());
	for_each(vtarget.begin(), vtarget.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### `reverse`

-   将容器内元素进行反转

- `reverse(iterator beg, iterator end);  `

    -   // 反转指定范围的元素
    
    -   // beg 开始迭代器
    
    -   // end 结束迭代器

```c++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(30);
	v.push_back(50);
	v.push_back(20);
	v.push_back(40);

	cout << "反转前： " << endl;
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;

	cout << "反转后： " << endl;

	reverse(v.begin(), v.end());
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



### STL 常用拷贝和替换算法

#### copy 

* 容器内指定范围的元素拷贝到另一容器中

- `copy(iterator beg, iterator end, iterator dest);  `

    -   // 按值查找元素，找到返回指定位置迭代器，找不到返回结束迭代器位置
    
    -   // beg  开始迭代器
    
    -   // end  结束迭代器
    
    -   // dest 目标起始迭代器

```c++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i + 1);
	}
	vector<int> v2;
	v2.resize(v1.size());
	copy(v1.begin(), v1.end(), v2.begin());

	for_each(v2.begin(), v2.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### replace

* 将容器内指定范围的旧元素修改为新元素

- `replace(iterator beg, iterator end, oldvalue, newvalue);  `

    -   // 将区间内旧元素 替换成 新元素
    
    -   // beg 开始迭代器
    
    -   // end 结束迭代器
    
    -   // oldvalue 旧元素
    
    -   // newvalue 新元素

```c++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v;
	v.push_back(20);
	v.push_back(30);
	v.push_back(20);
	v.push_back(40);
	v.push_back(50);
	v.push_back(10);
	v.push_back(20);

	cout << "替换前：" << endl;
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;

	//将容器中的20 替换成 2000
	cout << "替换后：" << endl;
	replace(v.begin(), v.end(), 20,2000);
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### replace_if

* 将区间内满足条件的元素，替换成指定元素

- `replace_if(iterator beg, iterator end, _pred, newvalue);  `

    -   // 按条件替换元素，满足条件的替换成指定元素
    
    -   // beg 开始迭代器
    
    -   // end 结束迭代器
    
    -   // _pred 谓词
    
    -   // newvalue 替换的新元素

```c++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

class ReplaceGreater30
{
public:
	bool operator()(int val)
	{
		return val >= 30;
	}

};

void test01()
{
	vector<int> v;
	v.push_back(20);
	v.push_back(30);
	v.push_back(20);
	v.push_back(40);
	v.push_back(50);
	v.push_back(10);
	v.push_back(20);

	cout << "替换前：" << endl;
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;

	//将容器中大于等于的30 替换成 3000
	cout << "替换后：" << endl;
	replace_if(v.begin(), v.end(), ReplaceGreater30(), 3000);
	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### swap

* 互换两个容器的元素

- `swap(container c1, container c2);  `

    -   // 互换两个容器的元素
    
    -   // c1容器1
    
    -   // c2容器2

```c++
#include <algorithm>
#include <vector>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
		v2.push_back(i+100);
	}

	cout << "交换前： " << endl;
	for_each(v1.begin(), v1.end(), myPrint());
	cout << endl;
	for_each(v2.begin(), v2.end(), myPrint());
	cout << endl;

	cout << "交换后： " << endl;
	swap(v1, v2);
	for_each(v1.begin(), v1.end(), myPrint());
	cout << endl;
	for_each(v2.begin(), v2.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



### STL 常用算术生成算法

* 算术生成算法属于小型算法，使用时包含的头文件为 `#include <numeric>`

- `accumulate`      // 计算容器元素累计总和

- `fill`                 // 向容器中添加元素



#### accumulate

*  计算区间内 容器元素累计总和

- `accumulate(iterator beg, iterator end, value);  `

    -   // 计算容器元素累计总和
    
    -   // beg 开始迭代器
    
    -   // end 结束迭代器
    
    -   // value 起始值

```c++
#include <numeric>
#include <vector>
void test01()
{
	vector<int> v;
	for (int i = 0; i <= 100; i++) {
		v.push_back(i);
	}

	int total = accumulate(v.begin(), v.end(), 0);

	cout << "total = " << total << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### fill

* 向容器中填充指定的元素

- `fill(iterator beg, iterator end, value);  `

    -   // 向容器中填充元素
    
    -   // beg 开始迭代器
    
    -   // end 结束迭代器
    
    -   // value 填充的值

```c++
#include <numeric>
#include <vector>
#include <algorithm>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{

	vector<int> v;
	v.resize(10);
	//填充
	fill(v.begin(), v.end(), 100);

	for_each(v.begin(), v.end(), myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



### STL 常用集合算法

- `set_intersection`          // 求两个容器的交集

- `set_union`                       // 求两个容器的并集

- `set_difference `              // 求两个容器的差集



#### set_intersection

* 求两个容器的交集

- `set_intersection(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);  `

    -   // 求两个集合的交集

-   // **注意:两个集合必须是有序序列**

    -   // beg1 容器1开始迭代器

    -   // end1 容器1结束迭代器

    -   // beg2 容器2开始迭代器

    -   // end2 容器2结束迭代器

    -   // dest 目标容器开始迭代器



```c++
#include <vector>
#include <algorithm>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++)
    {
		v1.push_back(i);
		v2.push_back(i+5);
	}

	vector<int> vTarget;
	//取两个里面较小的值给目标容器开辟空间
	vTarget.resize(min(v1.size(), v2.size()));

	//返回目标容器的最后一个元素的迭代器地址
	vector<int>::iterator itEnd = 
        set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), vTarget.begin());

	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### set_union

* 求两个集合的并集

- `set_union(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);  `

    -   // 求两个集合的并集

-   // **注意:两个集合必须是有序序列**

    -   // beg1 容器1开始迭代器

    -   // end1 容器1结束迭代器

    -   // beg2 容器2开始迭代器

    -   // end2 容器2结束迭代器

    -   // dest 目标容器开始迭代器

```c++
#include <vector>
#include <algorithm>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
		v2.push_back(i+5);
	}

	vector<int> vTarget;
	//取两个容器的和给目标容器开辟空间
	vTarget.resize(v1.size() + v2.size());

	//返回目标容器的最后一个元素的迭代器地址
	vector<int>::iterator itEnd = 
        set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), vTarget.begin());

	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```



#### set_difference 

* 求两个集合的差集

- `set_difference(iterator beg1, iterator end1, iterator beg2, iterator end2, iterator dest);  `

    -   // 求两个集合的差集

-   // **注意:两个集合必须是有序序列**

    -   // beg1 容器1开始迭代器

    -   // end1 容器1结束迭代器

    -   // beg2 容器2开始迭代器

    -   // end2 容器2结束迭代器

    -   // dest 目标容器开始迭代器

```c++
#include <vector>
#include <algorithm>

class myPrint
{
public:
	void operator()(int val)
	{
		cout << val << " ";
	}
};

void test01()
{
	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++) {
		v1.push_back(i);
		v2.push_back(i+5);
	}

	vector<int> vTarget;
	//取两个里面较大的值给目标容器开辟空间
	vTarget.resize( max(v1.size() , v2.size()));

	//返回目标容器的最后一个元素的迭代器地址
	cout << "v1与v2的差集为： " << endl;
	vector<int>::iterator itEnd = 
        set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), vTarget.begin());
	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;


	cout << "v2与v1的差集为： " << endl;
	itEnd = set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), vTarget.begin());
	for_each(vTarget.begin(), itEnd, myPrint());
	cout << endl;
}

int main() {

	test01();

	system("pause");

	return 0;
}
```

