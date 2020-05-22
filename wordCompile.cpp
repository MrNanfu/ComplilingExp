//
//  main.cpp
//  compilingExp01
//
//  Created by M-Mac on 2020/5/14.
//  Copyright © 2020年 Chenyanting. All rights reserved.
//


#include "wordCompile.h"

#define NUM_KEYWORD 15
#define NUM_SEPARATER 8
#define NUM_OPERATOR 8
#define NUM_FILTER 4
//
//定义有限正规集
string KEYWORD[NUM_KEYWORD] = {"if","else","void","return","while","then","for","do","int","cahr","double","float","case","cin","cout"};

char SEPARATER[NUM_SEPARATER] = {';',',','{','}','[',']','(',')'};

char OPERATOR[NUM_OPERATOR] = {'+','-','*','/','>','<','=','!'};

char FILTER[NUM_FILTER] = {' ','\t','\r','\n'};


//定义一个结构体
struct Token
{
    int type;
    char value[20];
};

//定义一个存储Token的向量
vector<Token> tokenVector;




// 定义单词验证函数
bool checkKeyword(string word)
{
    for (int i = 0; i < NUM_KEYWORD; i++)
    {
        if(KEYWORD[i] == word)
            return true;
    }
    return false;
}

bool checkSeparator(char ch)
{
    for (int i = 0; i < NUM_OPERATOR; i++) {
        if(SEPARATER[i] == ch)
            return true;
    }
    return false;
}


bool checkOperator(char ch)
{
    for (int i = 0; i < NUM_OPERATOR; i++) {
        if(OPERATOR[i] == ch)
            return true;
    }
    return false;
}

bool checkFilter(char ch)
{
    for (int i = 0; i < NUM_FILTER; i++) {
        if(FILTER[i] == ch)
            return true;
    }
    return false;
}

//词法分析函数
void wordAnalyse(FILE *stream)
{
    char value[20] = {'\0'};
    int valueIndex = 0;
    char ch = ' ';
    while (true) {
        ch = fgetc(stream);
        if(ch == EOF)  //文件全部读取完的终止语句
            break;
        if(checkFilter(ch))
        {
            continue;
        }
        else if (isalpha(ch)) // 如果读取的第一个字符为字母，那么这个词是关键字或者标识符
        {
            while (isalpha(ch)) { //连续往后面读取字母
                value[valueIndex] = ch;
                valueIndex++;
                ch = fgetc(stream);
            }
            fseek(stream,-1L,SEEK_CUR); // 指针回退
            string valueStr(value);
            if(checkKeyword(valueStr)) // 判断最后得到的词是否为关键词
            {
                Token token;
                for (int i = 0; i < 20; i++) {
                        token.value[i] = value[i];
                }
                token.type = 1;
                tokenVector.push_back(token);
                valueIndex = 0;
                for (int i = 0; i < 20; i++) {
                    value[i] = '\0';
                }
            }
            else // 不是关键词那么最后得到的词是标识符
            {
                Token token;
                for (int i = 0; i < 20; i++) {
                    token.value[i] = value[i];
                }
                token.type = 2;
                tokenVector.push_back(token);
                valueIndex = 0;
                for (int i = 0; i < 20; i++) {
                    value[i] = '\0';
                }
            }
        }
        else if(ch == '_'){ //如果是下划线开头，那么这个词必然为标识符
            ch = fgetc(stream);
            while (isalpha(ch) || isdigit(ch) || ch == '_') { //连续往后面读取字母、数字、下划线
                value[valueIndex] = ch;
                valueIndex++;
                ch = fgetc(stream);
            }
            fseek(stream,-1L,SEEK_CUR); // 指针回退
            Token token;
            for (int i = 0; i < 20; i++) {
                token.value[i] = value[i];
            }
            token.type = 2;
            tokenVector.push_back(token);
            valueIndex = 0;
            for (int i = 0; i < 20; i++) {
                value[i] = '\0';
            }
        }
        else if(isdigit(ch) || ch == '\'' || ch == '\"'){
            if(isdigit(ch)){
                while (isdigit(ch)) { //连续往后面读取字母
                    value[valueIndex] = ch;
                    valueIndex++;
                    ch = fgetc(stream);
                }
                fseek(stream,-1L,SEEK_CUR); // 指针回退
                Token token;
                for (int i = 0; i < 20; i++) {
                    token.value[i] = value[i];
                }
                token.type = 3;
                tokenVector.push_back(token);
                valueIndex = 0;
                for (int i = 0; i < 20; i++) {
                    value[i] = '\0';
                }
            }
            else
            {
                ch = fgetc(stream);
                while (isalnum(ch)) {
                    value[valueIndex] = ch;
                    valueIndex++;
                    ch = fgetc(stream);
                }
                fseek(stream,-1L,SEEK_CUR); // 指针回退
                Token token;
                for (int i = 0; i < 20; i++) {
                    token.value[i] = value[i];
                }
                token.type = 3;
                tokenVector.push_back(token);
                valueIndex = 0;
                for (int i = 0; i < 20; i++) {
                    value[i] = '\0';
                }
            }
        }
        else if(checkOperator(ch))
        {
            value[0] = ch;
            valueIndex++;
            Token token;
            for (int i = 0; i < 20; i++) {
                token.value[i] = value[i];
            }
            token.type = 4;
            tokenVector.push_back(token);
            valueIndex = 0;
            for (int i = 0; i < 20; i++) {
                value[i] = '\0';
            }
        }
        else if(checkSeparator(ch))
        {
            value[0] = ch;
            valueIndex++;
            Token token;
            for (int i = 0; i < 20; i++) {
                token.value[i] = value[i];
            }
            token.type = 5;
            tokenVector.push_back(token);
            valueIndex = 0;
            for (int i = 0; i < 20; i++) {
                value[i] = '\0';
            }
        }
        else
            printf("Error char:%c,please check your input!\n", ch);
    }
}

//int main(int argc, const char * argv[]) {
//    cout << "Please enter your file path:";
//    char path[40];
//    cin >> path;
//    FILE *stream = fopen(path, "r");
//    wordAnalyse(stream);
//    for(int i = 0 ; i < tokenVector.size();i++)
//    {
//        printf("type:%d  value:%s\n", tokenVector[i].type, tokenVector[i].value);
//    }
//}
