
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
        CodeBuffer::instance().emit("ret void\n");
    }
    else
    {
        string tmp = "ret ";
        tmp += retType;
        tmp+= " 0\n";
        tmp+= "}";
        CodeBuffer::instance().emit(tmp.c_str());
    }
}

bpList pushUncondBr(){
    bpList listToHandle;
//    string tmp = "br label @";
//    int index = CodeBuffer::instance().emit(tmp);
    int place = CodeBuffer::instance().emit("br label @");
    listToHandle.push_back(pair<int, BranchLabelIndex>(place, FIRST));
    return listToHandle;
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

    //Add the following code:
    CodeBuffer::instance().emit(funcToEmit.c_str());

    funcLocalVars = "%ptr = alloca [50 x i32], i32 50\n%inputPtr = alloca [";
    funcLocalVars += to_string(listOfFormalDec.size());
    funcLocalVars += " x i32], i32 ";
    funcLocalVars += to_string(listOfFormalDec.size());
    funcLocalVars+= '\n';


    CodeBuffer::instance().emit(funcLocalVars.c_str());

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

string handleBool()
{ //TBD: Order may be bad

    string toret = *(new string());
    long long idx12[2];
    const char* labels[3];
    bpList list = *(new bpList());
    const char* toWrite[3];
    toWrite[0] = " = phi i1 [ 1, %";
    toWrite[1] = " ], [ 0, %";
    toWrite[2] = "]\n";

    labels[0] = CodeBuffer::instance().genLabel().c_str(); //tlbl
    idx12[0] = CodeBuffer::instance().emit(" br label @");
    labels[1] = CodeBuffer::instance().genLabel().c_str(); //flbl
    idx12[1] = CodeBuffer::instance().emit(" br label @");

    pair<int, BranchLabelIndex> pair1 = *(new pair<int, BranchLabelIndex>(idx12[0], FIRST));
    pair<int, BranchLabelIndex> pair2 = *(new pair<int, BranchLabelIndex>(idx12[1], FIRST));

    list[0] = pair1;
    list[1] = pair2;

    labels[2] = CodeBuffer::instance().genLabel().c_str(); //bpatch this

    string rigister = rAlc();
    string tmp;
    tmp += rigister;
    tmp+= toWrite[0];
    tmp+= labels[0];
    tmp += toWrite[1];
    tmp += labels[1];
    tmp += toWrite[2];
    CodeBuffer::instance().emit(tmp);

    CodeBuffer::instance().bpatch(list, labels[2]);

    toret = rigister;
    toret += '#';
    toret += labels[0];
    toret += '#';
    toret += labels[1];

    return toret;
}


string returnFirstAppearance(string s)
{
    string ret;
    for(int i = 0; i<s.size(); i++)
    {
        if(s[i] != '#')
        {
            ret[i] = s[i];
        }
    }
    return ret;
}

string returnSecondAppearance(string s)
{
    string ret;
    int j = 0;
    for(int i = 0; i<s.size(); i++)
    {
        if(s[i]!='#')
        {
            j++;
        }
    }

    j+=2;

    for(int i = j; i<s.size(); i++)
    {
        if(s[i] != '#')
        {
            ret[i-j] = s[i];
        }
    }
    return ret;
}

string returnThirdAppearance(string s)
{
    string tmp1;
    string ret;
    int idx = 0;
    for(int i = s.size()-1; i>=0; i--)
    {
        if(s[i] != '#')
        {
            tmp1[idx] += s[i];
            idx++;
        }
    }

    idx = 0;

    for(int i = 0; i<tmp1.size(); i++)
    {
        ret[i] = tmp1[tmp1.size()-1-i];
    }

    return ret;
}


void finishFunctionScope(string info, string type, string rigister, string retTypeOfFunction)
{
    string strStorage[4];
    bool cond = (type.compare(("BYTE")) == 0) &&(retTypeOfFunction.compare("INT") == 0);

    strStorage[2] = "= zext i8 ";
    strStorage[3] = " to i32\n";
    long long caseVar = rigister.compare("NA") == 0 ? 1:0;
    if(cond)
    {
        type = *(new string("INT"));
        if (rigister.compare("NA") != 0)
        {
            string retReg = rAlc();
            strStorage[0] = retReg;
            strStorage[0] += strStorage[2];
            strStorage[0] += rigister;
            strStorage[0] += strStorage[3];
            rigister = *(new string(retReg.c_str()));
        }
    }

    switch(caseVar)
    {
        case 1:
            strStorage[1] = info.c_str();
            break;

        case 2:
            strStorage[1] = rigister.c_str();
            break;
    }

    strStorage[0] += "ret ";
    strStorage[0]+= retTypeOfFunction.c_str();
    strStorage[0]+=" ";
    strStorage[0]+=strStorage[1];
    strStorage[0]+= "\n";

    CodeBuffer::instance().emit(strStorage[0].c_str());
}
