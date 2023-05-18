#include <iostream>

#include "skiplist.h"

using namespace std;

int main() {
    cout << "skiplist test now starts..." << endl;

    // 给一个最大的整型值
    SkipList<int, int> skip_list(0x7fffffff);

    int length = 10;

    for (int i = 1; i <= length; ++i) {
        skip_list.Insert(i, i + 200);
    }

    cout << "The number of elements in skiplist is:" << skip_list.Size()
         << endl;

    if (skip_list.Size() != length) {
        cout << "insert failur." << endl;
    } else {
        cout << "insert success." << endl;
    }

    // 测试查找
    int value = -1;
    int key = 9;
    Node<int, int> *search_result = skip_list.Search(key);
    if (search_result != nullptr) {
        value = search_result->GetValue();
        cout << "search result for key " << key << ":" << value << endl;
    } else {
        cout << "search failure for key " << key << endl;
    }

    value = -1;

    key = 6;
    cout << endl << "start to remove" << endl;
    bool remove_result = skip_list.Remove(key, value);
    if (remove_result) {
        cout << "removed node whose key is " << key << " and value is " << value
             << endl;
    } else {
        cout << "removed failure" << endl;
    }

    return 0;
}
