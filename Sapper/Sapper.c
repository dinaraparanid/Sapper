#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

/** Player struct */

typedef struct
{
    char* name;
    uint16_t games;
    uint16_t victories;
    double percent;
    uint16_t easy;
    uint16_t hard;
    uint16_t doom_guy;
} player;

/** Generic pair struct */

typedef struct
{
    void* first;
    void* second;
} pair;

/**
 * Gets first param from pair
 * @param PAIR pair itself
 * @param FIELD field (first or second)
 * @param TYPE type of first
 * @return param of field
 */

#define GET_FROM_PAIR(PAIR, FIELD, TYPE) (*(TYPE*)(PAIR).FIELD)

 /**
  * Sets values to pair
  * @param PAIR pair itself
  * @param FIRST first value
  * @param SECOND second value
  * @param TYPE1 type of first
  * @param TYPE2 type of second
  */

#define SET_TO_PAIR(PAIR, FIRST, SECOND, TYPE1, TYPE2) \
	*(TYPE1*)(PAIR).first = (FIRST); \
	*(TYPE2*)(PAIR).second = (SECOND); 

  /**
   * scanf()-like function to parse strings.
   * Protects from buffer overflow.
   * @param format format of parseable string
   * @see scanf() function for more details
   */

inline int scanf_rng(const char* const format, ...)
{
    va_list arg_list;
    __crt_va_start(arg_list, format);
    const int player = _vfscanf_l(stdin, format, NULL, arg_list);
    __crt_va_end(arg_list);

    scanf("%*[^\n]");
    _fflush_nolock(stdin);
    return player;
}

// macro for comparing by field

#define COMPARE_BY(FIELD) ((const player*)a)->FIELD - ((const player*)b)->FIELD

/** compare by games amount */

int compare_games(const void* a, const void* b)
{
    return COMPARE_BY(games);
}

/** compare by easy level victories amount */

int compare_easy(const void* a, const void* b)
{
    return COMPARE_BY(easy);
}

/** compare by hard level victories amount */

int compare_hard(const void* a, const void* b)
{
    return COMPARE_BY(hard);
}

/** compare by doom guy level victories amount */

int compare_doom(const void* a, const void* b)
{
    return COMPARE_BY(doom_guy);
}

/** compare by percent of victories amount */

int compare_percent(const void* a, const void* b)
{
    return COMPARE_BY(percent);
}

/**
 * Safe to file.
 * @param save array of players to save
 * @param size size of array
 */

inline void save(const player* save, const uint16_t size)
{
    FILE* fout = fopen("players.bin", "wb");

    for (const player* p = save; p != save + size; p++)
    {
        const uint16_t sz = strlen(p->name);

        fwrite(&sz, sizeof(uint16_t), 1, fout);
        fwrite(p->name, sizeof(char), sz, fout);
        fwrite(&p->games, sizeof(uint16_t), 1, fout);
        fwrite(&p->victories, sizeof(uint16_t), 1, fout);
        fwrite(&p->percent, sizeof(double), 1, fout);
        fwrite(&p->easy, sizeof(uint16_t), 1, fout);
        fwrite(&p->hard, sizeof(uint16_t), 1, fout);
        fwrite(&p->doom_guy, sizeof(uint16_t), 1, fout);
    }

    fclose(fout);
}

/**
 * Print table.
 * @param desk_gr table to print
 * @param x amount of rows
 * @param y amount of columns
 */

inline void print_desk(const char** desk_gr, const uint16_t x, const uint16_t y)
{
    putchar(' '), putchar(' ');

    for (uint16_t i = 1; i < y + 1; i++)
        printf(" %d", i % 10);

    putchar('\n');

    for (uint16_t i = 0; i < x; i++)
    {
        putchar(' '), putchar(' ');

        for (uint16_t q = 1; q < y + 1; q++)
            putchar(' '), putchar('_');

        putchar('\n');
        printf("%d ", (i + 1) % 10);

        for (uint16_t q = 0; q < y; q++)
            printf("|%c", desk_gr[i][q]);

        putchar('|'), putchar('\n');
    }

    putchar(' '), putchar(' ');

    for (uint16_t q = 1; q < y + 1; q++)
        putchar(' '), putchar('_');

    putchar('\n');
}

/**
 * Converts mine amount to char
 * @param num number to convert (1 - 8)
 * @return char
 */

inline char open_cl(const uint16_t num)
{
    if (num > 8)
    {
        fputs("open_cl(): Incorrect number to parse", stderr);
        exit(-1);
    }

    return '0' + num;
}

/**
 * Parse str to correct int16_t (that I need)
 * @param str string to parse
 * @return correct number or -1
 */

inline int16_t to_hu(const char* str)
{
    uint16_t x = 0;

    if (str[1] > '0' && str[1] <= '9')
    {
        if (*str >= '0' && *str <= '3')
            x += (*str - '0') * 10;

        else
        {
            puts("Input is incorrect.");
            return -1;
        }

        if (str[1] >= '0' && str[1] <= '9')
            x += str[1] - '0';

        else
        {
            puts("Input is incorrect.");
            return -1;
        }


        if (str[2] != 0)
        {
            puts("Input is incorrect.");
            return -1;
        }
    }

    else if (!str[1])
    {
        if (*str >= '0' && *str <= '9')
            x += *str - '0';

        else
        {
            puts("Input is incorrect.");
            return -1;
        }
    }

    else
    {
        puts("Input is incorrect.");
        return -1;
    }

    return x;
}

/**
 * Update cell amount when constructing table.
 * @param bomb_num table with cell, that shows amount of bombs nearby
 * @param xc row coordinate
 * @param yc column coordinate
 * @param x row amount
 * @param y column amount
 */

inline void cell_count(
    uint16_t** bomb_num,
    const uint16_t xc,
    const uint16_t yc,
    const uint16_t x,
    const uint16_t y
)
{
    if (xc > 0)                     bomb_num[xc - 1][yc]++;
    if (xc < x - 1)                 bomb_num[xc + 1][yc]++;
    if (yc > 0)                     bomb_num[xc][yc - 1]++;
    if (yc < y - 1)                 bomb_num[xc][yc + 1]++;
    if (xc > 0 && yc > 0)           bomb_num[xc - 1][yc - 1]++;
    if (xc < x - 1 && yc > 0)       bomb_num[xc + 1][yc - 1]++;
    if (xc > 0 && yc < y - 1)       bomb_num[xc - 1][yc + 1]++;
    if (xc < x - 1 && yc < y - 1)   bomb_num[xc + 1][yc + 1]++;
}

/**
 * Open cells in table when type of game isn't needed.
 * @param ptr pointer on current cell 0 coordinates
 * @param zeroes array of cell 0 coordinates
 * @param zeroes_size size of array with cells 0
 * @param bomb_num table with cell, that shows amount of bombs nearby
 * @param desk_gr printable table
 * @param amount amount of opened cells
 * @param condition_x condition for row of current position
 * @param condition_y condition for column of current position
 */

inline void open_cell_easy(
    const pair* ptr,
    pair* zeroes,
    uint16_t* zeroes_size,
    uint16_t** bomb_num,
    char** desk_gr,
    uint16_t* amount,
    const int condition_x,
    const int condition_y
)
{
    if (GET_FROM_PAIR(*ptr, first, uint16_t) > 0)
    {
        if (bomb_num[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] == 0)
        {
            int potato = 0;

            for (pair* ptr2 = zeroes; ptr2 != zeroes + *zeroes_size; ptr2++)
            {
                if (GET_FROM_PAIR(*ptr2, first, uint16_t) == GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x &&
                    GET_FROM_PAIR(*ptr2, second, uint16_t) == GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y)
                {
                    potato = 1;
                    break;
                }
            }

            if (!potato)
            {
                zeroes = realloc(zeroes, ++ * zeroes_size * sizeof(pair));

                SET_TO_PAIR(
                    zeroes[*zeroes_size - 1],
                    GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x,
                    GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y,
                    uint16_t, uint16_t
                );
            }
        }

        else if (desk_gr[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] == 32)
        {
            desk_gr[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] =
                open_cl(bomb_num[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y]);
            (*amount)++;
        }
    }
}

/**
 * Open cells in table when type of game must known.
 * @param ptr pointer on current cell 0 coordinates
 * @param zeroes array of cell 0 coordinates
 * @param zeroes_size size of array with cells 0
 * @param bomb_num table with cell, that shows amount of bombs nearby
 * @param desk_gr printable table
 * @param amount amount of opened cells
 * @param border_x border for row
 * @param border_y border for column
 * @param condition_x condition for row of current position
 * @param condition_y condition for column of current position
 * @param type type of game
 */

inline void open_cell_hard(
    const pair* ptr,
    pair* zeroes,
    uint16_t* zeroes_size,
    uint16_t** bomb_num,
    char** desk_gr,
    uint16_t* amount,
    const int border_x,
    const int border_y,
    const int condition_x,
    const int condition_y,
    const uint16_t type
)
{
    if (type != 1)
    {
        if (GET_FROM_PAIR(*ptr, first, uint16_t) < type - 1)
        {
            if (bomb_num[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] == 0)
            {
                int potato = 0;

                for (pair* ptr2 = zeroes; ptr2 != zeroes + *zeroes_size; ptr2++)
                {
                    if (GET_FROM_PAIR(*ptr2, first, uint16_t) == GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x &&
                        GET_FROM_PAIR(*ptr2, second, uint16_t) == GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y)
                    {
                        potato = 1;
                        break;
                    }
                }

                if (!potato)
                {
                    zeroes_size++;
                    zeroes = realloc(zeroes, ++ * zeroes_size * sizeof(pair));

                    SET_TO_PAIR(
                        zeroes[*zeroes_size - 1],
                        GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x,
                        GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y,
                        uint16_t, uint16_t
                    );
                }
            }

            else if (desk_gr[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] == 32)
            {
                desk_gr[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] =
                    open_cl(bomb_num[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y]);
                (*amount)++;
            }
        }
    }

    else
    {
        if (border_x != 0 ? GET_FROM_PAIR(*ptr, first, uint16_t) < border_x - 1 : GET_FROM_PAIR(*ptr, first, uint16_t) > 0 &&
            border_x != 0 ? GET_FROM_PAIR(*ptr, second, uint16_t) < border_y - 1 : GET_FROM_PAIR(*ptr, second, uint16_t) > 0)
        {
            if (bomb_num[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] == 0)
            {
                int potato = 0;

                for (pair* ptr2 = zeroes; ptr2 != zeroes + *zeroes_size; ptr2++)
                {
                    if (GET_FROM_PAIR(*ptr2, first, uint16_t) == GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x &&
                        GET_FROM_PAIR(*ptr2, second, uint16_t) == GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y)
                    {
                        potato = 1;
                        break;
                    }
                }

                if (!potato)
                {
                    zeroes_size++;
                    zeroes = realloc(zeroes, *zeroes_size * sizeof(pair));

                    SET_TO_PAIR(
                        zeroes[*zeroes_size - 1],
                        GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x,
                        GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y,
                        uint16_t, uint16_t
                    );
                }
            }

            else if (desk_gr[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] == 32)
            {
                desk_gr[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y] =
                    open_cl(bomb_num[GET_FROM_PAIR(*ptr, first, uint16_t) + condition_x][GET_FROM_PAIR(*ptr, second, uint16_t) + condition_y]);
                (*amount)++;
            }
        }
    }
}

/**
 * Game itself
 * @param p array of players
 * @param player player
 * @param type type of game
 * @return 1 if player is won else 0
 */

int game(player* p, player player, const uint16_t type)
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t mines = 0;

    char** desk_gr = NULL;      // desk itself
    uint16_t** bomb_num = NULL; // amount of bombs nearby for each cell
    uint16_t count = 0;         // amount of lightened bombs
    uint16_t* position = NULL;  // mines number

    if (type != 1)
    {
        desk_gr = malloc(type * sizeof(char*));

        for (char** ptr = desk_gr; ptr != desk_gr + type; ptr++)
            *ptr = malloc(type * sizeof(char));

        for (char** ptr_i = desk_gr; ptr_i != desk_gr + type; ptr_i++)
            for (char* ptr_q = *ptr_i; ptr_q != *ptr_i + type; ptr_q++)
                *ptr_q = ' ';

        bomb_num = malloc(type * sizeof(uint16_t*));

        for (uint16_t** ptr = bomb_num; ptr != bomb_num + type; ptr++)
            *ptr = calloc(type, sizeof(uint16_t));

        print_desk(desk_gr, type, type);
        srand(time(NULL));

        count = 0;
        position = malloc(type * 2 * sizeof(uint16_t));

        // place mines on table

        while (count != type * 2)
        {
            // check if new mine position is correct

            int test = 0;
            uint16_t place = rand() % (type * type); // mine position

            for (uint16_t* ptr = position; ptr != position + count; ptr++)
            {
                if (place == *ptr)
                {
                    test = 1;
                    break;
                }
            }

            if (!test) position[++count - 1] = place;
        }

        uint16_t xc = 0;
        uint16_t yc = 0;

        // Fill bomb_num table

        for (uint16_t* ptr = position; ptr != position + count; ptr++)
        {
            xc = *ptr / type;       // row with mine
            yc = *ptr - type * xc;  // column with mine
            cell_count(bomb_num, xc, yc, type, type);
        }
    }

    else
    {
        char x_str[10];
        char y_str[10];
        puts("Set table size.");

        do
        {
            printf("Amount of rows (< 51, > 2): ");
            scanf_rng("%3s", x_str);
            sscanf(x_str, "%hu", x);

            if (x < 3 || x > 50)
                puts("Input is incorrect.");
        }     	while (x < 3 || x > 50);

        do
        {
            printf("Amount of columns (< 51, > 2): ");
            scanf_rng("%3s", y_str);
            sscanf(y_str, "%hu", y);

            if (y < 3 || y > 50)
                puts("Input is incorrect.");
        }     	while (y < 3 || y > 50);

        desk_gr = malloc(type * sizeof(char*));

        for (char** ptr = desk_gr; ptr != desk_gr + type; ptr++)
            *ptr = malloc(type * sizeof(char));

        for (char** ptr_i = desk_gr; ptr_i != desk_gr + type; ptr_i++)
            for (char* ptr_q = *ptr_i; ptr_q != *ptr_i + type; ptr_q++)
                *ptr_q = ' ';

        bomb_num = malloc(type * sizeof(uint16_t*));

        for (uint16_t** ptr = bomb_num; ptr != bomb_num + type; ptr++)
            *ptr = calloc(type, sizeof(uint16_t));

        print_desk(desk_gr, x, y);

        do
        {
            mines = 0;
            char mines_str[10];

            printf("Set amount of mines (>= 5%% of table's surface, <= 95%% of table's surface): ");
            scanf_rng("%4s", mines_str);
            sscanf(mines_str, "%hu", mines);

            if (mines < x * y / 100.0 * 5 || mines > x * y / 100.0 * 95)
                puts("Input is incorrect.");
        }     	while (mines < x * y / 100.0 * 5 || mines > x * y / 100.0 * 95);

        srand(time(NULL));

        count = 0;
        position = malloc(mines * sizeof(uint16_t));

        // place mines on table

        while (count != mines)
        {
            // check if new mine position is correct

            int test = 0;
            uint16_t place = rand() % x * y; // mine position

            for (uint16_t* ptr = position; ptr != position + count; ptr++)
            {
                if (place == *ptr)
                {
                    test = 0;
                    break;
                }
            }

            if (!test)
            {
                count++;
                position[count - 1] = place;
            }
        }

        uint16_t xc = 0;
        uint16_t yc = 0;

        for (uint16_t* ptr = position; ptr != position + count; ptr++)
        {
            xc = *ptr / y;      // row with mine
            yc = *ptr - y * xc; // column with mine
            cell_count(bomb_num, xc, yc, x, y);
        }
    }

    uint16_t xh = 0;
    uint16_t yh = 0;
    uint16_t mv = 0;
    uint16_t space = type != 1 ? type * type - type * 2 : x * y - mines; // amount of cell that free of mines

    char mv_str[100];
    char yesf_str[100];
    char yesd_str[100];
    char yesm_str[100];
    char xm_str[100];
    char ym_str[100];
    char xd_str[100];
    char yd_str[100];
    char xh_str[100];
    char yh_str[100];
    char th_str[100];

    const time_t start = time(NULL);
    do
    {
        do
        {
            puts("1. Place mark on mine\n2. Remove mark from mine\n3. Make a move");
            printf("Choose number: ");
            scanf_rng("%2s", mv_str);

            if (*mv_str < '1' || *mv_str > '3' || mv_str[1] != 0)
                puts("Input is incorrect.");
        }     	while (*mv_str < '1' || *mv_str > '3' || mv_str[1] != 0);

        mv = *mv_str - '0';

        switch (mv)
        {
        case 1:
            do
            {
                printf("If you have a guess about the location of the mines, you can put a + on this place. ");
                printf("If you want to do this, then enter 'YES', otherwise something else:");
                scanf_rng("%4s", yesf_str);

                if (strcmp(yesf_str, "YES") == 0)
                {
                    uint16_t xm = 0;    // mine coordinate (row)
                    uint16_t ym = 0;    // mine coordinate (column)
                    short xt = 0;       // check if mark coordinate is correct (row)
                    short yt = 0;       // check if mark coordinate is correct (column)

                    puts("Enter mine coordinate.");

                    do
                    {
                        xm = 0;

                        printf("Enter row coordinate: ");
                        scanf_rng("%3s", xm_str);

                        xt = to_hu(xm_str);
                    }                 	while (xt == -1);

                    xm = xt;

                    do
                    {
                        ym = 0;

                        printf("Enter column coordinate: ");
                        scanf_rng("%3s", ym_str);

                        yt = to_hu(ym_str);
                    }                 	while (yt == -1);

                    ym = yt;

                    if (type != 1)
                    {
                        if ((xm > type || xm < 1) && (ym > type || ym < 1))
                        {
                            puts("You've gone beyond the board");
                            break;
                        }
                    }

                    else if ((xm > x || xm < 1) && (ym > y || ym < 1))
                    {
                        puts("You've gone beyond the board");
                        break;
                    }

                    if (desk_gr[xm - 1][ym - 1] != ' ')
                    {
                        puts("I doubt you want to put a mark here...");
                        break;
                    }

                    desk_gr[xm - 1][ym - 1] = '+';
                    print_desk(desk_gr, type == 1 ? x : type, type == 1 ? x : type);
                }
            }         	while (strcmp(yesf_str, "YES") == 0);
            break;

        case 2:
            do
            {
                printf("You can remove the mark on the mine. ");
                printf("If you want to do this, then enter 'YES', otherwise something else: ");
                scanf_rng("%4s", yesd_str);

                if (strcmp(yesd_str, "YES") == 0)
                {
                    uint16_t xm = 0;    // mine coordinate (row)
                    uint16_t ym = 0;    // mine coordinate (column)
                    short xt = 0;       // check if mark coordinate is correct (row)
                    short yt = 0;       // check if mark coordinate is correct (column)

                    puts("Enter mine coordinate:");

                    do
                    {
                        xm = 0;

                        printf("Enter row coordinate: ");
                        scanf_rng("%3s", xd_str);

                        xt = to_hu(xd_str);
                    }                 	while (xt == -1);

                    xm = xt;

                    do
                    {
                        ym = 0;

                        printf("Enter column coordinate: ");
                        scanf_rng("%3s", yd_str);

                        yt = to_hu(yd_str);
                    }                 	while (yt == -1);

                    ym = yt;

                    if (type != 1)
                    {
                        if ((xm > type || xm < 1) && (ym > type || ym < 1))
                        {
                            puts("You've gone beyond the board");
                            break;
                        }
                    }

                    else if ((xm > x || xm < 1) && (ym > y || ym < 1))
                    {
                        puts("You've gone beyond the board");
                        break;
                    }

                    if (desk_gr[xm - 1][ym - 1] != 43)
                    {
                        puts("There is no mark...\n");
                        break;
                    }

                    desk_gr[xm - 1][ym - 1] = ' ';
                    print_desk(desk_gr, type == 1 ? x : type, type == 1 ? x : type);
                }
            }         	while (strcmp(yesd_str, "YES") == 0);
            break;

        default:
            puts("Are you sure you want to make a move? If yes, then enter 'YES', otherwise something else:");
            scanf_rng("%4s", yesm_str);

            if (strcmp(yesm_str, "YES") != 0)
                break;

            int16_t xt = 0; // row move coordinate check
            int16_t yt = 0; // column move coordinate check
            int beyond = 0; // check if move coordinate is beyond table borders

            do
            {
                puts("Enter mine coordinate:");

                do
                {
                    xh = 0;

                    printf("Enter row coordinate: ");
                    scanf_rng("%3s", xh_str);

                    xt = to_hu(xh_str);
                }             	while (xt == -1);

                xh = xt;

                do
                {
                    yh = 0;

                    printf("Enter column coordinate: ");
                    scanf_rng("%3s", yh_str);

                    yt = to_hu(yh_str);
                }             	while (yt == -1);

                yh = yt;
                beyond = 0;

                if (type != 1)
                {
                    if ((xh > type || xh < 1) && (yh > type || yh < 1))
                    {
                        puts("You've gone beyond the board");
                        beyond = 1;
                    }
                }

                else if ((xh > x || xh < 1) && (yh > y || yh < 1))
                {
                    puts("You've gone beyond the board");
                    beyond = 1;
                }
            }         	while (beyond);

            for (uint16_t* ptr = position; ptr != position + count; ptr++)
            {
                if (type != 1)
                {
                    if ((xh - 1) * type + yh - 1 == *ptr) // lose
                    {
                        for (uint16_t* ptr_dead = position; ptr_dead != position + count; ptr_dead++)
                            desk_gr[*ptr_dead / type][*ptr_dead - *ptr_dead / type * type] = 'x';

                        print_desk(desk_gr, type, type);
                        puts("You've blew up :(");
                        return 0;
                    }
                }

                else if ((xh - 1) * y + yh - 1 == *ptr) // lose
                {
                    for (uint16_t* ptr_dead = position; ptr_dead != position + count; ptr_dead++)
                        desk_gr[*ptr_dead / type][*ptr_dead - *ptr_dead / type * type] = 'x';

                    print_desk(desk_gr, x, y);
                    puts("You've blew up :(");
                    return 0;
                }
            }

            if (desk_gr[xh - 1][yh - 1] != 32) // already opened
                puts("This cell is already open");

            else if (desk_gr[xh - 1][yh - 1] == 32) // open cell
            {
                uint16_t amount = 0;

                if (bomb_num[xh - 1][yh - 1] == 0)
                {
                    uint16_t zeroes_size = 1;
                    pair* zeroes = malloc(1 * sizeof(pair));
                    SET_TO_PAIR(*zeroes, xh - 1, yh - 1, uint16_t, uint16_t);

                    for (pair* ptr = zeroes; ptr != zeroes + zeroes_size; ptr++)
                    {
                        desk_gr[GET_FROM_PAIR(*ptr, first, uint16_t)][GET_FROM_PAIR(*ptr, second, uint16_t)] = '0';
                        amount++;

                        open_cell_easy(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, -1, 0);
                        open_cell_hard(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, x, 0, 1, 0, type);
                        open_cell_easy(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, 0, -1);
                        open_cell_hard(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, y, 0, 0, 1, type);
                        open_cell_easy(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, -1, -1);
                        open_cell_hard(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, x, 0, 1, -1, type);
                        open_cell_hard(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, 0, y, -1, 1, type);
                        open_cell_hard(ptr, zeroes, &zeroes_size, bomb_num, desk_gr, &amount, x, y, 1, 1, type);
                    }

                    free(zeroes);
                }

                else
                {
                    desk_gr[xh - 1][yh - 1] = open_cl(bomb_num[xh - 1][yh - 1]);
                    amount++;
                }

                print_desk(desk_gr, type == 1 ? x : type, type == 1 ? y : type);
                space -= amount;
            }

            break;
        }
    } 	while (space != 0);

    puts("YOU'VE SURVIVED!");

    time_t finish = time(NULL);
    printf("Your time: %d seconds\n", (int)difftime(finish, start));

    free(position);

    for (char** ptr = desk_gr; ptr != desk_gr + type == 1 ? x : type; ptr++)
        free(*ptr);
    free(desk_gr);

    for (uint16_t** ptr = bomb_num; ptr != bomb_num + type == 1 ? x : type; ptr++)
        free(*ptr);
    free(bomb_num);

    return 1;
}

int main()
{
    uint16_t list_size = 0;
    player* list = malloc(1 * sizeof(player));

    FILE* fin = fopen("players.bin", "rb");

    if (fin == NULL)
    {
        FILE* create = fopen("players.bin", "wb");
        fclose(create);
    }

    else
    {
        while (!feof(fin))
        {
            player guy;
            uint16_t name_sz = 0;
            uint16_t games = 0;
            uint16_t vic = 0;
            uint16_t iz = 0;
            uint16_t hrd = 0;
            uint16_t slayer = 0;
            double pr = 0.0;

            fread(&name_sz, sizeof(uint16_t), 1, fin);

            if (name_sz != 0)
            {
                char* namez = malloc((name_sz + 1) * sizeof(char));

                for (char* ptr = namez; ptr != namez + name_sz; ptr++)
                {
                    char x = 0;
                    fread(&x, sizeof(char), 1, fin);
                    *ptr = x;
                }

                namez[name_sz] = '\0';

                fread(&games, sizeof(uint16_t), 1, fin);
                fread(&vic, sizeof(uint16_t), 1, fin);
                fread(&pr, sizeof(double), 1, fin);
                fread(&iz, sizeof(uint16_t), 1, fin);
                fread(&hrd, sizeof(uint16_t), 1, fin);
                fread(&slayer, sizeof(uint16_t), 1, fin);

                guy.name = namez;
                guy.games = games;
                guy.victories = vic;
                guy.percent = pr;
                guy.easy = iz;
                guy.hard = hrd;
                guy.doom_guy = slayer;

                list = realloc(list, ++list_size * sizeof(player));
                list[list_size - 1] = guy;
            }

            else break;
        }

        fclose(fin);
    }

    uint16_t choice = 0;
    uint16_t gm_ch = 0;
    uint16_t res_ch = 0;

    char choice_str[10];
    char game_ch_str[10];
    char res_ch_str[10];
    char away[10];
    char resaw[10];

    int g8sch = 0;
    int g16sch = 0;
    int g32sch = 0;

    do
    {
        g8sch = 0;
        g16sch = 0;
        g32sch = 0;

        do
        {
            puts("XXXXX_SAPPER_XXXXX");
            puts("1.Play\n2.Results\n3.Exit");
            printf("Enter a NUMBER to perform the action:");
            scanf_rng("%2s", choice_str);

            if (*choice_str < '1' || *choice_str > '3' || choice_str[1] != 0)
                puts("Incorrect input");
        }     	while (*choice_str < '1' || *choice_str > '3' || choice_str[1] != 0);

        choice = *choice_str - '0';
        player pl;

        switch (choice)
        {
        case 1:
            printf("Enter your name (maximum of 15 characters): ");
            char name[15];
            scanf_rng("%15s", name);
            pl.name = name;

            do
            {
                printf("Choose the difficulty level: 1. Simple (board 8x8, 16 min); ");
                printf("2.Not so simple(board 16x16, 32 min); 3. HARDCORE (board 32x32, 64 min); 4. Custom game: ");
                scanf_rng("%2s", game_ch_str);

                if (*game_ch_str < '1' || *game_ch_str > '4' || game_ch_str[1] != 0)
                    puts("Incorrect input");
            }         	while (*game_ch_str < '1' || *game_ch_str > '4' || game_ch_str[1] != 0);

            gm_ch = *game_ch_str - '0';

            switch (gm_ch)
            {
            case 1:
                for (player* ptr = list; ptr != list + list_size; ptr++)
                {
                    if (strcmp(ptr->name, pl.name) == 0)
                    {
                        g8sch = 1;

                        if (game(list, *ptr, 8))
                        {
                            ptr->victories++;
                            ptr->easy++;
                        }

                        ptr->games++;
                        ptr->percent = ptr->victories / (ptr->games / 100.0);
                        break;
                    }
                }

                if (!g8sch)
                {
                    pl.victories = 0;
                    pl.easy = 0;
                    pl.hard = 0;
                    pl.doom_guy = 0;

                    if (game(list, pl, 8))
                    {
                        pl.victories = 1;
                        pl.easy = 1;
                    }

                    pl.games = 1;
                    pl.percent = pl.victories / (pl.games / 100.0);

                    list = realloc(list, ++list_size * sizeof(player));
                    list[list_size - 1] = pl;
                }

                save(list, list_size);
                break;

            case 2:
                for (player* ptr = list; ptr != list + list_size; ptr++)
                {
                    if (strcmp(ptr->name, pl.name) == 0)
                    {
                        g16sch = 1;

                        if (game(list, *ptr, 16))
                        {
                            ptr->victories++;
                            ptr->hard++;
                        }
                        ptr->games++;
                        ptr->percent = ptr->victories / (ptr->games / 100.0);
                        break;
                    }
                }

                if (!g16sch)
                {
                    pl.victories = 0;
                    pl.easy = 0;
                    pl.hard = 0;
                    pl.doom_guy = 0;

                    if (game(list, pl, 16))
                    {
                        pl.victories = 1;
                        pl.hard = 1;
                    }

                    pl.games = 1;
                    pl.percent = pl.victories / (pl.games / 100.0);

                    list = realloc(list, ++list_size * sizeof(player));
                    list[list_size - 1] = pl;
                }

                save(list, list_size);
                break;

            case 3:
                for (player* ptr = list; ptr != list + list_size; ptr++)
                {
                    if (strcmp(ptr->name, pl.name) == 0)
                    {
                        g32sch = 1;

                        if (game(list, *ptr, 32))
                        {
                            ptr->victories++;
                            ptr->doom_guy++;
                        }

                        ptr->games++;
                        ptr->percent = (ptr->victories / (ptr->games / 100.0));
                        break;
                    }
                }

                if (!g32sch)
                {
                    pl.victories = 0;
                    pl.easy = 0;
                    pl.hard = 0;
                    pl.doom_guy = 0;

                    if (game(list, pl, 32))
                    {
                        pl.victories++;
                        pl.doom_guy++;
                    }

                    pl.games = 1;
                    pl.percent = pl.victories / (pl.games / 100.0);

                    list = realloc(list, ++list_size * sizeof(player));
                    list[list_size - 1] = pl;
                }

                save(list, list_size);
                break;

            default:
                for (player* ptr = list; ptr != list + list_size; ptr++)
                {
                    if (strcmp(ptr->name, pl.name) == 0)
                    {
                        g8sch = 1;
                        game(list, *ptr, 1);

                        if (game(list, *ptr, 1))
                            ptr->victories++;

                        ptr->games++;
                        ptr->percent = ptr->victories / (ptr->games / 100.0);
                        break;
                    }
                }

                if (!g8sch)
                {
                    pl.victories = 0;
                    pl.easy = 0;
                    pl.hard = 0;
                    pl.doom_guy = 0;

                    if (game(list, pl, 1))
                        pl.victories = 1;

                    pl.games = 1;
                    pl.percent = pl.victories / (pl.games / 100.0);

                    list = realloc(++list, list_size * sizeof(player));
                    list[list_size - 1] = pl;
                }

                save(list, list_size);
                break;
            }
            break;

        case 2:
            do
            {
                puts("Results");

                do
                {
                    puts(
                        "1.Games played\n2.Wins on the difficulty 'easy'\n3.Wins on difficulty 'not so easy'\n4.Wins on the 'HARDCORE' difficulty\n5.Winning percentage.\n6.Exit"
                    );
                    printf("Select the action NUMBER: ");
                    scanf_rng("%2s", res_ch_str);

                    if (*res_ch_str < '1' || *res_ch_str > '6' || res_ch_str[1] != 0)
                        puts("Input is incorrect");
                }             	while (*res_ch_str < '1' || *res_ch_str > '6' || res_ch_str[1] != 0);

                res_ch = *res_ch_str - '0';

                switch (res_ch)
                {
                case 1:
                    if (list_size == 0)
                        puts("Nobody was playing...");

                    else
                    {
                        qsort(list, list_size, sizeof(player), compare_games);

                        for (player* ptr = list + list_size; ptr != list; ptr--)
                            printf("%s : %hu\n", (ptr - 1)->name, (ptr - 1)->games);
                    }

                    break;

                case 2:
                    if (list_size == 0)
                        puts("Nobody was playing...");

                    else
                    {
                        qsort(list, list_size, sizeof(player), compare_easy);

                        for (player* ptr = list + list_size; ptr != list; ptr--)
                            printf("%s : %hu\n", (ptr - 1)->name, (ptr - 1)->easy);
                    }

                    break;

                case 3:
                    if (list_size == 0)
                        puts("Nobody was playing...");

                    else
                    {
                        qsort(list, list_size, sizeof(player), compare_hard);

                        for (player* ptr = list + list_size; ptr != list; ptr--)
                            printf("%s : %hu\n", (ptr - 1)->name, (ptr - 1)->hard);
                    }

                    break;

                case 4:
                    if (list_size == 0)
                        puts("Nobody was playing...");

                    else
                    {
                        qsort(list, list_size, sizeof(player), compare_doom);

                        for (player* ptr = list + list_size; ptr != list; ptr--)
                            printf("%s : %hu\n", (ptr - 1)->name, (ptr - 1)->doom_guy);
                    }
                    break;
                case 5:
                    if (list_size == 0)
                        puts("Nobody was playing...");

                    else
                    {
                        qsort(list, list_size, sizeof(player), compare_percent);

                        for (player* ptr = list + list_size; ptr != list; ptr--)
                            printf("%s : %d%%\n", (ptr - 1)->name, (int)round((ptr - 1)->percent));
                    }
                    break;

                default:
                    puts("Are you sure you want to exit? Enter YES if yes, or something else if no.");
                    scanf_rng("%4s", resaw);

                    if (strcmp(resaw, "YES") != 0)
                        res_ch = 0;

                    break;
                }
            }         	while (res_ch != 6);
            break;

        default:
            puts("Are you sure you want to exit? Enter YES if yes, or something else if no.");
            scanf_rng("%4s", away);

            if (strcmp(away, "YES") != 0)
                choice = 0;

            break;
        }
    } 	while (choice != 3);

    return 0;
}
