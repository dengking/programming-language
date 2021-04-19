# Operation

本文描述对list的各种操作。

## Iteration

### 一边遍历一边删除

使用`while`

```c++
#include <list>
#include <iostream>
int main(){
	std::list<int> l = {1,2,3,4,5};
	while(not l.empty()){
		auto it = l.begin();
        std::cout<<*it<<std::endl;
        l.pop_front(); // 删除
	}
}


```

使用`for`

```c++
#include <list>
#include <iostream>
int main()
{
	std::list<int> l = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 3; ++i, l.pop_front())
	{

		auto it = l.begin();
		std::cout << *it << std::endl;
	}
}
```





### 按批次遍历

使用上面提及的“一边遍历一边删除”策略。

```c++

#include <list>
#include <iostream>
#include <cmath>
int main()
{
	std::list<int> l = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int batch_size = 4;
	int batch_num = std::ceil(l.size() / double(batch_size));
	for (int batch = 0; batch < batch_num; ++batch)
	{
		std::cout << "第[" << batch << "]批" << std::endl;
		for (int i = 0; i < batch_size and not l.empty(); ++i, l.pop_front())
		{
			auto it = l.begin();
			std::cout << *it << std::endl;
		}
	}
}
```


