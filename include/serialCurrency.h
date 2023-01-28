/*
    0034430 - Artur Francisco Pereira Carvalho
    0034436 - Deivison Oliveira Costa
*/
#ifndef SERIALCURRENCY_H
#define SERIALCURRENCY_H

#include "serializable.h"
#include <iostream>

using namespace std;

class serialCurrency : public serializable {
    private:
        long long int intPart;
        int fracPart;

    public:
        serialCurrency(long long int intPart = 0, int fracPart = 0) : intPart(intPart), fracPart(fracPart) {
            while (fracPart >= 100) {
                intPart++;
                fracPart -= 100;
            }
        }

        virtual ~serialCurrency() {}

        // getters
        long long int getIntPart() const;
        int getFracPart() const;

        // setters
        void setIntPart(long long int intPart);
        void setFracPart(int fracPart);

        // sobrecarga dos operadores relacionais
        bool operator<(const serialCurrency& sc) const;

        bool operator>(const serialCurrency& sc) const;

        bool operator<=(const serialCurrency& sc) const;

        bool operator>=(const serialCurrency& sc) const;

        bool operator==(const serialCurrency& sc) const;

        bool operator!=(const serialCurrency& sc) const;

        // metodos de serialização
        virtual string toString();

        virtual void fromString(string repr);

        virtual string toXML();

        virtual void fromXML(string repr);

        virtual string toCSV();

        virtual void fromCSV(string repr);

        virtual string toJSON();

        virtual void fromJSON(string repr);

        virtual unsigned long long int size() const;

        // Sobrecarga do operador << para escrever o valor armazenado no objeto
        friend ostream& operator<<(ostream& os, const serialCurrency& sc)
        {
            os << sc.intPart << "." << sc.fracPart;
            return os;
        }

        // Sobrecarga do operador >> para ler o valor diretamente
        friend istream& operator>>(istream& is, serialCurrency& sc)
        {
            string repr;
            is >> repr;
            size_t dotPos = repr.find(".");
            sc.intPart = stoi(repr.substr(0, dotPos));
            sc.fracPart = stoi(repr.substr(dotPos + 1));
            return is;
        }
};

#endif // SERIALCURRENCY_H
