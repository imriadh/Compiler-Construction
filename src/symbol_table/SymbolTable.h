#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "ScopeTable.h"
#include <bits/stdc++.h>
using namespace std;

class SymbolTable {
private:
    ScopeTable* currentScopeTable; int bucketSize = 7;
    int currentSIDDeletedScopes = 1; FILE* logFile;
public:
    SymbolTable(int bucketSize, FILE* logFile = nullptr) {
        this->bucketSize = bucketSize; this->logFile = logFile;
        currentScopeTable = new ScopeTable(bucketSize, logFile);
        currentScopeTable->setUniqueId("1");
        if (logFile) fprintf(logFile, "\tScopeTable# 1 created\n");
    }
    ~SymbolTable() {
        while (currentScopeTable != nullptr) {
            ScopeTable *parentScopeTable = currentScopeTable->getParentScope();
            delete currentScopeTable;
            currentScopeTable = parentScopeTable;
        }
    }
    void enterScope() {
        ScopeTable *newScopeTable = new ScopeTable(bucketSize, logFile);
        newScopeTable->setParentScope(currentScopeTable);
        string uniqueId = currentScopeTable->getUniqueId();
        currentScopeTable = newScopeTable;
        string newUniqueId = uniqueId + "." + to_string(currentSIDDeletedScopes);
        currentScopeTable->setUniqueId(newUniqueId);
        currentSIDDeletedScopes = 1;
        if (logFile) fprintf(logFile, "\tScopeTable# %s created\n", newUniqueId.c_str());
    }
    bool insert(SymbolInfo *symbol) { return currentScopeTable->insert(symbol); }
    SymbolInfo* lookup(string key) {
        ScopeTable *tempScopeTable = currentScopeTable;
        SymbolInfo *symbol = nullptr;
        while (tempScopeTable != nullptr) {
            symbol = tempScopeTable->lookup(key);
            if (symbol != nullptr) return symbol;
            tempScopeTable = tempScopeTable->getParentScope();
        }
        return symbol;
    }
    void printAll(ofstream& file) {
        ScopeTable *tempScopeTable = currentScopeTable;
        while (tempScopeTable != nullptr) {
            tempScopeTable->print(file);
            tempScopeTable = tempScopeTable->getParentScope();
        }
    }
    bool exitScope() {
        if (currentScopeTable->getParentScope() == nullptr) {
            if (logFile) fprintf(logFile, "\tScopeTable# 1 cannot be deleted\n");
            return false;
        } else {
            ScopeTable *tempScopeTable = currentScopeTable;
            currentScopeTable = currentScopeTable->getParentScope();
            string id = tempScopeTable->getUniqueId();
            if (logFile) fprintf(logFile, "\tScopeTable# %s deleted\n", id.c_str());
            istringstream iss(id); string component; int lastNumber = 0;
            while (getline(iss, component, '.')) { int number; istringstream(component) >> number; lastNumber = number; }
            delete tempScopeTable;
            currentSIDDeletedScopes = lastNumber + 1;
            return true;
        }
    }
};
#endif