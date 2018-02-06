//
// Created by Wang Allen on 2018/2/5.
//

#ifndef SKIPLISTPRO_NODE_H
#define SKIPLISTPRO_NODE_H

//forward declaration
template<typename K, typename V>
class SkipList;
//class SkipList<K,V>;

template<typename K, typename V>
class Node {

    /*
    template<typename K1, typename V1>
    friend class SkipList<K1, V1>;
    */
    friend class SkipList<K, V>;


public:

    Node(K k, V v);

    ~Node();

    K getKey() const;

    V getValue() const;

private:
    K key;
    V value;
    //Node *forward[1];
    //一个指针数组,注意要声明成Node<K,V>**而不是Node** //TODO 是不是有必要记录每个Node本身的level呢?不然容易越界吧？
    Node<K, V> **forward;

    //TODO 这个是自己为了方便debug添加的，后面可能要去掉
    int nodeLevel;
};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v) {
    key = k;
    value = v;
};

template<typename K, typename V>
Node<K, V>::~Node() {
    delete[]forward;
};

template<typename K, typename V>
K Node<K, V>::getKey() const {
    return key;
}

template<typename K, typename V>
V Node<K, V>::getValue() const {
    return value;
}

#endif //SKIPLISTPRO_NODE_H
