#include <stdio.h>
#include <stdlib.h>
#define N 10000

int a[N]; // 用于存储奶牛产奶量

//Todo
//需要返回中位数奶牛产奶量。
//可以自行添加其他辅助函数,可以修改传参的数量
int solve1(int *a){
}

void test1(){
    int caseNum; //表示测试轮数
    int n;
    int ans[N];
    if (freopen("5_1_input_5.in", "r", stdin) == NULL) {
		printf("There is an error in reading file 5_1_input_5.in");
    }
    scanf("%d", &caseNum);
    for (int case1 = 0; case1 < caseNum; case1++) {
        printf("==== Case %d ====\n", case1 + 1);
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
        }
        ans[case1] = solve1(a);
        printf("ans is:%d\n", ans[case1]);
        printf("\n");
    }
    fclose(stdin);
}

int main() {
    test1();
    return 0;
}
