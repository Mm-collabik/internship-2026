#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 200
#define NAME_LEN      32
#define PHONE_LEN     20
#define EMAIL_LEN     64
#define ADDR_LEN     128
#define DATA_FILE    "contacts.dat"

/* 联系人结构体 */
typedef struct {
    int   id;
    char  name[NAME_LEN];
    char  phone[PHONE_LEN];
    char  email[EMAIL_LEN];
    char  address[ADDR_LEN];
    char  group[16];
} Contact;

/* 全局数据 */
static Contact contacts[MAX_CONTACTS];
static int     count  = 0;
static int     nextID = 1001;

/* 清空输入缓冲区 */
void clearBuf(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* 按ID查找索引 */
int findIndex(int id)
{
    for (int i = 0; i < count; i++)
        if (contacts[i].id == id) return i;
    return -1;
}

/* 打印表头 */
void printHeader(void)
{
    printf("%-8s %-10s %-16s %-24s %-24s %-10s\n",
           "编号", "姓名", "电话", "邮箱", "地址", "分组");
    for (int i = 0; i < 92; i++) printf("-");
    printf("\n");
}

/* 打印单个联系人 */
void printOne(Contact *c)
{
    printf("%-8d %-10s %-16s %-24s %-24s %-10s\n",
           c->id, c->name, c->phone, c->email, c->address, c->group);
}

/* 保存到文件 */
void save(void)
{
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) return;
    fwrite(&count,  sizeof(int), 1, fp);
    fwrite(&nextID, sizeof(int), 1, fp);
    fwrite(contacts, sizeof(Contact), count, fp);
    fclose(fp);
}

/* 从文件加载 */
void load(void)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return;
    fread(&count,  sizeof(int), 1, fp);
    fread(&nextID, sizeof(int), 1, fp);
    fread(contacts, sizeof(Contact), count, fp);
    fclose(fp);
}

/* 添加联系人 */
void add(void)
{
    if (count >= MAX_CONTACTS) {
        printf("[错误] 通讯录已满（最多%d人）\n", MAX_CONTACTS);
        return;
    }
    Contact c;
    c.id = nextID++;
    printf("\n--- 添加联系人 ---\n");
    printf("姓名：");  fgets(c.name, NAME_LEN, stdin);
    c.name[strcspn(c.name, "\n")] = 0;
    printf("电话：");  fgets(c.phone, PHONE_LEN, stdin);
    c.phone[strcspn(c.phone, "\n")] = 0;
    printf("邮箱：");  fgets(c.email, EMAIL_LEN, stdin);
    c.email[strcspn(c.email, "\n")] = 0;
    printf("地址：");  fgets(c.address, ADDR_LEN, stdin);
    c.address[strcspn(c.address, "\n")] = 0;
    printf("分组（同学/老师/其他）：");
    fgets(c.group, 16, stdin);
    c.group[strcspn(c.group, "\n")] = 0;

    contacts[count++] = c;
    save();
    printf("[成功] 联系人\"%s\"已添加，编号：%d\n", c.name, c.id);
}

/* 删除联系人 */
void del(void)
{
    if (count == 0) { printf("[提示] 通讯录为空\n"); return; }
    int id;
    printf("\n--- 删除联系人 ---\n");
    printf("请输入要删除的联系人编号：");
    scanf("%d", &id); clearBuf();
    int idx = findIndex(id);
    if (idx < 0) { printf("[错误] 未找到编号%d的联系人\n", id); return; }
    printf("确认删除\"%s\"？(y/n)：", contacts[idx].name);
    char ch; scanf("%c", &ch); clearBuf();
    if (ch == 'y' || ch == 'Y') {
        printf("联系人\"%s\"已删除\n", contacts[idx].name);
        for (int i = idx; i < count - 1; i++) contacts[i] = contacts[i + 1];
        count--;
        save();
    } else {
        printf("已取消\n");
    }
}

/* 修改联系人 */
void modify(void)
{
    if (count == 0) { printf("[提示] 通讯录为空\n"); return; }
    int id;
    printf("\n--- 修改联系人 ---\n");
    printf("请输入要修改的联系人编号：");
    scanf("%d", &id); clearBuf();
    int idx = findIndex(id);
    if (idx < 0) { printf("[错误] 未找到编号%d的联系人\n", id); return; }
    Contact *c = &contacts[idx];
    char tmp[ADDR_LEN];
    printf("（回车保留原值）\n");
    printf("姓名[%s]：", c->name);
    fgets(tmp, NAME_LEN, stdin);
    if (tmp[0] != '\n') { tmp[strcspn(tmp, "\n")] = 0; strcpy(c->name, tmp); }
    printf("电话[%s]：", c->phone);
    fgets(tmp, PHONE_LEN, stdin);
    if (tmp[0] != '\n') { tmp[strcspn(tmp, "\n")] = 0; strcpy(c->phone, tmp); }
    printf("邮箱[%s]：", c->email);
    fgets(tmp, EMAIL_LEN, stdin);
    if (tmp[0] != '\n') { tmp[strcspn(tmp, "\n")] = 0; strcpy(c->email, tmp); }
    printf("地址[%s]：", c->address);
    fgets(tmp, ADDR_LEN, stdin);
    if (tmp[0] != '\n') { tmp[strcspn(tmp, "\n")] = 0; strcpy(c->address, tmp); }
    printf("分组[%s]：", c->group);
    fgets(tmp, 16, stdin);
    if (tmp[0] != '\n') { tmp[strcspn(tmp, "\n")] = 0; strcpy(c->group, tmp); }
    save();
    printf("[成功] 联系人信息已更新\n");
}

/* 查询联系人 */
void search(void)
{
    if (count == 0) { printf("[提示] 通讯录为空\n"); return; }
    int choice;
    printf("\n--- 查询联系人 ---\n");
    printf("1. 按编号查询\n2. 按姓名查询\n请选择：");
    scanf("%d", &choice); clearBuf();
    if (choice == 1) {
        int id;
        printf("请输入编号："); scanf("%d", &id); clearBuf();
        int idx = findIndex(id);
        if (idx >= 0) { printHeader(); printOne(&contacts[idx]); }
        else printf("未找到\n");
    } else if (choice == 2) {
        char kw[NAME_LEN];
        printf("请输入姓名关键词：");
        fgets(kw, NAME_LEN, stdin);
        kw[strcspn(kw, "\n")] = 0;
        int found = 0;
        printHeader();
        for (int i = 0; i < count; i++)
            if (strstr(contacts[i].name, kw)) { printOne(&contacts[i]); found = 1; }
        if (!found) printf("未找到匹配的联系人\n");
    }
}

/* 列出所有联系人 */
void listAll(void)
{
    if (count == 0) { printf("[提示] 通讯录为空\n"); return; }
    printf("\n===== 全部联系人（共%d人）=====\n", count);
    printHeader();
    for (int i = 0; i < count; i++) printOne(&contacts[i]);
}

/* 菜单 */
void menu(void)
{
    printf("\n");
    printf("============================\n");
    printf("  校园通讯录管理系统\n");
    printf("============================\n");
    printf("  1. 添加联系人\n");
    printf("  2. 删除联系人\n");
    printf("  3. 修改联系人\n");
    printf("  4. 查询联系人\n");
    printf("  5. 浏览全部联系人\n");
    printf("  6. 退出系统\n");
    printf("============================\n");
}

int main(void)
{
    int choice;
    printf("正在加载数据...\n");
    load();
    printf("当前有%d位联系人\n", count);

    do {
        menu();
        printf("请选择（1-6）：");
        if (scanf("%d", &choice) != 1) { choice = -1; clearBuf(); }
        clearBuf();
        switch (choice) {
            case 1: add();      break;
            case 2: del();      break;
            case 3: modify();   break;
            case 4: search();   break;
            case 5: listAll();  break;
            case 6: save(); printf("数据已保存，再见！\n"); break;
            default: printf("[错误] 请输入1-6之间的数字\n");
        }
    } while (choice != 6);

    return 0;
}
