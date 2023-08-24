/*             С версия            */
#include <locale.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool victory = false;

struct result
{
    char* name;
    unsigned short games;
    unsigned short victories;
    double percent;
    unsigned short izzy;/*успешно пройденный просто*/
    unsigned short hard;/*успешно пройденный не просто*/
    unsigned short doom_guy;/*успешно пройденный ХАРДКОР*/
};
int scanf_rng(const char* const _Format, ...)
{
    int _Result;
    va_list _ArgList;
    va_start(_ArgList, _Format);
    _Result = vfscanf(stdin, _Format, _ArgList);
    va_end(_ArgList);
    scanf("%*[^\n]");
    fflush(stdin);
    return _Result;
}
int compare_games(const void* a, const void* b)
{
    const struct result* k = (const struct result*)a;
    const struct result* l = (const struct result*)b;
    return ((k->games) - (l->games));
}
int compare_izzy(const void* a, const void* b)
{
    const struct result* k = (const struct result*)a;
    const struct result* l = (const struct result*)b;
    return ((k->izzy) - (l->izzy));
}
int compare_hard(const void* a, const void* b)
{
    const struct result* k = (const struct result*)a;
    const struct result* l = (const struct result*)b;
    return ((k->hard) - (l->hard));
}
int compare_doom(const void* a, const void* b)
{
    const struct result* k = (const struct result*)a;
    const struct result* l = (const struct result*)b;
    return ((k->doom_guy) - (l->doom_guy));
}
int compare_percent(const void* a, const void* b)
{
    const struct result* k = (const struct result*)a;
    const struct result* l = (const struct result*)b;
    return ((k->percent) - (l->percent));
}
void save(struct result* save, const unsigned short f)
{
    FILE* fout = fopen("results.bin", "wbe");
    for (unsigned short q = 0; q < f; q++)
    {
        const unsigned short sz = strlen(save[q].name);
        fwrite(&sz, sizeof(unsigned short), 1, fout);
        fwrite(save[q].name, sizeof(char), sz, fout);
        fwrite(&save[q].games, sizeof(unsigned short), 1, fout);
        fwrite(&save[q].victories, sizeof(unsigned short), 1, fout);
        fwrite(&save[q].percent, sizeof(double), 1, fout);
        fwrite(&save[q].izzy, sizeof(unsigned short), 1, fout);
        fwrite(&save[q].hard, sizeof(unsigned short), 1, fout);
        fwrite(&save[q].doom_guy, sizeof(unsigned short), 1, fout);
    }
    fclose(fout);
}
void printdesk(char** deskgr, const unsigned short x, const unsigned short y)
{
    printf("  ");
    for (unsigned short i = 1; i < y + 1; i++)
        printf(" %d", i % 10);
    printf("\n");
    for (unsigned short i = 0; i < x; i++)
    {
        printf("  ");
        for (unsigned short q = 1; q < y + 1; q++)
            printf(" _");
        printf("\n");
        printf("%d ", (i + 1) % 10);
        for (unsigned short q = 0; q < y; q++)
            printf("|%c", deskgr[i][q]);
        printf("|\n");
    }
    printf("  ");
    for (unsigned short q = 1; q < y + 1; q++)
        printf(" _");
    printf("\n");
}
char opencl(const unsigned short num)
{
    switch (num)
    {
        case 0:
            return 48;
            break;
        case 1:
            return 49;
            break;
        case 2:
            return 50;
            break;
        case 3:
            return 51;
            break;
        case 4:
            return 52;
            break;
        case 5:
            return 53;
            break;
        case 6:
            return 54;
            break;
        case 7:
            return 55;
            break;
        default:
            return 56;
            break;
    }
}
short to_hu(char* XM)
{
    unsigned short xm = 0;
    bool correct = true;
    if (XM[1] == 49 || XM[1] == 50 || XM[1] == 51 || XM[1] == 52 || XM[1] == 53 || XM[1] == 54 || XM[1] == 55 || XM[1] == 56 || XM[1] == 57 || XM[1] == 58)
    {
        switch (XM[0])
        {
            case 49:
                xm = 10;
                break;
            case 50:
                xm = 20;
                break;
            case 51:
                xm = 30;
                break;
            default:
                printf("Ввод не корректен.\n");
                correct = false;
                break;
        }
        if (correct)
        {
            switch (XM[1])
            {
                case 48:
                    break;
                case 49:
                    xm++;
                    break;
                case 50:
                    xm += 2;
                    break;
                case 51:
                    xm += 3;
                    break;
                case 52:
                    xm += 4;
                    break;
                case 53:
                    xm += 5;
                    break;
                case 54:
                    xm += 6;
                    break;
                case 55:
                    xm += 7;
                    break;
                case 56:
                    xm += 8;
                    break;
                case 57:
                    xm += 9;
                    break;
                default:
                    printf("Ввод не корректен.\n");
                    correct = false;
                    break;
            }
        }
        if (correct && XM[2] != 0)
        {
            printf("Ввод не корректен.\n");
            correct = false;
        }
    }
    else if (XM[1] == 0)
    {
        switch (*XM)
        {
            case 48:
                break;
            case 49:
                xm++;
                break;
            case 50:
                xm += 2;
                break;
            case 51:
                xm += 3;
                break;
            case 52:
                xm += 4;
                break;
            case 53:
                xm += 5;
                break;
            case 54:
                xm += 6;
                break;
            case 55:
                xm += 7;
                break;
            case 56:
                xm += 8;
                break;
            case 57:
                xm += 9;
                break;
            default:
                printf("Ввод не корректен.\n");
                correct = false;
                break;
        }
    }
    else
    {
        printf("Ввод не корректен.\n");
        correct = false;
    }
    return correct ? xm : -1;
}
void game(struct result* p, struct result player, const unsigned short type)
{
    unsigned short x = 0, y = 0;/*custom*/
    unsigned short mines = 0;
    char** deskgr = 0;
    unsigned short** bnum = 0;
    unsigned short count = 0;/*кол-во мин*/
    unsigned short* position = 0;
    if (type != 1)
    {
        deskgr = (char**)malloc(type * sizeof(char*));
        for (unsigned short i = 0; i < type; i++)
            deskgr[i] = (char*)malloc(type * sizeof(char));
        for (unsigned short i = 0; i < type; i++)
            for (unsigned short q = 0; q < type; q++)
                deskgr[i][q] = 32;
        /*то, что отображается*/
        bnum = (unsigned short**)malloc(type * sizeof(unsigned short*));
        for (unsigned short i = 0; i < type; i++)
            bnum[i] = (unsigned short*)malloc(type * sizeof(unsigned short));
        for (unsigned short i = 0; i < type; i++)
            for (unsigned short q = 0; q < type; q++)
                bnum[i][q] = 0;
        /*номера(рядом с минами)*/
        switch (type)
        {
            case 8:
                printdesk(deskgr, 8, 8);
                break;
            case 16:
                printdesk(deskgr, 16, 16);
                break;
            default:
                printdesk(deskgr, 32, 32);
                break;
        }
        srand(time(NULL));
        count = 0;
        position = (unsigned short*)malloc(type * 2 * sizeof(unsigned short));/*номера мин*/
        while (count != type * 2)
        {
            bool test = false;
            unsigned short place = rand() % ((type * type - 1) - 0 + 1) + 0; /*позиция мины*/
            for (unsigned short i = 0; i < count; i++)
            {
                if (place == position[i])
                {
                    test = true;
                    break;
                }
            }
            if (!test)
            {
                count++;
                position[count - 1] = place;
            }
        }/*создали мины*/
        unsigned short xc = 0, yc = 0;
        for (unsigned short q = 0; q < count; q++)
        {
            xc = position[q] / type;/*строка с миной*/
            yc = position[q] - (type * xc);/*столбец с миной*/
            /*определили положение мины*/
            if (xc > 0)
                bnum[xc - 1][yc]++;
            if (xc < type - 1)
                bnum[xc + 1][yc]++;
            if (yc > 0)
                bnum[xc][yc - 1]++;
            if (yc < type - 1)
                bnum[xc][yc + 1]++;
            if (xc > 0 && yc > 0)
                bnum[xc - 1][yc - 1]++;
            if (xc < type - 1 && yc > 0)
                bnum[xc + 1][yc - 1]++;
            if (xc > 0 && yc < type - 1)
                bnum[xc - 1][yc + 1]++;
            if (xc < type - 1 && yc < type - 1)
                bnum[xc + 1][yc + 1]++;
        }/*Заполнили номерами для мин*/
    }
    else
    {
        char* X = (char*)malloc(0);
        char* Y = (char*)malloc(0);
        printf("Задайте размер доски.\n");
        do
        {
            printf("Кол-во строк(не больше 50, минимум 3): ");
            scanf_rng("%3s", X);
            x = atoi(X);
            if (x < 3 || x > 50)
                printf("Ввод не корректен\n");
        }
        while (x < 3 || x > 50);
        do
        {
            printf("Кол-во столбцов(не больше 50, минимум 3): ");
            scanf_rng("%3s", Y);
            y = atoi(Y);
            if (y < 3 || y > 50)
                printf("Ввод не корректен\n");
        }
        while (y < 3 || y > 50);
        deskgr = (char**)malloc(x * sizeof(char*));
        for (unsigned short i = 0; i < x; i++)
            deskgr[i] = (char*)malloc(y * sizeof(char));
        for (unsigned short i = 0; i < x; i++)
            for (unsigned short q = 0; q < y; q++)
                deskgr[i][q] = 32;
        /*то, что отображается*/
        bnum = (unsigned short**)malloc(x * sizeof(unsigned short*));
        for (unsigned short i = 0; i < x; i++)
            bnum[i] = (unsigned short*)malloc(y * sizeof(unsigned short));
        for (unsigned short i = 0; i < x; i++)
            for (unsigned short q = 0; q < y; q++)
                bnum[i][q] = 0;
        /*номера(рядом с минами)*/
        printdesk(deskgr, x, y);
        do
        {
            mines = 0;
            char* MINES = (char*)malloc(0);
            printf("Задайте кол-во мин (минимум 5%% от площади доски и максимум 95%%): ");
            scanf_rng("%4s", MINES);
            mines = atoi(MINES);
            if (mines < (x * y) / 100.0 * 5 || mines >(x * y) / 100.0 * 95)
                printf("Ввод не корректен\n");
        }
        while (mines < (x * y) / 100.0 * 5 || mines >(x * y) / 100.0 * 95);
        srand(time(NULL));
        count = 0;
        position = (unsigned short*)malloc(mines * sizeof(unsigned short));/*номера мин*/
        while (count != mines)
        {
            bool test = false;
            unsigned short place = rand() % ((x * y - 1) - 0 + 1) + 0; /*позиция мины*/
            for (unsigned short i = 0; i < count; i++)
            {
                if (place == position[i])
                {
                    test = true;
                    break;
                }
            }
            if (!test)
            {
                count++;
                position[count - 1] = place;
            }
        } /*создали мины*/
        unsigned short xc = 0, yc = 0;
        for (unsigned short q = 0; q < count; q++)
        {
            xc = position[q] / y;/*строка с миной*/
            yc = position[q] - y * xc;/*столбец с миной*/
            /*определили положение мины*/
            if (xc > 0)
                bnum[xc - 1][yc]++;
            if (xc < x - 1)
                bnum[xc + 1][yc]++;
            if (yc > 0)
                bnum[xc][yc - 1]++;
            if (yc < y - 1)
                bnum[xc][yc + 1]++;
            if (xc > 0 && yc > 0)
                bnum[xc - 1][yc - 1]++;
            if (xc < x - 1 && yc > 0)
                bnum[xc + 1][yc - 1]++;
            if (xc > 0 && yc < y - 1)
                bnum[xc - 1][yc + 1]++;
            if (xc < x - 1 && yc < y - 1)
                bnum[xc + 1][yc + 1]++;
        }/*Заполнили номерами для мин*/
    }
    unsigned short xh = 0, yh = 0, mch = 0, mv = 0;
    bool lose = false;
    unsigned short space = type != 1 ? type * type - type * 2 : x * y - mines;/*не мины*/;
    char* MV = (char*)malloc(0);
    char* YESf = (char*)malloc(0);
    char* YESd = (char*)malloc(0);
    char* YESm = (char*)malloc(0);
    char* XM = (char*)malloc(0);
    char* YM = (char*)malloc(0);
    char* XD = (char*)malloc(0);
    char* YD = (char*)malloc(0);
    char* XH = (char*)malloc(0);
    char* YH = (char*)malloc(0);
    time_t start = time(NULL);
    do
    {
        do
        {
            printf("1. Поставить метку\n2. Убрать метку\n3. Сделать ход\n");
            printf("Выберите ЦИФРУ: ");
            scanf_rng("%2s", MV);
            if (*MV != 49 && *MV != 50 && *MV != 51 || MV[1] != 0)
                printf("Ввод не коректен\n");
        }
        while (*MV != 49 && *MV != 50 && *MV != 51 || MV[1] != 0);
        switch (*MV)
        {
            case 49:
                mv = 1;
                break;
            case 50:
                mv = 2;
                break;
            default:
                mv = 3;
                break;
        }
        switch (mv)
        {
            case 1:
                do
                {
                    printf("Если у вас есть предположение о месте мин, то можете поставить + на это место. Если хотите это сделать, то введите YES, иначе что-то другое: ");
                    scanf_rng("%4s", YESf);
                    if (strcmp(YESf, "YES") == 0)
                    {
                        unsigned short xm = 0, ym = 0;/*координаты мин*/
                        short xt = 0, yt = 0;/*тест*/
                        printf("Введите координаты мины.\n");
                        do
                        {
                            xm = 0;
                            printf("Введите номер строки: ");
                            scanf_rng("%3s", XM);
                            xt = to_hu(XM);
                        }
                        while (xt == -1);
                        xm = xt;
                        do
                        {
                            ym = 0;
                            printf("Введите номер столбца: ");
                            scanf_rng("%3s", YM);
                            yt = to_hu(YM);
                        }
                        while (yt == -1);
                        ym = yt;
                        if (type != 1)
                        {
                            if ((xm > type || xm < 1) && (ym > type || ym < 1))
                            {
                                printf("Вы вышли за рамки доски\n");
                                break;
                            }
                        }
                        else if ((xm > x || xm < 1) && (ym > y || ym < 1))
                        {
                            printf("Вы вышли за рамки доски\n");
                            break;
                        }
                        if (deskgr[xm - 1][ym - 1] != 32)
                        {
                            printf("Сомневаюсь, что вы хотите поставить сюда метку...\n");
                            break;
                        }
                        deskgr[xm - 1][ym - 1] = 43;
                        switch (type)
                        {
                            case 8:
                                printdesk(deskgr, 8, 8);
                                break;
                            case 16:
                                printdesk(deskgr, 16, 16);
                                break;
                            case 32:
                                printdesk(deskgr, 32, 32);
                                break;
                            default:
                                printdesk(deskgr, x, y);
                                break;
                        }
                    }
                }
                while (strcmp(YESf, "YES") == 0);
                break;
            case 2:
                do
                {
                    printf("Вы можете убрать метку на мину. Если хотите это сделать, то введите YES, иначе что-то другое: ");
                    scanf_rng("%4s", YESd);
                    if (strcmp(YESd, "YES") == 0)
                    {
                        unsigned short xm = 0, ym = 0;/*координаты мин*/
                        short xt = 0, yt = 0;/*тест*/
                        printf("Введите координаты мины.\n");
                        do
                        {
                            xm = 0;
                            printf("Введите номер строки: ");
                            scanf_rng("%3s", XD);
                            xt = to_hu(XD);
                        }
                        while (xt == -1);
                        xm = xt;
                        do
                        {
                            ym = 0;
                            printf("Введите номер столбца: ");
                            scanf_rng("%3s", YD);
                            yt = to_hu(YD);
                        }
                        while (yt == -1);
                        ym = yt;
                        if (type != 1)
                        {
                            if ((xm > type || xm < 1) && (ym > type || ym < 1))
                            {
                                printf("Вы вышли за рамки доски\n");
                                break;
                            }
                        }
                        else if ((xm > x || xm < 1) && (ym > y || ym < 1))
                        {
                            printf("Вы вышли за рамки доски\n");
                            break;
                        }
                        if (deskgr[xm - 1][ym - 1] != 43)
                        {
                            printf("Здесь нет метки...\n");
                            break;
                        }
                        deskgr[xm - 1][ym - 1] = 32;
                        switch (type)
                        {
                            case 8:
                                printdesk(deskgr, 8, 8);
                                break;
                            case 16:
                                printdesk(deskgr, 16, 16);
                                break;
                            case 32:
                                printdesk(deskgr, 32, 32);
                                break;
                            default:
                                printdesk(deskgr, x, y);
                                break;
                        }
                    }
                }
                while (strcmp(YESd, "YES") == 0);
                break;
            default:
                printf("Вы уверены, что хотите совершить ход? Если да, то введите YES, иначе что-то другое: ");
                scanf_rng("%4s", YESm);
                if (strcmp(YESm, "YES") != 0)
                    break;
                short xt = 0, yt = 0;/*тест*/
                bool beyond = false;
                do
                {
                    printf("Введите координаты мины\n");
                    do
                    {
                        xh = 0;
                        printf("Введите номер строки: ");
                        scanf_rng("%3s", XH);
                        xt = to_hu(XH);
                    }
                    while (xt == -1);
                    xh = xt;
                    do
                    {
                        yh = 0;
                        printf("Введите номер столбца: ");
                        scanf_rng("%3s", YH);
                        yt = to_hu(YH);
                    }
                    while (yt == -1);
                    yh = yt;
                    beyond = false;
                    if (type != 1)
                    {
                        if ((xh > type || xh < 1) && (yh > type || yh < 1))
                        {
                            printf("Вы вышли за рамки доски\n");
                            beyond = true;
                        }
                    }
                    else if ((xh > x || xh < 1) && (yh > y || yh < 1))
                    {
                        printf("Вы вышли за рамки доски\n");
                        beyond = true;
                    }
                }
                while (beyond);
                for (unsigned short c = 0; c < count; c++)
                {
                    if (type != 1)
                    {
                        if ((xh - 1) * type + yh - 1 == position[c]) /*просрали*/
                        {
                            for (unsigned short o = 0; o < count; o++)
                                deskgr[position[o] / type][position[o] - (position[o] / type) * type] = 120;
                            switch (type)
                            {
                                case 8:
                                    printdesk(deskgr, 8, 8);
                                    break;
                                case 16:
                                    printdesk(deskgr, 16, 16);
                                    break;
                                default:
                                    printdesk(deskgr, 32, 32);
                                    break;
                            }
                            printf("Вы взорвались :(\n");
                            lose = true;
                            break;
                        }
                    }
                    else if ((xh - 1) * y + yh - 1 == position[c]) /*просрали*/
                    {
                        for (unsigned short o = 0; o < count; o++)
                            deskgr[position[o] / y][position[o] - (position[o] / y) * y] = 120;
                        printdesk(deskgr, x, y);
                        printf("Вы взорвались :(\n");
                        lose = true;
                        break;
                    }
                }
                if (!lose && deskgr[xh - 1][yh - 1] != 32)
                    printf("Эта клетка уже открыта\n");
                else if (!lose && deskgr[xh - 1][yh - 1] == 32) /*не просрали*/
                {
                    unsigned short i = 0;
                    unsigned short amount = 0;
                    if (bnum[xh - 1][yh - 1] == 0) /*открытие клеток*/
                    {
                        unsigned short* eternal_x = (unsigned short*)malloc(1 * sizeof(unsigned short));/*строки 0*/
                        *eternal_x = xh - 1;
                        unsigned short* eternal_y = (unsigned short*)malloc(1 * sizeof(unsigned short));/*столбцы 0*/
                        *eternal_y = yh - 1;
                        unsigned short eternal_size = 1;
                        for (i = 0; i < eternal_size; i++)
                        {
                            deskgr[eternal_x[i]][eternal_y[i]] = 48;
                            amount++;
                            if (eternal_x[i] > 0)
                            {
                                if (bnum[eternal_x[i] - 1][eternal_y[i]] == 0)
                                {
                                    bool potato = false;
                                    for (unsigned short c = 0; c < eternal_size; c++)
                                    {
                                        if (eternal_x[c] == eternal_x[i] - 1 && eternal_y[c] == eternal_y[i])
                                        {
                                            potato = true;
                                            break;
                                        }
                                    }
                                    if (!potato)
                                    {
                                        eternal_size++;
                                        eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                        eternal_x[eternal_size - 1] = eternal_x[i] - 1;
                                        eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                        eternal_y[eternal_size - 1] = eternal_y[i];
                                    }
                                }
                                else if (deskgr[eternal_x[i] - 1][eternal_y[i]] == 32)
                                {
                                    deskgr[eternal_x[i] - 1][eternal_y[i]] = opencl(bnum[eternal_x[i] - 1][eternal_y[i]]);
                                    amount++;
                                }
                            }
                            if (type != 1)
                            {
                                if (eternal_x[i] < type - 1)
                                {
                                    if (bnum[eternal_x[i] + 1][eternal_y[i]] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] + 1 && eternal_y[c] == eternal_y[i])
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] + 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i];
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] + 1][eternal_y[i]] == 32)
                                    {
                                        deskgr[eternal_x[i] + 1][eternal_y[i]] = opencl(bnum[eternal_x[i] + 1][eternal_y[i]]);
                                        amount++;
                                    }
                                }
                            }
                            else
                            {
                                if (eternal_x[i] < x - 1)
                                {
                                    if (bnum[eternal_x[i] + 1][eternal_y[i]] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] + 1 && eternal_y[c] == eternal_y[i])
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] + 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i];
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] + 1][eternal_y[i]] == 32)
                                    {
                                        deskgr[eternal_x[i] + 1][eternal_y[i]] = opencl(bnum[eternal_x[i] + 1][eternal_y[i]]);
                                        amount++;
                                    }
                                }
                            }
                            if (eternal_y[i] > 0)
                            {
                                if (bnum[eternal_x[i]][eternal_y[i] - 1] == 0)
                                {
                                    bool potato = false;
                                    for (unsigned short c = 0; c < eternal_size; c++)
                                    {
                                        if (eternal_x[c] == eternal_x[i] && eternal_y[c] == eternal_y[i] - 1)
                                        {
                                            potato = true;
                                            break;
                                        }
                                    }
                                    if (!potato)
                                    {
                                        eternal_size++;
                                        eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                        eternal_x[eternal_size - 1] = eternal_x[i];
                                        eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                        eternal_y[eternal_size - 1] = eternal_y[i] - 1;
                                    }
                                }
                                else if (deskgr[eternal_x[i]][eternal_y[i] - 1] == 32)
                                {
                                    deskgr[eternal_x[i]][eternal_y[i] - 1] = opencl(bnum[eternal_x[i]][eternal_y[i] - 1]);
                                    amount++;
                                }
                            }
                            if (type != 1)
                            {
                                if (eternal_y[i] < type - 1)
                                {
                                    if (bnum[eternal_x[i]][eternal_y[i] + 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] && eternal_y[c] == eternal_y[i] + 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i];
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] + 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i]][eternal_y[i] + 1] == 32)
                                    {
                                        deskgr[eternal_x[i]][eternal_y[i] + 1] = opencl(bnum[eternal_x[i]][eternal_y[i] + 1]);
                                        amount++;
                                    }
                                }
                            }
                            else
                            {
                                if (eternal_y[i] < y - 1)
                                {
                                    if (bnum[eternal_x[i]][eternal_y[i] + 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] && eternal_y[c] == eternal_y[i] + 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i];
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] + 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i]][eternal_y[i] + 1] == 32)
                                    {
                                        deskgr[eternal_x[i]][eternal_y[i] + 1] = opencl(bnum[eternal_x[i]][eternal_y[i] + 1]);
                                        amount++;
                                    }
                                }
                            }
                            if (eternal_x[i] > 0 && eternal_y[i] > 0)
                            {
                                if (bnum[eternal_x[i] - 1][eternal_y[i] - 1] == 0)
                                {
                                    bool potato = false;
                                    for (unsigned short c = 0; c < eternal_size; c++)
                                    {
                                        if (eternal_x[c] == eternal_x[i] - 1 && eternal_y[c] == eternal_y[i] - 1)
                                        {
                                            potato = true;
                                            break;
                                        }
                                    }
                                    if (!potato)
                                    {
                                        eternal_size++;
                                        eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                        eternal_x[eternal_size - 1] = eternal_x[i] - 1;
                                        eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                        eternal_y[eternal_size - 1] = eternal_y[i] - 1;
                                    }
                                }
                                else if (deskgr[eternal_x[i] - 1][eternal_y[i] - 1] == 32)
                                {
                                    deskgr[eternal_x[i] - 1][eternal_y[i] - 1] = opencl(bnum[eternal_x[i] - 1][eternal_y[i] - 1]);
                                    amount++;
                                }
                            }
                            if (type != 1)
                            {
                                if (eternal_x[i] < type - 1 && eternal_y[i] > 0)
                                {
                                    if (bnum[eternal_x[i] + 1][eternal_y[i] - 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] + 1 && eternal_y[c] == eternal_y[i] - 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }

                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] + 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] - 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] + 1][eternal_y[i] - 1] == 32)
                                    {
                                        deskgr[eternal_x[i] + 1][eternal_y[i] - 1] = opencl(bnum[eternal_x[i] + 1][eternal_y[i] - 1]);
                                        amount++;
                                    }
                                }
                            }
                            else
                            {
                                if (eternal_x[i] < x - 1 && eternal_y[i] > 0)
                                {
                                    if (bnum[eternal_x[i] + 1][eternal_y[i] - 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] + 1 && eternal_y[c] == eternal_y[i] - 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] + 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] - 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] + 1][eternal_y[i] - 1] == 32)
                                    {
                                        deskgr[eternal_x[i] + 1][eternal_y[i] - 1] = opencl(bnum[eternal_x[i] + 1][eternal_y[i] - 1]);
                                        amount++;
                                    }
                                }
                            }
                            if (type != 1)
                            {
                                if (eternal_x[i] > 0 && eternal_y[i] < type - 1)
                                {
                                    if (bnum[eternal_x[i] - 1][eternal_y[i] + 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] - 1 && eternal_y[c] == eternal_y[i] + 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] - 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] + 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] - 1][eternal_y[i] + 1] == 32)
                                    {
                                        deskgr[eternal_x[i] - 1][eternal_y[i] + 1] = opencl(bnum[eternal_x[i] - 1][eternal_y[i] + 1]);
                                        amount++;
                                    }
                                }
                            }
                            else
                            {
                                if (eternal_x[i] > 0 && eternal_y[i] < y - 1)
                                {
                                    if (bnum[eternal_x[i] - 1][eternal_y[i] + 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] - 1 && eternal_y[c] == eternal_y[i] + 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] - 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] + 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] - 1][eternal_y[i] + 1] == 32)
                                    {
                                        deskgr[eternal_x[i] - 1][eternal_y[i] + 1] = opencl(bnum[eternal_x[i] - 1][eternal_y[i] + 1]);
                                        amount++;
                                    }
                                }
                            }
                            if (type != 1)
                            {
                                if (eternal_x[i] < type - 1 && eternal_y[i] < type - 1)
                                {
                                    if (bnum[eternal_x[i] + 1][eternal_y[i] + 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] + 1 && eternal_y[c] == eternal_y[i] + 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] + 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] + 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] + 1][eternal_y[i] + 1] == 32)
                                    {
                                        deskgr[eternal_x[i] + 1][eternal_y[i] + 1] = opencl(bnum[eternal_x[i] + 1][eternal_y[i] + 1]);
                                        amount++;
                                    }
                                }
                            }
                            else
                            {
                                if (eternal_x[i] < x - 1 && eternal_y[i] < y - 1)
                                {
                                    if (bnum[eternal_x[i] + 1][eternal_y[i] + 1] == 0)
                                    {
                                        bool potato = false;
                                        for (unsigned short c = 0; c < eternal_size; c++)
                                        {
                                            if (eternal_x[c] == eternal_x[i] + 1 && eternal_y[c] == eternal_y[i] + 1)
                                            {
                                                potato = true;
                                                break;
                                            }
                                        }
                                        if (!potato)
                                        {
                                            eternal_size++;
                                            eternal_x = (unsigned short*)realloc(eternal_x, eternal_size * sizeof(unsigned short));
                                            eternal_x[eternal_size - 1] = eternal_x[i] + 1;
                                            eternal_y = (unsigned short*)realloc(eternal_y, eternal_size * sizeof(unsigned short));
                                            eternal_y[eternal_size - 1] = eternal_y[i] + 1;
                                        }
                                    }
                                    else if (deskgr[eternal_x[i] + 1][eternal_y[i] + 1] == 32)
                                    {
                                        deskgr[eternal_x[i] + 1][eternal_y[i] + 1] = opencl(bnum[eternal_x[i] + 1][eternal_y[i] + 1]);
                                        amount++;
                                    }
                                }
                            }
                        }
                        free(eternal_x);
                        free(eternal_y);
                    }
                    else
                    {
                        deskgr[xh - 1][yh - 1] = opencl(bnum[xh - 1][yh - 1]);
                        amount++;
                    }
                    switch (type)
                    {
                        case 8:
                            printdesk(deskgr, 8, 8);
                            break;
                        case 16:
                            printdesk(deskgr, 16, 16);
                            break;
                        case 32:
                            printdesk(deskgr, 32, 32);
                            break;
                        default:
                            printdesk(deskgr, x, y);
                            break;
                    }
                    space -= amount;
                }
                break;
        }
    }
    while (!lose && space != 0);
    if (!lose)
    {
        printf("ВЫ ВЫЖИЛИ!\n");
        victory = true;
    }
    time_t finish = time(NULL);
    printf("Ваше время: %.0lf секунд\n", difftime(finish, start));
    free(position);
    for (unsigned short i = 0; i < type; i++)
        free(deskgr[i]);
    free(deskgr);
    for (unsigned short i = 0; i < type; i++)
        free(bnum[i]);
    free(bnum);
}
int main()
{
    setlocale(LC_ALL, "Russian");
    unsigned short list_size = 0;
    struct result* list = (struct result*)malloc(list_size * sizeof(struct result));
    FILE* fin = fopen("results.bin", "rbe");
    if (fin == NULL)
    {
        FILE* create = fopen("results.bin", "wbe");
        fclose(create);
    }
    else
    {
        while (!feof(fin))
        {
            struct result guy;
            unsigned short name_sz = 0, games = 0, vic = 0, iz = 0, hrd = 0, slayer = 0;
            double pr = 0.0;
            char x = 0;
            fread(&name_sz, sizeof(unsigned short), 1, fin);
            if (name_sz != 0)
            {
                char* namez = (char*)malloc((name_sz + 1) * sizeof(char));
                for (unsigned short i = 0; i < name_sz; i++)
                {
                    fread(&x, sizeof(char), 1, fin);
                    namez[i] = x;
                }
                namez[name_sz] = '\0';
                fread(&games, sizeof(unsigned short), 1, fin);
                fread(&vic, sizeof(unsigned short), 1, fin);
                fread(&pr, sizeof(double), 1, fin);
                fread(&iz, sizeof(unsigned short), 1, fin);
                fread(&hrd, sizeof(unsigned short), 1, fin);
                fread(&slayer, sizeof(unsigned short), 1, fin);
                guy.name = namez;
                guy.games = games;
                guy.victories = vic;
                guy.percent = pr;
                guy.izzy = iz;
                guy.hard = hrd;
                guy.doom_guy = slayer; /*ОСТОРОЖНО, СПОЙЛЕР К DOOM ETERNAL :)*/
                list_size++;
                list = (struct result*)realloc(list, list_size * sizeof(struct result));
                list[list_size - 1] = guy;
            }
            else
                break;
        }
        fclose(fin);
    }
    unsigned short choice = 0, gm_ch = 0, res_ch = 0;
    char* CHOICE = (char*)malloc(0);
    char* GAME_CH = (char*)malloc(0);
    char* RES_CH = (char*)malloc(0);
    char* away = (char*)malloc(0);
    char* resaw = (char*)malloc(0);
    bool g8sch = false;
    bool g16sch = false;
    bool g32sch = false;
    do
    {
        g8sch = false;
        g16sch = false;
        g32sch = false;
        do
        {
            printf("ххххх_САПЁР_ххххх\n");
            printf("1.Играть\n2.Рекорды\n3.Выход\n");
            printf("Введите ЦИФРУ, чтобы выполнить действие: ");
            scanf_rng("%2s", CHOICE);
            if (*CHOICE != 49 && *CHOICE != 50 && *CHOICE != 51 || CHOICE[1] != 0)
                printf("Ввод не корректен\n");
        }
        while (*CHOICE != 49 && *CHOICE != 50 && *CHOICE != 51 || CHOICE[1] != 0);
        switch (*CHOICE)
        {
            case 49:
                choice = 1;
                break;
            case 50:
                choice = 2;
                break;
            default:
                choice = 3;
                break;
        }
        struct result player;
        switch (choice)
        {
            case 1:
                printf("Введите ваше имя (максимум 15 символов): ");
                char* name = (char*)malloc(15 * sizeof(char));
                scanf_rng("%15s", name);
                player.name = name;
                do
                {
                    printf("Выберите уровень сложности: 1.Просто(доска 8x8, 16 мин); 2.Не так просто(доска 16x16, 32 мины); 3.ХАРДКОР(доска 32x32, 64 мины); 4.Своя игра: ");
                    scanf_rng("%2s", GAME_CH);
                    if (*GAME_CH != 49 && *GAME_CH != 50 && *GAME_CH != 51 && *GAME_CH != 52 || GAME_CH[1] != 0)
                        printf("Ввод не корректен\n");
                }
                while (*GAME_CH != 49 && *GAME_CH != 50 && *GAME_CH != 51 && *GAME_CH != 52 || GAME_CH[1] != 0);
                switch (*GAME_CH)
                {
                    case 49:
                        gm_ch = 1;
                        break;
                    case 50:
                        gm_ch = 2;
                        break;
                    case 51:
                        gm_ch = 3;
                        break;
                    default:
                        gm_ch = 4;
                        break;
                }
                switch (gm_ch)
                {
                    case 1:
                        for (unsigned short e = 0; e < list_size; e++)
                        {
                            if (strcmp(list[e].name, player.name) == 0)
                            {
                                g8sch = true;
                                game(list, list[e], 8);
                                if (victory)
                                {
                                    list[e].victories++;
                                    list[e].izzy++;
                                    victory = false;
                                }
                                list[e].games++;
                                list[e].percent = (list[e].victories / (list[e].games / 100.0));
                                break;
                            }
                        }
                        if (!g8sch)
                        {
                            player.victories = 0;
                            player.izzy = 0;
                            player.hard = 0;
                            player.doom_guy = 0;
                            game(list, player, 8);
                            if (victory)
                            {
                                player.victories = 1;
                                player.izzy = 1;
                                victory = false;
                            }
                            player.games = 1;
                            player.percent = (player.victories / (player.games / 100.0));
                            list_size++;
                            list = (struct result*)realloc(list, list_size * sizeof(struct result));
                            list[list_size - 1] = player;
                        }
                        save(list, list_size);
                        break;
                    case 2:
                        for (unsigned short e = 0; e < list_size; e++)
                        {
                            if (strcmp(list[e].name, player.name) == 0)
                            {
                                g16sch = true;
                                game(list, list[e], 16);
                                if (victory)
                                {
                                    list[e].victories++;
                                    list[e].hard++;
                                    victory = false;
                                }
                                list[e].games++;
                                list[e].percent = (list[e].victories / (list[e].games / 100.0));
                                break;
                            }
                        }
                        if (!g16sch)
                        {
                            player.victories = 0;
                            player.izzy = 0;
                            player.hard = 0;
                            player.doom_guy = 0;
                            game(list, player, 16);
                            if (victory)
                            {
                                player.victories = 1;
                                player.hard = 1;
                                victory = false;
                            }
                            player.games = 1;
                            player.percent = (player.victories / (player.games / 100.0));
                            list_size++;
                            list = (struct result*)realloc(list, list_size * sizeof(struct result));
                            list[list_size - 1] = player;
                        }
                        save(list, list_size);
                        break;
                    case 3:
                        for (unsigned short e = 0; e < list_size; e++)
                        {
                            if (strcmp(list[e].name, player.name) == 0)
                            {
                                g32sch = true;
                                game(list, list[e], 32);
                                if (victory)
                                {
                                    list[e].victories++;
                                    list[e].doom_guy++;
                                    victory = false;
                                }
                                list[e].games++;
                                list[e].percent = (list[e].victories / (list[e].games / 100.0));
                                break;
                            }
                        }
                        if (!g32sch)
                        {
                            player.victories = 0;
                            player.izzy = 0;
                            player.hard = 0;
                            player.doom_guy = 0;
                            game(list, player, 32);
                            if (victory)
                            {
                                player.victories++;
                                player.doom_guy++;
                                victory = false;
                            }
                            player.games = 1;
                            player.percent = (player.victories / (player.games / 100.0));
                            list_size++;
                            list = (struct result*)realloc(list, list_size * sizeof(struct result));
                            list[list_size - 1] = player;
                        }
                        save(list, list_size);
                        break;
                    default:
                        for (unsigned short e = 0; e < list_size; e++)
                        {
                            if (strcmp(list[e].name, player.name) == 0)
                            {
                                g8sch = true;
                                game(list, list[e], 1);
                                if (victory)
                                {
                                    list[e].victories++;
                                    victory = false;
                                }
                                list[e].games++;
                                list[e].percent = (list[e].victories / (list[e].games / 100.0));
                                break;
                            }
                        }
                        if (!g8sch)
                        {
                            player.victories = 0;
                            player.izzy = 0;
                            player.hard = 0;
                            player.doom_guy = 0;
                            game(list, player, 1);
                            if (victory)
                            {
                                player.victories = 1;
                                victory = false;
                            }
                            player.games = 1;
                            player.percent = (player.victories / (player.games / 100.0));
                            list_size++;
                            list = (struct result*)realloc(list, list_size * sizeof(struct result));
                            list[list_size - 1] = player;
                        }
                        save(list, list_size);
                        break;
                }
                break;
            case 2:
                do
                {
                    printf("Результаты\n");
                    do
                    {
                        printf(
                                "1.Сыграно игр\n2.Побед на сложности 'легко'\n3.Побед на сложности 'не так легко'\n4.Побед на сложности 'ХАРДКОР'\n5.Процент побед.\n6.Выход\n");
                        printf("Выберите ЦИФРУ действия: ");
                        scanf_rng("%2s", RES_CH);
                        if (*RES_CH != 49 && *RES_CH != 50 && *RES_CH != 51 && *RES_CH != 52 && *RES_CH != 53 && *RES_CH !=54 || RES_CH[1] != 0)
                            printf("Ввод не корректен\n");
                    }
                    while (*RES_CH != 49 && *RES_CH != 50 && *RES_CH != 51 && *RES_CH != 52 && *RES_CH != 53 && *RES_CH != 54 || RES_CH[1] != 0);
                    switch (*RES_CH)
                    {
                        case 49:
                            res_ch = 1;
                            break;
                        case 50:
                            res_ch = 2;
                            break;
                        case 51:
                            res_ch = 3;
                            break;
                        case 52:
                            res_ch = 4;
                            break;
                        case 53:
                            res_ch = 5;
                            break;
                        default:
                            res_ch = 6;
                            break;
                    }
                    switch (res_ch)
                    {
                        case 1:
                            if (list_size == 0)
                                printf("Ещё никто не играл...\n");
                            else
                            {
                                qsort(list, list_size, sizeof(*list), compare_games);
                                for (unsigned short i = list_size; i > 0; i--)
                                    printf("%s : %hu\n", list[i - 1].name, list[i - 1].games);
                            }
                            break;
                        case 2:
                            if (list_size == 0)
                                printf("Ещё никто не играл...\n");
                            else
                            {
                                qsort(list, list_size, sizeof(*list), compare_izzy);
                                for (unsigned short i = list_size; i > 0; i--)
                                    printf("%s : %hu\n", list[i - 1].name, list[i - 1].izzy);
                            }
                            break;
                        case 3:
                            if (list_size == 0)
                                printf("Ещё никто не играл...\n");
                            else
                            {
                                qsort(list, list_size, sizeof(*list), compare_hard);
                                for (unsigned short i = list_size; i > 0; i--)
                                    printf("%s : %hu\n", list[i - 1].name, list[i - 1].hard);
                            }
                            break;
                        case 4:
                            if (list_size == 0)
                                printf("Ещё никто не играл...\n");
                            else
                            {
                                qsort(list, list_size, sizeof(*list), compare_doom);
                                for (unsigned short i = list_size; i > 0; i--)
                                    printf("%s : %hu\n", list[i - 1].name, list[i - 1].doom_guy);
                            }
                            break;
                        case 5:
                            if (list_size == 0)
                                printf("Ещё никто не играл...\n");
                            else
                            {
                                qsort(list, list_size, sizeof(*list), compare_percent);
                                for (unsigned short i = list_size; i > 0; i--)
                                {
                                    int perc = list[i - 1].percent;
                                    printf("%s : %d%%\n", list[i - 1].name, perc);
                                }
                            }
                            break;
                        default:
                            printf("Вы точно хотите выйти? Введите YES, если да или что-то другое, если нет.\n");
                            scanf_rng("%4s", resaw);
                            if (strcmp(resaw, "YES") != 0)
                                res_ch = 0;
                            break;
                    }
                }
                while (res_ch != 6);
                break;
            default:
                printf("Вы точно хотите выйти? Введите YES, если да или что-то другое, если нет.\n");
                scanf_rng("%4s", away);
                if (strcmp(away, "YES") != 0)
                    choice = 0;
                break;
        }
    }
    while (choice != 3);
    return 0;
}