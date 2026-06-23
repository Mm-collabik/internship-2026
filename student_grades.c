#include <stdio.h>

#define N 5      /* 学生人数 */
#define M 3      /* 课程数量 */

/* 定义学生结构体，存储学号、姓名、各科成绩、总分和平均分 */
struct student {
    int id;               /* 学号 */
    char name[20];        /* 姓名 */
    float score[M];       /* 三门课的成绩 */
    float sum;            /* 总分 */
    float avg;            /* 平均分 */
};

int main()
{
    /* 声明要用到的变量 */
    struct student stu[N];                /* 学生数组 */
    char *kemu[M] = {"数学", "英语", "C语言"}; /* 课程名称 */
    int i, j;                            /* 循环变量 */
    int best_idx;                        /* 记录最高分学生的下标 */
    struct student temp;                 /* 排序时用于交换的临时变量 */

    printf("===== 学生成绩管理系统 =====\n");
    printf("请输入%d名学生的信息（学号 姓名 三门课成绩）\n\n", N);

    /* ===== 1. 输入学生信息 ===== */
    for (i = 0; i < N; i++) {
        printf("第%d位学生：\n", i + 1);
        printf("学号：");
        scanf("%d", &stu[i].id);
        printf("姓名：");
        scanf("%s", stu[i].name);
        for (j = 0; j < M; j++) {
            printf("%s成绩：", kemu[j]);
            scanf("%f", &stu[i].score[j]);
        }
        printf("\n");
    }

    /* ===== 2. 计算每位学生的总分和平均分 ===== */
    for (i = 0; i < N; i++) {
        stu[i].sum = 0.0;                /* 先清零 */
        for (j = 0; j < M; j++) {
            stu[i].sum += stu[i].score[j];
        }
        stu[i].avg = stu[i].sum / M;
    }

    /* ===== 3. 按总分从高到低排序（冒泡排序） ===== */
    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - 1 - i; j++) {
            if (stu[j].sum < stu[j + 1].sum) {
                temp = stu[j];            /* 结构体可以直接赋值 */
                stu[j] = stu[j + 1];
                stu[j + 1] = temp;
            }
        }
    }

    /* ===== 4. 输出排序后的成绩单 ===== */
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

    /* ===== 5. 查找每门课程的最高分学生 ===== */
    printf("\n===== 各课程最高分学生 =====\n");
    for (j = 0; j < M; j++) {
        best_idx = 0;                    /* 先假设第0个学生最高 */
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
