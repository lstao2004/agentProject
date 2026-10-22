/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-10-22 14:01:37
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-10-22 16:08:14
 * @FilePath: \vscodeProject\Student.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>

//学生管理系统有学生编号,姓名，年龄等信息

#define MAX_STUDENTS 100    //数组容量上限,声明与校验共用,避免魔法数字

typedef struct {
    int id;
    char name[100];
    int age;

} Student;
//查询学生信息

void checkStudentInfo(const Student *stus, int count) {
    //越界访问的第一道防线:空指针/非正数直接返回
    if (stus == NULL || count <= 0) {
        printf("学生信息为空\n");
        return;
    }
    //第二道防线:count 不能超过实际数组容量,否则下面的循环会越界读写
    if (count > MAX_STUDENTS) {
        printf("学生信息超过最大数量\n");
        return;
    }
    printf("编号\t姓名\t年龄\n");
    for (int i = 0; i < count; i++) {
        const Student *s = &stus[i];    //取地址而非整体拷贝,每次循环省下约 108 字节复制
        printf("%d\t%s\t%d\n", s->id, s->name, s->age);
    }
    printf("学生信息查询完毕\n");
}

int main() {
    Student stus[MAX_STUDENTS];
    int count = 0;      //必须初始化:未初始化的局部变量是随机值,直接当数组长度用属于未定义行为
    int choice;

    while (1) {         //原为 whlie,拼写错误导致编译失败
        printf("\n1.录入学生信息  2.查询学生信息  0.退出\n请选择: ");
        //scanf 返回值不是 1 说明输入了非数字,此时必须清空缓冲区,否则会无限循环
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {
                //丢弃本行剩余字符
            }
            printf("输入格式错误,请输入数字\n");
            continue;
        }

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            if (count >= MAX_STUDENTS) {
                printf("已达到最大录入数量 %d\n", MAX_STUDENTS);
                continue;
            }
            Student *s = &stus[count];
            printf("请输入 编号 姓名 年龄: ");
            //%99s 限制最多读入 99 个字符,留 1 个位置给 '\0',防止 name 缓冲区溢出
            if (scanf("%d %99s %d", &s->id, s->name, &s->age) != 3) {
                while (getchar() != '\n') {
                }
                printf("输入格式错误,本条记录未保存\n");
                continue;
            }
            if (s->age < 0 || s->age > 150) {
                printf("年龄 %d 不合理,本条记录未保存\n", s->age);
                continue;
            }
            count++;
        } else if (choice == 2) {
            checkStudentInfo(stus, count);
        } else {
            printf("无效的选择\n");
        }
    }
    return 0;
}
