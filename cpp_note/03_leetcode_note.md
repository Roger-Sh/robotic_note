# LeetCode Note



## 数组

### 删除有序数组重复项  双指针

给你一个 **非严格递增排列** 的数组 `nums` ，请你**[ 原地](http://baike.baidu.com/item/原地算法)** 删除重复出现的元素，使每个元素 **只出现一次** ，返回删除后数组的新长度。元素的 **相对顺序** 应该保持 **一致** 。然后返回 `nums` 中唯一元素的个数。

考虑 `nums` 的唯一元素的数量为 `k` ，你需要做以下事情确保你的题解可以被通过：

- 更改数组 `nums` ，使 `nums` 的前 `k` 个元素包含唯一元素，并按照它们最初在 `nums` 中出现的顺序排列。`nums` 的其余元素与 `nums` 的大小不重要。
- 返回 `k` 。



- 双指针，快慢指针删除重复元素 快指针的值覆盖慢指针的重复值

  - 一个左指针(慢指针)原地不动，另一个右指针(快指针)向右遍历数组，
  - 当右指针与左指针值相同时，左指针不动
  - 当右指针与左指针值不同时，左指针往前移动一格，将右指针的值给左指针

  - 时间复杂度O(n)	

  - 空间复杂度O(1)

```cpp
class Solution 
{
public:
    int removeDuplicates(vector<int>& nums) 
    {
        // check size
        int n = nums.size();
        if (n == 0)
        {
            return 0;
        }

        int fast = 1, slow = 0;
        while (fast < nums.size())
        {
            // 快指针的值与慢指针不同时，慢指针移动一格，并得到快指针的值
            if (nums[slow] != nums[fast])
            {
                // 慢指针需要移动一格, 
                slow++;
                nums[] = nums[fast]; 
            }
            // 当慢指针与快指针的值相同时, 慢指针停留在原地, 快指针继续前进
            // 这样下一次不同时, 快指针的值会给慢指针, 相当于把慢指针的值覆盖了
            fast++;
        }

        // 最后输出数组长度，需要将数组最后一个元素的下标再加1
        return slow+1;
    }
};
```



### 买卖股票最佳时机 贪心算所有上升区间总和

**这里的股票可以买卖多次**

给你一个整数数组 `prices` ，其中 `prices[i]` 表示某支股票第 `i` 天的价格。

在每一天，你可以决定是否购买和/或出售股票。你在任何时候 **最多** 只能持有 **一股** 股票。你也可以先购买，然后在 **同一天** 出售。

返回 ***你能获得的** **最大** 利润* 。

- 贪心算法 双指针
  计算所有上升区间的总和

  ```cpp
  class Solution 
  {
  public:
      int maxProfit(vector<int>& prices) 
      {
          int max_profit = 0;
          int buy_i = -1, sell_i = 0;
          for(int i = 1; i < prices.size(); i++)
          {
              // 价格上升
              if (prices[i] > prices[i-1])
              {
                  // 记录买点, 买点在一次买卖中只记录一次
                  if (buy_i == -1)
                  {
                      buy_i = i - 1;
                  }
                  
                  // 记录卖点, 如果一直上涨, 卖点一直更新
                  sell_i = i;
  
                  // 如果一直上升到最后一天，执行买卖点
                  if (i == prices.size() - 1)
                  {
                      max_profit += prices[sell_i] - prices[buy_i];
                  }
              }
              // 价格下降, 且有买卖点
              else if (buy_i != -1)
              {
                  // 计算当前区间的利润
                  max_profit += prices[sell_i] - prices[buy_i];
                  
                  // 重置买卖点
                  buy_i = -1;
                  sell_i = 0;
              }
          }
  
          return max_profit;
      }
  };
  ```

  

### 轮转数组

给定一个整数数组 `nums`，将数组中的元素向右轮转 `k` 个位置，其中 `k` 是非负数。



**示例 1:**

```
输入: nums = [1,2,3,4,5,6,7], k = 3
输出: [5,6,7,1,2,3,4]
解释:
向右轮转 1 步: [7,1,2,3,4,5,6]
向右轮转 2 步: [6,7,1,2,3,4,5]
向右轮转 3 步: [5,6,7,1,2,3,4]
```

**示例 2:**

```
输入：nums = [-1,-100,3,99], k = 2
输出：[3,99,-1,-100]
解释: 
向右轮转 1 步: [99,-1,-100,3]
向右轮转 2 步: [3,99,-1,-100]
```



方法1：使用额外数组
时间复杂度O(n)
空间复杂度O(n)

```cpp
void rotate(vector<int>& nums, int k) 
{
    // 计算有效K值
	k %= nums.size();
    
    int n = nums.size();
	vector<int> newArr(n);
	for (int i = 0; i < n; ++i) 
	{
		newArr[(i + k) % n] = nums[i];
	}
	nums.assign(newArr.begin(), newArr.end());
}
```

方法2：环状替代
时间复杂度O(n)
空间复杂度O(1)

```cpp
void rotate(vector<int>& nums, int k)
{
	int n = nums.size();
	// 计算有效k值
	k = k % n;

	// 最大公约数，或者通过计数更换了几个元素作为标识
	int count = gcd(k, n);
	for (int start = 0; start < count; ++start) 
	{
		int current = start;
		int prev_val = nums[start];
		do
		{
			int next = (current + k) % n;
			swap(nums[next], prev_val);
			current = next;
		} while (start != current);
	}
}
```

方法3：反转法
时间复杂度O(n)
空间复杂度O(1)

```cpp
void reverse(vector<int>& nums, int start, int end) 
{
    // 双指针翻转数组
	while (start < end) 
	{
		swap(nums[start], nums[end]);
    start += 1;
    end -= 1;
	}
}

void rotate(vector<int>& nums, int k) 
{
    // 计算有效K值
	k %= nums.size();

    // 原始数据 1 2 3 4 5 6 7
	// 反转全部  7 6 5 4 3 2 1
	reverse(nums, 0, nums.size() - 1);

	// 反转前k个 5 6 7    4 3 2 1 
	reverse(nums, 0, k - 1);

	// 反转剩余的 5 6 7 1 2 3 4
	reverse(nums, k, nums.size() - 1);
}
```



### 判断重复元素

给你一个整数数组 `nums` 。如果任一值在数组中出现 **至少两次** ，返回 `true` ；如果数组中每个元素互不相同，返回 `false` 。

方法1: 排序后判断相邻元素是否相同
时间复杂度 O(NlogN) 需要排序
空间复杂度 O(logN)

```cpp
bool containsDuplicate(vector<int>& nums) 
{
	std::sort(nums.begin(), nums.end());

	for(int i = 1; i < nums.size(); i++)
	{
		if (nums[i-1] == nums[i] )
		return true;
	}

	return false;
}
```

方法2: 通过哈希表判断能否插入
时间复杂度 O(N)
空间复杂度 O(N)

```cpp
bool containsDuplicate(vector<int>& nums) 
{
	std::unordered_set<int> uset;
	for(int x : nums)
	{
		// 找到元素
		if (uset.find(x) != uset.end())
		{
			return true;
		}
	
        // 没找到元素
		uset.insert(x);
	}

	return false;
}
```



方法3:

双指针迭代

时间复杂度O(N^2)

空间复杂度O(1)



### 去除重复元素



给你一个 **非空** 整数数组 `nums` ，除了某个元素只出现一次以外，其余**每个元素均出现两次**。找出那个只出现了一次的元素。

你必须设计并实现线性时间复杂度的算法来解决此问题，且该算法只使用常量额外空间。

 

**示例 1 ：**

```
输入：nums = [2,2,1]
输出：1
```

**示例 2 ：**

```
输入：nums = [4,1,2,1,2]
输出：4
```

**示例 3 ：**

```
输入：nums = [1]
输出：1
```



方法1: 异或算法
相异为真,相同为假, 同时满足交换率

需要除了要找的数外, 每个数均出现两次

时间复杂度 O(N)

空间复杂度 O(1)

```cpp
int singleNumber(vector<int>& nums) 
{
		int result = 0;
		for (int num : nums)
		{
				result = result ^ num;
		}
		return result;
}
```

方法2:
通过哈希表去除重复元素

时间复杂度 O(N)

空间复杂度 O(N)

```cpp
int singleNumber(vector<int>& nums) 
{
		std::unordered_set<int> uset;
		for(int num : nums)
		{
				// 如果找到元素, 说明该元素为重复元素
            	// 去除重复元素
				if (uset.find(num) != uset.end())
				{
						uset.erase(num);
				}
				else
				{
						uset.insert(num);
				}
		}

		// 返回剩余的元素
		return *uset.begin();
}
```



### 两个数组交集 

给你两个整数数组 `nums1` 和 `nums2` ，请你以数组形式返回两数组的**交集**。返回结果中每个元素出现的次数，应与元素在两个数组中都出现的次数一致（如果出现次数不一致，则考虑取较小值）。可以不考虑输出结果的顺序。

 

**示例 1：**

```
输入：nums1 = [1,2,2,1], nums2 = [2,2]
输出：[2,2]
```

**示例 2:**

```
输入：nums1 = [4,9,5], nums2 = [9,4,9,8,4]
输出：[4,9]
```



方法1: 哈希表
由于同一个数字在两个数组中都可能出现多次，因此需要用哈希表存储每个数字出现的次数。
对于一个数字，其在交集中出现的次数等于该数字在两个数组中出现次数的最小值。
首先遍历第一个数组，并在哈希表中记录第一个数组中的每个数字以及对应出现的次数，
然后遍历第二个数组，对于第二个数组中的每个数字，如果在哈希表中存在这个数字，
则将该数字添加到答案，并减少哈希表中该数字出现的次数。
为了降低空间复杂度，首先遍历较短的数组并在哈希表中记录每个数字以及对应出现的次数，然后遍历较长的数组得到交集。

时间复杂度: O(m+n), m与n分别为两个数组的长度
空间复杂度: O(min(m, n)),

```cpp
vector<int> intersect(vector<int>& nums1, vector<int>& nums2) 
{
    // 比较nums1和nums2的大小, 先从小的开始
    if (nums1.size() > nums2.size())
    {
        return intersect(nums2, nums1);
    }
    
    // 初始化nums1的哈希表, 并计数每个元素出现的次数
    std::unordered_map<int, int> m;
    for( int num : nums1)
    {
        m[num]++;
    }
    
    // 初始化交集
    std::vector<int> intersection;
    
    // 遍历nums2
    for (int num : nums2)
    {
        // 如果m中有对应的num
        if (m.count(num))
        {
            // 加入到交集并减少m中记录的数量
            intersection.push_back(num);
            m[num]--;
            
            // 如果m中记录的数量为0, 则删除该元素
            if (m[num] == 0)
            {
                m.erase(num);
            }
        }
    }
    
    return intersection;
}
```



方法2: 排序+双指针
如果两个数组是有序的，则可以使用双指针的方法得到两个数组的交集。
首先对两个数组进行排序，然后使用两个指针遍历两个数组。
初始时，两个指针分别指向两个数组的头部。
每次比较两个指针指向的两个数组中的数字，
如果两个数字不相等，则将指向较小数字的指针右移一位，
如果两个数字相等，将该数字添加到答案，并将两个指针都右移一位。
当至少有一个指针超出数组范围时，遍历结束。

时间复杂度：O(mlog⁡m+nlog⁡n)，其中 m 和 n 分别是两个数组的长度。
空间复杂度：O(min⁡(m,n))

```cpp
vector<int> intersect(vector<int>& nums1, vector<int>& nums2) 
{
    // 检查大小
    // 先从小的开始
    if (nums1.size() > nums2.size())
    {
        return intersect(nums2, nums1);
    }
    
    // 排序
    std::sort(nums1.begin(), nums1.end());
    std::sort(nums2.begin(), nums2.end());
    
    // 双指针法
    // 如果两个数字不相等，则将指向较小数字的指针右移一位，
	// 如果两个数字相等，将该数字添加到答案，并将两个指针都右移一位。
    int index1 = 0;
    int index2 = 0;
    std::vector<int> intersection;
    while(index1 < nums1.size() &&
        index2 < nums2.size())
    {
        // 不相等, 小值的指针右移
        if (nums1[index1] < nums2[index2])
        {
            index1++;
        }
        else if (nums2[index2] < nums1[index1])
        {
            index2++;
        }
        
        // 相等, 加入并集, 同时移位
        else
        {
            intersection.push_back(nums1[index1]);
            index1++;
            index2++;
        }
    }

    return intersection;
}
```



### 移动零 快慢指针

给定一个数组 `nums`，编写一个函数将所有 `0` 移动到数组的末尾，同时保持非零元素的相对顺序。

**请注意** ，必须在**不复制数组**的情况下原地对数组进行操作。



通过快慢指针移动零
当慢指针的数值为0, 快指针的数值不为0时, 交换数据
慢指针遇到0停止
快指针每次+1

```cpp
void moveZeroes(vector<int>& nums) 
{
	// 快慢指针
	int slow = 0;
	int fast = 0;
	while(fast < nums.size())
	{
		// 当慢指针对应数值为0, 快指针对应数值不为0时
		// 交换快慢指针数值
		if (nums[slow] == 0 &&
			nums[fast] != 0)
		{
			nums[slow] = nums[fast];
			nums[fast] = 0;
		}

		// 快指针一直+1
		fast++;

		// 慢指针遇到数值0则停止, 直到数值交换后不为0
		if (nums[slow] != 0)
		{
			slow++;
		}
	}
}
```



### 加一 



给定一个由 **整数** 组成的 **非空** 数组所表示的非负整数，在该数的基础上加一。

最高位数字存放在数组的首位， 数组中每个元素只存储**单个**数字。

你可以假设除了整数 0 之外，这个整数不会以零开头。



**示例 1：**

```
输入：digits = [1,2,3]
输出：[1,2,4]
解释：输入数组表示数字 123。
```

**示例 2：**

```
输入：digits = [4,3,2,1]
输出：[4,3,2,2]
解释：输入数组表示数字 4321。
```

**示例 3：**

```
输入：digits = [0]
输出：[1]
```



方法1: 从后往前遍历vector
如果遇到9, 则变0并继续
如果遇到不是9, 则+1后结束循环
最后检查第一位是否是0, 是0则在第一位插入1

```cpp
vector<int> plusOne(vector<int>& digits) 
{
		// iterate all digits
		for(int i = digits.size() - 1; i >= 0; i--)
		{
				if (digits[i]  ==  9)
				{
						digits[i] = 0;
				}
				else
				{
						digits[i]++;
						break;
				}
		}

		// 如果第一位变成了0, 则在最开始插入1
		if (digits[0] == 0)
		{
				digits.insert(digits.begin(), 1);
		}

		return digits;
}
```



### 有效数独 哈希表

请你判断一个 `9 x 9` 的数独是否有效。只需要 **根据以下规则** ，验证已经填入的数字是否有效即可。

1. 数字 `1-9` 在每一行只能出现一次。
2. 数字 `1-9` 在每一列只能出现一次。
3. 数字 `1-9` 在每一个以粗实线分隔的 `3x3` 宫内只能出现一次。（请参考示例图）

**注意：**

- 一个有效的数独（部分已被填充）不一定是可解的。
- 只需要根据以上规则，验证已经填入的数字是否有效即可。
- 空白格用 `'.'` 表示。





通过哈希表记忆行, 列, 块中是否有重复数字
一次遍历记录行, 列, 块的哈希表

```cpp
bool isValidSudoku(vector<vector<char>>& board) 
{
	// 建立哈希表
	int rows[9][9];
	int columns[9][9];
	int subboxes[3][3][9];

	// 初始化哈希表内结果为0
	memset(rows,0,sizeof(rows));
	memset(columns,0,sizeof(columns));
	memset(subboxes,0,sizeof(subboxes));

	// 遍历整个棋盘
	for (int i = 0; i < 9; i++) 
	{
		for (int j = 0; j < 9; j++) 
		{
			char c = board[i][j];
			if (c != '.') 
			{
				// char 转换为数字
				int index = c - '0' - 1;

				// 加入行哈希表
				rows[i][index]++;

				// 加入列哈希表
				columns[j][index]++;

				// 加入块哈希表
				subboxes[i / 3][j / 3][index]++;

				// 判断哈希表内是否计数大于1
				if (rows[i][index] > 1 || 
					columns[j][index] > 1 || 
					subboxes[i / 3][j / 3][index] > 1) 
				{
					return false;
				}
			}
		}
	}
	return true;
}
```



### 矩阵旋转

给定一个 *n* × *n* 的二维矩阵 `matrix` 表示一个图像。请你将图像顺时针旋转 **90 度**。

你必须在**[ 原地](https://baike.baidu.com/item/原地算法)** 旋转图像，这意味着你需要直接修改输入的二维矩阵。**请不要** 使用另一个矩阵来旋转图像。





方法1: 行翻转后对称轴翻转

```cpp
void rotate(vector<vector<int>>& matrix) 
{
	// 行翻转
	for (int i = 0; i < matrix.size()/2; i++)
	{
		matrix[i].swap(matrix[matrix.size() - 1 - i]);
	}

	// 对称轴翻转
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = i + 1; j < matrix.size(); j++ )
		{
			int tmp;
			tmp = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = tmp;
		}
	}
}
```

方法2: 旋转遍历

```cpp
void rotate(vector<vector<int>>& matrix) 
{
	int n = matrix.size();
	for (int i = 0; i < n / 2; ++i) 
	{
		for (int j = 0; j < (n + 1) / 2; ++j) 
		{
			int temp = matrix[i][j];
			matrix[i][j] = matrix[n - j - 1][i];
			matrix[n - j - 1][i] = matrix[n - i - 1][n - j - 1];
			matrix[n - i - 1][n - j - 1] = matrix[j][n - i - 1];
			matrix[j][n - i - 1] = temp;
		}
	}
}
```





### 归并排序

给你两个按 **非递减顺序** 排列的整数数组 `nums1` 和 `nums2`，另有两个整数 `m` 和 `n` ，分别表示 `nums1` 和 `nums2` 中的元素数目。

请你 **合并** `nums2` 到 `nums1` 中，使合并后的数组同样按 **非递减顺序** 排列。

**注意：**最终，合并后数组不应由函数返回，而是存储在数组 `nums1` 中。为了应对这种情况，`nums1` 的初始长度为 `m + n`，其中前 `m` 个元素表示应合并的元素，后 `n` 个元素为 `0` ，应忽略。`nums2` 的长度为 `n` 。

 

**示例 1：**

```
输入：nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
输出：[1,2,2,3,5,6]
解释：需要合并 [1,2,3] 和 [2,5,6] 。
合并结果是 [1,2,2,3,5,6] ，其中斜体加粗标注的为 nums1 中的元素。
```

**示例 2：**

```
输入：nums1 = [1], m = 1, nums2 = [], n = 0
输出：[1]
解释：需要合并 [1] 和 [] 。
合并结果是 [1] 。
```

**示例 3：**

```
输入：nums1 = [0], m = 0, nums2 = [1], n = 1
输出：[1]
解释：需要合并的数组是 [] 和 [1] 。
合并结果是 [1] 。
注意，因为 m = 0 ，所以 nums1 中没有元素。nums1 中仅存的 0 仅仅是为了确保合并结果可以顺利存放到 nums1 中。
```

方法1：双指针法，需要额外空间
时间复杂度：O(M+N)
空间复杂度：O(M+N)

```CPP
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) 
{
	int i = 0;
	int j = 0;

	std::vector<int> merged;
	while (!(i == m && j == n))
	{
		if (i == m)
		{
			merged.push_back(nums2[j]);
			j++;
			continue;
		}

		if (j == n)
		{
			merged.push_back(nums1[i]);
			i++;
			continue;
		}

		if (nums1[i] < nums2[j])
		{
			merged.push_back(nums1[i]);
			i++;
		}
		else
		{
			merged.push_back(nums2[j]);
			j++;
		}
	}

	nums1 = merged;
}
```

方法2：合并后利用sort快排
时间复杂度：O((m+n)log(m+n))
空间复杂度：O(log(m+n))

```cpp
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n)
{
	for(int i = 0; i < n; i++)
	{
		nums1[m+i] = nums2[i];
	}
	std::sort(nums1.begin(), nums1.end());
}
```



方法3：从后往前遍历
充分利用nums1中后部已有的空白空间，
从 nums1 与 nums2 有效值开始从后往前遍历
大的放到nums1的最后空白处

```CPP
void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) 
{
	int i = m - 1;
	int j = n - 1;

	for (int index = m + n -1; index >= 0; index--)
	{
		// nums1 is finished
		if (i < 0)
		{
			nums1[index] = nums2[j];
			j--;
			continue;
		}

		// nums2 is finished
		if (j < 0)
		{
			nums1[index] = nums1[i];
			i--;
			continue;
		}

		// compare current nums1 and nums2
		if (nums1[i] > nums2[j])
		{
			nums1[index] = nums1[i];
			i--;
		}
		else
		{
			nums1[index] = nums2[j];
			j--;
		}
	}
}
```











## 字符串

### 反转字符串

编写一个函数，其作用是将输入的字符串反转过来。输入字符串以字符数组 `s` 的形式给出。

不要给另外的数组分配额外的空间，你必须**[原地](https://baike.baidu.com/item/原地算法)修改输入数组**、使用 O(1) 的额外空间解决这一问题。

` 

**示例 1：**

```
输入：s = ["h","e","l","l","o"]
输出：["o","l","l","e","h"]
```

**示例 2：**

```
输入：s = ["H","a","n","n","a","h"]
输出：["h","a","n","n","a","H"]
```



双指针翻转

```cpp
void reverseString(vector<char>& s) 
{
	char tmp;
	for (int i = 0; i < s.size()/2; i++)
	{
		tmp = s[i];
		s[i] = s[s.size() - 1- i];
		s[s.size() - 1 - i] = tmp;
	}
}
```



### 反转子字符串

input = "the sky is blue"

output = "blue is sky the"

intput = "hello   world "

output = "world hello"



- 思路
  - 分割子串
  - 反转顺序后拼接



```CPP
#include <iostream>
#include <stack>
#include <vector>
#include <unordered_set>

using namespace std;

int main()
{
    std::string s = "the sky is blue ";
    // std::string s = "hello  world";

    std::vector<std::string> sub_strs;
    std::string sub_str;

    for (size_t i = 0; i < s.size(); i++)
    {
        // std::cout << s[i] << std::endl;
        
        // 如果不是空格,加入到当前子串
        if (s[i] != ' ')
        {
            sub_str += s[i];
        }
        // 是空格
        else
        {
            // 如果子串有长度, 加入到列表, 并清空当前子串
            if (sub_str.size() != 0)
            {
                sub_strs.push_back(sub_str);
                sub_str = "";
                continue;
            }
        }

        // 到最后一个字符, 并且子串不为空, 加入到列表
        if (i == s.size() - 1 && sub_str.size()!= 0)
        {
            sub_strs.push_back(sub_str);
        }
    }

    std::string ans = "";
    for (size_t i = 0; i < sub_strs.size(); i++)
    {
        std::cout << '\"' << sub_strs[i] << '\"' << std::endl;
        ans += sub_strs[sub_strs.size() - 1 - i];
		
        // 中间需要space, 但最后一次不需要
        if (i != sub_strs.size() - 1)
        {
            ans += ' ';
        }
    }

    std::cout << "\"" << ans << "\"" << std::endl;
    return 0;
}
```



### 整数翻转

给你一个 32 位的有符号整数 `x` ，返回将 `x` 中的数字部分反转后的结果。

如果反转后整数超过 32 位的有符号整数的范围 `[−231, 231 − 1]` ，就返回 0。

**假设环境不允许存储 64 位整数（有符号或无符号）。**

**示例 1：**

```
输入：x = 123
输出：321
```

**示例 2：**

```
输入：x = -123
输出：-321
```

**示例 3：**

```
输入：x = 120
输出：21
```

**示例 4：**

```
输入：x = 0
输出：0
```



通过每次对x取10的模, 可得其个位数字
对x除以10, 可得其10位以上的部分
则新的数字更新的方式为
digit = x%10;
rev = rev*10 + digit

注意需要提前判断是否下一次即将超过32位的限制
首先判断 rev 不超过INT_MAX/10 或者INT_MIN/10
由于 x 不会超过INT_MAX 或 INT_MIN, 其最高位digit永远小于等与2,
满足小于247483647的最小位7
所以只需要判断rev<INT_MIN / 10
或者rev > INT_MAX / 10 即可

```cpp
int reverse(int x) 
{
	int rev = 0;
	while (x != 0) 
	{
		// digit 本身不会超过INT_MAX, 所以只需要判断rev<INT_MIN / 10
		// 或者rev > INT_MAX / 10 即可
		if (rev < INT_MIN / 10 || rev > INT_MAX / 10) {
			return 0;
		}

		// 取模获取digit
		int digit = x % 10;

		// 减少 x 位数
		x /= 10;

		// 更新rev
		rev = rev * 10 + digit;
	}
	return rev;
}
```



### 寻找第一个不重复字符 哈希表

给定一个字符串 `s` ，找到 *它的第一个不重复的字符，并返回它的索引* 。如果不存在，则返回 `-1` 。

 

**示例 1：**

```
输入: s = "leetcode"
输出: 0
```

**示例 2:**

```
输入: s = "loveleetcode"
输出: 2
```

**示例 3:**

```
输入: s = "aabb"
输出: -1
```

方法1: 通过哈希表储存频率
时间复杂度：O(n)
空间复杂度：O(∣Σ∣)

```cpp
int firstUniqChar(string s) 
{
	std::unordered_map <char, int> freq;
	for (char ch : s)
	{
		freq[ch]++;
	}

	for (int i = 0; i < s.size(); i++)
	{
		if (freq[s[i]] == 1)
		{
			return i;
		}
	}

	return -1;
}
```



方法2:
优化哈希表法, 直接保存第一次出现的索引到哈希表中, 如果之后重复则改为-1
时间复杂度：O(n)
空间复杂度：O(∣Σ∣)

```cpp
int firstUniqChar(string s) 
{
	// 将字符和索引存入哈希表
	unordered_map<int, int> position;
	int n = s.size();
	for (int i = 0; i < n; ++i) 
	{
		// 重复项
		if (position.count(s[i])) 
		{
			position[s[i]] = -1;
		}
		// 不重复项
		else 
		{
			position[s[i]] = i;
		}
	}

	// 找到不重复元素的最小索引
	int min_index = n;
	for (auto [_, pos]: position) 
	{
		if (pos != -1 && pos < min_index) 
		{
			min_index = pos;
		}
	}

	// 排除未找到的情况
	if (min_index == n) 
	{
		min_index = -1;
	}
	return min_index;
}
```

方法3: 通过队列找到第一个不重复的字符
时间复杂度：O(n)
空间复杂度：O(∣Σ∣)

```cpp
int firstUniqChar(string s) 
{
	// 新建哈希表储存元素和第一次出现的索引, 重复后索引记为-1
	unordered_map<char, int> position;
	// 新建队列储存元素和出现的索引
	queue<pair<char, int>> q;

	// 遍历
	int n = s.size();
	for (int i = 0; i < n; ++i) 
	{
		// 不重复的情况
		// 保存第一次出现的元素和其索引
		if (!position.count(s[i])) 
		{
			// 保存到哈希表
			position[s[i]] = i;
			// 保存到队列
			q.emplace(s[i], i);
		}
		// 出现重复的情况
		else 
		{
			// 哈希表中的索引设置为-1
			position[s[i]] = -1;

			// 当队列不为空, 并且队列第一个元素在哈希表中标识为-1即重复项
			// 弹出队列第一个元素
			// 直到不重复项出现为止
			while (!q.empty() && position[q.front().first] == -1) 
			{
				// 弹出队列第一个元素
				q.pop();
			}
		}
	}

	// 当队列为空时, 返回-1
	// 当队列不为空时, 说明第一个元素是不重复项, 则返回该不重复项的索引
	return q.empty() ? -1 : q.front().second;
}
```



### 有效的字母异位词

给定两个字符串 `*s*` 和 `*t*` ，编写一个函数来判断 `*t*` 是否是 `*s*` 的字母异位词。

**注意：**若 `*s*` 和 `*t*` 中每个字符出现的次数都相同，则称 `*s*` 和 `*t*` 互为字母异位词。

 

**示例 1:**

```
输入: s = "anagram", t = "nagaram"
输出: true
```

**示例 2:**

```
输入: s = "rat", t = "car"
输出: false
```



方法1: 通过哈希表记录第一个字符串的每个字符出现频率
时间复杂度：O(n)，其中 n 为 s 的长度
空间复杂度：O(S)，其中 S 为字符集大小，此处 S=26

```cpp
bool isAnagram(string s, string t) 
{
	if (s.size() != t.size())
	{
		return false;
	}

	std::unordered_map<char, int> s_freq;
	std::unordered_map<char, int> t_freq;
	for (int i = 0; i < s.size(); i++)
	{
		s_freq[s[i]]++;
		t_freq[t[i]]++;
	}

	for (auto [ch, _] : s_freq)
	{
		if (s_freq[ch] != t_freq[ch])
		{
			return false;
		}
	}

	return true;
}
```



### 验证回文串 双指针

如果在将所有大写字符转换为小写字符、并移除所有非字母数字字符之后，短语正着读和反着读都一样。则可以认为该短语是一个 **回文串** 。

字母和数字都属于字母数字字符。

给你一个字符串 `s`，如果它是 **回文串** ，返回 `true` ；否则，返回 `false` 。

 

**示例 1：**

```
输入: s = "A man, a plan, a canal: Panama"
输出：true
解释："amanaplanacanalpanama" 是回文串。
```

**示例 2：**

```
输入：s = "race a car"
输出：false
解释："raceacar" 不是回文串。
```

**示例 3：**

```
输入：s = " "
输出：true
解释：在移除非字母数字字符之后，s 是一个空字符串 "" 。
由于空字符串正着反着读都一样，所以是回文串。
```



方法2: 直接在遍历过程中过滤非法字符以及判断回文, 通过双指针判断回文
时间复杂度：O(∣s∣)，其中 ∣s∣ 是字符串 s 的长度。
空间复杂度：O(1)



`isalnum()` 是 C 和 C++ 标准库中的一个函数,它用于检查一个给定的字符是否是字母或数字。具体来说:

- 如果字符是大写字母 (A-Z)、小写字母 (a-z) 或数字 (0-9)，则该函数返回一个非零值(通常是 1)。
- 如果字符不是字母也不是数字,则该函数返回 0。



```cpp
bool isPalindrome(string s) 
{
	int left = 0;
	int right = s.size() - 1;

	while(left < right)
	{
		// 判断left的元素是否非法
		while (!std::isalnum(s[left]) && left < right)
		{
			left++;
		}

		// 判断right的元素是否非法
		while (!std::isalnum(s[right]) && left < right)
		{
			right--;           
		}

		// 判断是否回文
		if (left < right && std::tolower(s[left]) != std::tolower(s[right]))
		{
			return false;
		}

		left++;
		right--;
	}

	return true;
}
```



### 字符串转换整数 atoi 状态机法找出所有边界条件

请你来实现一个 `myAtoi(string s)` 函数，使其能将字符串转换成一个 32 位有符号整数。

函数 `myAtoi(string s)` 的算法如下：

1. **空格：**读入字符串并丢弃无用的前导空格（`" "`）
2. **符号：**检查下一个字符（假设还未到字符末尾）为 `'-'` 还是 `'+'`。如果两者都不存在，则假定结果为正。
3. **转换：**通过跳过前置零来读取该整数，直到遇到非数字字符或到达字符串的结尾。如果没有读取数字，则结果为0。
4. **舍入：**如果整数数超过 32 位有符号整数范围 `[−231, 231 − 1]` ，需要截断这个整数，使其保持在这个范围内。具体来说，小于 `−231` 的整数应该被舍入为 `−231` ，大于 `231 − 1` 的整数应该被舍入为 `231 − 1` 。

返回整数作为最终结果。

 

**示例 1：**

**输入：**s = "42"

**输出：**42

**解释：**加粗的字符串为已经读入的字符，插入符号是当前读取的字符。

```
带下划线线的字符是所读的内容，插入符号是当前读入位置。
第 1 步："42"（当前没有读入字符，因为没有前导空格）
         ^
第 2 步："42"（当前没有读入字符，因为这里不存在 '-' 或者 '+'）
         ^
第 3 步："42"（读入 "42"）
           ^
```

**示例 2：**

**输入：**s = " -042"

**输出：**-42

**解释：**

```
第 1 步："   -042"（读入前导空格，但忽视掉）
            ^
第 2 步："   -042"（读入 '-' 字符，所以结果应该是负数）
             ^
第 3 步："   -042"（读入 "042"，在结果中忽略前导零）
               ^
```

**示例 3：**

**输入：**s = "1337c0d3"

**输出：**1337

**解释：**

```
第 1 步："1337c0d3"（当前没有读入字符，因为没有前导空格）
         ^
第 2 步："1337c0d3"（当前没有读入字符，因为这里不存在 '-' 或者 '+'）
         ^
第 3 步："1337c0d3"（读入 "1337"；由于下一个字符不是一个数字，所以读入停止）
             ^
```

**示例 4：**

**输入：**s = "0-1"

**输出：**0

**解释：**

```
第 1 步："0-1" (当前没有读入字符，因为没有前导空格)
         ^
第 2 步："0-1" (当前没有读入字符，因为这里不存在 '-' 或者 '+')
         ^
第 3 步："0-1" (读入 "0"；由于下一个字符不是一个数字，所以读入停止)
          ^
```

**示例 5：**

**输入：**s = "words and 987"

**输出：**0

**解释：**

读取在第一个非数字字符“w”处停止。





- 通过状态机处理所有的情况

![img](./03_leetcode_note.assets/fig1.png)

```cpp
class Automaton 
{
	// init state as start
	string state = "start";
	
	// state machine table
	unordered_map<string, vector<string>> table = 
	{
		{"start", {"start", "signed", "in_number", "end"}},
		{"signed", {"end", "end", "in_number", "end"}},
		{"in_number", {"end", "end", "in_number", "end"}},
		{"end", {"end", "end", "end", "end"}}
	};

	// get next state
	int get_col(char c) 
	{
		// space state
		if (isspace(c)) return 0;
		
		// +- state
		if (c == '+' or c == '-') return 1;
		
		// digit state
		if (isdigit(c)) return 2;
		
		// other state
		return 3;
	}
	
public:
	int sign = 1;
	long long ans = 0;

	// update with new char
	void get(char c) 
	{
		// update state
		state = table[state][get_col(c)];
    
		// update answer based on current state
		if (state == "in_number") 
		{
            if (sign == 1)
            {
                ans = ans * 10 + c - '0';
                ans = min(ans, (long long)INT_MAX);
            }
            else
            {
                ans = ans * 10 -  (c - '0');
                ans = max(ans, (long long)INT_MIN);
            }
		}
		// check sign 
		else if (state == "signed")
		{
			sign = c == '+' ? 1 : -1;
		}
	}
};

class Solution 
{
public:
	int myAtoi(string str) 
	{
		Automaton automaton;
		for (char c : str)
		{
			automaton.get(c);
		}
		return automaton.ans;
	}
};
```



### 找出字符串第一个匹配项的下标 KMP

给你两个字符串 `haystack` 和 `needle` ，请你在 `haystack` 字符串中找出 `needle` 字符串的第一个匹配项的下标（下标从 0 开始）。如果 `needle` 不是 `haystack` 的一部分，则返回 `-1` 。

 

**示例 1：**

```
输入：haystack = "sadbutsad", needle = "sad"
输出：0
解释："sad" 在下标 0 和 6 处匹配。
第一个匹配项的下标是 0 ，所以返回 0 。
```

**示例 2：**

```
输入：haystack = "leetcode", needle = "leeto"
输出：-1
解释："leeto" 没有在 "leetcode" 中出现，所以返回 -1 。
```



- 方法1: 暴力匹配法

  ```cpp
  int strStr(string haystack, string needle) 
  {
  	for (int i = 0; i < haystack.size(); i++)
  	{
  		// check first char of needle
  		if (haystack[i] == needle[0])
  		{
  			// check range
  			if (needle.size() + i > haystack.size())
  			{
  				// no enough ch in haystack for needle
  				return -1;
  			}
                  
  			// check rest ch of needle
  			bool flag = true;
  			for (int j = 0; j < needle.size(); j++)
  			{
  				if (needle[j] != haystack[i+j])
  				{
  					flag = false;
  					break;
  				}
  			}
                  
  			// check flag
  			if (flag)
  			{
  				return i;
  			}
  		}
  	}
          
  	return -1;
  }
  ```

  

- 方法2: KMP算法注释版

  KMP 算法是一种高效的字符串匹配算法,它可以在字符串中查找一个模式字符串的出现位置。与简单的暴力匹配算法相比,KMP 算法的时间复杂度仅为 O(n+m)，其中 n 是文本串长度,m 是模式串长度。

  KMP 算法的核心思想是利用部分匹配表(也称部分匹配值)来加速字符串的匹配过程。具体步骤如下:

  1. 预处理模式串,构建部分匹配表。部分匹配表记录了模式串中每个位置前缀和后缀的最长公共元素长度。
  2. 在文本串中滑动模式串,利用部分匹配表跳过不需要比较的字符,加快匹配过程。

  ```cpp
  class Solution {
  public:
  
      /**
      方法2: KMP算法
      **/
      int strStr(string haystack, string needle) 
      {
          // 检查子串大小
          if (needle.size() > haystack.size())
          {
              return -1;
          }
  
          /**
          // step 1: 求解匹配串的next数组
          **/
  
          // 初始化next数组, 用于储存匹配子串的每个字符代表的共同前后缀数量
          std::vector<int> next = {0};   
          // 当前字符的可能共同前后缀对应的索引
          int curr_len = 0; 
          // 起始索引, 从1开始遍历子串用于
          int index = 1;
          while (index < needle.size())
          {
              if (needle[curr_len] == needle[index])
              {
                  // 如果当前的可能共同前后缀数量索引对应的字符等于当前字符, 
                  // 说明当前的可能共同前后缀数量有效, 
                  // 将其+1表示为共同前后缀数量后添加到next数组中
                  curr_len++;
                  next.push_back(curr_len);
                  index++;
              }
              else
              {
                  // 如果当前的可能共同前后缀数量无效
                  // 则需要判断当前可能共同前后缀数量是否为0
                  if (curr_len == 0)
                  {
                      // 如果可能共同前后缀数量已经为0
                      // 将其添加到next数组中, 说明当前字符对应的共同前后缀数量为0
                      next.push_back(0);
                      index++;
                  }
                  else
                  {
                      // 如果可能共同前后缀数量无效且不为0
                      // 则去寻找之前已经匹配的共同前后缀字符串中的共同前后缀数量
                      // 用之前的共同前后缀来验证其可能性
                      curr_len = next[curr_len - 1];
                  }
              }
          }
  
          /**
          step 2: 通过子串的next数组寻找匹配子串
          **/
          int i = 0;  // 主串索引
          int j = 0;  // 子串索引
          while (i < haystack.size())
          {
              if (haystack[i] == needle[j])
              {
                  // 如果字符匹配, 主串和子串都前进一步
                  i++;
                  j++;
              }
              else if (j > 0)
              {
                  // 如果字符不匹配, 并且子串索引>0
                  // 说明子串已经有匹配的字符
                  // 在子串已经匹配的字符串中, 检查其共同前后缀的数量
                  // 则可以跳过前缀
                  j = next[j - 1];
              }
              else
              {
                  // 如果字符不匹配, 并且子串索引为0
                  // 说明子串完全没有匹配
                  // 只需要前进主串
                  i++;
              }
  
              // 判断此时子串是否已经完成匹配
              if (j == needle.size())
              {
                  // 如果子串已经完成匹配
                  // 返回主串开始此次匹配的索引
                  return i - j;
              }
          }
  
          return -1;
      }
  };
  ```





### 外观数列

「外观数列」是一个数位字符串序列，由递归公式定义：

- `countAndSay(1) = "1"`
- `countAndSay(n)` 是 `countAndSay(n-1)` 的行程长度编码。

 

[行程长度编码](https://baike.baidu.com/item/行程长度编码/2931940)（RLE）是一种字符串压缩方法，其工作原理是通过将连续相同字符（重复两次或更多次）替换为字符重复次数（运行长度）和字符的串联。例如，要压缩字符串 `"3322251"` ，我们将 `"33"` 用 `"23"` 替换，将 `"222"` 用 `"32"` 替换，将 `"5"` 用 `"15"` 替换并将 `"1"` 用 `"11"` 替换。因此压缩后字符串变为 `"23321511"`。

给定一个整数 `n` ，返回 **外观数列** 的第 `n` 个元素。

**示例 1：**

**输入：**n = 4

**输出：**"1211"

**解释：**

countAndSay(1) = "1"

countAndSay(2) = "1" 的行程长度编码 = "11"

countAndSay(3) = "11" 的行程长度编码 = "21"

countAndSay(4) = "21" 的行程长度编码 = "1211"

**示例 2：**

**输入：**n = 1

**输出：**"1"

**解释：**

这是基本情况。

 

**提示：**

- `1 <= n <= 30`



- 方法1：普通遍历法

  ```cpp
  class Solution 
  {
  public:
      std::string countAndSayRecursive(std::string in_str)
      {
          // check in_str == "0"
          if (in_str.compare("0") == 0)    
          {
              std::string out_str = "1";
              return out_str;
          }
  
          char curr_digit = in_str[0];
          int count = 1;
          std::string digits_str;
          
          // check size == 1
          if (in_str.size() == 1)
          {
                  digits_str += std::to_string(count);
                  digits_str += curr_digit;
                  return digits_str;
          }
  
          for (int i = 1; i < in_str.size(); i++)
          {
  			// 如果数字相同，计数器+1
              if (curr_digit == in_str[i])
              {
                  count++;
              }
              // 如果数字不同方法，保存计数器，保存数字，归零计数器，更新数字
              else
              {
                  digits_str += std::to_string(count);
                  digits_str += curr_digit;
                  count = 1;
                  curr_digit = in_str[i];
              }
  
              // 到达末尾
              if (i == in_str.size() - 1)
              {
                  digits_str += std::to_string(count);
                  digits_str += curr_digit;
              }
          }
  
          return digits_str;
      }
  
      string countAndSay(int n) 
      {
          std::string result = "0";
          for (int i = 0; i < n; i++)
          {
              result = countAndSayRecursive(result);
          }
  
          return result;
      }
  };
  ```





### 最长公共前缀

编写一个函数来查找字符串数组中的最长公共前缀。

如果不存在公共前缀，返回空字符串 `""`。

 

**示例 1：**

```
输入：strs = ["flower","flow","flight"]
输出："fl"
```

**示例 2：**

```
输入：strs = ["dog","racecar","car"]
输出：""
解释：输入不存在公共前缀。
```



方法1：纵向遍历法
时间复杂度：O(mn)
空间复杂度：O(1)

```CPP
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) 
    {
        // 如果前缀为空时，第一个字符串长度为0，则直接返回prefix
        std::string prefix;
        if (strs[0].size() == 0)
        {
            return prefix;
        }

        int index = 0;
        char curr_char;
        while (true)
        {
            // 检查当前index对应的所有字符是否相等
            // 获取第一个字符串的index对应字符
            // 如果超出范围则直接返回prefix
            if (index == strs[0].size())
            {
                return prefix;
            }
            curr_char = strs[0][index];

            // 检查剩余字符串的index字符
            for (int i = 1; i < strs.size(); i++)
            {
                // 如果index超出当前字符串，则返回当前prefix
                if (index == strs[i].size())
                {
                    return prefix;
                }

                // 如果当前字符串的index对应字符与第一个字符串的index字符不相等
                // 直接返回prefix
                if (strs[i][index] != curr_char)
                {
                    return prefix;
                }
            }
            prefix += curr_char;
            index++;
        }

        return prefix;
    }
};
```









## 链表

### 删除链表中的节点

有一个单链表的 `head`，我们想删除它其中的一个节点 `node`。

给你一个需要删除的节点 `node` 。你将 **无法访问** 第一个节点 `head`。

链表的所有值都是 **唯一的**，并且保证给定的节点 `node` 不是链表中的最后一个节点。

删除给定的节点。注意，删除节点并不是指从内存中删除它。这里的意思是：

- 给定节点的值不应该存在于链表中。
- 链表中的节点数应该减少 1。
- `node` 前面的所有值顺序相同。
- `node` 后面的所有值顺序相同。

**自定义测试：**

- 对于输入，你应该提供整个链表 `head` 和要给出的节点 `node`。`node` 不应该是链表的最后一个节点，而应该是链表中的一个实际节点。
- 我们将构建链表，并将节点传递给你的函数。
- 输出将是调用你函数后的整个链表。





方法1：本问题中没有上个节点的信息，所以将当前节点直接变成下个节点
时间复杂度：O(1)
空间复杂度：O(1)

```CPP
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    void deleteNode(ListNode* node) 
    {
        node->val = node->next->val;
        node->next = node->next->next;
    }
};
```





### 删除链表倒数第N个结点 双指针法

给你一个链表，删除链表的倒数第 `n` 个结点，并且返回链表的头结点。

**示例 1：**

![img](./03_leetcode_note.assets/remove_ex1.jpg)

```
输入：head = [1,2,3,4,5], n = 2
输出：[1,2,3,5]
```

**示例 2：**

```
输入：head = [1], n = 1
输出：[]
```

**示例 3：**

```
输入：head = [1,2], n = 1
输出：[1]
```



双指针法，快指针比慢指针快n-1个元素
当快指针到达链表最后一个元素时，慢指针是倒数第n个元素
时间复杂度：O(L)
空间复杂度：O(1)

```CPP
ListNode* removeNthFromEnd(ListNode* head, int n) 
{
	// 虚拟头节点, 指向真正头节点
	ListNode* dummy = new ListNode(0, head);
	ListNode* fast = head;
	ListNode* slow = dummy;

	// fast 前进n个节点
	for (int i = 0; i < n; i++) 
	{
		fast = fast->next;
	}

	// fast前进到最后一个元素， slow跟着前进
	while (fast) 
	{
		fast = fast->next;
		slow = slow->next;
	}

	// 删除第n个节点 
	slow->next = slow->next->next;

	// 返回第一个节点
	ListNode* ans = dummy->next;
	delete dummy;
	return ans;
}
```



### 反转链表

方法1：迭代法
遍历链表时，将当前节点的next指向前一个节点。
每次需要储存前一个节点和后一个节点。
时间复杂度：O(n)，其中 n 是链表的长度。需要遍历链表一次。
空间复杂度：O(1)



```CPP
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while(curr){
        // save next
        ListNode* next = curr->next;
        
        // change next
        curr->next = prev;
        
        // update prev
        prev = curr;
        
        // update current
        curr = next;
    }

    return prev;
}
```

方法2：递归法
当递归到n(k)时，将n(k+1)的下一个节点指向n(k)
需要注意n(1)的下一个节点需要指向null

```CPP
ListNode* reverseList(ListNode* head) {
	// check end node
	if(!head || !head->next){
		return head;
	}

	// get new head
	ListNode* new_head = reverseList(head->next);

	// reverse n(k+1) ->n(k)
	head->next->next = head;

	// set n(k)->nullptr, in next step it will be set to n(k-1)
	head->next = nullptr;

	// return new_head
	return new_head;
}
```





### 合并两个有序链表

将两个升序链表合并为一个新的 **升序** 链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。 

 

**示例 1：**

![img](./03_leetcode_note.assets/merge_ex1.jpg)

```
输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]
```

**示例 2：**

```
输入：l1 = [], l2 = []
输出：[]
```

**示例 3：**

```
输入：l1 = [], l2 = [0]
输出：[0]
```



方法1：递归法
中止条件：当两个链表当前指针都指向空时，说明合并完成
递归操作：判断l1与l2的当前节点哪个小，则小的节点的next指向其余节点的合并结果
时间复杂度：O(m+n)
空间复杂度：O(m+n)

```CPP
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
	// check list1 null
	if(list1 == NULL){
		return list2;
	}

	// check list2 null
	if (list2 == NULL){
		return list1;
	}

	// if list1 <= list2, merge list1->next and list2, then return list1
	if (list1->val <= list2->val){
		list1->next = mergeTwoLists(list1->next, list2);
		return list1;
	}

	// if list2 < list1, merge list1 and list2->next, then return list2
	list2->next = mergeTwoLists(list1, list2->next);
	return list2;
}
```

方法2：迭代法
时间复杂度：O(n+m)
空间复杂度：O(1)

```CPP
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
	// 设置共同节点，用于最开始的合并
	ListNode* preHead = new ListNode(-1);
	ListNode* prev = preHead;

	// 遍历两个链表，直到其中一个链表结束
	while (l1 != nullptr && l2 != nullptr) {
		if (l1->val < l2->val) {
			// l1<l2, 合并l1
			prev->next = l1;

			// l1前进
			l1 = l1->next;
		} 
        else
        {
			// l1>=l2, 合并l2
			prev->next = l2;

			// l2前进
			l2 = l2->next;
		}

		// prev前进
		prev = prev->next;
	}

	// 合并剩下的链表
	prev->next = l1 == nullptr ? l2 : l1;

	// 返回链表头节点
	return preHead->next;
}
```



### 回文链表 数组双指针法 递归法

给你一个单链表的头节点 `head` ，请你判断该链表是否为

回文链表

。如果是，返回 `true` ；否则，返回 `false` 。



 

**示例 1：**

![img](./03_leetcode_note.assets/pal1linked-list.jpg)

```
输入：head = [1,2,2,1]
输出：true
```

**示例 2：**

![img](./03_leetcode_note.assets/pal2linked-list.jpg)

```
输入：head = [1,2]
输出：false
```

 

**提示：**

- 链表中节点数目在范围`[1, 105]` 内
- `0 <= Node.val <= 9`

 

**进阶：**你能否用 `O(n)` 时间复杂度和 `O(1)` 空间复杂度解决此题？



方法1：复制到数组后双指针法检查
时间复杂度：O(N)
空间复杂度：O(N)

```CPP
bool isPalindrome(ListNode* head) 
{
	// copy data into vector
	std::vector<int> data;
	ListNode* current_node = head;
	while (current_node != nullptr)
	{
		data.emplace_back(current_node->val);
		current_node = current_node->next;
	}

	// check palindorme
	for (int i = 0; i < data.size()/2; i++)
	{
		if (data[i] != data[data.size() - 1 - i])
		{
			return false;
		}
	}

	return true;
}
```



方法2：递归法
保存head为front，
head 递归遍历到end
**解递归的过程可以视为 head从end返回，并与front比较，随后移动front到下一个**
如果front与head此时的值不同，则说明不是回文

时间复杂度：O(N)
空间复杂度：O(1)

```CPP
class Solution {
public:
	ListNode* front;

	bool isPalindrome(ListNode* head) 
	{
		front = head;
		return checkPalindromeRecursively(head);
	}

	bool checkPalindromeRecursively(ListNode* current_node)
	{
		// list not end
		if (current_node != nullptr)
		{
			// current to next, check palindrome
			// return false if false
			if (!checkPalindromeRecursively(current_node->next))
			{
				return false;
			}

			// val not equal, return false
			if (current_node->val != front->val)
			{
				return false;
			}

			// front to next
			front = front->next;
		}

		// list end, return true
		return true;
	}
};
```



### 环形链表

给你一个链表的头节点 `head` ，判断链表中是否有环。

如果链表中有某个节点，可以通过连续跟踪 `next` 指针再次到达，则链表中存在环。 为了表示给定链表中的环，评测系统内部使用整数 `pos` 来表示链表尾连接到链表中的位置（索引从 0 开始）。**注意：`pos` 不作为参数进行传递** 。仅仅是为了标识链表的实际情况。

*如果链表中存在环* ，则返回 `true` 。 否则，返回 `false` 。

 

**示例 1：**

![img](./03_leetcode_note.assets/circularlinkedlist.png)

```
输入：head = [3,2,0,-4], pos = 1
输出：true
解释：链表中有一个环，其尾部连接到第二个节点。
```

**示例 2：**

![img](./03_leetcode_note.assets/circularlinkedlist_test2.png)

```
输入：head = [1,2], pos = 0
输出：true
解释：链表中有一个环，其尾部连接到第一个节点。
```

**示例 3：**

![img](./03_leetcode_note.assets/circularlinkedlist_test3.png)

```
输入：head = [1], pos = -1
输出：false
解释：链表中没有环。
```



方法1：哈希表储存节点，判断是否重复
时间复杂度：O(N)
空间复杂度：O(N)

```CPP
bool hasCycle(ListNode *head) 
{
	std::unordered_set<ListNode*> seen;

	while(head != nullptr)
	{
		if (seen.count(head))
		{
			return true;
		}

		seen.insert(head);
		head = head->next;
	}

	return false;
}
```



方法2：快慢指针
时间复杂度：O(N)
空间复杂度：O(1)
原理：
如果没有环，快指针直接到尾部
如果有环，快指针与慢指针相遇

快指针一次走两步

```CPP
bool hasCycle(ListNode *head) 
{
	// check head nullptr
	if (head == nullptr || head->next == nullptr)
	{
		return false;
	}

	// fast 
	ListNode* slow = head;
	ListNode* fast = head->next; // 刚开始快指针比慢指针快一步
	while(slow != fast) 
	{
        // 快指针到达末尾, 没有环
		if (fast == nullptr || fast->next == nullptr)
		{
			return false;
		}
        
        // 更新快慢指针
		slow = slow->next;
		fast = fast->next->next;	// 快指针比慢指针多走一步
	}

	// slow = fast
	return true;
}
```



## 二叉树



### 二叉树最大深度

给定一个二叉树 `root` ，返回其最大深度。

二叉树的 **最大深度** 是指从根节点到最远叶子节点的最长路径上的节点数。

 

**示例 1：**

![img](./03_leetcode_note.assets/tmp-tree.jpg)

 

```
输入：root = [3,9,20,null,null,15,7]
输出：3
```

**示例 2：**

```
输入：root = [1,null,2]
输出：2
```





方法1：递归法 深度优先搜索
比较每个节点左和右的深度，返回更深的深度+1
时间复杂度：O(N)
空间复杂度：O(height)

```cpp
int maxDepth(TreeNode* root) 
{
	if (root == nullptr)
	{
			return 0;
	}

	return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}
```



方法2：广度优先搜索
时间复杂度：O(N)
空间复杂度：最坏情况 O(N)
把每一层的node加入queue，
每加一层，depth+1
检查在queue中的每个节点是否能加入后续节点
并弹出该层的节点，加入后续节点
直到queue中没有节点，此时的深度为最大深度

```CPP
int maxDepth(TreeNode* root) 
{
	if (root == nullptr) return 0;

    // 队列用来保存当前层的节点
	std::queue<TreeNode*> q;
	q.push(root);
	int depth = 0;

    // 遍历队列中所有待探索的节点
	while(!q.empty())
	{
        // 遍历当前层
		int size = q.size();
		while (size > 0)
		{
			// 队列弹出一个节点
			TreeNode* node = q.front();
			q.pop();
			size--;

			// 加入两个子节点
			if (node->left)
			{
				q.push(node->left);
			}
			if (node->right)
			{
				q.push(node->right);
			}
		}

        // 层数+1
		depth += 1;
	}

	return depth;
}
```



### 验证有效二叉树

给你一个二叉树的根节点 `root` ，判断其是否是一个有效的二叉搜索树。

**有效** 二叉搜索树定义如下：

- 节点的左子树只包含小于当前节点的数。
- 节点的右子树只包含大于当前节点的数。
- 所有左子树和右子树自身必须也是二叉搜索树。

 

**示例 1：**

![img](./03_leetcode_note.assets/tree1.jpg)

```
输入：root = [2,1,3]
输出：true
```

**示例 2：**

![img](./03_leetcode_note.assets/tree2.jpg)

```
输入：root = [5,1,4,null,null,3,6]
输出：false
解释：根节点的值是 5 ，但是右子节点的值是 4 。
```



方法1：递归法判断
时间复杂度：O(N)
空间复杂度：O(N)

需要考虑上下界

```CPP
bool helper(TreeNode* root, long long lower, long long upper) {
    // check nullptr
    if (root == nullptr) 
    {
        return true;
    }

    // 比下界小，或比上界大，就返回false
    if (root -> val <= lower || root -> val >= upper) {
        return false;
    }

    // 当前节点作为根节点
    // 左子树需要小于当前节点
    // 右子树需要大于当前节点
    return helper(root -> left, lower, root -> val) && helper(root -> right, root -> val, upper);
}

bool isValidBST(TreeNode* root) {
    return helper(root, LONG_MIN, LONG_MAX);
}
```



方法2：中序遍历法
有效二叉搜索树「中序遍历」得到的值构成的序列一定是升序的

```CPP
bool isValidBST(TreeNode* root) 
{
    stack<TreeNode*> stack;
    long long inorder = (long long)INT_MIN - 1;

    // 只要 root 不是 nullptr 或者 stack 不是空，就执行以下内容
    while (!stack.empty() || root != nullptr) 
    {
        // 将root左分支加入stack
        while (root != nullptr) 
        {
            stack.push(root);
            root = root -> left;
        }
        
        // 获取stack top
        root = stack.top();
        stack.pop();

        // 如果中序遍历得到的节点的值小于等于前一个 inorder，说明不是二叉搜索树
        if (root -> val <= inorder) {
            return false;
        }

        inorder = root -> val;
        root = root -> right;
    }
    return true;
}
```



方法3: 递归中序遍历 查看结果是不是升序

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:

    void MiddleRecursive(TreeNode* root, std::vector<int>& vec)
    {
        if (root == nullptr)
        {
            return;
        }

        MiddleRecursive(root->left, vec);
        vec.push_back(root->val);
        MiddleRecursive(root->right, vec);
    }

    bool isValidBST(TreeNode* root) {
        if (root == nullptr)
        {
            return true;
        }

        std::vector<int> vec;
        MiddleRecursive(root, vec);
        if (vec.size() == 1)
        {
            return true;
        }

        int i = 0;
        int j = 1;
        while (j < vec.size())
        {
            if (vec[i] >= vec[j])
            {
                return false;
            }

            i++;
            j++;
        }

        return true;
    }
};
```





### 对称二叉树

给你一个二叉树的根节点 `root` ， 检查它是否轴对称。

 

**示例 1：**

![img](./03_leetcode_note.assets/1698026966-JDYPDU-image.png)

```
输入：root = [1,2,2,3,4,4,3]
输出：true
```

**示例 2：**

![img](./03_leetcode_note.assets/1698027008-nPFLbM-image.png)



方法1：递归法
时间复杂度：O(N)
空间复杂度：O(N)

```CPP
bool isSymmetric(TreeNode* root) {
	return checkSymmetric(root->left, root->right);
}

bool checkSymmetric(TreeNode* left, TreeNode* right)
{
	// check nullptr
	if (left == nullptr && right == nullptr)
	{
		return true;
	}
	if (left == nullptr && right != nullptr)
	{
		return false;
	}
	if (left != nullptr && right == nullptr)
	{
		return false;
	}

	// check current node
	if (left->val != right->val)
	{
		return false;
	}

	// check next level node
	if (!checkSymmetric(left->left, right->right))
	{
		return false;
	}
	if (!checkSymmetric(left->right, right->left))
	{
		return false;
	}

	return true;
}
```



方法2：迭代法
时间复杂度：O(N)
空间复杂度：O(N)

```CPP
bool isSymmetric(TreeNode* root) 
{
	std::queue<TreeNode*> q;

    // 一次压入两个对称的节点, 比较对称节点的值, 并继续压入对称的子节点, 注意顺序
	q.push(root);
	q.push(root);

	while(!q.empty())
	{
		TreeNode* u = q.front();
		q.pop();
		TreeNode* v = q.front();
		q.pop();

		// check both nullptr
		if (!u && !v)
		{
		continue;
		}

		// check not equal
		if ((!u || !v) ||              // only one nullptr
		(u->val != v->val)   // not equal
		)
		{
			return false;
		}

		// add next two node
		q.push(u->left);
		q.push(v->right);
		q.push(u->right);
		q.push(v->left);
	}
	return true;
}
```



### 二叉树层序遍历

给你二叉树的根节点 `root` ，返回其节点值的 **层序遍历** 。 （即逐层地，从左到右访问所有节点）。



方法1：迭代法广度优先
每次加一层节点到queue，
从queue拿出所有节点，保存当前层所有数值，并把下一层所有节点记录到queue
直到queue为空
时间复杂度：O(N)
空间复杂度：O(N)

```CPP
vector<vector<int>> levelOrder(TreeNode* root) 
{
	std::vector<std::vector<int>> tree_in_level_vec;
	std::queue<TreeNode*> q;

	// check root nullptr
	if (root == nullptr)
	{
		return tree_in_level_vec;
	}

	// start to get levels
	q.push(root);
	while(!q.empty())
	{
		std::vector<int> current_level_vec;
		std::queue<TreeNode*> q_next_level;
		while(!q.empty())
		{
			// add val to current level
			TreeNode* current_node = q.front();
			q.pop();
			current_level_vec.push_back(current_node->val);

			// save left to next level
			if (current_node->left != nullptr)
			{
				q_next_level.push(current_node->left);
			}

			// save right to next level
			if (current_node->right != nullptr)
			{
				q_next_level.push(current_node->right);
			}   
		}

		// get current_level_vec
		tree_in_level_vec.push_back(current_level_vec);

		// get q_next_level
		q = q_next_level;
	}

	return tree_in_level_vec;
}
```



方法2：递归法深度优先

这个方法需要记住目前在哪一层

时间复杂度：O(N)
空间复杂度：O(h)

```CPP
vector<vector<int>> levelOrder(TreeNode* root) 
{
	std::vector<vector<int>> tree_in_level;

	GetLevelInOrderRecursively(root, tree_in_level, 1);

	return tree_in_level;
}

void GetLevelInOrderRecursively(
	TreeNode* current_node, 
	std::vector<std::vector<int>>& tree_in_level, 
	int depth)
{
	// check nullptr
	if (current_node == nullptr)
	{
		return;
	}

	// check if current level exists
	if (depth > tree_in_level.size())
	{
		std::vector<int> current_level;
		tree_in_level.push_back(current_level);
	}

	// add val to current level
	tree_in_level[depth-1].push_back(current_node->val);

	// check next left node
	GetLevelInOrderRecursively(current_node->left, tree_in_level, depth+1);

	// check next right node
	GetLevelInOrderRecursively(current_node->right, tree_in_level, depth+1);
}
```



### 平衡二叉树

给你一个整数数组 `nums` ，其中元素已经按 **升序** 排列，请你将其转换为一棵 

平衡

 二叉搜索树。



 

**示例 1：**

![img](./03_leetcode_note.assets/btree1.jpg)

```
输入：nums = [-10,-3,0,5,9]
输出：[0,-3,9,-10,null,5]
解释：[0,-10,5,null,-3,null,9] 也将被视为正确答案：
```

**示例 2：**

![img](./03_leetcode_note.assets/btree.jpg)

```
输入：nums = [1,3]
输出：[3,1]
解释：[1,null,3] 和 [3,1] 都是高度平衡二叉搜索树。
```





方法1：递归法
每次将数组中间的值作为根节点的值，数组左侧作为左子树，右侧作为右子树
时间复杂度：O(N)
空间复杂度：O(log(N))

```CPP
TreeNode* sortedArrayToBST(vector<int>& nums) {
	return arrayToTreeRecursively(nums, 0, nums.size() - 1);
}

TreeNode* arrayToTreeRecursively(vector<int>& nums, int left, int right) {
	// check bad range
	if (left > right) {
		return nullptr;
	}

	// set middle node
	// middle val can be left, right or random
	int mid = (left + right) / 2;
	TreeNode* root = new TreeNode(nums[mid]);

	// set left tree
	root->left = arrayToTreeRecursively(nums, left, mid - 1);

	// set right tree
	root->right = arrayToTreeRecursively(nums, mid + 1, right);
	return root;
}
```



### 二叉树纵向求和

假如有一棵树

```
      1
    /  \
  3     5
 / \    /
4  2 9
```

求和规则

134  + 132 + 159



- 思路
  - 用递归前序遍历
  - 递归时需要同时传递 temp_sum 和 total_sum
  - temp_sum 每深入一层
    - temp_sum * 10 + val
  - temp_sum 每返回一层 需要恢复成前一次的数据
    - temp_sum - val
    - temp_sum / 10
  - 当出现末端节点时, 需要将temp_sum 加入total_sum

```CPP
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_set>

// tree node
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr){};
    TreeNode(int x) : val(x), left(nullptr), right(nullptr){};
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right){};
};

void get_tree_sum(TreeNode *node, int &temp_sum, int &total_sum)
{
    // check nullptr
    if (node == nullptr)
    {
        return;
    }

    // 计算当前临时和
    temp_sum *= 10;
    temp_sum += node->val;

    // 末端叶子, 加入总和 
    if (node->left == nullptr && node->right == nullptr)
    {
        std::cout << "temp_sum: " << temp_sum << std::endl;
        total_sum += temp_sum;
    }
    // 不是末端叶子, 依次跑左子树和右子树
    else
    {
        get_tree_sum(node->left, temp_sum, total_sum);
        get_tree_sum(node->right, temp_sum, total_sum);
    }

    // 当前节点跑完后, 将sum恢复到上一个状态
    temp_sum -= node->val;
    temp_sum /= 10;
}

int main()
{
    // 构建二叉树
    /*
              1
            /  \
          3     5
         / \    /
        4  2 9
    */
    TreeNode *node_1 = new TreeNode(1);
    TreeNode *node_2 = new TreeNode(3);
    TreeNode *node_3 = new TreeNode(5);
    TreeNode *node_4 = new TreeNode(4);
    TreeNode *node_5 = new TreeNode(2);
    TreeNode *node_6 = new TreeNode(9);
    node_1->left = node_2;
    node_1->right = node_3;
    node_2->left = node_4;
    node_2->right = node_5;
    node_3->left = node_6;

    int sum = 0;
    int total_sum = 0;
    get_tree_sum(node_1, sum, total_sum);

    std::cout << "total sum: " << total_sum << std::endl;

    return 0;
}
```









## 动态规划

动态规划是一种解决复杂问题的有效方法。它通过将问题分解成更小的子问题并且逐步解决这些子问题来得到最终的解决方案。这种方法通常可以大大提高问题的求解效率。

动态规划的一般步骤如下:

1. **定义子问题**: 将原问题分解成一系列相关的子问题。这些子问题应该是相互独立的,并且可以通过前一个子问题的结果得到下一个子问题的解决。
2. **建立递推关系**: 找到子问题之间的递推关系,即如何利用前一个子问题的解决结果来得到下一个子问题的解。这个递推关系是动态规划的核心。
3. **计算最优解**: 根据前面建立的递推关系,自底向上或自顶向下地计算出原问题的最优解。这一步通常需要使用数组或表格来存储和更新中间结果。
4. **追溯最优解**: 如果不仅需要得到最优值,还需要得到最优方案,则需要额外的步骤来追踪最优解的路径。



### 爬楼梯

假设你正在爬楼梯。需要 `n` 阶你才能到达楼顶。

每次你可以爬 `1` 或 `2` 个台阶。你有多少种不同的方法可以爬到楼顶呢？

 

**示例 1：**

```
输入：n = 2
输出：2
解释：有两种方法可以爬到楼顶。
1. 1 阶 + 1 阶
2. 2 阶
```

**示例 2：**

```
输入：n = 3
输出：3
解释：有三种方法可以爬到楼顶。
1. 1 阶 + 1 阶 + 1 阶
2. 1 阶 + 2 阶
3. 2 阶 + 1 阶
```



方法1：动态规划
假设有x级台阶，最后一步可以走一级或二级
**转移方程：**
f(x) = f(x-1) + f(x-2)

这里f(x) 表示x级台阶一共有的走法

x级台阶的走法 = x-1 级台阶的走法 + x-2 级台阶的走法



**边界条件：**
0级台阶的走法 f(0) = 1
1级台阶的走法 f(1) = 1



**后续计算：**
2级台阶的走法 f(2) = f(1) + f(0) = 2
3级台阶的走法 f(3) = f(2) + f(1) = 3
4级台阶的走法 f(4) = f(3) + f(2) = 5
...
时间复杂度：O(n)
空间复杂度：O(1)

```cpp
int climbStairs(int n) {
	int k1 = 0; // 0级台阶的走法
	int k2 = 1; // 1级台阶的走法
	int k3;	// 2级台阶的走法

    // 后续台阶走法等于前一级和前两级走法之和
	for (int i = 1; i <= n; i++)
	{
		k3 = k1 + k2;
		k1 = k2;
		k2 = k3;
	}

	return k3;
}
```





### 买卖股票最佳时机

这题的股票只能买卖一次

给定一个数组 `prices` ，它的第 `i` 个元素 `prices[i]` 表示一支给定股票第 `i` 天的价格。

你只能选择 **某一天** 买入这只股票，并选择在 **未来的某一个不同的日子** 卖出该股票。设计一个算法来计算你所能获取的最大利润。

返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 `0` 。

 

**示例 1：**

```
输入：[7,1,5,3,6,4]
输出：5
解释：在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。
     注意利润不能是 7-1 = 6, 因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。
```

**示例 2：**

```
输入：prices = [7,6,4,3,1]
输出：0
解释：在这种情况下, 没有交易完成, 所以最大利润为 0。
```



方法1：遍历法 动态规划
如果当前点更低，更新最低点
否则计算当前利润，比较是否大于历史最大利润，并更新最大利润

```cpp
int maxProfit(vector<int>& prices) {
	int min = prices[0];
	int max_profit = 0;
	for(int i = 0; i < prices.size(); i++)
	{
        // 如果当前点更低，更新最低点
		if (prices[i] < min)
		{
			min = prices[i];
		}
        // 否则计算当前利润，比较是否大于历史最大利润，并更新最大利润
		else if (max_profit < prices[i] - min)
		{
			max_profit = prices[i] - min;
		}
	}

	return max_profit;
}
```



### 最大子数组和

给你一个整数数组 `nums` ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。



**子数组**

是数组中的一个连续部分。



**示例 1：**

```
输入：nums = [-2,1,-3,4,-1,2,1,-5,4]
输出：6
解释：连续子数组 [4,-1,2,1] 的和最大，为 6 。
```

**示例 2：**

```
输入：nums = [1]
输出：1
```

**示例 3：**

```
输入：nums = [5,4,-1,7,8]
输出：23
```



方法1：动态规划

求到每个数为止的和, 当前和 = 前一次的和 + 当前值



**初始条件**

dp(0) = val(0)



**状体转移**

if dp(i-1) <= 0

​	dp(i) = val(i)

else

​	dp(i) = dp(i-1) + val(i)



**DP 最大结果**

mac_profit = max(dp)



如果到前一个数的和不大于0，
则当前数就是它本身的连续最大和
时间复杂度：O(N)
空间复杂度：O(N)

```cpp
int maxSubArray(vector<int>& nums) 
{
	int len = nums.size();

	// 定义子问题：
	// dp[i] 表示：以 nums[i] 结尾的连续子数组的最大和
	int dp[len];
	dp[0] = nums[0];

	// 实现转移方程：
	for (int i = 1; i < len; i++)
	{
		if (dp[i-1] > 0)
		{
			dp[i] = dp[i-1] + nums[i];
		}
		else
		{
			dp[i] = nums[i];
		}
	}

	// 初始状态
	int res = nums[0];
	for(int i = 0; i < len; i++)
	{
		res = std::max(res, dp[i]);
	}

	return res;
}
```



方法2：空间优化后的动态规划

```cpp
 int maxSubArray(vector<int>& nums) 
 {
	 int pre = 0;
	 int res = nums[0];

	for(int i = 0; i < nums.size(); i++)
	{
		// update pre
		pre = std::max(pre + nums[i], nums[i]);

		// update res
		res = std::max(res, pre);
	}

	return res;
 }
```



### 打家劫舍

你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，**如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警**。

给定一个代表每个房屋存放金额的非负整数数组，计算你 **不触动警报装置的情况下** ，一夜之内能够偷窃到的最高金额。

 

**示例 1：**

```
输入：[1,2,3,1]
输出：4
解释：偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。
     偷窃到的最高金额 = 1 + 3 = 4 。
```

**示例 2：**

```
输入：[2,7,9,3,1]
输出：12
解释：偷窃 1 号房屋 (金额 = 2), 偷窃 3 号房屋 (金额 = 9)，接着偷窃 5 号房屋 (金额 = 1)。
     偷窃到的最高金额 = 2 + 9 + 1 = 12 。
```



方法1：动态规划法
问题：不相邻的情况下，从全部房子的最大金额，
子问题：从k个房子中偷到的最大金额，f(k)



**转移公式：**

K个房子中偷到的 = MAX{k-2 个房子中偷到的+第k个房子的, k-1个房子中偷到的}

f(k) = max{f(k-2) + H_{k-1}, f(k-1)}

**初始状态:**

f(k=0) = 0	// 0个房子中偷到的
f(k=1) = H_0   // 1个房子中偷到的



方法1：动态规划法
问题：不相邻的情况下，从全部房子的最大金额，
子问题：从k个房子中偷到的最大金额，f(k)
转移公式：
f(k) = max{f(k-2) + H_{k-1}, f(k-1)}
f(k=0) = 0
f(k=1) = H_0

```cpp
int rob(vector<int>& nums) {
    if (nums.size() == 0)
    {
        return 0;
    }

    // sub task
    // f(0) = 0
    // f(1) = nums[0]
    // f(k) = max(rob(k-1), nums[k-1] + rob(k-2))
    int N = nums.size();
    std::vector<int> dp(N+1, 0);
    dp[0] = 0;
    dp[1] = nums[0];

    for(int k = 2; k <= N; k++)
    {
        dp[k] = std::max(dp[k-1], nums[k-1] + dp[k-2]);
    }

    return dp[N];
}
```



方法2：空间优化后的动态规划
用两个数代替dp数组

```CPP
int rob(vector<int>& nums)
{
    int prev = 0;
    int curr = 0;

    // 循环计算偷到当前房子的金额
    for (int num : nums)
    {
        int temp = max(curr, prev + num);
        prev = curr;        // update prev dp[k-2]
        curr = temp;      // update curr dp[k-1]
    }

    return curr;
}
```



## 设计问题



### 打乱数组

给你一个整数数组 `nums` ，设计算法来打乱一个没有重复元素的数组。打乱后，数组的所有排列应该是 **等可能** 的。

实现 `Solution` class:

- `Solution(int[] nums)` 使用整数数组 `nums` 初始化对象
- `int[] reset()` 重设数组到它的初始状态并返回
- `int[] shuffle()` 返回数组随机打乱后的结果

**示例 1：**

```
输入
["Solution", "shuffle", "reset", "shuffle"]
[[[1, 2, 3]], [], [], []]
输出
[null, [3, 1, 2], [1, 2, 3], [1, 3, 2]]

解释
Solution solution = new Solution([1, 2, 3]);
solution.shuffle();    // 打乱数组 [1,2,3] 并返回结果。任何 [1,2,3]的排列返回的概率应该相同。例如，返回 [3, 1, 2]
solution.reset();      // 重设数组到它的初始状态 [1, 2, 3] 。返回 [1, 2, 3]
solution.shuffle();    // 随机返回数组 [1, 2, 3] 打乱后的结果。例如，返回 [1, 3, 2]
```

时间复杂度：
初始化：O(n)，其中 n 为数组中的元素数量。我们需要 O(n) 来初始化 nums
reset：O(n)。我们需要 O(n) 将 original 复制到 nums。
shuffle：O(n)。我们只需要遍历 n 个元素即可打乱数组。
空间复杂度：O(n)。记录初始状态需要存储 n 个元素。

```cpp
class Solution {
public:
    Solution(vector<int>& nums) {
        // init nums
        this->nums = nums;

        // copy to original
        this->original.resize(nums.size());
        std::copy( this->nums.begin(), this->nums.end(), this->original.begin());
    }
    
    vector<int> reset() {
        // copy original to nums
        std::copy(this->original.begin(), this->original.end(), this->nums.begin());

        return this->nums;
    }
    
    // Fisher-Yates 洗牌算法
    vector<int> shuffle() {
        for (int i = 0; i < nums.size(); i++)
        {
            int j = i + rand()%(nums.size() - i);
            swap(nums[i], nums[j]);
         }
        return nums;
    }

private:
    std::vector<int> nums;
    std::vector<int> original;
};
```



### 最小栈

设计一个支持 `push` ，`pop` ，`top` 操作，**并能在常数时间内检索到最小元素的栈**。

实现 `MinStack` 类:

- `MinStack()` 初始化堆栈对象。
- `void push(int val)` 将元素val推入堆栈。
- `void pop()` 删除堆栈顶部的元素。
- `int top()` 获取堆栈顶部的元素。
- `int getMin()` 获取堆栈中的最小元素。

 

**示例 1:**

```
输入：
["MinStack","push","push","push","getMin","pop","top","getMin"]
[[],[-2],[0],[-3],[],[],[],[]]

输出：
[null,null,null,null,-3,null,0,-2]

解释：
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin();   --> 返回 -3.
minStack.pop();
minStack.top();      --> 返回 0.
minStack.getMin();   --> 返回 -2.
```



维护正式栈的同时，维护一个辅助栈
**辅助栈中储存的是当前的最小值**
当正式栈中压入的值小于当前最小值时，压入辅助栈的是最新的最小值

```cpp
class MinStack {
private:
    stack<int> x_stack;
    stack<int> min_stack;

public:
    MinStack() {
        // 初始化辅助栈
        min_stack.push(INT_MAX);
    }
    
    void push(int val) {
        // 压入正式栈
        x_stack.push(val);

        // 压入辅助栈，判断是否要更新当前的最小值
        min_stack.push(min(min_stack.top(), val));
    }
    
    void pop() {
        // 弹出正式栈
        x_stack.pop();

        // 弹出辅助栈
        min_stack.pop();
    }
    
    int top() {
        // 返回正式栈的顶部
        return x_stack.top();
    }
    
    int getMin() {
        // 返回辅助栈顶部的最小值
        return min_stack.top();
    }
};
```





## 搜索

### 二分法查找第一个错误版本

你是产品经理，目前正在带领一个团队开发新的产品。不幸的是，你的产品的最新版本没有通过质量检测。由于每个版本都是基于之前的版本开发的，所以错误的版本之后的所有版本都是错的。

假设你有 `n` 个版本 `[1, 2, ..., n]`，你想找出导致之后所有版本出错的第一个错误的版本。

你可以通过调用 `bool isBadVersion(version)` 接口来判断版本号 `version` 是否在单元测试中出错。实现一个函数来查找第一个错误的版本。你应该尽量减少对调用 API 的次数。

 

**示例 1：**

```
输入：n = 5, bad = 4
输出：4
解释：
调用 isBadVersion(3) -> false 
调用 isBadVersion(5) -> true 
调用 isBadVersion(4) -> true
所以，4 是第一个错误的版本。
```

**示例 2：**

```
输入：n = 1, bad = 1
输出：1
```



- 二分法
  - 注意确认最后查找边界
  - 时间复杂度：O(log(N))
  - 空间复杂度：O(1)
  - 可以画图理解

```CPP
int firstBadVersion(int n) {
	
    long mid = -1;
	long left = 1;
	long right = n;

	// check only one version
	if (left == right)
	{
		return left;
	}

	while(true)
	{
		// right is first bad version
		if (mid == (left + right)/2)
		{
			return right;
		}

		// update mid
		mid = (left + right)/2;

		// update next search area left area
		if (isBadVersion(mid))
		{
			right = mid;
		}

		// update next search right area
		else
		{
			left = mid;
		}
	}
}
```

