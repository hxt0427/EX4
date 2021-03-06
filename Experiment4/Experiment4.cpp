// Experiment4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <memory>
#include <stdlib.h>  
using namespace std;

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

struct SeqList {
	double *items;		//基址--系数
	int * indexs;			//基址--指数
	int length;
	int capacity;

	SeqList()
	{
		items = NULL;
		indexs = NULL;
		length = 0;
		resize(LIST_INIT_SIZE);			//分配空间
	}
	~SeqList()
	{
							//析构
	}
	void resize(int c)
	{
		capacity = c;
		double *newdata = new double[capacity];
		int *newdata2 = new int[capacity];
		if (items != NULL)
		{
			memcpy(newdata, items, length * sizeof(double));
			memcpy(newdata2, indexs, length * sizeof(int));
			delete[] items;
			delete[] indexs;
		}
		items = newdata;
		indexs = newdata2;
	}
	void output(int n)				//输出
	{
		if (length == 0)
		{
			cout << "list为空"<<endl;
			return;
		}
		cout <<"List"<<n<<": " <<items[0] << "X^(" << indexs[0] <<")";
		for (int i = 1; i < length; i++)
		{
			if(items[i] >= 0)
				cout << "+";
			cout<< items[i] << "X^(" << indexs[i]<<")";
		}
		cout<< endl;
	}
	void addpend(double coef , int exp)	//添加元素  按指数升序
	{
		if (length == capacity)
			resize(capacity + LISTINCREMENT);
		if (length == 0 )		//第一个元素
		{
			length++;
			items[0] = coef;
			indexs[0] = exp;
			return;
		}
		if (indexs[0] < exp) //插入的元素指数最大
		{
			length++;
			for (int j = length; j > 0; j--)
			{
				items[j] = items[j - 1];
				indexs[j] = indexs[j - 1];
			}
			items[0] = coef;
			indexs[0] = exp;
			return;
		}
		int i = 0;
		for (i = 0; i < length ; i++)
			if (indexs[i] == exp)
			{
				items[i] += coef;
				return;
			}
		for (i = 0; i < length - 1 ; i++) 
			if(indexs[i] > exp && indexs[i+1] < exp)
				break;
		length++;
		for (int j = length; j > i + 1; j--)
		{
			items[j] = items[j - 1];
			indexs[j] = indexs[j - 1];
		}
		items[i + 1] = coef;
		indexs[i + 1] = exp;
	}
	void deleteAt(int index)	//删除index处的元素
	{
		for (int i = index; i < length - 1; i++)
		{
			items[i] = items[i + 1];
			indexs[i] = indexs[i + 1];
		}
		length--;
	}
	double getCoef(int index)	//返回index处的元素--系数
	{
		return items[index];
	}
	int getExp(int index)	//返回index处的元素--指数
	{
		return indexs[index];
	}
	int getLength()
	{
		return length;
	}
	int getCapacity()
	{
		return capacity;
	}
	void setCoef(int exp, double coef)	//修改指数为exp的元素的系数
	{
		int i;
		for (i = 0; i < length; i++)
			if (exp == indexs[i])
				break;
		if (i == length)
			addpend(coef, exp);
		else
			items[i] = coef;
	}
	void clear()		//清空并重新分配空间
	{
		length = 0;
		resize(LIST_INIT_SIZE);
	}
	void AddSeqList(SeqList list1, SeqList list2,int length1 , int length2)
	{
		clear();
		for (int i = 0; i < length1; i++)
			addpend(list1.getCoef(i), list1.getExp(i));
		for (int i = 0; i < length2; i++)
			addpend(list2.getCoef(i), list2.getExp(i));
	}
	void SubSeqList(SeqList list1, SeqList list2, int length1, int length2)
	{
		clear();
		for (int i = 0; i < length1; i++)
			addpend(list1.getCoef(i), list1.getExp(i));
		for (int i = 0; i < length2; i++)
			addpend(-list2.getCoef(i), list2.getExp(i));
	}
	void MulSeqList(SeqList list1, SeqList list2, int length1, int length2)
	{
		clear();
		for (int i = 0; i < length1; i++)
			for (int j = 0; j < length2; j++)
				addpend(list1.getCoef(i)*list2.getCoef(j), list1.getExp(i) + list2.getExp(j));
	}
	void DivSeqList(SeqList list1, SeqList list2, int length1, int length2)
	{				//当前顺序表存放当前商   list顺序表存放当前余数   list3存放部分积
		this->clear();
		SeqList list, list3,list4;
		Copy(list1, list);				//list1 -->list
		double coef;
		int exp;
		while (list.getExp(0) >= list2.getExp(0) && list.items != NULL) //被除数最高次比除数的最高次高
		{
			coef = list.getCoef(0) / list2.getCoef(0);
			exp = list.getExp(0) - list2.getExp(0);
			this->addpend(coef,exp);
			list3.clear();
			for (int i = 0; i < length2; i++)			//liat3 = list2 * 部分商
				list3.addpend(list2.getCoef(i) * coef , list2.getExp(i) + exp);
			list4.clear();
			list4.SubSeqList(list, list3, list.getLength(), list3.getLength());
			Copy(list4, list);
		}
		cout << "商：";
		this->output(6);
		cout << "余数： ";
		list.output(7);
	}
	void Copy(SeqList list1, SeqList list2) //list1-->list2
	{
		list2.clear();
		for (int i = 0; i < list1.getLength(); i++)
		{
			list2.addpend(list1.getCoef(i), list1.getExp(i));
		}
	}
};

int main()
{
	SeqList list1;			//多项式1
	SeqList list2;			//多项式2
	SeqList	list3;			//多项式3=多项式1+多项式2
	SeqList	list4;			//多项式4=多项式1-多项式2
	SeqList	list5;			//多项式5=多项式1*多项式2
	SeqList	list6;			//多项式6=多项式1/多项式2
	SeqList	list7;			//多项式7=多项式1%多项式2
	int Num = 10;
	for (int i = 0; i < Num; i++)
		list1.addpend((rand() / (double)(RAND_MAX / 20) -10),( rand() % 20 - 10));
	list1.output(1);					//输出多项式1
	cout << endl;
	for (int i = 0; i < Num; i++)
		list2.addpend((rand() / (double)(RAND_MAX / 20) - 10), (rand() % 20 - 10));
	list2.output(2);					//输出多项式2
	cout << endl<<"加法："<<endl;
	list3.AddSeqList(list1, list2, list1.getLength(), list2.getLength());
	list3.output(3);					//输出多项式3
	cout << endl << "减法：" << endl;
	list4.SubSeqList(list1, list2, list1.getLength(), list2.getLength());
	list4.output(4);					//输出多项式4
	cout << endl << "乘法：" << endl;
	list5.MulSeqList(list1, list2, list1.getLength(), list2.getLength());
	list5.output(5);					//输出多项式5
//	cout << endl << "除法：" << endl;
	//list6.DivSeqList(list1, list2, list1.getLength(), list2.getLength());
	//list6.Copy(list1, list6);
//	cout << list6.getLength();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
