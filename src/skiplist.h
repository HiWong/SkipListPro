//
// Created by Wang Allen on 2018/2/6.
//

#ifndef SKIPLISTPRO_SKIPLIST_H
#define SKIPLISTPRO_SKIPLIST_H

#include <cassert>
#include <cstddef>
#include <ctime>
#include <iostream>

#include "node.h"
#include "random.h"

using namespace std;

template <typename K, typename V>
class SkipList {
   public:
    SkipList(K footer_key) : rnd(0x12345678) { CreateList(footer_key); }

    ~SkipList() { FreeList(); delete footer;}

    // 注意:这里要声明成Node<K,V>而不是Node,否则编译器会把它当成普通的类
    Node<K, V> *Search(K key) const;

    bool Insert(K key, V value);

    bool Remove(K key, V &value);

    int Size() { return node_count; }

    int GetLevel() { return level; }

   private:
    // 初始化表
    void CreateList(K footer_key);

    // 释放表
    void FreeList();

    // 创建一个新的结点，节点的层数为level
    void CreateNode(int level, Node<K, V> *&node);

    void CreateNode(int level, Node<K, V> *&node, K key, V value);

    // 随机生成一个level
    int GetRandomLevel();

    void DumpAllNodes();

    void DumpNodeDetail(Node<K, V> *node, int node_level);

   private:
    int level;
    Node<K, V> *header;
    Node<K, V> *footer;

    size_t node_count;

    static const int max_level = 16;

    Random rnd;
};

template <typename K, typename V>
void SkipList<K, V>::CreateList(K footer_key) {
    CreateNode(0, footer);

    footer->key = footer_key;
    this->level = 0;
    // 设置头结
    CreateNode(max_level, header);
    for (int i = 0; i < max_level; ++i) {
        header->forward[i] = footer;
    }
    node_count = 0;
}

template <typename K, typename V>
void SkipList<K, V>::CreateNode(int level, Node<K, V> *&node) {
    node = new Node<K, V>(0, 0);
    // 需要初始化数组
    // 注意:这里是level+1而不是level,因为数组是从0-level
    if (level > 0) {
        node->forward = new Node<K, V> *[level + 1];
    }
    node->node_level = level;
    assert(node != nullptr);
};

template <typename K, typename V>
void SkipList<K, V>::CreateNode(int level, Node<K, V> *&node, K key, V value) {
    node = new Node<K, V>(key, value);
    // 需要初始化数组
    if (level > 0) {
        node->forward = new Node<K, V> *[level + 1];
    }
    node->node_level = level;
    assert(node != nullptr);
};

template <typename K, typename V>
void SkipList<K, V>::FreeList() {
    Node<K, V> *p = header;
    Node<K, V> *q;
    while (p != nullptr && p != footer) {
        q = p->forward[0];
        delete p;
        p = q;
    }
}

template <typename K, typename V>
Node<K, V> *SkipList<K, V>::Search(const K key) const {
    Node<K, V> *node = header;
    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = *(node->forward + i);
        }
    }
    node = node->forward[0];
    if (node->key == key) {
        return node;
    }
    return nullptr;
};

template <typename K, typename V>
bool SkipList<K, V>::Insert(K key, V value) {
    Node<K, V> *update[max_level];

    Node<K, V> *node = header;

    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = node->forward[i];
        }
        update[i] = node;
    }
    // 首个结点插入时，node->forward[0]其实就是footer
    node = node->forward[0];

    // 如果key已存在，则直接返回false
    if (node->key == key) {
        return false;
    }

    int node_level = GetRandomLevel();

    if (node_level > level) {
        node_level = ++level;
        update[node_level] = header;
    }

    // 创建新结点
    Node<K, V> *new_node;
    CreateNode(node_level, new_node, key, value);

    // 调整forward指针
    for (int i = node_level; i >= 0; --i) {
        node = update[i];
        new_node->forward[i] = node->forward[i];
        node->forward[i] = new_node;
    }
    ++node_count;

#ifdef DEBUG
    DumpAllNodes();
#endif

    return true;
};

template <typename K, typename V>
void SkipList<K, V>::DumpAllNodes() {
    Node<K, V> *tmp = header;
    while (tmp->forward[0] != footer) {
        tmp = tmp->forward[0];
        DumpNodeDetail(tmp, tmp->node_level);
        cout << "----------------------------" << endl;
    }
    cout << endl;
}

template <typename K, typename V>
void SkipList<K, V>::DumpNodeDetail(Node<K, V> *node, int node_level) {
    if (node == nullptr) {
        return;
    }
    cout << "node->key:" << node->key << ",node->value:" << node->value << endl;
    // 注意是i<=node_level而不是i<node_level
    for (int i = 0; i <= node_level; ++i) {
        cout << "forward[" << i << "]:"
             << "key:" << node->forward[i]->key
             << ",value:" << node->forward[i]->value << endl;
    }
}

template <typename K, typename V>
bool SkipList<K, V>::Remove(K key, V &value) {
    Node<K, V> *update[max_level];
    Node<K, V> *node = header;
    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = node->forward[i];
        }
        update[i] = node;
    }
    node = node->forward[0];
    // 如果结点不存在就返回false
    if (node->key != key) {
        return false;
    }

    value = node->value;
    for (int i = 0; i <= level; ++i) {
        if (update[i]->forward[i] != node) {
            break;
        }
        update[i]->forward[i] = node->forward[i];
    }

    // 释放结点
    delete node;

    // 更新level的值，因为有可能在移除一个结点之后，level值会发生变化，及时移除可避免造成空间浪费
    while (level > 0 && header->forward[level] == footer) {
        --level;
    }

    --node_count;

#ifdef DEBUG
    DumpAllNodes();
#endif

    return true;
};

template <typename K, typename V>
int SkipList<K, V>::GetRandomLevel() {
    int level = static_cast<int>(rnd.Uniform(max_level));
    if (level == 0) {
        level = 1;
    }
    return level;
}

#endif  // SKIPLISTPRO_SKIPLIST_H
