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
    //TODO 为什么如果这里用了const K key的话，友元就会不起作用？
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

    //TODO 为什么友元不起作用? 因为自己没有写成Node<K,V>,只是声明成Node*header的话，编译器会把它当成普通类而非模板类
    footer->key = footerKey;
    //TODO 如果是int类型，这里用NULL会不会出错？
    //header = new Node<K,V>(NULL, NULL);
    this->level = 0;
    //设置头结点//TODO 头结点不能做成动态增长吗?
    createNode(MAX_LEVEL, header);
    for (int i = 0; i < MAX_LEVEL; ++i) {
        //header->forward[i] = footer;
        header->forward[i] = *footer;
    }
    nodeCount = 0;
}

template<typename K, typename V>
void SkipList<K, V>::createNode(int level, Node<K, V> *&node) {
    //TODO 这样不行，结点初始化需要有一个默认值!
    node = new Node<K, V>(NULL, NULL);
    //需要初始化数组
    //TODO 这里需要对数组逐个初始化吗?
    if (level > 0) {
        //node->forward = new Node<K, V> *[level];
        node->forward=new Node<K,V>[level];
    }
    ////////////////////////////TODO 有必要这样吗？但是目前发现好像还真是这个原因!!!难道是编译器把它当成了二维数组的原因？///////////////////
    /*
    for (int i = 0; i < level; ++i) {
        node->forward[i] = new Node<K, V>(NULL, NULL);
    }
    */
    //////////////////////////////////////////////////////////////

    node->nodeLevel = level;
    assert(node != NULL);
};


template<typename K, typename V>
void SkipList<K, V>::createNode(int level, Node<K, V> *&node, K key, V value) {
    node = new Node<K, V>(key, value);
    //需要初始化数组
    //TODO 这里需要对数组逐个初始化吗?
    if (level > 0) {
        //node->forward = new Node<K, V> *[level];
        node->forward = new Node<K, V> [level];
    }
    node->nodeLevel = level;
    assert(node != NULL);
};


template<typename K, typename V>
void SkipList<K, V>::freeList() {
    /*
    Node<K, V> *p = header;
    Node<K, V> *q;
    while (p != NULL) {
        q = p->forward[0];
        //注意:在Node的
        delete p;
        p = q;
    }
    delete p;
    */

}

template<typename K, typename V>
Node<K, V> *SkipList<K, V>::search(const K key) const {
    Node<K, V> node = *header;
    for (int i = level; i >= 0; --i) {
        while ((node.forward[i]).key < key) {
            node = node.forward[i];
        }
    }
    node = node.forward[0];
    if (node.key == key) {
        return node;
    } else {
        return nullptr;
    }
};


//TODO 插入代码还有很大的问题!
template<typename K, typename V>
bool SkipList<K, V>::insert(K key, V value) {
    Node<K, V> *update[MAX_LEVEL];
    //TODO 这里要记得进行回收
    //Node<K,V>**update=new Node<K,V>*[MAX_LEVEL];

    Node<K, V> *node = header;

    //TODO just for debug
    if (key == 6) {
        cout << "attention" << endl;
    }

    //TODO 刚开始时level==0时怎么办?另外，这里node->forward[i]有可能越界吧?
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

    cout << "key:" << key << ",value:" << value << ",nodeLevel:" << nodeLevel << endl;

    if (nodeLevel > level) {
        nodeLevel = ++level;
        //TODO 问题出现在这里!
        update[nodeLevel] = header;
    }

    cout << "finally nodeLevel:" << nodeLevel << endl;

    //创建新结点
    Node<K, V> *newNode;

    createNode(nodeLevel, newNode);
    //TODO 这里在编辑的时候，确实会出现友元好像不起作用的情况，等整个函数写完之后，再过一下下，刷新之后就不会了，这应该是IDE对于友元的支持不太好
    newNode->key = key;
    newNode->value = value;

    //createNode(nodeLevel,newNode,key,value);

    //调整forward指针
    for (int i = nodeLevel; i >= 0; --i) {
        node = update[i];
        //TODO 这里有bug吧，万一update数组有效长度还没有newNode的有效长度那么长呢?
        //不会的，实际上理一下思路就会发现不可能出现那种情况，实际上update是集合了所有指向下一个节点的指针
        //TODO 问题就出在这里，update数组是在栈中创建的，而不是在堆上创建的，一旦执行完就会回收它的地址! 但是也不对啊，因为update中保存的都是其他节点的地址，它只是起一个中转和保存的作用呀
        newNode->forward[i] = node->forward[i];
        node->forward[i] = newNode;
    }

    /////////////////start of debug/////////////////
    //TODO just for debug
    //dumpNodeDetail(newNode, nodeLevel);
    Node<K, V> *tmp = header;
    while (tmp->forward[0] != footer) {
        tmp = tmp->forward[0];
        dumpNodeDetail(tmp, tmp->nodeLevel);
        cout << "----------------------------" << endl;
    }
    cout << endl;
    ////////////////end of debug////////////////
    ++nodeCount;
    return true;
};


template<typename K, typename V>
void SkipList<K, V>::dumpNodeDetail(Node<K, V> *node, int nodeLevel) {
    if (node == nullptr) {
        return;
    }
    cout << "node->key:" << node->key << ",node->value:" << node->value << endl;
    //注意是i<=nodeLevel而不是i<nodeLevel //TODO debug发现是在插入(6，206）时访问到forward[4]时出错，但是此时nodeLevel不是5吗？
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

    //TODO debug发现node的下一个节点竟然是11,说明插入有问题
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
