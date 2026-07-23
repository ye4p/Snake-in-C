#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define HEIGHT 15
#define WIDTH 15
#define TOTAL_SQUARES (HEIGHT * WIDTH)

// clang-format off
enum direction
{
    left, up, right, down
};

enum field 
{
    empty, snake, apple
};
// clang-format on

typedef struct Coords
{
    int x;
    int y;
} Coords;

typedef struct Node Node;

typedef struct Snake
{
    int dir;
    Node *beg;
    Node *end;
} Snake;

// For a snake body, use a doubly linked list
typedef struct Node
{
    Node *prev;
    // Coords c;
    int c[2];
    Node *next;
} Node;

void run(int board[TOTAL_SQUARES], Snake *s, int *play);

void handle_input(Snake *s);

void draw(int board[TOTAL_SQUARES], Snake *s);
void erase(int board[TOTAL_SQUARES], Snake *s);

void clear_tail(Snake *s);
void update_game(int board[255], Snake *s);

int is_valid(int board[255], Snake *s);

void render(int board[TOTAL_SQUARES]);
void wait(int time); // in ms
void start_game(int board[TOTAL_SQUARES], Snake *s, Node *head, Node *tail);
void end_game(Snake *s);

void place_apple(int board[TOTAL_SQUARES], Snake *s);

void run(int board[TOTAL_SQUARES], Snake *s, int *play)
{
    int count = 0;
    while (play)
    {
        handle_input(s);
        erase(board, s);
        update_game(board, s);

        if (is_valid(board, s))
            *play = 0;
        printf("end is_valid operation\n");

        draw(board, s);
        printf("end draw() operation\n");

        if (count % 5 == 0)
        {
            place_apple(board, s);
            printf("end is_valid operation\n");
        }

        render(board);

        wait(200);
        count++;
    }
}

void handle_input(Snake *s)
{
    // if (!_kbhit())
    //     return;
    if (_kbhit())
    {
        char c = _getch();
        switch (c)
        {
        case 'w':
            s->dir = up;
            break;
        case 'a':
            s->dir = left;
            break;
        case 's':
            s->dir = down;
            break;
        case 'd':
            s->dir = right;
            break;
        default:
            break;
        }
    }
}

void draw(int board[TOTAL_SQUARES], Snake *s)
{
    Node *cur = s->beg;
    while (cur != NULL)
    {
        board[cur->c[0] + cur->c[1] * WIDTH] = snake;
        cur = cur->next;
    }
}

void erase(int board[TOTAL_SQUARES], Snake *s)
{
    Node *cur = s->beg;
    printf("erasure beg\n");
    while (cur != NULL)
    {
        board[cur->c[0] + cur->c[1] * WIDTH] = empty;
        if (!cur->next)
            break;
        cur = cur->next;
    }
    printf("erasure end\n");
}

void clear_tail(Snake *s)
{
    Node *old_tail = s->end;
    s->end = s->end->prev;
    s->end->next = NULL;
    free(old_tail);
}

void update_game(int board[255], Snake *s)
{

    // Move head
    Node *new = malloc(sizeof(Node));
    new->next = NULL;
    new->prev = s->beg;

    int new_coords[2] = {s->beg->c[0], s->beg->c[1]};

    switch (s->dir)
    {
    case down:
        --new_coords[1];
        break;
    case up:
        ++new_coords[1];
        break;
    case right:
        ++new_coords[0];
        break;
    case left:
        --new_coords[0];
        break;
    default:
        printf("Invalid direction %i\n", s->dir);
        break;
    }

    new->c[0] = new_coords[0];
    new->c[1] = new_coords[1];

    s->beg->next = new;
    s->beg = s->beg->next;
    printf("head assignment successful\n");

    // Clearing tail if no apple was consumed
    int sq = new_coords[0] + WIDTH * new_coords[1];
    printf("new sq: %i\n", sq);
    if (board[sq] != apple)
    {
        clear_tail(s);
        printf("clearing tail successful\n");
    }
    printf("end update game operation\n");
}

int is_valid(int board[255], Snake *s)
{
    int *arr = calloc(HEIGHT * WIDTH, sizeof(int));
    Node *p = s->beg;
    while (p)
    {
        // check if OOB
        if (p->c[0] < 0 || p->c[0] >= WIDTH || p->c[1] < 0 || p->c[1] >= HEIGHT)
        {
            free(arr);
            return 0;
        }

        // check if hits itself
        int square = p->c[0] + WIDTH * p->c[1];
        if (arr[square]++)
        {
            free(arr);
            return 0;
        }

        p = p->next;
    }
    free(arr);
    return 1;
}

void render(int board[TOTAL_SQUARES])
{
    char *square_str = "[]";
    char *space_str = "  ";
    char *apple_str = "@@";
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (board[x + WIDTH * y] == snake)
                printf(square_str);
            else if (board[x + WIDTH * y] == apple)
                printf(apple_str);
            else
                printf(space_str);
        }
        printf("\n");
    }
}

void wait(int time)
{
    Sleep(time);
}

void start_game(int board[TOTAL_SQUARES], Snake *s, Node *head, Node *tail)
{
    s->dir = right;

    head->prev = NULL;
    head->c[0] = 7;
    head->c[1] = 7;

    tail->c[0] = 6;
    tail->c[1] = 7;
    tail->next = NULL;

    tail->prev = head;
    head->next = tail;
}

void end_game(Snake *s)
{
    Node *head = s->beg;
    while (head)
    {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

void place_apple(int board[TOTAL_SQUARES], Snake *s)
{
    srand(time(NULL));
    int random_number = rand() % TOTAL_SQUARES;
    while (board[random_number] == snake || board[random_number] == apple)
    {
        random_number = rand() % TOTAL_SQUARES;
    }
    board[random_number] == apple;
}

// ******************************************************************************************************************
// *  DEBUGGING METHODS
// ******************************************************************************************************************

void print_board(int board[TOTAL_SQUARES])
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            printf("%i, ", board[x + y * WIDTH]);
        }
        printf("\n");
    }
}

int main()
{

    int board[HEIGHT * WIDTH] = {0};
    Node *head = malloc(sizeof(Node));
    Node *tail = malloc(sizeof(Node));
    printf("heap allocation successful\n");
    // Snake s = {.beg = head, .end = tail};
    // start_game(board, &s, head, tail);

    head->prev = NULL;
    head->c[0] = 7;
    head->c[1] = 7;

    tail->c[0] = 6;
    tail->c[1] = 7;
    tail->next = NULL;

    tail->prev = head;
    head->next = tail;

    Snake s;
    s.dir = right;
    s.beg = head;
    s.end = tail;

    printf("snake construction successful\n");
    int play = 1;
    run(board, &s, &play);
    return 0;
}