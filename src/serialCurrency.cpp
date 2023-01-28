/*
    0034430 - Artur Francisco Pereira Carvalho
    0034436 - Deivison Oliveira Costa
*/
#include "serialCurrency.h"

long long int serialCurrency::getIntPart() const {
    return intPart;
}
int serialCurrency::getFracPart() const {
    return fracPart;
}

void serialCurrency::setIntPart(long long int intPart) {
    this->intPart = intPart;
}
void serialCurrency::setFracPart(int fracPart) {
    this->fracPart = fracPart;
}

bool serialCurrency::operator<(const serialCurrency& sc) const {
    if (intPart < sc.intPart) {
        return true;
    } else if (intPart == sc.intPart) {
        return fracPart < sc.fracPart;
    }
    return false;
}

bool serialCurrency::operator>(const serialCurrency& sc) const {
    return sc < *this;
}

bool serialCurrency::operator<=(const serialCurrency& sc) const {
    return !(sc < *this);
}

bool serialCurrency::operator>=(const serialCurrency& sc) const {
    return !(*this < sc);
}

bool serialCurrency::operator==(const serialCurrency& sc) const {
    return intPart == sc.intPart && fracPart == sc.fracPart;
}

bool serialCurrency::operator!=(const serialCurrency& sc) const {
    return !(*this == sc);
}

string serialCurrency::toString() {
    string aux = "";
    aux += string(reinterpret_cast<char*>(&intPart), sizeof(intPart));
    aux += string(reinterpret_cast<char*>(&fracPart), sizeof(fracPart));
    return aux;
}

void serialCurrency::fromString(string repr) {
    size_t pos = sizeof(intPart);
    repr.copy(reinterpret_cast<char*>(&intPart), sizeof(intPart));
    repr.copy(reinterpret_cast<char*>(&fracPart), sizeof(fracPart), pos);
}

string serialCurrency::toXML() {
    return "<serialCurrency intPart='" + to_string(intPart) + "' fracPart='" + to_string(fracPart) + "'/>";
}

void serialCurrency::fromXML(string repr) {
    size_t intStart = repr.find("intPart='") + 9;
    size_t intEnd = repr.find("'", intStart);
    intPart = stoi(repr.substr(intStart, intEnd - intStart));
    size_t fracStart = repr.find("fracPart='") + 10;
    size_t fracEnd = repr.find("'", fracStart);
    fracPart = stoi(repr.substr(fracStart, fracEnd - fracStart));
}

string serialCurrency::toCSV() {
    return to_string(intPart) + "," + to_string(fracPart);
}

void serialCurrency::fromCSV(string repr) {
    size_t commaPos = repr.find(",");
    intPart = stoi(repr.substr(0, commaPos));
    fracPart = stoi(repr.substr(commaPos + 1));
}

string serialCurrency::toJSON() {
    return "{\"intPart\": " + to_string(intPart) + ", \"fracPart\": " + to_string(fracPart) + "}";
}

void serialCurrency::fromJSON(string repr) {
    size_t intStart = repr.find("intPart\":") + 9;
    size_t intEnd = repr.find(",", intStart);
    intPart = stoi(repr.substr(intStart, intEnd - intStart));
    size_t fracStart = repr.find("fracPart\":") + 10;
    size_t fracEnd = repr.find("}", fracStart);
    fracPart = stoi(repr.substr(fracStart, fracEnd - fracStart));
}

unsigned long long int serialCurrency::size() const {
    return sizeof(intPart) + sizeof(fracPart);
}
