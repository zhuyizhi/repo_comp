//
//  main.cpp
//  interview_proj
//
//  Created by xiazhou on 9/28/14.
//  Copyright (c) 2014 xiazhou. All rights reserved.
//

#include <iostream>
#include "stack"
#include "File.h"
#include "set"

using namespace std;

bool solve(vector<vector<char> > &board, unsigned int *mask, unsigned int *rowMask, unsigned int *colMask, unsigned int *squareMask, vector<int> &vec, int pos)
{
	if (pos == vec.size())
		return true;
    
	int x = vec[pos] / 9;
	int y = vec[pos] % 9;
	int square = ((int)(x / 3)) * 3 + ((int)(y / 3));
	for (int i = 1; i < 10; i++)
	{
		if (!(rowMask[x] & mask[i]) &&
			!(colMask[y] & mask[i]) &&
			!(squareMask[square] & mask[i])
			)//possible solution
		{
			board[x][y] = '0' + i;
			rowMask[x] |= mask[i];
			colMask[y] |= mask[i];
			squareMask[square] |= mask[i];
            
			bool res = solve(board, mask, rowMask, colMask, squareMask, vec, pos + 1);
			if (res)
				return true;
            
			board[x][y] = '.';
			rowMask[x] ^= mask[i];
			colMask[y] ^= mask[i];
			squareMask[square] ^= mask[i];
		}
	}
    
	return false;
}

void solveSudoku(vector<vector<char> > &board) {
	unsigned int mask[10];
	mask[0] = 0;
	mask[1] = 1;
	for (int i = 2; i < 10; i++)
		mask[i] = mask[i - 1] << 1;
    
	unsigned int rowMask[9];
	unsigned int colMask[9];
	unsigned int squareMask[9];
    memset(rowMask, 0, sizeof(unsigned int) * 9);
	memset(colMask, 0, sizeof(unsigned int) * 9);
	memset(squareMask, 0, sizeof(unsigned int) * 9);
    
	vector<int> posVec;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == '.')
				posVec.push_back(i * 9 + j);
			else
			{
				int val = board[i][j] - '0';
				rowMask[i] |= mask[val];
				colMask[j] |= mask[val];
				int square = ((int)(i / 3)) * 3 + ((int)(j / 3));
				squareMask[square] |= mask[val];
			}
		}
	}
    
	solve(board, mask, rowMask, colMask, squareMask, posVec, 0);
}

void test_solveSudoku()
{
    char *arr[9] = {
        "..9748...","7........",".2.1.9...","..7...24.",".64.1.59.",".98...3..","...8.3.2.","........6","...2759.."
    };
    
    vector<vector<char> > board;
    for(int i = 0; i < 9; i++)
    {
        vector<char> vec;
        for(int j = 0; j < 9; j++)
            vec.push_back(arr[i][j]);
        board.push_back(vec);
    }
    stack<int *> s;
    
    solveSudoku(board);
    cout<<"s"<<endl;
}

struct TreeNode {
         int val;
         TreeNode *left;
         TreeNode *right;
         TreeNode(int x) : val(x), left(NULL), right(NULL) {}
     };

vector<int> postorderTraversal(TreeNode *root) {
    vector<int> res;
    if(!root)
        return res;
    
    stack<TreeNode *> s;
    s.push(root);
    
    TreeNode *last_visit = NULL;
    while(!s.empty())
    {
        TreeNode * t = s.top();
        bool visit = false;
        if(!last_visit)
        {
            if(t->left)
                s.push(t->left);
            else if(t->right)
                s.push(t->right);
            else
                visit = true;
        }
        else if(!t->left && !t->right)
        {
            visit = true;
        }
        else if(t->left == last_visit)
        {
            if(t->right)
                s.push(t->right);
            else
                visit = true;
        }
        else if(t->right == last_visit)
        {
            visit = true;
        }
        else//has children, but last_visit is in siblings
        {
            if(t->left)
                s.push(t->left);
            else if(t->right)
                s.push(t->right);
            else
                visit = true;
        }

        
        if(visit)
        {res.push_back(t->val); last_visit = t; s.pop();}
    }
    
    return res;
}

TreeNode * build_tree()
{
    int arr[] = {3,2,4, -1, -1, 1};
    
    int arr_size = sizeof(arr)/sizeof(int);
    
    TreeNode **trees = new TreeNode*[arr_size];
    for(int i = 0; i < arr_size; i++)
    {
        if(arr[i] > 0)
        {
            trees[i] = new TreeNode(arr[i]);
        }
    }
    
    for(int i = 0; i < arr_size; i++)
    {
        if(2*i + 1 < arr_size && trees[2*i + 1])
            trees[i]->left = trees[2*i + 1];
        
        if(2*i + 2 < arr_size && trees[2*i + 2])
            trees[i]->right = trees[2*i + 2];
    }
    
    return trees[0];
}

void test_postOrder()
{
    TreeNode *p = build_tree();
    
    vector<int> vec = postorderTraversal(p);
    cout<<vec.size()<<endl;
}

TreeNode *build(vector<int> &pre, int ps, int pe, vector<int> &in, int is, int ie)
{
    if(pe < ps)
        return NULL;
    
    TreeNode *root = new TreeNode(pre[ps]);
    if(pe == ps)
        return root;
    
    int i_pos = is;
    for(; i_pos <= ie; i_pos++)
    {
        if(in[i_pos] == pre[ps])
            break;
    }
    
    TreeNode *lt = build(pre, ps + 1, ps + i_pos - is, in, is, i_pos - 1);
    TreeNode *rt = build(pre, ps + i_pos - is + 1, pe, in, i_pos + 1, ie);
    
    root->left = lt;
    root->right = rt;
    
    return root;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
    return build(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
}

void test_buildTree()
{
    int pre[] = {1,2,3};
    int in[] = {3,2,1};
    
    vector<int> pv;
    vector<int> iv;
    
    int arr_len = sizeof(pre)/sizeof(int);
    
    for(int i = 0; i < arr_len; i++)
    {
        pv.push_back(pre[i]);
        iv.push_back(in[i]);
    }
    
    TreeNode *root = buildTree(pv, iv);
    
    cout<<root<<endl;
}

void swap(int arr[], int p1, int p2)
{int tmp = arr[p1]; arr[p1] = arr[p2]; arr[p2] = tmp;}

void adapt(int arr[], int end, int pos)//small top
{
    int left = 2 * pos + 1, right = 2 * pos + 2;
    int smaller = left;
    if(right <= end && arr[right] < arr[left]) smaller = right;
    
    if(arr[pos] > arr[smaller])
    {
        swap(arr, smaller, pos);
        adapt(arr, end, smaller);
    }
}

void build_heap(int arr[])
{
    int arr_len = sizeof(arr);

    for(int i = arr_len/2 - 1; i >= 0; i--)
    {
        adapt(arr, arr_len, i);
    }
}

void heap_sort(int arr[])
{
    build_heap(arr);
    int arr_len = sizeof(arr);
    
    for(int i = arr_len - 1; i >= 1; i--)
    {
        swap(arr, i, 0);
        adapt(arr, i - 1, 0);
    }
}

void test_heap_sort()
{
    int arr[] = {8,5,3,4,7,1,2};
    heap_sort(arr);
    cout<<arr<<endl;
}

vector<vector<int> > composite(int arr[], int size, int start)
{
    vector<vector<int> > res;
    if(start == size - 1)
    {
        vector<int> v;
        v.push_back(arr[start]);
        res.push_back(v);
        return res;
    }
    
    for(int j = start; j < size; j++)
    {
        swap(arr, start, j);
        
        vector<vector<int> > part = composite(arr, size, start + 1);
        
        for(int i = 0; i < part.size(); i++)
        {
            part[i].push_back(arr[start]);
            res.push_back(part[i]);
        }
        
        swap(arr, j, start);
    }
    
    return res;
}

set<int> possVal(int arr[], int len, int start)
{
    set<int> res;
    
    if(start  == len - 1){
        res.insert(arr[start]);
        res.insert(0);
        return res;
    }
    
    res = possVal(arr, len, start + 1);
    set<int> backup(res.begin(), res.end());
    for(int val : res)
        backup.insert(val + arr[start]);
    
    return backup;
}


vector<int> possVal_dup(int arr[], int len, int start)
{
    vector<int> res;
    
    if(start  == len - 1){
        res.push_back(arr[start]);
        res.push_back(0);
        return res;
    }
    
    res = possVal_dup(arr, len, start + 1);
    vector<int> backup(res.begin(), res.end());
    for(int val : res)
        backup.push_back(val + arr[start]);
    
    return backup;
}


void test_val()
{
//    int arr[] = {3,5,6,7};
    int arr[] = {5, 9, 11, 12, 13};
    int arr_len = sizeof(arr)/sizeof(int);
    
    int *d_a = new int[10];
    int num = 1;
    for(int i = 1; i < 10; i++)
    {
        d_a[i] = 500 - num;
        num = num << 1;
    }
    
//    vector<vector<int> > comp = composite(arr, arr_len, 0);
//    set<int> s;
//    
//    for(int i = 0; i < comp.size(); i++)
//    {
//        int sum = 0;
//        for(int val : comp[i]){
//            sum += val;
//            s.insert(sum);
//        }
//        cout<<"sum = " <<sum<<endl;
//    }
//    cout<<comp.size()<<endl;
//    
//    for(int val : s)
//        cout<<"set val: " <<val<<endl;
//    cout<<"size of s "<<s.size()<<endl;
    
    vector<int> res = possVal_dup(arr, arr_len, 0);
    std::sort(res.begin(), res.end());
    for(int val : res)
        cout<<val<<endl;
    
    cout<<"size of res :" << res.size()<<endl;
    
    set<int> se(res.begin(), res.end());
    cout<<"size of se :" << se.size()<<endl;
}

int main(int argc, const char * argv[])
{
//    test_solveSudoku();
//    stack<int> a;
//    a.push(0);
//    a.push(1);
//    cout<<a.top()<<endl;
//    a.pop();
//    cout<<a.top()<<endl;
    
    test_val();
    return 0;
}

