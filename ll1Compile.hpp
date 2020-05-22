//
//  ll1Compile.hpp
//  compilingExp
//
//  Created by M-Mac on 2020/5/15.
//  Copyright © 2020年 Chenyanting. All rights reserved.
//

//
#ifndef ll1Compile_hpp
#define ll1Compile_hpp

#include "wordCompile.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cctype>
#include <set>
#include <map>
#include <stack>
using namespace std;

bool grammerAnalyse(FILE *stream);

struct RgOfGrammer
{
    string left;
    vector<string> right;
};

struct FirstAndFollowOfChar
{
    char ch;
    set<char> firstSet;
    set<char> followSet;
};

set<char> getFirstSetOfChar(vector<FirstAndFollowOfChar> firstAndFollowOfCharVector,vector<RgOfGrammer> rgVector,char ch);
set<char> getFollowSetOfChar(vector<FirstAndFollowOfChar> firstAndFollowOfCharVector,vector<RgOfGrammer> rgVector,char ch);
int getIndexOfCharInFirstAndFollowOfCharVector(vector<FirstAndFollowOfChar> firstAndFollowOfCharVector,char ch);
int getIndexOfRgOfGrammerVector(vector<RgOfGrammer> rgVector,char ch);
#endif /* ll1Compile_hpp */
