
#include "helpers.h"
#include "bp.hpp"
#include <stdio.h>


#define div 0
#define mul 1
#define plus 2
#define minus 3

#define equalss 0
#define not_equals 1
#define bigger_then 2
#define smaller_then 3
#define bigger_equal_then 4
#define smaller_equal_then 5



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

void handleLW(short offst, string type, short sSize, short isExistsOnStack, string * toRet)
{
    string tmp;
    const char* vPointer;
    const char* lw_reg;
    short offset;
    string toReturn;
    offset = offst + 1;
    if(!offst >= 0) {
        offset = -1 * offset;
        vPointer = rAlc();
        lw_reg = rAlc();
        toReturn = lw_reg;
        tmp += vPointer;
        tmp += " =  getelementptr [";
        tmp += to_string(sSize);
        tmp += "x i32], [";
        tmp += to_string(sSize);
        tmp += "x i32]* %inputPtr, i32 0, i32 ";
        tmp += to_string(offset);
        tmp += "\n";

        tmp += lw_reg;
        tmp += " = load i32, i32* ";
        tmp += vPointer;
        tmp += "\n";


        const char *retThis;

        if (type.compare(string("INT")) == 0) {
            retThis = rAlc();
            tmp += retThis;
            tmp += " = trunc i32 ";
            tmp += lw_reg;
            tmp += " to ";
            tmp += type;
            tmp += "\n";
            CodeBuffer::instance().emit(tmp.c_str());
            *toRet = *(new string(retThis));
            return;
        }
        CodeBuffer::instance().emit(tmp.c_str());
        retThis = lw_reg;
        *toRet = *(new string(retThis));
        return;
    }

    const char* v2Pointer = rAlc();
    const char* lw_reg2 = rAlc();

    tmp += v2Pointer;
    tmp += " =  getelementptr [50 x i32], [50 x i32]* %ptr, i32 0, i32 ";
    tmp += to_string(offset);
    tmp += "\n";
    tmp += lw_reg2;
    tmp += " = load i32, i32* ";
    tmp += v2Pointer;
    tmp += "\n";
    if (type.compare(string("INT")) == 0) {
        string retThis;
        retThis = rAlc();
        tmp += retThis;
        tmp += " = trunc i32 ";
        tmp += lw_reg2;
        tmp += " to ";
        tmp += type;
        tmp += "\n";
        CodeBuffer::instance().emit(tmp.c_str());
        *toRet = *(new string(retThis));
        return;
    }

    CodeBuffer::instance().emit(tmp.c_str());
    *toRet = *(new string(lw_reg2));
    return;
}

void emitFuncRetType(const char* retType)
{
    if(string("VOID").compare(string(retType)) == 0)
    {
        CodeBuffer::instance().emit(string("ret void\n"));
    }
    else
    {
        string tmp = "ret ";
        tmp += retType;
        tmp+= " 0\n";
        tmp+= "}";
        CodeBuffer::instance().emit(tmp);
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

const char* emitSW_local( const char* type, const char* reg, long long offst)
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
            tmp = emitSW_local( type, reg, offset);
            break;
    }

    //TODO: Add the following code.
    CodeBuffer::instance().emit(tmp);
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
    CodeBuffer::instance().emit(funcToEmit);

    funcLocalVars = "%ptr = alloca [50 x i32], i32 50\n%inputPtr = alloca [";
    funcLocalVars += to_string(listOfFormalDec.size());
    funcLocalVars += " x i32], i32 ";
    funcLocalVars += to_string(listOfFormalDec.size());
    funcLocalVars+= '\n';


    CodeBuffer::instance().emit(funcLocalVars);

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

void callFuncEmitHandler(const char* toRet, const char* funcId, const char* retType, vector<Exp> * expArr = nullptr, vector<string> * inputArr = nullptr)
{
    unsigned short j = 0;
    unsigned short inc = 0;
    string tmp;
    string retThis = *(new string());
    const char* toEmit[6] = {"call void @print( i8* getelementptr (", " , ", "* ", ", i32 0, i32 0) )", " = zext i8 ", " to i32"};
    bool indicator = expArr != nullptr? true : false;

    vector<Exp>  expArrTmp = vector<Exp>();

    if(expArr != nullptr)
    {
        expArrTmp = *expArr;
    }

    if(string(funcId).compare(string("print")) == 0)
    {
        tmp = toEmit[0];
        tmp += (*expArr)[0].ptr0;
        tmp += toEmit[1];
        tmp += (*expArr)[0].ptr0;
        tmp += toEmit[2];
        tmp += (*expArr)[0].ptr1;
        tmp += toEmit[3];
        tmp += '\n';
        CodeBuffer::instance().emit(tmp);
        return;
    }

    inc=1;

    while (j < expArrTmp.size())
    {
       if((expArrTmp)[j].type.compare("BYTE") == 0)
       {
           if((*inputArr)[j].compare("INT") == 0) {
               (expArrTmp)[j].type = string("INT");
                if(expArrTmp[j].rigister.compare("NA")!=0)
                {
                    const char* rg = rAlc(); //toEmit
                    tmp += rg;
                    tmp += toEmit[4];
                    tmp += expArrTmp[j].rigister.c_str();
                    tmp += toEmit[4];
                    tmp += "\n";
                    expArrTmp[j].rigister = rg;
                }
           }
       }
       ++j;
    }

    if(string(retType).compare("VOID") != 0)
    {
        retThis = rAlc();
        tmp += retThis;
        tmp += " = ";
    }
    tmp += "call ";
    tmp += retType;
    tmp +=  " @";
    tmp += funcId;
    tmp += "(";

    j = 0;

    while(j<expArrTmp.size())
    {
        if(expArrTmp[j].rigister.compare("NA"))
        {
            tmp += expArrTmp[j].type.c_str();
            tmp += " ";
            tmp += expArrTmp[j].info;
            tmp += ", ";
        }
        else
        {
            tmp += expArrTmp[j].type.c_str();
            tmp += " ";
            tmp += expArrTmp[j].rigister;
            tmp += ", ";
        }

        if(inc >= expArrTmp.size())
        {
                tmp.pop_back();
        }
        ++inc;

    }

    tmp += ')';

    CodeBuffer::instance().emit(tmp);

    toRet = retThis.c_str();
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
    idx12[0] = CodeBuffer::instance().emit(string(" br label @\n"));
    labels[1] = CodeBuffer::instance().genLabel().c_str(); //flbl
    idx12[1] = CodeBuffer::instance().emit(string(" br label @\n"));

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

    CodeBuffer::instance().emit(strStorage[0]);
}

void handleUBR(bpList * toRet)
{
    (*toRet)[0] = (pair<int, BranchLabelIndex>(CodeBuffer::instance().emit(string("br label @\n")), FIRST));
}

void emitConditionalBranchSavedArg(string rg, bpList * tl, bpList * fl)
{
    long long idx;
    string tmp;
    tmp = "br i1 ";
    tmp += rg;
    tmp += ", label @, label @";

    idx = CodeBuffer::instance().emit(tmp);

    pair<int, BranchLabelIndex >  p1((int)idx, FIRST);
    pair<int, BranchLabelIndex >  p2((int)idx, SECOND);

    (*tl)[0] = p1;
    (*fl)[0] = p2;

}

void emitRelopHandler(const char* op, const char* left, const char* right, const char* leftType, const char* rightType, bpList ** list)
{
    int case_num;
    const char* oper;
    const char* t_type;
    bool cond = (string(leftType).compare(string(rightType)) == 0) && string(leftType).compare(string("BYTE"));

    if(string(op).compare(string("==")) == 0)
    {
        case_num = equalss;
    }
    if(string(op).compare(string("!=")) == 0)
    {
        case_num = not_equals;
    }
    if(string(op).compare(string("<")) == 0)
    {
        case_num = smaller_then;
    }
    if(string(op).compare(string("<=")) == 0)
    {
        case_num = smaller_equal_then;
    }
    if(string(op).compare(string(">")) == 0)
    {
        case_num = bigger_then;
    }
    if(string(op).compare(string(">=")) == 0)
    {
        case_num = bigger_equal_then;
    }

    switch(case_num)
    {
        case equalss:
            oper = "eq";
            if(cond) {
                t_type = "i8";
            }
            else {
                t_type = "i32";
            }
            break;

        case not_equals:
            oper = "ne";
            if(cond) {
                t_type = "i8";
            }
            else {
                t_type = "i32";
            }
            break;

        case smaller_then:
            if(cond) {
                t_type = "i8";
                oper = "ult";
            }
            else {
                t_type = "i32";
                oper = "slt";
            }
            break;

        case bigger_then:
            if(cond) {
                t_type = "i8";
                oper = "ugt";
            }
            else {
                t_type = "i32";
                oper = "sgt";
            }
            break;

        case smaller_equal_then:
            if(cond) {
                t_type = "i8";
                oper = "ule";
            }
            else {
                t_type = "i32";
                oper = "sle";
            }
            break;

        case bigger_equal_then:
            if(cond) {
                oper = "uge";
                t_type = "i8";
            }
            else {
                oper = "sge";
                t_type = "i32";
            }
            break;

        default:
            break;

    }

    string tmpRg;
    string blRg;
    string tmp;

    if(string(leftType).compare("BYTE") == 0 && string(rightType).compare("BYTE") != 0)
    {
        tmpRg = rAlc();
        blRg = rAlc();
        tmp = tmpRg;
        tmp += " = zext i8 ";
        tmp += leftType;
        tmp += " to i32\n";
        tmp += blRg;
        tmp +=" = icmp ";
        tmp += oper;
        tmp += " i32 ";
        tmp += tmpRg;
        tmp += " , ";
        tmp+= rightType;
        tmp += "\n";

    }

    if(string(leftType).compare("BYTE") != 0 && string(rightType).compare("BYTE") == 0)
    {
        tmpRg = rAlc();
        blRg = rAlc();
        tmp = tmpRg;
        tmp += " = zext i8 ";
        tmp += rightType;
        tmp += " to i32\n";
        tmp += blRg;
        tmp +=" = icmp ";
        tmp += oper;
        tmp += " i32 ";
        tmp += tmpRg;
        tmp += " , ";
        tmp+= leftType;
        tmp += "\n";
    }
    else
    {
        blRg = rAlc();
        tmp += blRg;
        tmp += " = icmp ";
        tmp += oper;
        tmp += " ";
        tmp += left;
        tmp += " , ";
        tmp+= right;
        tmp += "\n";
    }
    CodeBuffer::instance().emit(tmp);
    tmp = "br i1 ";
    tmp += blRg;
    tmp += " , label @, label @\n";
    unsigned short idx;
    idx = CodeBuffer::instance().emit(tmp);

    bpList * toRet = new bpList();
    (*toRet)[0] = *(new pair<int, BranchLabelIndex>(idx, FIRST));
    (*toRet)[1] = *(new pair<int, BranchLabelIndex>(idx, SECOND));

    *list = toRet;

}

const char* emitBinaryHandler(int op, const char* left, const char* right, const char* leftType, const char* rightType)
{
    string tmp;
    const char* opArr[4] = {"sdiv ", "mul ", "add ", "sub "};
    if(string(leftType).compare("BYTE") != 0 && string(rightType).compare("BYTE") != 0) {
        const char* rg = rAlc();
        tmp += rg;
        tmp += " = ";
        tmp += opArr[op];
        tmp += "i32 ";
        tmp += left;
        tmp += " , ";
        tmp += right;
        tmp += '\n';

        CodeBuffer::instance().emit(tmp.c_str());

        return rg;
    }

    if(string(left).compare("BYTE") == 0 && string(right).compare("BYTE") == 0)
    {
        const char* rg = rAlc();
        tmp += rg;
        tmp += " = ";
        tmp += opArr[op];
        tmp += "i8 ";
        tmp += left;
        tmp += " , ";
        tmp += right;
        tmp += '\n';

        CodeBuffer::instance().emit(tmp.c_str());

        return rg;
    }

    if(string(left).compare("BYTE") == 0)
    {
        const char* one = rAlc();
        const char* rg = rAlc();
        tmp = one;
        tmp += "= zext i8 ";
        tmp += left;
        tmp += " to i32\n";
        tmp += rg;
        tmp += " = sub i32 ";
        tmp += one;
        tmp += " , ";
        tmp += right;
        tmp +="\n";
        CodeBuffer::instance().emit(tmp);
        return rg;
    }

    const char* two = rAlc();
    const char* rg = rAlc();
    tmp = two;
    tmp += "= zext i8 ";
    tmp += right;
    tmp += " to i32 \n";

    tmp += rg;
    tmp += " = sub i32 ";
    tmp += left;
    tmp += " , ";
    tmp += two;
    tmp += "\n";

    CodeBuffer::instance().emit(tmp);
    return rg;

}

void ESbool(const char* rg,  string * toRet) // ammit sweach bul
{
    string tmp;
    string reg;
    reg = rAlc();
    tmp = reg;
    tmp += " = add i1 ";
    tmp += rg;
    tmp += " , 1\n";
    CodeBuffer::instance().emit(tmp.c_str());
    *toRet = reg;
}

void emitBinary(const char* op, const char* left, const char* right, const char* leftType, const char* rightType, const char ** ret)
{
    int opId;
    string toret;

    if(string(op).compare(string("/")) == 0)
    {
        opId = div;
    }

    if(string(op).compare(string("*")) == 0)
    {
        opId = mul;
    }

    if(string(op).compare(string("+")) == 0)
    {
        opId = plus;
    }

    if(string(op).compare(string("-")) == 0)
    {
        opId = minus;
    }

    toret = emitBinaryHandler(opId, left, right, leftType, rightType);
    *ret = toret.c_str();
}


