//
// Created by Wang Allen on 2018/2/5.
//

#ifndef SKIPLISTPRO_NODE_H
#define SKIPLISTPRO_NODE_H

//forward declaration
template<typename K, typename V>
class SkipList;

template<typename K, typename V>
class Node {

    friend class SkipList<K, V>;

public:

    Node() {}

    Node(K k, V v);

    ~Node();

    K getKey() const;

    V getValue() const;

private:
    K key;
    V value;
    Node<K, V> **forward;
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
