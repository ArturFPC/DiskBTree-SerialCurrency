/*
    0034430 - Artur Francisco Pereira Carvalho
    0034436 - Deivison Oliveira Costa
*/
#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <fstream>
#include <type_traits>
#include <vector>
#include "serializable.h"
#include "header.h"
#include "node.h"

using namespace std;

const ios::openmode mode = ios::in | ios::out | ios::binary;

template <class T>
class bTree : private fstream {
    static_assert(is_base_of<serializable, T>::value, "T must be serializable");
    public:
        bTree();
        virtual ~bTree();
        bTree(const string name, const string type,
                 const unsigned int version, ios::openmode openmode = mode);

        bool open(const string name, const string type,
                 const unsigned int version, ios::openmode openmode = mode);
        node<T>  *getRoot();
        void setRoot(node<T> node);
        unsigned long long int getFirstDeleted();
        bool isOpen();
        bool close();
        void insert(T k);
        void printAux(node<T> x, vector<string> &v, unsigned int lvl);
        void print();
        void searchKey(T k, node<T> *x);
        void removeKey(T k, node<T> *x,unsigned long long int writeAux = 1);

    protected:
        bool readNode(node<T> &r, unsigned long long int i);
        bool writeNode(node<T> &r, unsigned long long int i);
        node<T> *root;
        header head;
        bool readHeader(header &h);
        bool writeHeader(header &h);
        unsigned long long int index2pos(unsigned long long int i);
        unsigned long long int pos2index(unsigned long long int p);
        unsigned long long int allocateNode();
        bool splitChild(node<T> *x, unsigned long long int ix, unsigned long long int i);
        bool insertNonFull(node<T> *x, unsigned long long int ix, T k);
        bool delKey(node<T> &node, unsigned long long int ix, T key);
        void auxRemove(node<T> &node, unsigned long long int n);
};
template <class T>
bTree<T>::bTree(){

}
template <class T>
bTree<T>::~bTree(){

}
// Observação: a raiz é sempre armazenada na posição 1 do arquivo em disco
template <class T>
bTree<T>::bTree(const string name, const string type,
                const unsigned int version, ios::openmode openmode) : fstream(name.c_str(), mode) {
    // caso não encontre o nome de árvore especificado -> criar nova árvore
    if(!isOpen()){
        fstream::open(name.c_str(), mode | ios::trunc);
        if(isOpen()){
            // Arquivo criado com sucesso
            header aux(type, version);
            head = aux;
            head.setFirstValid(1);
            this->root = new node<T>;
            if(writeHeader(aux)){
                // Cabecalho gravado
                if(writeNode(*this->root, 1)){
                    // Raiz gravada
                }
            }
        }else cerr << "Erro ao criar arquivo" << endl;
    }else{ // caso encontre o arquivo com o nome especificado
        // Arquivo aberto com sucesso
        if(readHeader(head) && head.getType() == type && head.getVersion() == version){
            node<T> *aux = new node<T>;
            // Cabecalhos iguais. acesso permitido
            if(readNode(*aux, 1)){
                // Raiz lida com sucesso
                this->root = aux;
            }
        }else{ // caso os cabeçalhos sejam diferentes
            this->close();
            // Cabecalhos diferentes. acesso negado
        }
    }
}

template <class T>
node<T> * bTree<T>::getRoot(){
    return this->root;
}

template <class T>
void bTree<T>::setRoot(node<T> node){
    this->root = node;
}

template <class T>
bool bTree<T>::open(const string name, const string type,
                const unsigned int version, ios::openmode openmode){
    fstream::open(name.c_str(), openmode);
    if(isOpen()){
        header aux(type, version);
        if(readHeader(aux) && aux.getType() == type && aux.getVersion() == version){
            node<T> *aux = new node<T>(NULL);
            if(readNode(*aux, 1)){
                //Cabecalhos iguais. acesso permitido
                this->root = aux;
            }
        }else{
            // Cabecalhos diferentes, acesso negado
            this->close();
        }
    }else{
        fstream::open(name.c_str(), mode | ios::trunc);
        if(isOpen()){
            //Arquivo criado com sucesso
            header aux(type, version);
            head = aux;
            head.setFirstValid(1);
            this->root = new node<T>(true);
            // gravar nó raiz
            if(writeHeader(aux)){
                // Cabecalho gravado
                if(writeNode(*this->root, 1)){
                    //Raiz gravada
                }
            }
        }
    }
    return fstream::good();
}

template <class T>
bool bTree<T>::isOpen() {
    return fstream::is_open();
}

template <class T>
bool bTree<T>::close() {
    fstream::close();
    return !isOpen();
}

template <class T>
bool bTree<T>::readNode(node<T> &r, unsigned long long int i){
    if(isOpen()){
        fstream::seekg(index2pos(i));
        char *aux = new char[r.size()+100];
        fstream::read(aux, r.size()+100);
        r.fromString(string(aux, r.size()+100));
        delete aux;
        return fstream::good();
    }else{
        cerr << "Erro ao ler o registro" << endl;
        return false;
    }

}

template <class T>
bool bTree<T>::writeNode(node<T> &r, unsigned long long int i){
    if(isOpen()){
        fstream::seekp(index2pos(i), ios::beg);
        fstream::write(r.toString().c_str(), r.size()+100);
        return fstream::good();
    }else{
        return false;
    }
}

template <class T>
unsigned long long int bTree<T>::index2pos(unsigned long long int i) {
    node<T> r;
    return head.size() + ((i - 1) * r.size()+100);
}

template <class T>
unsigned long long int bTree<T>::pos2index(unsigned long long int p) {
    node<T> r;
    return ((p - head.size()) / r.size()+100) + 1;
}

template <class T>
bool bTree<T>::readHeader(header &h){
    if(isOpen()){
        char *aux = new char[h.size()];
        fstream::seekg(0, ios::beg);
        fstream::read(aux, h.size());
        h.fromString(string(aux, h.size()));
        delete aux;
        return fstream::good();
    }else{
        return false;
    }
}

template <class T>
bool bTree<T>::writeHeader(header &h){
    if(isOpen()){
        fstream::seekp(0, ios::beg);
        fstream::write(h.toString().c_str(), h.size());
        return fstream::good();
    }else{
        return false;
    }
}

template <class T>
unsigned long long int bTree<T>::allocateNode(){
    if(isOpen()){
        unsigned long long int pos;
        if(head.getFirstDeleted() == 0){
            fstream::seekp(0, ios::end);
            pos = fstream::tellp();
            return pos;
        }else{
            node<T> aux;
            pos = head.getFirstDeleted();
            if(readNode(aux, pos)){
                head.setFirstDeleted(aux.getNext());
                if(writeHeader(head)) return pos;
                else return 0;
            } else return 0;
        }
    }else return 0;
}


// realizando split do i-ésimo filho do nó encontrado na posição ix
template <class T>
bool bTree<T>::splitChild(node<T> *x, unsigned long long int ix, unsigned long long int i){
    node<T> *y = new node<T>;

    // carregando nó y do disco para a memória
    if(readNode(*y, x->getChild(i))){
        node<T> *z = new node<T>;

        node<T> *aux = new node<T>;
        // copiar para z as t-1 maiores chaves de y
        for(unsigned long long int j = 0; j < MIN; j++){
            z->insertKey(y->getKey(j + t));
            aux->insertKey(y->getKey(j));
        }


        // se y não for folha, é necessário copiar os filhos
        if(!y->isLeaf()){
            z->setLeaf(false);

            for(unsigned long long int j = 0; j < t; j++){
                z->setChild(y->getChild(t + j), j);
            }
        }
        // rearanjando os filhos de X para inserção de z como novo filho
        for(unsigned long long int j = x->sizeNode(); j > i; j--){
            x->setChild(x->getChild(j), j + 1);
        }

         // o registro mediano de y é inserido apropriadamente em x
        x->insertKey(y->getKey(MIN));
        unsigned long long posy = pos2index(allocateNode());
        x->setChild(posy,i);

        y->setKeys(aux->getKeys());
        y->resizeNode(MIN);
        writeNode(*y,posy);
        // pegando posição para gravar z em disco
        unsigned long long posz = pos2index(allocateNode());

        // z vira novo filho de x
        x->setChild(posz, i+1);

        z->resizeNode(MIN);

        return writeNode(*x, ix) && writeNode(*z, posz); // GRAVAR X, Y e Z em disco

    }else{
        return false;
    }

}

// inserir no nó não cheio x localizado em disco na posição ix ou em seu descendente apropriado a chave k
template <class T>
bool bTree<T>::insertNonFull(node<T> *x, unsigned long long int ix, T k){
    int i = x->sizeNode() - 1;
    node<T> *y = new node<T>;

    // se o nó x for folha -> inserir k na posição apropriada
    if(x->isLeaf()){
        x->insertKey(k);
        return writeNode(*x, ix);
    }else{ // se x não é folha -> buscar o filho apropriado
        while(i >= 0 && k < x->getKey(i)){ // encontrando índice do filho apropriado
            i--;
        }
        i++; // posição do filho apropriado em children

        if(readNode(*y, x->getChild(i))){ // lendo do disco para a memória o filho apropriado
            if(y->sizeNode() == MAX){ // caso o devido filho esteja cheio
                this->splitChild(x, ix, i); // realizar split
                if(k > x->getKey(i)){
                    i++;
                }
                readNode(*y,x->getChild(i));
            }
        }
        return insertNonFull(y, x->getChild(i), k);
    }
}

template <class T>
void bTree<T>::insert(T k){
    node<T> *r = root, *s = NULL;
    node<T> taux;
    if(r->sizeNode() == MAX){ // se a raiz estiver cheia
        s = new node<T>; // alocar novo nó para ser nova raiz
        s->setLeaf(false);
        unsigned long long int i = pos2index(allocateNode()); // alocar espaço em disco para gravar um novo nó
        root = s;

        if(writeNode(*root, 1));
        if(writeNode(*r, i));
        root->setChild(i, 0); // novo filho da raiz será o novo nó alocado


        if(splitChild(root, 1, 0)); // realizar split no filho [0] de s (antiga raiz cheia)
        if(insertNonFull(root, 1, k));
    }else{
        if(insertNonFull(r, 1, k)) cout << "Registro inserido com sucesso!" << endl;
        else cout << "Erro ao inserir" << endl;
    }
}


template <class T>
void bTree<T>::printAux(node<T> x, vector<string> &v, unsigned int lvl){
    node<T> *y = new node<T>;
    string aux = "[";
    unsigned int i = 0;
    //verificando se o vetor é maior do que lvl+1
    if (v.size() < lvl + 1) {
        //alterando o tamanho do vetor
        v.resize(lvl + 1);
    }
    //caso em que o nó não é folha, é chamado recursivamente o printAux
    if (!x.isLeaf()) {
        for (i = 0; i <= x.getN(); i++) {
            if (readNode(*y,x.getChild(i))){
                //Esta chamada faz com que seja preenchido a primeira posição do vetor apenas com nós folhas
                printAux(*y, v, lvl + 1);
            }
        }
    }
    //preenchendo o vetor com as chaves
    for (i = 0; i < x.getN(); i++) {
        aux += to_string(x.getKey(i).getIntPart())+"."+ to_string(x.getKey(i).getFracPart()) + ", ";
    }
    //posiciona o cursor no final da string para inserir ]
    if (aux.length() > 1) {
        aux += "\b\b] ";
    } else {
        aux += "] ";
    }
    //incrementando o vetor com a string de chaves
    v[lvl] += aux;
}

template <class T>
void bTree<T>::print(){
    //método para printar a árvore
    int lvl = 0;
    vector<string> levels(1);
    printAux(*root,levels,lvl);

    for (string s : levels){
        //laço para percorrer e printar cada nível da árvore
        cout << s <<endl;
    }
}

template <class T>
void bTree<T>::searchKey(T k, node<T> *x){
    string found = x->searchNode(k);//procurar a chave no nó
    bool full = false;
    string sk = to_string(k.getIntPart())+"."+ to_string(k.getFracPart());// string recebendo o valor da chave
    if (found == sk){//caso chave encontrada
        cout << endl;
        cout << "A chave " << k << " foi encontrada" << endl;
    }else{
        if (x->isLeaf()){//verficiando se o nó x é folha
            cout << "Chave não existente" << endl;
        }else{
            if (root->getN() == MAX && !root->isLeaf()){
                //caso raiz cheia com filhos
                node<T> *r = root;
                node<T> *s = new node<T>; // alocar novo nó para raiz
                s->setLeaf(false);
                unsigned long long int i = pos2index(allocateNode()); // alocar espaço em disco para gravar um novo nó
                root = s;
                s->setChild(i, 0); // novo filho da raiz será o novo nó alocado
                if(writeNode(*root, 1));
                if(writeNode(*r, i));
                if(splitChild(s, 1, 0)); // realizar split no filho [0] de s (antiga raiz cheia)
                searchKey(k,root);
                full = true;
            }
            if (full == false){//caso não entrar no root->getN() == MAX mas a raiz é não folha
                node<T> *y;
                node<T> aux;
                if(readNode(aux,x->getChild(stoi(found)))){
                    y=&aux;
                    if(y->getN() == MAX){
                        this->splitChild(x, 1, stoi(found));
                        searchKey(k,x);
                    }else{
                        searchKey(k,y);
                    }
                }
            }
        }
    }
}
template <class T>
void bTree<T>::auxRemove(node<T> &node, unsigned long long int n){
    //percorre um nó e realoca as chaves de forma a reorganizar o nó e excluir a chave da posição n
    for (unsigned long long int i = n; i<node.getN(); i++){
        node.setKey(node.getKey(i+1),i);
    }
    node.setN(node.getN()-1);

}
template <class T>
void bTree<T>::removeKey(T key, node<T> *x, unsigned long long int writeAux){
    string found = x->searchNode(key); // proucura a chave no nó
    string sk = to_string(key.getIntPart())+"."+ to_string(key.getFracPart());//convertendo a chave pra string
    node<T> aux;//no auxiliar
    if (found == sk){//caso a chave pertença ao nó realiza a remoção direta
        //só ocorre quando a remoção é na raiz da árvore
        int cont = 0;
        int pos = 0;
        for (unsigned long long int i = 0; i<x->getN();i++){
            if (x->getKey(i) != key){
                aux.setKey(x->getKey(i),cont);
                cont++;
            }else{
                pos = i;
            }
        }
        //caso 1 de remoção
        if (x->isLeaf()){
            x->setKeys(aux.getKeys());
            x->setN(cont);
            writeNode(*x,writeAux);
        }else{
            //caso 2a
            if (readNode(aux,x->getChild(pos)) && aux.getN()>MIN){
                    x->setKey(aux.getKey(aux.getN()-1),pos);
                    writeNode(*x,writeAux);
                    aux.setN(aux.getN()-1);
                    writeNode(aux,x->getChild(pos));
            }else{
                //caso 2b
                if (readNode(aux,x->getChild(pos+1)) && aux.getN()>MIN){
                    x->setKey(aux.getKey(0),pos);
                    writeNode(*x,writeAux);
                    auxRemove(aux,0);
                    writeNode(aux,x->getChild(pos+1));
                }else{
                    //caso 2c
                    node<T> leftSon;
                    if (readNode(leftSon,x->getChild(pos))){
                        for (unsigned long long int i = 0; i<aux.getN();i++){
                            leftSon.setKey(aux.getKey(i),i+MIN);
                            leftSon.setN(leftSon.getN()+1);
                        }
                        writeNode(leftSon,x->getChild(pos));
                        for (unsigned long long int i = pos+1; i<= x->getN();i++){
                            node<T> iChild;
                            if (readNode(iChild,x->getChild(i+1))){
                                x->setChild(x->getChild(i+1),i);
                                writeNode(iChild,x->getChild(i));
                            }
                        }
                        auxRemove(*x, pos);
                        writeNode(*x,writeAux);
                    }

                }
            }
        }
   }else{//caso a chave não for encontrada no nó, será feito a busca no filho do nó
        if (readNode(aux,x->getChild(stoi(found)))){
            string snaux = aux.searchNode(key);
            bool stop = false;
            unsigned long long int c = 0;
            while (c<aux.getN() && stop == false){
                if (aux.getKey(c) == key){
                    stop = true;
                }else{
                    c++;
                }
            }
            //caso em que foi encontrado a chave no nó filho
            if (snaux == sk){
                if (aux.getN()== MIN){
                    //caso 3A predecessor
                    node<T> leftBrother;
                    if (readNode(leftBrother,x->getChild(stoi(found)-1)) && leftBrother.getN()>MIN){
                        T auxKey = x->getKey(stoi(found)-1);
                        x->setKey(leftBrother.getKey(leftBrother.getN()-1),stoi(found)-1);
                        writeNode(*x,writeAux);
                        leftBrother.setN(leftBrother.getN()-1);
                        writeNode(leftBrother, x->getChild(stoi(found)-1));
                        auxRemove(aux,c);
                        aux.insertKey(auxKey);
                        writeNode(aux,x->getChild(stoi(found)));
                    }else{
                        //3A sucessor
                        node<T> rigthBrother;
                        if (readNode(rigthBrother,x->getChild(stoi(found)+1)) && rigthBrother.getN()>MIN){
                            T auxKey = x->getKey(stoi(found));
                            x->setKey(rigthBrother.getKey(0),stoi(found));
                            writeNode(*x,writeAux);
                            auxRemove(rigthBrother,0);
                            writeNode(rigthBrother,x->getChild(stoi(found)+1));
                            auxRemove(aux,c);
                            aux.insertKey(auxKey);
                            writeNode(aux,x->getChild(stoi(found)));
                        }else{
                            node<T> leftSon;
                            if (readNode(leftSon,x->getChild(stoi(found)-1))){
                                auxRemove(aux,c);
                                leftSon.setKey(aux.getKey(0),MIN);
                                leftSon.setN(leftSon.getN()+1);
                                leftSon.insertKey(x->getKey(stoi(found)-1));
                                writeNode(leftSon,x->getChild(stoi(found)-1));
                                for (unsigned long long int i = stoi(found); i<= x->getN();i++){
                                    node<T> iChild;
                                    if (readNode(iChild,x->getChild(i+1))){
                                        x->setChild(x->getChild(i+1),i);
                                        writeNode(iChild,x->getChild(i));
                                    }
                                }
                                auxRemove(*x, stoi(found));
                                writeNode(*x,writeAux);
                            }
                        }
                    }

                }else{
                    //caso 1 para quando o nó não é raiz
                    auxRemove(aux,c);
                    writeNode(aux,x->getChild(stoi(found)));
                }

            }else{
                //chamando recursivamente o removeKey para realizar todo o processo em cima do nó filho que possa ter a chave
                removeKey(key,&aux, x->getChild(stoi(found)));
            }
        }
   }
   cout<<"Registro removido com sucesso!"<<endl;
}

#endif // BTREE_H_INCLUDED
