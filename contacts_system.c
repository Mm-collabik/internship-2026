#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200                    /* 最多存200个联系人 */
#define DATA_FILE "contacts.dat"   /* 数据文件名 */

/* 联系人结构体，记录一个联系人的全部信息 */
struct contact {
    int id;                        /* 编号（自动生成，唯一） */
    char name[32];                 /* 姓名 */
    char phone[20];                /* 电话号码 */
    char email[64];                /* 邮箱地址 */
    char addr[128];                /* 地址 */
    char group[16];                /* 分组：同学/老师/其他 */
};

/* 全局变量：联系人数组、当前人数、下一个可用编号 */
struct contact list[MAX];
int total = 0;                     /* 当前联系人总数 */
int next_id = 1001;                /* 编号从1001开始 */

/* ---------- 工具函数 ---------- */

/* 清空输入缓冲区（解决scanf后fgets跳过的问题） */
void clear_buffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* 根据编号查找联系人，返回数组下标，找不到返回-1 */
int find_by_id(int id) {
    int i;
    for (i = 0; i < total; i++) {
        if (list[i].id == id) return i;
    }
    return -1;
}

/* 打印一条联系人信息 */
void print_one(struct contact *p) {
    printf("%-8d %-10s %-16s %-24s %-24s %-10s\n",
           p->id, p->name, p->phone, p->email, p->addr, p->group);
}

/* 打印表头 */
void print_head() {
    printf("%-8s %-10s %-16s %-24s %-24s %-10s\n",
           "编号", "姓名", "电话", "邮箱", "地址", "分组");
    printf("--------------------------------------------------------");
    printf("-------------------------------\n");
}

/* ---------- 数据读写 ---------- */

/* 从文件加载数据 */
void load_data() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        /* 第一次运行，还没有数据文件，这是正常情况 */
        return;
    }
    fread(&total, sizeof(int), 1, fp);      /* 读入总人数 */
    fread(&next_id, sizeof(int), 1, fp);    /* 读入下一个编号 */
    fread(list, sizeof(struct contact), total, fp);  /* 读入全部联系人 */
    fclose(fp);
    printf("数据加载成功，当前有%d位联系人。\n", total);
}

/* 保存数据到文件 */
void save_data() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("警告：保存数据失败！\n");
        return;
    }
    fwrite(&total, sizeof(int), 1, fp);
    fwrite(&next_id, sizeof(int), 1, fp);
    fwrite(list, sizeof(struct contact), total, fp);
    fclose(fp);
}

/* ---------- 功能模块 ---------- */

/* 1. 添加联系人 */
void add_contact() {
    struct contact c;                /* 新联系人 */

    if (total >= MAX) {
        printf("通讯录已满，最多只能存%d人。\n", MAX);
        return;
    }

    printf("\n===== 添加联系人 =====\n");
    c.id = next_id++;                /* 分配编号 */

    printf("姓名：");
    fgets(c.name, 32, stdin);
    c.name[strcspn(c.name, "\n")] = '\0';  /* 去掉末尾换行 */

    printf("电话：");
    fgets(c.phone, 20, stdin);
    c.phone[strcspn(c.phone, "\n")] = '\0';

    printf("邮箱：");
    fgets(c.email, 64, stdin);
    c.email[strcspn(c.email, "\n")] = '\0';

    printf("地址：");
    fgets(c.addr, 128, stdin);
    c.addr[strcspn(c.addr, "\n")] = '\0';

    printf("分组（同学/老师/其他）：");
    fgets(c.group, 16, stdin);
    c.group[strcspn(c.group, "\n")] = '\0';

    list[total] = c;                 /* 存入数组 */
    total++;
    save_data();                     /* 自动保存 */
    printf("添加成功！联系人\"%s\"，编号%d\n", c.name, c.id);
}

/* 2. 删除联系人 */
void del_contact() {
    int id, idx, i;
    char yesno;

    if (total == 0) {
        printf("通讯录为空，没有可删除的联系人。\n");
        return;
    }

    printf("\n===== 删除联系人 =====\n");
    printf("请输入要删除的联系人编号：");
    scanf("%d", &id);
    clear_buffer();

    idx = find_by_id(id);
    if (idx == -1) {
        printf("没有找到编号为%d的联系人。\n", id);
        return;
    }

    printf("确定要删除\"%s\"吗？(y/n)：", list[idx].name);
    scanf("%c", &yesno);
    clear_buffer();

    if (yesno == 'y' || yesno == 'Y') {
        printf("联系人\"%s\"已删除。\n", list[idx].name);
        /* 把后面的元素依次往前移一位 */
        for (i = idx; i < total - 1; i++) {
            list[i] = list[i + 1];
        }
        total--;
        save_data();
    } else {
        printf("已取消删除。\n");
    }
}

/* 3. 修改联系人 */
void edit_contact() {
    int id, idx;
    char tmp[128];

    if (total == 0) {
        printf("通讯录为空，没有可修改的联系人。\n");
        return;
    }

    printf("\n===== 修改联系人 =====\n");
    printf("请输入要修改的联系人编号：");
    scanf("%d", &id);
    clear_buffer();

    idx = find_by_id(id);
    if (idx == -1) {
        printf("没有找到编号为%d的联系人。\n", id);
        return;
    }

    printf("（直接按回车表示保留原来的值）\n");

    printf("姓名[%s]：", list[idx].name);
    fgets(tmp, 32, stdin);
    if (tmp[0] != '\n') {
        tmp[strcspn(tmp, "\n")] = '\0';
        strcpy(list[idx].name, tmp);
    }

    printf("电话[%s]：", list[idx].phone);
    fgets(tmp, 20, stdin);
    if (tmp[0] != '\n') {
        tmp[strcspn(tmp, "\n")] = '\0';
        strcpy(list[idx].phone, tmp);
    }

    printf("邮箱[%s]：", list[idx].email);
    fgets(tmp, 64, stdin);
    if (tmp[0] != '\n') {
        tmp[strcspn(tmp, "\n")] = '\0';
        strcpy(list[idx].email, tmp);
    }

    printf("地址[%s]：", list[idx].addr);
    fgets(tmp, 128, stdin);
    if (tmp[0] != '\n') {
        tmp[strcspn(tmp, "\n")] = '\0';
        strcpy(list[idx].addr, tmp);
    }

    printf("分组[%s]：", list[idx].group);
    fgets(tmp, 16, stdin);
    if (tmp[0] != '\n') {
        tmp[strcspn(tmp, "\n")] = '\0';
        strcpy(list[idx].group, tmp);
    }

    save_data();
    printf("修改成功！\n");
}

/* 4. 查询联系人 */
void search_contact() {
    int choice, id, i, found;
    char keyword[32];

    if (total == 0) {
        printf("通讯录为空。\n");
        return;
    }

    printf("\n===== 查询联系人 =====\n");
    printf("1. 按编号查询\n");
    printf("2. 按姓名查询（支持模糊匹配）\n");
    printf("请选择（1或2）：");
    scanf("%d", &choice);
    clear_buffer();

    if (choice == 1) {
        printf("请输入编号：");
        scanf("%d", &id);
        clear_buffer();
        id = find_by_id(id);
        if (id >= 0) {
            print_head();
            print_one(&list[id]);
        } else {
            printf("没有找到该联系人的信息。\n");
        }
    } else if (choice == 2) {
        printf("请输入姓名关键词：");
        fgets(keyword, 32, stdin);
        keyword[strcspn(keyword, "\n")] = '\0';
        found = 0;
        print_head();
        for (i = 0; i < total; i++) {
            if (strstr(list[i].name, keyword) != NULL) {
                print_one(&list[i]);
                found = 1;
            }
        }
        if (found == 0) {
            printf("没有找到姓名包含\"%s\"的联系人。\n", keyword);
        }
    } else {
        printf("输入有误，请选择1或2。\n");
    }
}

/* 5. 列出全部联系人 */
void list_all() {
    int i;

    if (total == 0) {
        printf("通讯录为空。\n");
        return;
    }

    printf("\n===== 全部联系人（共%d人）=====\n", total);
    print_head();
    for (i = 0; i < total; i++) {
        print_one(&list[i]);
    }
}

/* 显示主菜单 */
void show_menu() {
    printf("\n");
    printf("============================\n");
    printf("   校园通讯录管理系统\n");
    printf("============================\n");
    printf("   1. 添加联系人\n");
    printf("   2. 删除联系人\n");
    printf("   3. 修改联系人\n");
    printf("   4. 查询联系人\n");
    printf("   5. 浏览全部联系人\n");
    printf("   6. 退出系统\n");
    printf("============================\n");
}

/* ---------- 主函数 ---------- */
int main() {
    int choice;

    printf("========================================\n");
    printf("   校园通讯录管理系统 v1.0\n");
    printf("========================================\n");

    /* 启动时加载已有数据 */
    load_data();

    /* 主循环：显示菜单，执行用户选择的操作 */
    do {
        show_menu();
        printf("请输入你的选择（1-6）：");
        if (scanf("%d", &choice) != 1) {
            /* 用户输入的不是数字 */
            choice = 0;
            clear_buffer();
        }
        clear_buffer();

        switch (choice) {
            case 1: add_contact();    break;
            case 2: del_contact();    break;
            case 3: edit_contact();   break;
            case 4: search_contact(); break;
            case 5: list_all();       break;
            case 6:
                printf("正在保存数据...\n");
                save_data();
                printf("数据已保存，再见！\n");
                break;
            default:
                printf("输入有误，请输入1到6之间的数字！\n");
        }
    } while (choice != 6);

    return 0;
}
