/*
    0034430 - Artur Francisco Pereira Carvalho
    0034436 - Deivison Oliveira Costa
*/
#include "bTree.h"
#include "serialCurrency.h"

using namespace std;

int main(){
    bTree<serialCurrency> t("arvore.dat", "TPG", 1);
    serialCurrency sc;
    serialCurrency insercao[] = {
        serialCurrency(5,50),
        serialCurrency(2,01),
        serialCurrency(-5,33),
        serialCurrency(10,05),
        serialCurrency(21,13),
        serialCurrency(1,88),
        serialCurrency(-3,14),
        serialCurrency(2,86),
        serialCurrency(37,55),
        serialCurrency(2,99),
        serialCurrency(34,31),
        serialCurrency(49,00),
        serialCurrency(38,26),
        serialCurrency(20,85),
        serialCurrency(23,02),
        serialCurrency(17,01),
        serialCurrency(14,92),
        serialCurrency(13,81),
        serialCurrency(39,89),
        serialCurrency(-13,22),
        serialCurrency(7,20)
    };
    serialCurrency remocao[] = {
        serialCurrency(5,50),
        serialCurrency(-5,33),
        serialCurrency(21,13),
        serialCurrency(1,88),
        serialCurrency(-3,14),
        serialCurrency(34,31),
        serialCurrency(38,26),
        serialCurrency(23,2),
        serialCurrency(14,92),
        serialCurrency(39,89),
        serialCurrency(7,20)
    };

    int opcao;
    bool sair = false;

    do{//menu
        cout << "\nArvore B em disco\n"
        << "Selecione uma opcao:\n\n"
        << "1. Inserir um valor\n"
        << "2. Remover um valor\n"
        << "3. Buscar um valor\n"
        << "4. Mostrar a arvore\n"
        << "5. Rodar caso de teste\n"
        << "6. Sair\n\n"
        << "Sua opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1://caso de inserção
                cout << "Digite o valor da moeda: ";
                cin >> sc;
                t.insert(sc);
                break;
            case 2://caso de remoção
                cout << "Digite o valor da moeda: ";
                cin >> sc;
                t.removeKey(sc,t.getRoot());
                break;
            case 3:{//caso de busca
                cout << "Digite o valor da moeda a buscar: ";
                cin >> sc;
                node<serialCurrency> *x = t.getRoot();
                t.searchKey(sc, x);
                break;
            }
            case 4://print
                t.print();
                break;
            case 5:
                cout << "Casos teste de insercao:" << endl;
                for (serialCurrency i : insercao) {
                    cout << "\nInserindo a chave " << i << endl;
                    t.insert(i);
                    t.print();
                }
                cout << "\nCasos teste de remocao:" << endl;
                for (serialCurrency i : remocao) {
                    cout << "\nRemovendo a chave "<< i <<endl;
                    t.removeKey(i, t.getRoot());
                    t.print();
                }
                break;
            case 6:
                sair = true;
                break;
            default:
                cout << "Opcao invalida, por vavor tente novamente.\n" << endl;
                break;
        }
    }
    while(!sair);

    return 0;
}
