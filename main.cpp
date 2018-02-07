#include <iostream>
#include "SkipList.h"

using namespace std;

int main() {

    cout << "skiplist test now starts..." << endl;

    //给一个最大的整型值
    SkipList<int, int> skipList(0x7fffffff);

    int length = 10;

    for (int i = 1; i <= length; ++i) {
        skipList.insert(i, i + 200);
    }

    cout << "The number of elements in skiplist is:" << skipList.size() << endl;

    if (skipList.size() != length) {
        cout << "insert failur." << endl;
    } else {
        cout << "insert success." << endl;
    }

    //测试查找
    int value = -1;
    int key = 9;
    Node<int, int> *searchResult = skipList.search(key);
    if (searchResult != nullptr) {
        value = searchResult->getValue();
        cout << "search result for key " << key << ":" << value << endl;
    } else {
        cout << "search failure for key " << key << endl;
    }

    //重置value
    value = -1;

    //测试移除,测试不通过
    key = 6;
    cout<<endl<<"start to remove"<<endl;
    bool removeResult = skipList.remove(key, value);
    if (removeResult) {
        cout << "removed node whose key is " << key << " and value is " << value << endl;
    } else {
        cout << "removed failure" << endl;
    }


    return 0;
}

/**
 //如下为一次正确的输出
 /Users/wangallen/CLionProjects/SkipListPro/cmake-build-debug/SkipListPro
skiplist test now starts...
key:1,value:201,nodeLevel:14
finally nodeLevel:1
node->key:1,node->value:201
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
----------------------------

key:2,value:202,nodeLevel:9
finally nodeLevel:2
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
----------------------------

key:3,value:203,nodeLevel:10
finally nodeLevel:3
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:3,value:203
forward[1]:key:3,value:203
forward[2]:key:3,value:203
----------------------------
node->key:3,node->value:203
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
forward[3]:key:2147483647,value:0
----------------------------

key:4,value:204,nodeLevel:11
finally nodeLevel:4
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:3,value:203
forward[1]:key:3,value:203
forward[2]:key:3,value:203
----------------------------
node->key:3,node->value:203
forward[0]:key:4,value:204
forward[1]:key:4,value:204
forward[2]:key:4,value:204
forward[3]:key:4,value:204
----------------------------
node->key:4,node->value:204
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
forward[3]:key:2147483647,value:0
forward[4]:key:2147483647,value:0
----------------------------

key:5,value:205,nodeLevel:2
finally nodeLevel:2
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:3,value:203
forward[1]:key:3,value:203
forward[2]:key:3,value:203
----------------------------
node->key:3,node->value:203
forward[0]:key:4,value:204
forward[1]:key:4,value:204
forward[2]:key:4,value:204
forward[3]:key:4,value:204
----------------------------
node->key:4,node->value:204
forward[0]:key:5,value:205
forward[1]:key:5,value:205
forward[2]:key:5,value:205
forward[3]:key:2147483647,value:0
forward[4]:key:2147483647,value:0
----------------------------
node->key:5,node->value:205
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
----------------------------

attention
key:6,value:206,nodeLevel:11
finally nodeLevel:5
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:3,value:203
forward[1]:key:3,value:203
forward[2]:key:3,value:203
----------------------------
node->key:3,node->value:203
forward[0]:key:4,value:204
forward[1]:key:4,value:204
forward[2]:key:4,value:204
forward[3]:key:4,value:204
----------------------------
node->key:4,node->value:204
forward[0]:key:5,value:205
forward[1]:key:5,value:205
forward[2]:key:5,value:205
forward[3]:key:6,value:206
forward[4]:key:6,value:206
----------------------------
node->key:5,node->value:205
forward[0]:key:6,value:206
forward[1]:key:6,value:206
forward[2]:key:6,value:206
----------------------------
node->key:6,node->value:206
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
forward[3]:key:2147483647,value:0
forward[4]:key:2147483647,value:0
forward[5]:key:2147483647,value:0
----------------------------

key:7,value:207,nodeLevel:9
finally nodeLevel:6
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:3,value:203
forward[1]:key:3,value:203
forward[2]:key:3,value:203
----------------------------
node->key:3,node->value:203
forward[0]:key:4,value:204
forward[1]:key:4,value:204
forward[2]:key:4,value:204
forward[3]:key:4,value:204
----------------------------
node->key:4,node->value:204
forward[0]:key:5,value:205
forward[1]:key:5,value:205
forward[2]:key:5,value:205
forward[3]:key:6,value:206
forward[4]:key:6,value:206
----------------------------
node->key:5,node->value:205
forward[0]:key:6,value:206
forward[1]:key:6,value:206
forward[2]:key:6,value:206
----------------------------
node->key:6,node->value:206
forward[0]:key:7,value:207
forward[1]:key:7,value:207
forward[2]:key:7,value:207
forward[3]:key:7,value:207
forward[4]:key:7,value:207
forward[5]:key:7,value:207
----------------------------
node->key:7,node->value:207
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
forward[3]:key:2147483647,value:0
forward[4]:key:2147483647,value:0
forward[5]:key:2147483647,value:0
forward[6]:key:2147483647,value:0
----------------------------

key:8,value:208,nodeLevel:7
finally nodeLevel:7
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:3,value:203
forward[1]:key:3,value:203
forward[2]:key:3,value:203
----------------------------
node->key:3,node->value:203
forward[0]:key:4,value:204
forward[1]:key:4,value:204
forward[2]:key:4,value:204
forward[3]:key:4,value:204
----------------------------
node->key:4,node->value:204
forward[0]:key:5,value:205
forward[1]:key:5,value:205
forward[2]:key:5,value:205
forward[3]:key:6,value:206
forward[4]:key:6,value:206
----------------------------
node->key:5,node->value:205
forward[0]:key:6,value:206
forward[1]:key:6,value:206
forward[2]:key:6,value:206
----------------------------
node->key:6,node->value:206
forward[0]:key:7,value:207
forward[1]:key:7,value:207
forward[2]:key:7,value:207
forward[3]:key:7,value:207
forward[4]:key:7,value:207
forward[5]:key:7,value:207
----------------------------
node->key:7,node->value:207
forward[0]:key:8,value:208
forward[1]:key:8,value:208
forward[2]:key:8,value:208
forward[3]:key:8,value:208
forward[4]:key:8,value:208
forward[5]:key:8,value:208
forward[6]:key:8,value:208
----------------------------
node->key:8,node->value:208
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
forward[3]:key:2147483647,value:0
forward[4]:key:2147483647,value:0
forward[5]:key:2147483647,value:0
forward[6]:key:2147483647,value:0
forward[7]:key:2147483647,value:0
----------------------------

key:9,value:209,nodeLevel:5
finally nodeLevel:5
node->key:1,node->value:201
forward[0]:key:2,value:202
forward[1]:key:2,value:202
----------------------------
node->key:2,node->value:202
forward[0]:key:3,value:203
forward[1]:key:3,value:203
forward[2]:key:3,value:203
----------------------------
node->key:3,node->value:203
forward[0]:key:4,value:204
forward[1]:key:4,value:204
forward[2]:key:4,value:204
forward[3]:key:4,value:204
----------------------------
node->key:4,node->value:204
forward[0]:key:5,value:205
forward[1]:key:5,value:205
forward[2]:key:5,value:205
forward[3]:key:6,value:206
forward[4]:key:6,value:206
----------------------------
node->key:5,node->value:205
forward[0]:key:6,value:206
forward[1]:key:6,value:206
forward[2]:key:6,value:206
----------------------------
node->key:6,node->value:206
forward[0]:key:7,value:207
forward[1]:key:7,value:207
forward[2]:key:7,value:207
forward[3]:key:7,value:207
forward[4]:key:7,value:207
forward[5]:key:7,value:207
----------------------------
node->key:7,node->value:207
forward[0]:key:8,value:208
forward[1]:key:8,value:208
forward[2]:key:8,value:208
forward[3]:key:8,value:208
forward[4]:key:8,value:208
forward[5]:key:8,value:208
forward[6]:key:8,value:208
----------------------------
node->key:8,node->value:208
forward[0]:key:9,value:209
forward[1]:key:9,value:209
forward[2]:key:9,value:209
forward[3]:key:9,value:209
forward[4]:key:9,value:209
forward[5]:key:9,value:209
forward[6]:key:2147483647,value:0
forward[7]:key:2147483647,value:0
----------------------------
node->key:9,node->value:209
forward[0]:key:2147483647,value:0
forward[1]:key:2147483647,value:0
forward[2]:key:2147483647,value:0
forward[3]:key:2147483647,value:0
forward[4]:key:2147483647,value:0
forward[5]:key:2147483647,value:0
----------------------------


Process finished with exit code 9


 */