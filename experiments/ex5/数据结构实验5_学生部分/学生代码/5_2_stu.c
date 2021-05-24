#include<stdio.h>
#include<stdlib.h>

#define MAX 1000

//Todo
//需要返回一个数组，数组元素是原始数组里最大的k个数
//注意按照从小到大的顺序排序
//可以自行添加其他辅助函数,可以根据实际需要修改函数参数
int *solve2(){	
}

void test2(){	
	int caseNum; //表示测试轮数
	if (freopen("5_2_input_5.in", "r", stdin) == NULL) {
		printf("There is an error in reading file 5_2_input_5.in");
    }	
	scanf("%d", &caseNum);
    int case1 = 0;
    for (; case1 < caseNum; case1++) {
		int k, N;
        scanf("%d%d", &k, &N);
        int arr[MAX];
        int i = 0;
        for (; i < N; i++) {
            scanf("%d", &arr[i]);
        }        
        int *res;
		res = solve2();
		printf("==== Case %d ====\nres : ", case1 + 1);
        for (i = 0; i < k; i++) {
            printf("%d ",res[i]);
        }
    }
	fclose(stdin);
}


int main(void) {
    test2();
    return 0;
}