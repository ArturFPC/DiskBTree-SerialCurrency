/*
    0034430 - Artur Francisco Pereira Carvalho
    0034436 - Deivison Oliveira Costa
*/
#ifndef NODE_H
#define NODE_H

#include <type_traits>
#include <iostream>
#include "serializable.h"

using namespace std;

const unsigned int t = 3;
const unsigned int MAX = 2 * t - 1;
const unsigned int MIN = t - 1;

template <class T>
class node : public serializable {
    static_assert(is_base_of<serializable, T>::value, "T must be serializable");
    public:
        node();
        virtual ~node();
        node(const node<T> &other);
        node<T> operator=(const node<T> &other);
        unsigned long long int sizeNode() const;
        void resizeNode(unsigned long long int n);
        T getKey(unsigned long long int i) const;
        void setKey(T key, unsigned long long int i);
        T* getKeys(){return this->keys;};
        void setKeys(T *nkey){this->keys = nkey;}
        unsigned long long int getN(){return this->n;}
        void setN(unsigned long long int nn){this->n = nn;}
        unsigned long long int getChild(unsigned long long int i) const;
        void setChild(unsigned long long int child, unsigned long long int i);
        unsigned long long int* getChilds(){return this->children;}
        void setChilds(unsigned long long int *nChildren){this->children = nChildren;}
        bool isLeaf() const;
        void setLeaf(bool leaf);
        bool isDeleted() const;
        void del();
        void undel();
        void insertKey(T key);
        unsigned long long int getNext() const;
        void setNext(unsigned long long int n);
        string searchNode(T k);
        void printChildren();
        virtual string toString();
        virtual void fromString(string repr);
        virtual string toCSV() { return ""; }
        virtual void fromCSV(string repr) {}
        virtual string toJSON() { return ""; }
        virtual void fromJSON(string repr) {}
        virtual string toXML() { return ""; }
        virtual void fromXML(string repr) {}
        virtual unsigned long long int size() const;
    protected:
        unsigned long long int n;
        bool leaf;
        bool deleted;
        T *keys;
        unsigned long long int *children;
        unsigned long long int next;
};

template <class T>
node<T>::node() : serializable() {
    this->next = 0;
    this->n = 0;
    this->leaf = true;
    this->deleted = false;
    this->keys = new T[MAX];

    for(unsigned long long int i = 0; i < MAX; i++){
        keys[i] = T(0,0);
    }

    this->children = new unsigned long long int[MAX + 1];

    for(unsigned long long int i = 0; i < MAX + 1; i++){
        children[i] = 0;
    }
}
template <class T>
node<T>::~node(){
    //dtor
}

template <class T>
node<T>::node(const node<T> &other){
    this->n = other.sizeNode();
    this->leaf = other.isLeaf();
    this->deleted = other.isDeleted();
    this->keys = new T[MAX];
    for(unsigned long long int i = 0; i < other.sizeNode(); i++){
        this->keys[i] = other.getKey(i);
    }
    this->children = new unsigned long long int[MAX + 1];
    for(unsigned long long int i = 0; i < MAX + 1; i++){
        this->children[i] = other.getChild(i);
    }
}

template <class T>
node<T> node<T>::operator=(const node<T> &other) {
    node<T> aux(other);
    if(this == &other)
        return *this;

    this->t = other.getDegree();
    this->n = other.getNkeys();
    this->leaf = other.isLeaf();
    this->deleted = other.isDeleted();
    this->keys = new T[2 * other.getDegree() - 1];
    for(int i = 0; i < other.getNkeys(); i++){
        this->keys[i] = other.getKey(i);
    }
    this->children = new unsigned long long int[2 * other.getDegree()];
    for(int i = 0; i < 2 * other.getDegree(); i++){
        this->children[i] = other.getChild(i);
    }

    return aux;
}

template <class T>
unsigned long long int node<T>::sizeNode() const {
    return this->n;
}

template <class T>
void node<T>::resizeNode(unsigned long long int n){
    this->n = n;
}

template <class T>
bool node<T>::isLeaf() const {
    return this->leaf;
}

template <class T>
T node<T>::getKey(unsigned long long int i) const {
    return this->keys[i];
}

template <class T>
void node<T>::setKey(T key, unsigned long long int i){
    this->keys[i] = key;
}

template <class T>
unsigned long long int node<T>::getChild(unsigned long long int i) const {
    return this->children[i];
}

// colocar o nó da posição child como i-ésimo filho
template <class T>
void node<T>::setChild(unsigned long long int child, unsigned long long int i){
    this->children[i] = child;
}

template <class T>
void node<T>::setLeaf(bool leaf){
    this->leaf = leaf;
}

template <class T>
bool node<T>::isDeleted() const {
    return this->deleted;
}

template <class T>
void node<T>::del(){
    this->deleted = true;
}

template <class T>
void node<T>::undel(){
    this->deleted = false;
}

// insere a chave key na posição apropriada dos filhos
template <class T>
void node<T>::insertKey(T key){
    if(this->n < 2 * t - 1){
        int i = this->n - 1;
        while(i >= 0 && key < keys[i]){
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = key;
        this->n++;

    }else{
        cout << "No cheio" << endl;
    }

}

// usado apra quando o nó entrar na lista de expluídos
template <class T>
unsigned long long int node<T>::getNext() const {
    return this->next;
}

template <class T>
void node<T>::setNext(unsigned long long int n){
    this->next = n;
}

template <class T>
string node<T>::searchNode(T k){
    vector<T>aux;
    if(this->n == 0) cout << "--Vazio--" << endl;
    else{
        for(unsigned long long int i = 0; i < this->n; i++){
            aux.push_back(this->keys[i]);
        }
    }
    string foundChildren="-1";
    for (unsigned long long int i = 0; i<aux.size();i++){
        if (aux[i]==k){
            string sk = to_string(aux[i].getIntPart())+"."+ to_string(aux[i].getFracPart());
            return sk;
        }else{
            if (k<aux[i]){
                foundChildren = to_string(i);
                return foundChildren;
            }
        }
    }
    if (foundChildren =="-1"){
        foundChildren = to_string(aux.size());
    }
    return foundChildren;
}
template <class T>
void node<T>::printChildren(){
    cout << "[";
    for(unsigned long long int i = 0; i < MAX + 1; i++ ){
        cout << getChild(i) << ", ";
    }
    cout << "]" << endl;
}

// Métodos para serialização e deserialização do nó

template <class T>
string node<T>::toString(){
    string aux = "";
    aux += string(reinterpret_cast<char*>(&n), sizeof(n));
    aux += string(reinterpret_cast<char*>(&next), sizeof(next));
    aux += string(reinterpret_cast<char*>(&leaf), sizeof(leaf));
    aux += string(reinterpret_cast<char*>(&deleted), sizeof(deleted));
    T key;
    for(unsigned long long int i = 0; i < 2 * t - 1; i++){
        key = keys[i];
        aux += keys[i].toString();
    }
    for(unsigned long long int i = 0; i < 2 * t; i++){

        aux += string(reinterpret_cast<char*>(&children[i]), this->size());
    }
    return aux;
}

template <class T>
void node<T>::fromString(string repr){
    int pos = 0;
    repr.copy(reinterpret_cast<char*>(&n), sizeof(n), pos);
    pos += sizeof(n);
    repr.copy(reinterpret_cast<char*>(&next), sizeof(next), pos);
    pos += sizeof(next);
    repr.copy(reinterpret_cast<char*>(&leaf), sizeof(leaf), pos);
    pos += sizeof(leaf);
    repr.copy(reinterpret_cast<char*>(&deleted), sizeof(deleted), pos);
    pos += sizeof(deleted);

    if(this->keys == NULL) this->keys = new T[2 * t - 1];

    for(unsigned long long int i = 0; i < 2 * t - 1; i++){
        keys[i].fromString(string(repr, pos, keys[i].size()));
        pos += keys[i].size();
    }

    if(this->children == NULL) this->children = new unsigned long long int[2 * t];

    for(unsigned long long int i = 0; i < 2 * t ; i++){
        repr.copy(reinterpret_cast<char*>(&children[i]), sizeof(unsigned long long int), pos);
        pos += sizeof(unsigned long long int);
    }
}

template <class T>
unsigned long long int node<T>::size() const{
    T key;
    node<T> child;
    return sizeof(n) + sizeof(leaf) + sizeof(deleted) + ((2 * t-1) * key.size()) + (2 * t * sizeof(unsigned long long int));
}

#endif //node_H
