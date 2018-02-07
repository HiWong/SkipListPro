//
// Created by Wang Allen on 2018/2/6.
//

#ifndef SKIPLISTPRO_SKIPLIST_H
#define SKIPLISTPRO_SKIPLIST_H

#include <cstddef>
#include <cassert>
#include <ctime>
#include "Node.h"
#include "random.h"

using namespace std;

#define DEBUG

template<typename K, typename V>
class SkipList {

public:
    SkipList(K footerKey) : rnd(0x12345678) {
        createList(footerKey);
    }

    ~SkipList() {
        freeList();
    }

    //注意:这里要声明成Node<K,V>而不是Node,否则编译器会把它当成普通的类
    Node<K, V> *search(K key) const;

    bool insert(K key, V value);

    bool remove(K key, V &value);

    int size() {
        return nodeCount;
    }

    int getLevel() {
        return level;
    }

private:
    //初始化表
    void createList(K footerKey);

    //释放表
    void freeList();

    //创建一个新的结点，节点的层数为level
    void createNode(int level, Node<K, V> *&node);

    void createNode(int level, Node<K, V> *&node, K key, V value);

    //随机生成一个level
    int getRandomLevel();

    void dumpAllNodes();

    void dumpNodeDetail(Node<K, V> *node, int nodeLevel);

private:
    int level;
    Node<K, V> *header;
    Node<K, V> *footer;

    size_t nodeCount;

    static const int MAX_LEVEL = 16;

    Random rnd;
};

template<typename K, typename V>
void SkipList<K, V>::createList(K footerKey) {
    createNode(0, footer);

    footer->key = footerKey;
    this->level = 0;
    //设置头结
    createNode(MAX_LEVEL, header);
    for (int i = 0; i < MAX_LEVEL; ++i) {
        header->forward[i] = footer;
    }
    nodeCount = 0;
}

template<typename K, typename V>
void SkipList<K, V>::createNode(int level, Node<K, V> *&node) {
    node = new Node<K, V>(NULL, NULL);
    //需要初始化数组
    //注意:这里是level+1而不是level,因为数组是从0-level
    node->forward = new Node<K, V> *[level + 1];
    node->nodeLevel = level;
    assert(node != NULL);
};

template<typename K, typename V>
void SkipList<K, V>::createNode(int level, Node<K, V> *&node, K key, V value) {
    node = new Node<K, V>(key, value);
    //需要初始化数组
    if (level > 0) {
        node->forward = new Node<K, V> *[level + 1];
    }
    node->nodeLevel = level;
    assert(node != NULL);
};

template<typename K, typename V>
void SkipList<K, V>::freeList() {

    Node<K, V> *p = header;
    Node<K, V> *q;
    while (p != NULL) {
        q = p->forward[0];
        delete p;
        p = q;
    }
    delete p;
}

template<typename K, typename V>
Node<K, V> *SkipList<K, V>::search(const K key) const {
    Node<K, V> *node = header;
    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = *(node->forward + i);
        }
    }
    node = node->forward[0];
    if (node->key == key) {
        return node;
    } else {
        return nullptr;
    }
};

template<typename K, typename V>
bool SkipList<K, V>::insert(K key, V value) {
    Node<K, V> *update[MAX_LEVEL];

    Node<K, V> *node = header;

    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = node->forward[i];
        }
        update[i] = node;
    }
    //首个结点插入时，node->forward[0]其实就是footer
    node = node->forward[0];

    //如果key已存在，则直接返回false
    if (node->key == key) {
        return false;
    }

    int nodeLevel = getRandomLevel();

    if (nodeLevel > level) {
        nodeLevel = ++level;
        update[nodeLevel] = header;
    }

    //创建新结点
    Node<K, V> *newNode;
    createNode(nodeLevel, newNode, key, value);

    //调整forward指针
    for (int i = nodeLevel; i >= 0; --i) {
        node = update[i];
        newNode->forward[i] = node->forward[i];
        node->forward[i] = newNode;
    }
    ++nodeCount;

#ifdef DEBUG
    dumpAllNodes();
#endif

    return true;
};

template<typename K, typename V>
void SkipList<K, V>::dumpAllNodes() {
    Node<K, V> *tmp = header;
    while (tmp->forward[0] != footer) {
        tmp = tmp->forward[0];
        dumpNodeDetail(tmp, tmp->nodeLevel);
        cout << "----------------------------" << endl;
    }
    cout << endl;
}

template<typename K, typename V>
void SkipList<K, V>::dumpNodeDetail(Node<K, V> *node, int nodeLevel) {
    if (node == nullptr) {
        return;
    }
    cout << "node->key:" << node->key << ",node->value:" << node->value << endl;
    //注意是i<=nodeLevel而不是i<nodeLevel
    for (int i = 0; i <= nodeLevel; ++i) {
        cout << "forward[" << i << "]:" << "key:" << node->forward[i]->key << ",value:" << node->forward[i]->value
             << endl;
    }
}

template<typename K, typename V>
bool SkipList<K, V>::remove(K key, V &value) {
    Node<K, V> *update[MAX_LEVEL];
    Node<K, V> *node = header;
    for (int i = level; i >= 0; --i) {
        while ((node->forward[i])->key < key) {
            node = node->forward[i];
        }
        update[i] = node;
    }
    node = node->forward[0];
    //如果结点不存在就返回false
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

    //释放结点
    delete node;

    //更新level的值，因为有可能在移除一个结点之后，level值会发生变化，及时移除可避免造成空间浪费
    while (level > 0 && header->forward[level] == footer) {
        --level;
    }

    --nodeCount;

#ifdef DEBUG
    dumpAllNodes();
#endif

    return true;
};

template<typename K, typename V>
int SkipList<K, V>::getRandomLevel() {
    int level = static_cast<int>(rnd.Uniform(MAX_LEVEL));
    if (level == 0) {
        level = 1;
    }
    return level;
}

#endif //SKIPLISTPRO_SKIPLIST_H
