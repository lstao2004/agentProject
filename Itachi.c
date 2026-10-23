#include <stdio.h>
#include <string.h>

// 火影忍者·宇智波鼬 角色模型程序
// 用结构体对角色进行数据建模，写法与 Student.c 保持一致
// 菜单式查询:资料 / 能力值 / 忍术

#define MAX_JUTSU 16    // 忍术数组容量上限,声明与校验共用,避免魔法数字

// 八项能力值,满分 5 分(参考《火影忍者》官方设定书)
typedef struct {
    float ninjutsu;      // 忍:忍术
    float taijutsu;      // 体:体术
    float genjutsu;      // 幻:幻术
    float intelligence;  // 贤:知识/分析
    float strength;      // 力:力量
    float speed;         // 速:速度
    float stamina;       // 精:查克拉量/耐力
    float handSeals;     // 印:结印
} NinjaStats;

// 单个忍术/能力条目
typedef struct {
    char name[64];    // 名称
    char type[32];    // 类型:血继限界/瞳术/火遁/水遁/幻术/体术等
    char desc[256];   // 简介
} Jutsu;

// 角色主模型
typedef struct {
    char name[64];       // 姓名
    char village[64];    // 所属忍者村
    char clan[64];       // 家族
    char rank[64];       // 忍者等级/身份
    int  age;            // 年龄
    char birthday[32];   // 生日
    char bloodType[8];   // 血型
    float heightCm;      // 身高(cm)
    float weightKg;      // 体重(kg)
    NinjaStats stats;    // 八项能力值
} Ninja;

// 鼬的忍术/能力一览(静态数据,集中声明便于统一校验)
static const Jutsu ITACHI_JUTSU[MAX_JUTSU] = {
    {"万花筒写轮眼",  "血继限界·瞳术", "宇智波一族开眼至万花筒后获得的高级瞳术,可施展月读、天照等"},
    {"月读",          "幻术·瞳术",     "最强精神攻击,将对手拖入自己掌控的精神世界,数秒内造成巨大精神伤害"},
    {"天照",          "瞳术·火遁",     "目光聚焦处燃起黑色火焰,不将目标烧尽绝不熄灭"},
    {"须佐能乎",      "血继限界",       "以查克拉凝聚的巨大战神,攻防一体,持有十拳剑与八咫镜"},
    {"伊邪那美",      "幻术·瞳术",     "决定命运的禁术,让对手困于无限循环直至认清自我"},
    {"火遁·豪火球之术","火遁",         "喷出巨大火球,宇智波一族的入门火遁"},
    {"火遁·凤仙火之术","火遁",         "连续吐出多枚小火球,如凤仙花籽般散射"},
    {"水遁·水龙弹之术","水遁",         "凝聚水流形成巨龙冲击对手"},
    {"水遁·水分身之术","水遁",         "以水制造分身,可配合爆破使用"},
    {"手里剑术",      "体术·忍具",     "精准操控苦无与手里剑,可拐弯、弹射,命中率极高"},
    {"影分身之术",    "忍术",           "以查克拉制造实体分身,常用于侦察与迷惑"},
    {"乌鸦分身之术",  "忍术",           "化作鸦群的分身,鼬的标志性替身术"},
    {"转写封印·天照", "封印术",         "将天照预先封印,条件触发时自动发动"},
    {"伊邪那岐",      "幻术·瞳术",     "将现实改写为梦境的禁术(需付出失明代价)"},
    {"豪火球连发",    "火遁",           "高速连续喷发豪火球形成压制"},
    {"须佐能乎·八坂之勾玉","血继限界","须佐能乎投掷的巨型勾玉,远程毁灭性攻击"},
};

// 打印角色资料
void printNinjaProfile(const Ninja *n) {
    if (n == NULL) {                     // 空指针防御,避免解引用崩溃
        printf("角色数据为空\n");
        return;
    }
    printf("===== 角色资料 =====\n");
    printf("姓名: %s\n", n->name);
    printf("所属: %s\n", n->village);
    printf("家族: %s\n", n->clan);
    printf("身份: %s\n", n->rank);
    printf("年龄: %d\n", n->age);
    printf("生日: %s\n", n->birthday);
    printf("血型: %s\n", n->bloodType);
    printf("身高: %.1f cm\n", n->heightCm);
    printf("体重: %.1f kg\n", n->weightKg);
}

// 打印八项能力值(带满分 5 分基准)
void printNinjaStats(const Ninja *n) {
    if (n == NULL) {
        printf("角色数据为空\n");
        return;
    }
    const NinjaStats *s = &n->stats;     // 取地址避免整体拷贝
    printf("===== 八项能力值(满分 5.0) =====\n");
    printf("忍(忍术):   %.1f\n", s->ninjutsu);
    printf("体(体术):   %.1f\n", s->taijutsu);
    printf("幻(幻术):   %.1f\n", s->genjutsu);
    printf("贤(智力):   %.1f\n", s->intelligence);
    printf("力(力量):   %.1f\n", s->strength);
    printf("速(速度):   %.1f\n", s->speed);
    printf("精(查克拉): %.1f\n", s->stamina);
    printf("印(结印):   %.1f\n", s->handSeals);
}

// 打印忍术列表,与 checkStudentInfo 同样做两层越界防御
void printJutsuList(const Jutsu *jutsus, int count) {
    if (jutsus == NULL || count <= 0) {  // 第一道防线:空指针/非正数
        printf("忍术列表为空\n");
        return;
    }
    if (count > MAX_JUTSU) {             // 第二道防线:count 不能超过容量
        printf("忍术数量超过最大容量\n");
        return;
    }
    printf("===== 忍术/能力列表(共 %d 项) =====\n", count);
    for (int i = 0; i < count; i++) {
        const Jutsu *j = &jutsus[i];     // 取地址,省去每次复制整个结构体
        printf("[%02d] %s(%s): %s\n", i + 1, j->name, j->type, j->desc);
    }
}

int main() {
    // 用静态数据初始化宇智波鼬的角色模型
    Ninja itachi = {
        "宇智波鼬(Uchiha Itachi)",
        "木叶隐村",
        "宇智波一族",
        "暗部精英 / S级叛忍",
        21,                  // 终年约 21 岁
        "6月9日",
        "AB",
        178.0f,              // 身高
        58.0f,               // 体重
        {                    // 八项能力值
            5.0f,            // 忍
            4.5f,            // 体
            5.0f,            // 幻
            5.0f,            // 贤
            3.5f,            // 力
            5.0f,            // 速
            2.5f,            // 精(因病查克拉较低)
            5.0f             // 印
        }
    };
    const int jutsuCount = 14;    // 实际展示前 14 项,其余留作扩展位

    int choice;
    while (1) {
        printf("\n1.查看角色资料  2.查看能力值  3.查看忍术列表  0.退出\n请选择: ");
        // scanf 返回值不是 1 说明输入了非数字,清空缓冲区避免死循环
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {
                // 丢弃本行剩余字符
            }
            printf("输入格式错误,请输入数字\n");
            continue;
        }

        if (choice == 0) {
            printf("再见。\n");
            break;
        } else if (choice == 1) {
            printNinjaProfile(&itachi);
        } else if (choice == 2) {
            printNinjaStats(&itachi);
        } else if (choice == 3) {
            printJutsuList(ITACHI_JUTSU, jutsuCount);
        } else {
            printf("无效的选择\n");
        }
    }
    return 0;
}
