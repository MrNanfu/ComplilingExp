//
//  ll1Compile.cpp
//  compilingExp
//
//  Created by M-Mac on 2020/5/15.
//  Copyright © 2020年 Chenyanting. All rights reserved.
//


#include "ll1Compile.hpp"
using namespace std;

struct Token
{
    int type;
    char value[20];
};

#define NUM_KEYWORD 15
#define NUM_SEPARATER 8
#define NUM_OPERATOR 8
#define NUM_FILTER 4
#define MAX_STR 20
#define TABLE_ROW 5
#define TABLE_CLOUMN 6

extern vector<Token> tokenVector;

//定义有限正规集
extern string KEYWORD[NUM_KEYWORD];
extern char SEPARATER[NUM_SEPARATER];
extern char OPERATOR[NUM_OPERATOR];
extern char FILTER[NUM_FILTER];

//定义语法规则数组
vector<RgOfGrammer> rgVector;

//定义一个存放每个字符的first集和follow集的向量
vector<FirstAndFollowOfChar> firstAndFollowOfCharVector;

//定义分析表向量矩阵
string ll1Table[TABLE_ROW][TABLE_CLOUMN];

//定义字母到数字的映射
map<char,int> chToNum = {{'E',0},{'T',1},{'F',2},{'H',3},{'J',4},{'i',0},{'(',1},{')',2},{'+',3},{'*',4},{'#',5}};

//定义一个临时随机字符数组，便于左递归消除
char randomChars[8] = {'H','J','K','M','N','O','Q','R'};
int indexOfRandomChars = 0;



int getIndexOfCharInFirstAndFollowOfCharVector(vector<FirstAndFollowOfChar> firstAndFollowOfCharVector,char ch)
{
    for (int k = 0; k < firstAndFollowOfCharVector.size(); k++) {
        if(firstAndFollowOfCharVector[k].ch == ch)
            return k;
    }
    return  -1;
}

int getIndexOfRgOfGrammerVector(vector<RgOfGrammer> rgVector,char ch)
{
    for (int k = 0; k < rgVector.size(); k++) {
        if(rgVector[k].left[0] == ch)
            return k;
    }
    return  -1;
}

set<char> getFirstSetOfChar(vector<FirstAndFollowOfChar> firstAndFollowOfCharVector,vector<RgOfGrammer> rgVector,char ch)
{
    set<char> firstSet;
    int index = getIndexOfRgOfGrammerVector(rgVector, ch);
    for (int i = 0 ; i < rgVector[index].right.size(); i++)
    {
        if (isupper(rgVector[index].right[i][0]))
        {
            set<char> tempSet  = getFirstSetOfChar(firstAndFollowOfCharVector,rgVector,rgVector[index].right[i][0]);
            set<char>::iterator iterator;
            for (iterator = tempSet.begin();iterator != tempSet.end();iterator++) {
                firstSet.insert(*iterator);
            }
        }
        else if(rgVector[index].right[i][0] == '\0')
        {
            firstSet.insert('\0');
        }
        else
            firstSet.insert(rgVector[index].right[i][0]);
    }
    return firstSet;
}

set<char> getFollowSetOfChar(vector<FirstAndFollowOfChar> firstAndFollowOfCharVector,vector<RgOfGrammer> rgVector,char ch)
{
    set<char> followSet;
    for (int i = 0; i < rgVector.size(); i++)
    {
        if (i == 0 && ch == rgVector[0].left[0])
        {
            followSet.insert('#');
        }
            for (int j = 0; j < rgVector[i].right.size(); j++)
            {
                int indexOfStr = rgVector[i].right[j].find(ch);
                if(indexOfStr != -1)
                {
                    if (indexOfStr == rgVector[i].right[j].size()-1 && rgVector[i].right[j][indexOfStr] != rgVector[i].left[0])
                    {
                        set<char> tempSet = getFollowSetOfChar(firstAndFollowOfCharVector,rgVector,rgVector[i].left[0]);
                        set<char>::iterator iterator;
                        for (iterator = tempSet.begin(); iterator != tempSet.end(); iterator++) {
                            followSet.insert(*iterator);
                        }
                    }
                    else
                    {
                        if (!isupper(rgVector[i].right[j][indexOfStr+1]))
                        {
                            followSet.insert(rgVector[i].right[j][indexOfStr+1]);
                        }
                        else
                        {
                            int indexInSetVector = getIndexOfCharInFirstAndFollowOfCharVector(firstAndFollowOfCharVector, rgVector[i].right[j][indexOfStr+1]);
                            if(firstAndFollowOfCharVector[indexInSetVector].firstSet.count('\0') != 0 && rgVector[i].right[j][indexOfStr] != rgVector[i].left[0])
                            {
                                set<char> tempSet = getFollowSetOfChar(firstAndFollowOfCharVector,rgVector,rgVector[i].left[0]);
                                set<char>::iterator iterator;
                                for (iterator = tempSet.begin(); iterator != tempSet.end(); iterator++) {
                                    followSet.insert(*iterator);
                                }
                            }
                            set<char> tempSet = firstAndFollowOfCharVector[indexInSetVector].firstSet;
                            set<char>::iterator iterator;
                            for (iterator = tempSet.begin(); iterator != tempSet.end(); iterator++) {
                                followSet.insert(*iterator);
                            }
                        }

                    }

                }
            }

    }
    return followSet;
}


bool grammerAnalyse(FILE *stream)
{
    char chars[MAX_STR];
    char charsLeft[MAX_STR];
    int indexOfCharsLeft = 0;
    char charsRight[MAX_STR];
    int indexOfCharsRight = 0;
    //读取文件的语法信息转化为语法规则数组,先将一行字符长读入chars，然后根据分隔符把chars分割成charsLeft与charsRight
    //注意！！文件的最后一行一定需要打一个空格！
    while (!feof(stream))
    {
        //初始化变量
        indexOfCharsRight = 0;
        indexOfCharsLeft = 0;
        for(int i = 0 ;i < MAX_STR; i++)
        {
            charsLeft[i] = '\0';
            chars[i] = '\0';
            charsRight[i] = '\0';
        }
        //在文件读取新的一行
        fgets(chars, MAX_STR, stream);
        if(chars[0] == '#')
            break;
//        string strs(str);
//        cout << strs;
        
        //将chars分割为charsLeft和charsRight
        bool isLeft = true;
        for(int i = 0; i < MAX_STR; i++)
        {
            if(chars[i] != '\0')
            {
                if(chars[i] == '>')
                    isLeft = false;
                if(isLeft)
                {
                    charsLeft[indexOfCharsLeft] = chars[i];
                    indexOfCharsLeft++;
                }
                else if(chars[i] != '>')
                {
                    charsRight[indexOfCharsRight] = chars[i];
                    indexOfCharsRight++;
                }
            }
        }
        RgOfGrammer rg;//定义rg对象
        string tmpStr1(charsLeft);
        rg.left = tmpStr1;
        //定义一个temp字符串数组，用来存放语法表达式右边的一组值
        char tempCharsRight[MAX_STR] = {'\0'};
        int indexOftempCharsRight = 0;
        for(int i = 0 ; i < MAX_STR; i++)
        {

            if(charsRight[i] != '\0')
            {
                if(charsRight[i] != '|' && charsRight[i] != '\n' )
                {
                    tempCharsRight[indexOftempCharsRight] = charsRight[i];
                    indexOftempCharsRight++;
                }
                else
                {
                    string tmpStr2(tempCharsRight);
                    for(int j = 0 ; j < MAX_STR;j++)
                    tempCharsRight[j] = '\0';
                    indexOftempCharsRight = 0;
                    rg.right.push_back(tmpStr2);
                }
            }
        }
        //将得到的一个表达式压入语法表达式向量中
        rgVector.push_back(rg);
    }
    
    //输出语法表达式的读取情况
    cout << "---------------------------未经处理语法---------------------------------------" << endl;
    for(int i = 0 ; i < rgVector.size();i++)
    {
        for(int j = 0 ; j < rgVector[i].right.size();j++)
        {
            cout << "Left:" << rgVector[i].left << ",Right:" << rgVector[i].right[j] << endl;
        }
    }


    //消除直接左递归
    for (int i = 0; i < rgVector.size(); i++)
    {
        vector<string> tempRecursiveVector;
        vector<string> tempNoRecursiveVector;
        bool reset = false;
        for (int j = 0; j < rgVector[i].right.size(); j++)//判断是否需要进行左递归消除操作
        {
            if (rgVector[i].left[0] == (rgVector[i].right[j])[0])
            {
                reset = true;
            }
        }
        if(reset)//如果需要进行左递归消除，那么就将产生式右边的式子分成左递归和非左递归
        {
            for (int j = 0; j < rgVector[i].right.size(); j++)
            {
                if (rgVector[i].left[0] == (rgVector[i].right[j])[0])
                {
                    tempRecursiveVector.push_back(rgVector[i].right[j]);
                }
                else
                    tempNoRecursiveVector.push_back(rgVector[i].right[j]);
            }
            rgVector[i].right.clear();//清除右边的产生式,以便消除左递归、构成新的右部产生式
            char randomChar = randomChars[indexOfRandomChars];
            indexOfRandomChars++;
            for (int j = 0; j < tempNoRecursiveVector.size(); j++)
            {//根据原无递归右部产生式来生成新的右部产生式
                rgVector[i].right.push_back(tempNoRecursiveVector[j].append(1,randomChar));
            }
            //对于原右部递归产生式，生成新的右部产生式
            RgOfGrammer rg;
            string strLeft = "";
            strLeft.append(1,randomChar);
            rg.left = strLeft;
            for (int j = 0; j < tempRecursiveVector.size(); j++)
            {
                string tempStr = tempRecursiveVector[j].substr(1).append(1,randomChar);
                rg.right.push_back(tempStr);
            }
            //最后添加一个空字符
            rg.right.push_back("");
            rgVector.push_back(rg);
        }
    }
    
    //输出消除左递归后表达式的情况
    cout << "---------------------------消除左递归后---------------------------------------" << endl;
        for(int i = 0 ; i < rgVector.size();i++)
        {
            for(int j = 0 ; j < rgVector[i].right.size();j++)
            {
                cout << "Left:" << rgVector[i].left << ",Right:" << rgVector[i].right[j] << endl;
            }
        }
    

    
    //提取公共左因子,注意，可能会有多个首公共左因子
    for (int i = 0; i < rgVector.size(); i++)//遍历每个产生式
    {
        //用来存放产生式右部，多个不同类型相同首字符的字符串的集合，一个没有相同首字符的字符串的集合
        vector<vector<string>> sameVectors;
        vector<string> noSameVector;
        bool reset = false;
        bool hasSame[10] = {false};
        for (int j = 0; j < rgVector[i].right.size(); j++)//遍历每个产生式右部
        {
            vector<string> sameVector;//对于右部的每个产生式，自己肯定跟自己是同一组
            sameVector.push_back(rgVector[i].right[j]);
            for(int k = 0; k < rgVector[i].right.size() && k != j;k++)
            {

                if(!hasSame[k])
                {
                    if(rgVector[i].right[j][0] == rgVector[i].right[k][0])//如果找到了与自己首字符相同的右部产生式
                    {
                        sameVector.push_back(rgVector[i].right[k]);
                        hasSame[k] = true;
                        hasSame[j] = true;
                        reset = true;
                    }
                }
            }
            if(hasSame[j])
                sameVectors.push_back(sameVector);
        }
        for (int j = 0; j < rgVector[i].right.size(); j++)
        {
            if(!hasSame)
            {
                noSameVector.push_back(rgVector[i].right[j]);
            }
        }
        if(reset)
        {
            rgVector[i].right.clear();
            for (int j = 0; j < sameVectors.size(); j++)
            {
                RgOfGrammer rg;
                char randomChar = randomChars[indexOfRandomChars];
                indexOfRandomChars++;
                string tempStr = "";
                tempStr.append(1,randomChar);
                rg.left = tempStr;
                for (int k = 0; k < sameVectors[j].size(); k++)
                {
//                    rgVector[i].right.push_back(sameVectors[j][k].substr(1));
//                    string tempStr = "";
//                    tempStr.append(1,sameVectors[j][0][0]);
//                    rgVector[i].right.push_back(tempStr.append(1,randomChar));
                    rg.right.push_back(sameVectors[j][k].substr(1));
                }
                rgVector.push_back(rg);
                string tempStr2 = "";
                tempStr2.append(1,sameVectors[j][0][0]);
                tempStr2.append(1,randomChar);
                rgVector[i].right.push_back(tempStr2);
            }
            for (int j = 0; j < noSameVector.size(); j++)
            {
                rgVector[i].right.push_back(noSameVector[j]);
            }
        }
    }
    
//    提取公共左因子后
    cout << "---------提取公共左因子后-----------------" << endl;
            for(int i = 0 ; i < rgVector.size();i++)
            {
                for(int j = 0 ; j < rgVector[i].right.size();j++)
                {
                    cout << "Left:" << rgVector[i].left << ",Right:" << rgVector[i].right[j] << endl;
                }
            }
    
    //计算FIRST和FOLLOW集合
    for (int i = 0; i < rgVector.size(); i++) {//初始化firstAndFollowOfCharVector
        FirstAndFollowOfChar firstAndFollowOfChar;
        firstAndFollowOfChar.ch = rgVector[i].left[0];
        firstAndFollowOfCharVector.push_back(firstAndFollowOfChar);
    }
    //递归计算First集合
    for (int i = 0;  i < rgVector.size(); i++)
    {
        int index = getIndexOfCharInFirstAndFollowOfCharVector(firstAndFollowOfCharVector, rgVector[i].left[0]);
        firstAndFollowOfCharVector[index].firstSet = getFirstSetOfChar(firstAndFollowOfCharVector, rgVector, rgVector[i].left[0]);
    }
    
    //显示计算得出的First
    cout << "--------------------FirstSet---------------------" << endl;
    for (int i = 0; i < firstAndFollowOfCharVector.size();i++) {
        cout << "char:" << firstAndFollowOfCharVector[i].ch << endl;
        set<char>::iterator iterator;
        for (iterator = firstAndFollowOfCharVector[i].firstSet.begin(); iterator != firstAndFollowOfCharVector[i].firstSet.end(); iterator++) {
            cout << *iterator << " ";
        }
        cout << endl;
    }
    
//    递归计算Follow集合
    for (int i = 0;  i < rgVector.size(); i++)
    {
        int index = getIndexOfCharInFirstAndFollowOfCharVector(firstAndFollowOfCharVector, rgVector[i].left[0]);
        firstAndFollowOfCharVector[index].followSet = getFollowSetOfChar(firstAndFollowOfCharVector, rgVector, rgVector[i].left[0]);
    }
    
    //显示计算得出的Follow集合
    cout << "--------------------Follow---------------------" << endl;
    for (int i = 0; i < firstAndFollowOfCharVector.size();i++) {
        cout << "char:" << firstAndFollowOfCharVector[i].ch << endl;
        set<char>::iterator iterator;
        for (iterator = firstAndFollowOfCharVector[i].followSet.begin(); iterator != firstAndFollowOfCharVector[i].followSet.end(); iterator++) {
            cout << *iterator << " ";
        }
        cout << endl;
    }
    
    //根据FIRST和FOLLOW求出分析表
    for (int i = 0; i < TABLE_ROW; i++) {//初始化LL1表
        for (int j = 0; j < TABLE_CLOUMN; j++) {
            ll1Table[i][j] = "error";
        }
    }
    
    for (int i = 0; i < rgVector.size();i++) {
        for (int j = 0; j < rgVector[i].right.size(); j++) {
            if (!isupper(rgVector[i].right[j][0]) ) {
                if(rgVector[i].right[j][0] != '\0')
                    ll1Table[chToNum[rgVector[i].left[0]]][chToNum[rgVector[i].right[j][0]]] = rgVector[i].right[j];
                else
                {
                    int indexOfLeftFirstAndFollow = getIndexOfCharInFirstAndFollowOfCharVector(firstAndFollowOfCharVector, rgVector[i].left[0]);
                    set<char> tempSet = firstAndFollowOfCharVector[indexOfLeftFirstAndFollow].followSet;//获得左边符号的FOLLOW集
                    set<char>::iterator iterator;
                    for (iterator = tempSet.begin(); iterator != tempSet.end(); iterator++) {
                        char ch = *iterator;
                        if (ch != '\0') {
                            ll1Table[chToNum[rgVector[i].left[0]]][chToNum[ch]] = rgVector[i].right[j];
                        }
                    }
                }
            }
            else{
                int indexOfRightFirstAndFollow = getIndexOfCharInFirstAndFollowOfCharVector(firstAndFollowOfCharVector, rgVector[i].right[j][0]);
                if (firstAndFollowOfCharVector[indexOfRightFirstAndFollow].firstSet.count('\0') != 0 ) {//当右边产生式的First集存在空字试
                    int indexOfLeftFirstAndFollow = getIndexOfCharInFirstAndFollowOfCharVector(firstAndFollowOfCharVector, rgVector[i].left[0]);
                    set<char> tempSet = firstAndFollowOfCharVector[indexOfLeftFirstAndFollow].followSet;//获得左边符号的FOLLOW集
                    set<char>::iterator iterator;
                    for (iterator = tempSet.begin(); iterator != tempSet.end(); iterator++) {
                        char ch = *iterator;
                        if (ch != '\0') {
                            ll1Table[chToNum[rgVector[i].left[0]]][chToNum[ch]] = rgVector[i].right[j];
                        }
                    }
                }
                if (firstAndFollowOfCharVector[indexOfRightFirstAndFollow].firstSet.count('\0') != 0 && firstAndFollowOfCharVector[indexOfRightFirstAndFollow].firstSet.size() == 1) {
                    continue;
                }
                set<char> tempSet = firstAndFollowOfCharVector[indexOfRightFirstAndFollow].firstSet;
                set<char>::iterator iterator;
                for (iterator = tempSet.begin(); iterator != tempSet.end(); iterator++) {
                    char ch = *iterator;
                    if (ch != '\0') {
                        ll1Table[chToNum[rgVector[i].left[0]]][chToNum[ch]] = rgVector[i].right[j];
                    }
                }
            }
        }
    }
    //打印出分析表
    for (int i = 0; i < TABLE_ROW; i++) {
        for (int j = 0; j < TABLE_CLOUMN; j++) {
            if((j+1) % TABLE_CLOUMN == 0)
                cout << ll1Table[i][j] << endl;
            else
                cout << ll1Table[i][j]  << " ";
        }
    }
    
    //通过分析表进行语法分析
    Token token;
    for (int i = 0; i < strlen(token.value); i++) {
        token.value[i] = '\0';
    }
    token.value[0] = '#';
    tokenVector.push_back(token);
    stack<char> stack;
    stack.push('#');
    stack.push('E');
    int point = 0;
    while (true) {
        char chOfStack = stack.top();
        char chOfStr = tokenVector[point].value[0];//这边做了特殊处理，简化情况
        point++;
        if (isupper(chOfStack)) {
            string temp = ll1Table[chToNum[chOfStack]][chToNum[chOfStr]];
            if (temp != "error") {
                for (int i = temp.size()-1; i>=0; i--) {
                    stack.push(temp[i]);
                }
            }
            else
                return false;
        }
        else if (chOfStack == chOfStr && chOfStr == '#') {
            return true;
        }
        else if (chOfStack == chOfStr && chOfStr != '#') {
            stack.pop();
            point++;
        }
        else if(!isupper(chOfStack) && chOfStack != chOfStr)
            return false;
    }
    return false;
}



// 一定特别注意！！！存放语法的文件，必须最后一行为'\n'！！！
int main(int argc, const char * argv[])
{
    //输入保存单词的文件路径
    cout << "Please enter your word file path:";
    char path[40];
    cin >> path;
    FILE *stream = fopen(path, "r");
    wordAnalyse(stream);
    for(int i = 0 ; i < tokenVector.size();i++)
    {
        printf("type:%d  value:%s\n", tokenVector[i].type, tokenVector[i].value);
    }
    cout << "Please enter your grammer file path:";
    cin >> path;
    stream = fopen(path, "r");
    bool flag = grammerAnalyse(stream);
    if (flag) {
        cout << "The words matches the grammer" << endl;
    }
    else
        cout << "The words don't matches the grammer" << endl;
    fclose(stream);
}
