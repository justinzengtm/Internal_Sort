/*基数排序 - 次位优先
 *time:2018/12/6
 *tutor:justinzeng
 */

#include <stdio.h>
#include <stdlib.h>
#define MaxSize 100
#define MaxDigit 3 //关键字最有三位 
#define Radix 10 //基数为10
#define ElemType int

//待排序列的存储结构
typedef struct SqlList *PtrlSqlList;
struct SqlList {
	ElemType arr[MaxSize+1]; //待排序列
	int length; //待排序列的长度 
};

//桶元素的结点的存储结果 
typedef struct BucketNode *PtrlBucketNode; //桶以链表的形式存储 
struct BucketNode {
	int key;
	PtrlBucketNode Next;
}; 

//桶的头结点
struct BucketHeadNode {
	PtrlBucketNode Head; //指向桶链表头的头结点
	PtrlBucketNode Tail; //指向桶链表尾的为结点 
}; 
typedef struct BucketHeadNode Bucket[Radix]; //多个桶的数组

//函数声明
PtrlSqlList InitialList(); //序列初始化
PtrlBucketNode InitialBucket(); //初始化一个桶 
void Insert_Array(PtrlSqlList P, ElemType K); //序列插入元素
void PrintList(PtrlSqlList P, int count); //输出待排序列
int GetDigit(int K, int Down); //获得位数字  
void Least_Significant_Digit_Sort(PtrlSqlList P, int N); //LSD次位优先基数排序
void PrintBucketList(PtrlBucketNode PBL); //输出桶内元素

int main(int argc, char const *argv[]) 
{
	ElemType data=0;
	
	//创建一个待排序列
	PtrlSqlList P=InitialList();
	printf("请创建待排序列:");
	while (data!=-1) {
		scanf("%d", &data);
		if (data!=-1) {
			Insert_Array(P, data);
		}
	}
	
	//输出待排序列看看
	printf("\n待排序列为:");
	PrintList(P, 0); 
	//获得序列长度 
	P->length=GetListLength(P);
	printf("\n待排序列长度为: %d\n\n", P->length);
	
	//LSD次位优先基数排序
	Least_Significant_Digit_Sort(P, P->length);
	
	printf("\n基数排序后序列:");
	PrintList(P, 0); 
	return 0; 
}
//初始化一个桶
PtrlBucketNode InitialBucket()
{
	PtrlBucketNode Bucket;
	Bucket=(PtrlBucketNode)malloc(sizeof(struct BucketNode));
	
	return Bucket;
}

//序列初始化
PtrlSqlList InitialList()
{
	PtrlSqlList P=(PtrlSqlList)malloc(sizeof(struct SqlList)); 
	P->arr[0]=-1;	
	P->length=0; //初始化序列长度为0,不算哨兵
	
	return P; 
} 

//序列插入元素
void Insert_Array(PtrlSqlList P, ElemType K)
{
	int i=0;
	
	while (P->arr[i]!=-1) {
		i++;
	}
	P->arr[i]=K;
	P->arr[i+1]=-1;
} 

//输出序列
void PrintList(PtrlSqlList P, int count)
{
	int i=0;
	
	if (count!=0) {
		printf("第%d次排序:", count);
		while (P->arr[i]!=-1) {
			printf("%d ", P->arr[i]);
			i++;
		}
	} else {
		while (P->arr[i]!=-1) {
			printf("%d ", P->arr[i]);
			i++;
		}
	}
} 

//计算序列的长度
int GetListLength(PtrlSqlList P) 
{
	int length=0;
	
	while (P->arr[length]!=-1) {
		length++;
	}
	
	return length;
} 

//输出桶内元素
void PrintBucketList(PtrlBucketNode PBL) 
{
	int i=0;
	PtrlBucketNode P=PBL;
	
	while (P) {
		printf("%d ", P->key);
		P=P->Next;
	} 

	return;
} 

//获得位数字 
int GetDigit(int K, int Digit) 
{
	//次位一开始为Down=1,主位为D<=MaxDigit.
	int i, digit;
	
	for (i=1; i<=Digit; i++) {
		digit=K%Radix;
		K/=Radix;
	} 
	
	return digit;
} 

//LSD次位优先基数排序 
void Least_Significant_Digit_Sort(PtrlSqlList P, int N)
{
	//基数排序 - 次位优先
	int D, Di, i;
	Bucket B; //建立一个桶的数组,也就是一堆桶
	PtrlBucketNode tmp, p, List=NULL; //建立了三个桶,且初始化为NULL
	
	//初始化每个桶为一个空链表 
	//数据最多有Radix位,所以需要Radix个桶 
	for (i=0; i<Radix; i++) {
		B[i].Head=B[i].Tail=NULL; //桶的头尾指针均初始化为NULL 
	} 
	
	//!!!!将待排序列逆序存入初始链表List中 
	for (i=0; i<N; i++) {
		tmp=InitialBucket(); //初始化一个桶
		//接下来的操作是把待排序列逆序存储到List桶中 
		tmp->key=P->arr[i];
		tmp->Next=List;
		List=tmp;
	} 
	
	//然后开始排序过程
	for (D=1; D<=MaxDigit; D++) {
		//对数据的每一位进行处理,这里设置数据最多有三位
		p=List; 
		while (p) {
			//D一开始从1开始,也就是一开始只看个位 
			//获得当前元素的当前位数字 
			Di=GetDigit(p->key, D); 
			tmp=p;
			p=p->Next; 
			//tmp保存了p,get到p的数据的数位后,将其从List桶中移除 
			tmp->Next=NULL;
			//获得数位后,插入到B[Di]桶里
			//特殊情况判断如果桶B[Di]的链表为空
			if (B[Di].Head==NULL) {
				B[Di].Head=B[Di].Tail=tmp;
			} else {
				B[Di].Tail->Next=tmp;
				B[Di].Tail=tmp;
			}
		}
		
		printf("第%d次次位优先基数排序\n", D);
		for (i=0; i<Radix-1; i++) {
			printf("B[%d]：", i);
			PrintBucketList(B[i].Head);
			printf("\n");
		}	
		
		//做完一趟桶排序后,收集桶数组B中的元素
		List=NULL; //List作为桶链表头 
		//0-9十种情况,逆序收入List桶中,所以Di从Radix-1也就是9开始 
		for (Di=Radix-1; Di>=0; Di--) {
			if (B[Di].Head) {
				//整个桶插入List表头
				B[Di].Tail->Next=List; //此时的List==NULL 
				List=B[Di].Head; 
				//清空桶B[Di]
				B[Di].Head=B[Di].Tail=NULL; 
			} 
		}
		printf("收集桶：");
		PrintBucketList(List);
		printf("\n\n");
	} 	
	
	//做完所有排序后,将List放回用户的序列中并释放掉List
	for (i=0; i<N; i++) {
		tmp=List;
		List=List->Next;
		P->arr[i]=tmp->key;
		free(tmp);
	}  
}
