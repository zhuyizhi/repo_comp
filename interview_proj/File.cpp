//
//  File.cpp
//  interview_proj
//
//  Created by xiazhou on 9/28/14.
//  Copyright (c) 2014 xiazhou. All rights reserved.
//

#include "File.h"
#include "iostream"
#include "stack"
/*
 Dynamic programming.
 
 For A[0...n], max_product(n) := MAX( (1), (2) ).
 (1) max_product(n-1);
 (2) max product result that contains A[n]. Keeps both negative(neg_max)/positive(pos_max) maximu product results that contains A[n-1]
    2.1 if A[n] is positive, then it's pos_max(n-1) * A[n];
    2.2 else it's neg_max(n-1) * A[n];
 
 */
using namespace std;
int Solution::maxProduct(int A[], int n)
{
    if(n == 0)
        return 0;
    
    int *result = new int[n];
    bool pos_valid = false, neg_valid = false;

    int pos_max = 1, neg_max = -1;
    
    for(int i = 0; i < n; i++)
    {
        //1st, update pos_max, neg_max
        if(A[i] == 0)
        {
            pos_valid = neg_valid = false;
        }
        else if(A[i] > 0)
        {
            if(!pos_valid)
            {
                pos_valid = true;
                pos_max = A[i];
            }
            else
            {
                pos_max *= A[i];
            }
            
            if(neg_valid)
                neg_max *= A[i];
        }
        else//A[i] < 0
        {
            if(neg_valid)
            {
                pos_max = A[i] * neg_max;
                pos_valid = true;
            }
            neg_valid = true;
            neg_max = pos_valid ? pos_max * A[i] : A[i];
        }
        
        if(i == 0)
            result[i] = A[0];
        else
        {
            result[i] = result[i - 1];
            if(pos_valid)
            {
                if(pos_max > result[i])
                    result[i] = pos_max;
            }
            else if(neg_valid && neg_max > result[i])
                result[i] = neg_max;
                
        }
        
    }
    
    return result[n - 1];
}

int Solution::maxProductSimple(int A[], int n)
{
    if(n == 0)
        return 0;
    if(n == 1)
        return A[0];
    
    int *result = new int[n];
    int s = A[0];
    int pos_max = s >= 0 ? s : 0;
    int neg_max = s >= 0 ? 0 : s;
    
    result[0] = A[0];
    
    for(int i = 1; i < n; i++)
    {
        int new_pos = pos_max, new_neg = neg_max;
        //1st, update pos_max, neg_max
        if(A[i] >= 0)
        {
            
            new_pos = (pos_max == 0) ? A[i] : pos_max * A[i];
            new_neg = (neg_max == 0) ? 0 : neg_max * A[i];
        }
        else if(A[i] < 0)
        {
            new_pos = (neg_max == 0) ? 0 : neg_max * A[i];
            new_neg = (pos_max == 0) ? A[i] : pos_max * A[i];
        }
//        pos_max = new_pos;
//        neg_max = new_neg;
        std::swap(pos_max, new_pos);
        std::swap(neg_max, new_neg);
        
        result[i] = pos_max > result[i - 1] ? pos_max : result[i - 1];
    }
    
    return result[n - 1];
}

int Solution::evalRPN(vector<string> &tokens)
{
    stack<int> value;
    int i = 0;
    
    while(i < tokens.size())
    {
        string word = tokens[i];
        
        if(word != "+"
           && word != "-"
           && word != "*"
           && word != "/"
           )
        {
            int val = atoi(word.data());
            value.push(val);
        }
        else
        {
            int val2 = value.top();
            value.pop();
            int val1 = value.top();
            value.pop();
            
            if(word == "+")
                value.push(val2 + val1);
            else if(word == "-")
                value.push(val1 - val2);
            else if("*" == word)
                value.push(val1 * val2);
            else
                value.push(val1/val2);
            
        }
        
        i++;
    }
    
    return value.top();
}
