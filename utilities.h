
#include "helpers.h"
#include "bp.hpp"


long long regCount = 0;
const char* rAlc()
{
    string tmp = "";
    tmp[0] = '%';
    tmp[1] = 't';
    tmp+= to_string(regCount).c_str();
    regCount = regCount + 1;
    return tmp.c_str();
}

void emitFuncRetType(const char* retType)
{
    if(string("VOID").compare(string(retType)) == 0)
    {
        //TODO: Addthis -> CodeBuffer::instance().emit("ret void\n");
    }
    else
    {
        string tmp = "ret ";
        tmp += retType;
        tmp+= " 0\n";
        tmp+= "}";
        //TODO: Addthis -> CodeBuffer::instance().emit("tmp");
    }
}

const char* emitSW_sig(long long size, const char* type, const char* reg, long long offst)
{
    string emit_this;
    long long offset = offst + 1;
    offset = -1 * offset;
    bool cond1 = (reg[0] == '%');
    bool cond2 = string(type).compare(string("INT")) == 0? true : false;
    bool final = cond1 & cond2;
    string regg;
    regg = rAlc();
    if(final)
    {
        string nvar = rAlc();
        emit_this = nvar;
        emit_this += " = zext ";
        emit_this += type;
        emit_this[emit_this.size()] = ' ';
        emit_this += reg;
        emit_this += string(" to i32\n");
        reg = nvar.c_str();
    }

    emit_this += regg;
    emit_this +=string(" =  getelementptr [");
    emit_this += to_string(size).c_str();
    emit_this += "x i32], [";
    emit_this += to_string(size).c_str();
    emit_this += "x i32]* ";
    emit_this += "%inputPtr";
    emit_this += ", i32 0, ";
    emit_this +="i32 \"";
    emit_this += to_string(offset);
    emit_this += '\n';
    emit_this += "store ";
    emit_this += " i32";
    emit_this += reg;
    emit_this += ", i32* ";
    emit_this += regg;
    emit_this += '\n';

    return emit_this.c_str();

}

const char* emitSW_local(long long size, const char* type, const char* reg, long long offst)
{
    string emit_this;
    long long offset = offst + 1;
    offset = -1 * offset;
    bool cond1 = (reg[0] == '%');
    bool cond2 = string(type).compare(string("INT")) == 0? true : false;
    bool final = cond1 & cond2;
    string regg = rAlc();

    if(final)
    {
        string nVr = rAlc();
        emit_this = nVr;
        emit_this += string(" = zext ");
        emit_this += string(type);
        emit_this[emit_this.size()] = ' ';
        emit_this += reg;
        emit_this += " to i32\n";
        reg = nVr.c_str();

    }

    emit_this += regg;
    emit_this += " =  getelementptr [50 x i32], [50 x i32]* %ptr, i32 0, i32 ";
    emit_this += to_string(offset);
    emit_this += "\n";
    emit_this += "store i32 ";
    emit_this += reg;
    emit_this += string(", i32* ");
    emit_this += regg;
    emit_this += "\n";

    return emit_this.c_str();

}

void emitSW(long long size, const char* type, const char* reg, long long offset)
{
    string tmp;

    int offst_case = 1;
    if(offset < 0)
    {
        offst_case = -1;
    }

    switch(offst_case)
    {
        case -1:
            tmp = emitSW_sig(size, type, reg, offset);
            break;

        default:
            break;
            //tmp = emitSW_localvar(size, type, reg, offset);
    }

    //TODO: Add the following code.
    //CodeBuffer::instance().emit(tmp.str());
}

void funcHandler(const char* funcName, const char* retType, const vector<string>& listOfFormalDec)
{
    int j = 0;

    string funcToEmit = "define ";
    string funcLocalVars;
    string funcArgsInSig;
    funcToEmit+=retType;
    funcToEmit+= " @";
    funcToEmit+= funcName;
    funcToEmit+="(";

    while(j<listOfFormalDec.size())
    {
        funcToEmit += listOfFormalDec[j];
        funcToEmit += ", ";
        j = j+1;
    }
    if(listOfFormalDec.size() > 0)
    {
        funcToEmit[funcToEmit.size()-2] = '\0';
    }
    funcToEmit += "){\n";

    //TODO: Add the following code:
    //CodeBuffer::instance().emit(s.str());

    funcLocalVars = "%ptr = alloca [50 x i32], i32 50\n%inputPtr = alloca [";
    funcLocalVars += to_string(listOfFormalDec.size());
    funcLocalVars += " x i32], i32 ";
    funcLocalVars += to_string(listOfFormalDec.size());
    funcLocalVars+= '\n';

    //TODO: Add the following code:
    //CodeBuffer::instance().emit(funcLocalVars.str());

    j = 0;

    while(j<listOfFormalDec.size())
    {
        char* c_rigister = "%";
        string * s_rigister = new string (c_rigister);
        *s_rigister += to_string(j);
        emitSW(listOfFormalDec.size(), listOfFormalDec[j].c_str(), s_rigister->c_str(), -j);
    }

}

vector<string> formalsVecToStringVec(vector<formalDeclaration> formalsListDec)
{
    vector<string> toRet = (*new vector<string>());
    for(int i = 0; i<formalsListDec.size(); i++)
    {
        toRet[i] = formalsListDec[i].type;
    }

    return toRet;
}