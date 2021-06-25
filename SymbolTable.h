//
// Created by guy on 24/05/2021.
//

#ifndef COMPI_3_SYMBOLTABLE_H
#define COMPI_3_SYMBOLTABLE_H

#include "helpers.h"

typedef int type;

class Symbol
{
public:
    string ID;
    string Type;
    string value;
    int offset;

    Symbol(const char* id, const char* t_type, const char* val, int offst)
    {
        ID = *(new string(id));
        Type = *(new string(t_type));
        value = *(new string(val));
        offset = offst;
    }

    ~Symbol() = default;

};

class SymbolTable
{
public:
    bool isMainExists;

    SymbolTable()
    {
        isMainExists = false;
    }

    int symbolOffset(const char* id) {
    //iterate over the 2d array of vectors, get the relevant
        for (vector<Symbol> symbolsVector : variableTable)
            for (Symbol sym : symbolsVector)
                if (sym.Name.compare(id.c_str()) == 0)
                    return sym.Offset;
        return INT32_MIN;
    }

    string symbolType(const char* id) {
        //iterate over the 2d array of vectors, get the relevant
        for (vector<Symbol> symbolsVector : variableTable)
            for (Symbol sym : symbolsVector)
                if (sym.Name.compare(id.c_str()) == 0)
                    return sym.Type;
        return INT32_MIN;
    }
    bool isMainExist()
    {
        return isMainExists;
    }

    void pop()
    {
        //TODO: Pop from current scope
    }

    int addFunc(funcDeclaration & funcToAdd)
    {
        //TODO: Add func to database
        return 0;
    }

    int isFuncExists(funcDeclaration & funcToAdd)
    {
        return 0;
        //TODO: Check if func exists in DB
    }

    int argDuplicateInFuncSig(funcDeclaration & funcToAdd)
    {
        return 0;
        //TODO: Check for arg duplication
    }

    int AddNewTable(const char* type)
    {
        return 0;
    }

    int isArgExists(const char* ID)
    {
        return 0;
    }

    int addNewArg(Symbol s)
    {
        return 0;
    }


    bool isAssignLegal(const char* type1, const char* type2)
    {
        return true;
    }

    Symbol searchSymbolByName(const char* name)
    {

    }
};


#endif //COMPI_3_SYMBOLTABLE_H
