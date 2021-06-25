//
// Created by guy on 24/05/2021.
//

#ifndef COMPI_3_SYMBOLTABLE_H
#define COMPI_3_SYMBOLTABLE_H

#include "helpers.h"
#include "string.h"

typedef int type;

class Symbol
{
public:
    string Name;
    string Type;
    bool funcSym;
    int Offset;

    Symbol(const char* id, const char* t_type, int offset, bool fSym)
    {
        //printf("creating new symnol\n");
        Name = *(new string(id));
        Type = *(new string(t_type));
        Offset = offset;
        funcSym = fSym;
    }

    ~Symbol() = default;

};

class SymbolTable
{
public:
    bool isMainExists;
    vector<vector<Symbol>> variableTable;
    vector<funcDeclaration> funcList;
    vector<string> scopeTypes;
    vector<int> offsetTable;


    SymbolTable()
    {
        //printf("SymbolTable constructor start\n");

        //printf("creating new symbol table3\n");

        isMainExists = false;
        variableTable = *(new vector<vector<Symbol>>);
        funcList = *(new vector<funcDeclaration>);
        offsetTable = *(new vector<int>);
        scopeTypes = *(new vector<string>);
        offsetTable.push_back(0);

        this->addPrint();

        //printf("SymbolTable constructor finish\n");

    }

    void addPrint()
    {
        funcDeclaration print = *(new funcDeclaration("print",*(new vector<formalDeclaration>) ,"VOID"));
        funcDeclaration printi = *(new funcDeclaration("printi",*(new vector<formalDeclaration>) ,"VOID"));
        print.paramList.push_back(*new formalDeclaration("STRING", "STRING", 0));
        printi.paramList.push_back(*new formalDeclaration("INT", "INT", 0));
        this->addFunc(print);
        this->addFunc(printi);
    }

    bool isMainExist()
    {
        //printf("return main exists\n");
        return isMainExists;
    }

    bool isPrintOrPrinti(funcDeclaration f)
    {
        if(!f.funcName.compare("print") && !f.funcName.compare("printi"))
        {
            return false;
        }

        if(f.funcName.compare("print") == 0)
        {
            if(f.returnType.compare("VOID")==0)
            {
                if(f.paramList.size() == 1)
                {
                    if(f.paramList.back().type.compare("STRING")==0)
                    {
                        return true;
                    }
                }
            }
        }

        if(f.funcName.compare("printi") == 0)
        {
            if(f.returnType.compare("VOID")==0)
            {
                if(f.paramList.size() == 1)
                {
                    if(f.paramList.back().type.compare("INT")==0)
                    {
                        return true;
                    }
                }
            }
        }

        return false;

    }
    void printFuncs()
    {
        output::endScope();


        for(int i = 0; i<funcList.size();i++)
        {
            string retType = *(new string(funcList[i].returnType));
            vector<string> argTypeList = *(new vector<string>());

            for(int j = 0; j<funcList[i].paramList.size(); j++)
            {
                argTypeList.push_back(funcList[i].paramList[j].type);
            }
            string final = output::makeFunctionType(retType, argTypeList);
            output::printID(funcList[i].funcName, 0, final);
        }

    }


    void print()
    {
        output::endScope();
        if(variableTable.size()==0)
        {
            printf("1\n");

            for(int i = 0; i<funcList.size();i++)
            {
                printf("2\n");
                string retType = funcList[i].returnType;
                vector<string> argTypeList;
                for(int j = 0; j<funcList[i].paramList.size(); j++)
                {
                    printf("3\n");
                    argTypeList[j] = funcList[i].paramList[j].type;
                }
                printf("4\n");
                string final =  output::makeFunctionType(retType, argTypeList);

                output::printID(final, 0, retType);
            }
            return;
        }

        if(variableTable.size()==0)
        {
            return;
        }

        if(scopeTypes.back().compare(string("SWITCH"))!=0 && scopeTypes.back().compare(string("IF"))!=0 && scopeTypes.back().compare(string("ELSE"))!=0 && scopeTypes.back().compare(string("WHILE"))!=0
           && scopeTypes.back().compare(string("BLOCK"))!=0) //Current block is a function block.
        {

            //printf("Print func #1 \n");

            string tmp = scopeTypes.back();
            int i = 0;

            //printf("Print func #2 \n");

            while(tmp.compare(funcList[i].funcName)!=0)
            {
                //printf("Print func #3 \n");
                i++;
            }

            //printf("Print func #4 \n");

            for(int j = 0; j<funcList[i].paramList.size(); j++)
            {
                //printf("Print func #5 \n");
                output::printID(funcList[i].paramList[j].name, -j-1, funcList[i].paramList[j].type);
            }


        }

        //printf("Print func #6 \n");
        for(int i = 0;i<variableTable.back().size(); i++)
        {
            //printf("Print func #7 \n");
            if(!variableTable.back()[i].funcSym)
            {
                //printf("Print func #8 \n");
                output::printID(variableTable.back()[i].Name, variableTable.back()[i].Offset,variableTable.back()[i].Type);
                //printf("Print func #9 \n");
            }
        }
    }

    void pop()
    {
        print();
        if(variableTable.size()!=0)
        {
            variableTable.pop_back();
            offsetTable.pop_back();
            scopeTypes.pop_back();
        }
    }

    int addFunc(funcDeclaration & funcToAdd)
    {

        if(funcToAdd.funcName.compare(string("main"))==0 && (funcToAdd.returnType.compare("VOID")==0) && funcToAdd.paramList.size()==0)
        {
            isMainExists=true;
        }
        funcList.push_back(funcToAdd);
        return 0;
    }

    bool isFuncExistsByName(funcDeclaration funcToSearch)
    {

        //printf("isFuncExistsByName\n");

        string fName = funcToSearch.funcName;
        for(int i = 0; i<funcList.size(); i++)
        {
            int x = 8;
            if(strcmp(fName.c_str(),funcList[i].funcName.c_str())==0)
            {
                //printf("fName= %s funcList[i].funcName= %s;\n",fName, funcList[i].funcName);
                return true;
            }
        }
        return false;
    }

    bool isFuncExistsByNameStr(string fName)
    {

        for(int i = 0; i<funcList.size(); i++)
        {
            if(strcmp(fName.c_str(),funcList[i].funcName.c_str())==0)
            {
                //printf("fName= %s funcList[i].funcName= %s;\n",fName, funcList[i].funcName);
                return true;
            }
        }
        return false;
    }

    int argDuplicateInFuncSig(funcDeclaration & funcToAdd)
    {
        //printf("argDuplicateInFuncSig\n");

        for(int i = 0; i<funcToAdd.paramList.size(); i++)
        {
            for(int j = i+1; j<funcToAdd.paramList.size(); j++)
            {
                if(funcToAdd.paramList[i].name.compare(funcToAdd.paramList[j].name)==0)
                {
                    return i;
                }
            }
        }
        return -1;
    }

    int AddNewTable(const char* type)
    {
        //printf("AddNewTable\n");
        offsetTable.push_back(offsetTable.back());
        variableTable.push_back(*(new vector<Symbol>()));
        scopeTypes.push_back(*(new string(type)));

        return 1;
    }

    bool isVarExists(const char* ID)
    {
        //printf("isVarExists\n");
        for(int i = 0; i<variableTable.size(); i++)
        {
            for(int j = 0; j<variableTable[i].size(); j++)
            {
                if(variableTable[i][j].Name.compare(string(ID))==0)
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool isInsideWhile()
    {
        //printf("isInsideWhile\n");
        if(scopeTypes.back().compare(string("WHILE"))==0)
        {
            return true;
        }
        return false;
    }

    bool isInsideSwitch()
    {
        if(scopeTypes.back().compare(string("SWITCH"))==0)
        {
            return true;
        }
        return false;

    }

    int addNewArg(Symbol & s)
    {
        //printf("addNewArg\n");
        if(variableTable.size() == 0)
        {
            return -1;
        }
        if(s.Name.compare(string("NA"))!=0) {
            s.Offset = offsetTable[offsetTable.size() - 1];
            variableTable.back().push_back(s);

            if(!s.funcSym) {
                offsetTable[offsetTable.size() - 1]++;
            }

            return 1;
        }
        return -2;

    }

    bool isAssignLegal(const char* type1, const char* type2)
    {
        //("isAssignLegal\n");
        string stype1 = string(type1);
        string stype2 = string(type2);

        if(stype1.compare(stype2)==0 || ( stype1.compare(string("INT"))==0 && stype2.compare(string("BYTE"))==0) )
        {
            return true;
        }

        return false;
    }

    Symbol searchSymbolByName(const char* name)
    {
        //printf("searchSymbolByName\n");
        for(int i = 0; i<variableTable.size(); i++)
        {
            for(int j = 0; j<variableTable[i].size(); j++)
            {
                if(variableTable[i][j].Name.compare(string(name))==0)
                {
                    return variableTable[i][j];
                }
            }
        }

        return variableTable[0][0]; //should not reach here

    }

    funcDeclaration * getLastFunc()
    {
        //printf("getLastFunc\n");
        return &funcList.back();
    }

    bool equals(funcDeclaration f1, funcDeclaration f2)
    {
        //printf("equals\n");

        if(f1.paramList.size() != f2.paramList.size() || f1.funcName.compare(f2.funcName)!=0)
        {
            return false;
        }

        for(int i = 0; i<f1.paramList.size(); i++)
        {
            if(f1.paramList[i].type.compare(f2.paramList[i].type)!=0)
            {
                return false;
            }
        }

        return true;

    }


    bool isFunctionExist(funcDeclaration f, bool checkType)
    {
        //printf("isFunctionExist\n");

        for(int i = 0; i<funcList.size(); i++)
        {
            if(equals(f,funcList[i]))
            {
                if(checkType)
                {
                    if(f.returnType.compare(funcList[i].returnType)!=0)
                    {
                        return false;

                    }
                }

                return true;

            }
        }
        return false;
    }

    const char* getFunctionType(const char* funcName)
    {
        //printf("getFunctionType\n");

        for(int i = 0; i<funcList.size(); i++)
        {
            if(funcList[i].funcName.compare(string(funcName))==0)
            {
                return funcList[i].returnType.c_str();
            }
        }
        return "NULL";
    }

    funcDeclaration * getFunction(const char* funcName)
    {
        for(int i = 0; i<funcList.size(); i++)
        {
            if(funcList[i].funcName.compare(string(funcName))==0)
            {
                return &funcList[i];
            }
        }
        return nullptr;
    }

    bool isContainedByScopeType(const char* scope_Type)
    {
        string type = scope_Type;
        for(int i = 0; i<scopeTypes.size(); i++)
        {
            if(scopeTypes[i].compare(type)==0)
            {
                return true;
            }
        }
        return false;
    }

};


#endif //COMPI_3_SYMBOLTABLE_H
