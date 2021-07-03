# Huffman
使用Huffman对字符串进行编码和解码

使用方法

```C++
#include "Huffman.h"

Huffman test;
std::string str="hello world";
std::string ans=test.encode(str);//得到编码后字符串
std::string bef=test.decode(ans);//得到解码前字符串

//已经在头部写入了字符频率表用于构造哈夫曼树。
//对于较小数据可能变大
//对于只有一种字符会出问题，因此这两种情况暂且请勿使用。
```

