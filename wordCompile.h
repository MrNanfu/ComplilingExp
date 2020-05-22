//
//  wordCompile.h
//  compilingExp
//
//  Created by M-Mac on 2020/5/15.
//  Copyright © 2020年 Chenyanting. All rights reserved.
//

#ifndef wordCompile_h
#define wordCompile_h
#include <iostream>
#include <stdio.h>
#include <cctype>
#include <vector>
using namespace std;


bool checkKeyword(string word);

bool checkSeparator(char ch);

bool checkOperator(char ch);

bool checkFilter(char ch);

void wordAnalyse(FILE *stream);

#endif /* wordCompile_h */
