#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node{
    char StuID[11];
    int Grade;
    struct node *next;
}StudentLinkedListNode;


/* 创建相交链表 */
void createCrossLink(StudentLinkedListNode* a, StudentLinkedListNode* b, int beforeCross1, int beforeCross2){
    // a和b后面若干结点值相同
    // beforeCross1为跳过的a中的个数，从第beforeCross1 + 1个结点开始相交
    // beforeCross2为跳过的b中的个数，从第beforeCross2 + 1个结点开始相交
    // 相交方法是将b中的前一结点指向a中的首个相交结点
    StudentLinkedListNode *p, *q;
    while(beforeCross1--)a=a->next;
    while(--beforeCross2)b=b->next;
    p = b->next;
    b->next = a;
    //销毁野指针结点
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
}

void destroyCrossLink(StudentLinkedListNode* a, StudentLinkedListNode* b, StudentLinkedListNode* crossNode){
    StudentLinkedListNode* p = crossNode->next, *q;
    while(p){
        q = p->next;
        free(p);
        p = q;
    }
    while(a != crossNode){
        q = a->next;
        free(a);
        a = q;
    }
    while(b != crossNode){
        q = b->next;
        free(b);
        b = q;
    }
    free(crossNode);
}

/* 打印单个节点 */
void printLinkedListNode(StudentLinkedListNode * node){
    printf("{ID:%s, Grade:%d}", node->StuID, node->Grade);
    if(node->next!=NULL){
        printf("->");
    }else{
        printf("\n");
    }

}

/** 输出该表的成绩情况 */
void outputStudentLinkedList(StudentLinkedListNode* head){
    // 用于单个节点输出的函数printLinkedListNode已提供
    //请你实现遍历链表的逻辑
	//TODO
}


/** 新建一个链表node并返回 */
StudentLinkedListNode* studentLinkedListCreate(char student_id[], int grade) {
    //tips:malloc的时候记得为转化为结构体指针
	//TODO
}


/** 按照降序插入学生的成绩情况,并返回链表头指针 */
StudentLinkedListNode* studentLinkedListAdd(StudentLinkedListNode* head, StudentLinkedListNode* node) {
    //TODO
    node->next = head;
    return node;
}


/** 反转链表 */
StudentLinkedListNode* reverseLinkedList(StudentLinkedListNode*head){
    //TODO
}

/** 找到相交的第一个结点 */
StudentLinkedListNode* findCrossBeginNode(StudentLinkedListNode* class1, StudentLinkedListNode* class2) {
    //class1和class2在后一部分完全重合（结点的内存地址相同），请找出并返回开始相交的第一个结点。
    //请不要简单地通过结点结构体的成员来判断。
    //TODO
}

int main(){
    freopen("./gradeImport.in","r",stdin);

    StudentLinkedListNode *class1=NULL, *class2=NULL;
    int num1, num2, i;
    char student_id[11];
    int grade;
    int beforeCross1, beforeCross2;
    StudentLinkedListNode *node;

    while(~scanf("%d%d", &num1, &num2)){
        class1 = class2 = NULL;
        // 存储数据到链表
        for(i = 0;i < num1;i++){
            scanf("%s%d", student_id, &grade);
            node = studentLinkedListCreate(student_id, grade);
            class1 = studentLinkedListAdd(class1, node);
        }
        for(i = 0;i < num2;i++){
            scanf("%s%d", student_id, &grade);
            node = studentLinkedListCreate(student_id, grade);
            class2 = studentLinkedListAdd(class2, node);
        }
        printf("* part1:\nclass1:\n");
        outputStudentLinkedList(class1);
        printf("class2:\n");
        outputStudentLinkedList(class2);

        // 反转链表
        class1 = reverseLinkedList(class1);
        class2 = reverseLinkedList(class2);
        printf("* part2:\nclass1:\n");
        outputStudentLinkedList(class1);
        printf("class2:\n");
        outputStudentLinkedList(class2);

        // 生成相交链表
        scanf("%d%d",&beforeCross1,&beforeCross2);
        createCrossLink(class1, class2, beforeCross1, beforeCross2);

        // 打印相交结点
        node = findCrossBeginNode(class1, class2);
        printf("* part3:\n");
        printf("{ID:%s, Grade:%d}\n", node->StuID, node->Grade);

        //销毁相交链表
        destroyCrossLink(class1, class2, node);

        printf("\n");
    }
    return 0;
}
