#include <stdio.h>

#define N 5      
#define M 3      


struct student {
    int id;               /* 学号 */
    char name[20];        /* 姓名 */
    float score[M];       /* 三门课的成绩 */
    float sum;            /* 总分 */
    float avg;            /* 平均分 */
};

int main()
{
   
    struct student stu[N];                
    char *kemu[M] = {"数学", "英语", "C语言"}; 
    int i, j;                            
    int best_idx;                        
    struct student temp;                

    printf("========================================\n");
    printf("     学生成绩管理系统\n");
    printf("========================================\n");
    printf("请输入%d名学生的信息（学号 姓名 三门课成绩）\n\n", N);

    /*输入学生信息*/
    for (i = 0; i < N; i++) {
        printf("第%d位学生：\n", i + 1);
        printf("学号：");
        scanf("%d", &stu[i].id);
        printf("姓名：");
        scanf("%s", stu[i].name);
        for (j = 0; j < M; j++) {
            printf("%s成绩：", kemu[j]);
            scanf("%f", &stu[i].score[j]);
            /* 成绩范围检查，防止输入错误 */
            while (stu[i].score[j] < 0 || stu[i].score[j] > 100) {
                printf("成绩应在0-100之间，请重新输入：");
                scanf("%f", &stu[i].score[j]);
            }
        }
        printf("\n");
    }

    /*计算每位学生的总分和平均分*/
    for (i = 0; i < N; i++) {
        stu[i].sum = 0.0;                
        for (j = 0; j < M; j++) {
            stu[i].sum += stu[i].score[j];
        }
        stu[i].avg = stu[i].sum / M;
    }

    
    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - 1 - i; j++) {
            if (stu[j].sum < stu[j + 1].sum) {
                temp = stu[j];            
                stu[j] = stu[j + 1];
                stu[j + 1] = temp;
            }
        }
    }

   
    printf("\n===== 按总分排序结果（从高到低） =====\n");
    printf("学号    姓名     ");
    for (j = 0; j < M; j++) {
        printf("%-8s", kemu[j]);
    }
    printf("%-10s%-8s\n", "总分", "平均分");
    printf("--------------------------------------------------------------\n");

    for (i = 0; i < N; i++) {
        printf("%-8d%-8s", stu[i].id, stu[i].name);
        for (j = 0; j < M; j++) {
            printf("%-8.1f", stu[i].score[j]);
        }
        printf("%-10.1f%-8.2f\n", stu[i].sum, stu[i].avg);
    }

    
    printf("\n===== 各课程最高分学生 =====\n");
    for (j = 0; j < M; j++) {
        best_idx = 0;                    
        for (i = 1; i < N; i++) {
            if (stu[i].score[j] > stu[best_idx].score[j]) {
                best_idx = i;
            }
        }
        printf("%s最高分：%s（学号%d），成绩%.1f分\n",
               kemu[j], stu[best_idx].name, stu[best_idx].id, stu[best_idx].score[j]);
    }

    printf("\n程序运行结束，感谢使用！\n");
    return 0;
}
