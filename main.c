#include <stdio.h>
#include <windows.h>

#define HEIGHT 15
#define WIDTH 15
#define TOTAL_SQUARES (HEIGHT * WIDTH)

// clang-format off
enum direction
{
    left, up, right, down
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

void run(int board[TOTAL_SQUARES], Snake *s);

void handle_input(Snake *s);

void draw(int board[TOTAL_SQUARES], Snake *s);
void erase(int board[TOTAL_SQUARES], Snake *s);
void update_game(int board[255], Snake *s);

int is_valid(int board[255], Snake *s);

void render(int board[TOTAL_SQUARES]);
void wait(int time); // in ms
void start_game(int board[TOTAL_SQUARES], Snake *s);

void run(int board[TOTAL_SQUARES], Snake *s)
{
    handle_input(s);
    update_game(board, s);
    render(board);
    wait(200);
}

void handle_input(Snake *s)
{
    if (!_kbhit())
        return;
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

void draw(int board[TOTAL_SQUARES], Snake *s)
{
    Node *cur = s->beg;
    while (cur != NULL)
    {
        board[cur->c[0] + cur->c[1] * 15] = 1;
        cur = cur->next;
    }
}

void erase(int board[TOTAL_SQUARES], Snake *s)
{
    Node *cur = s->beg;
    while (cur != NULL)
    {
        board[cur->c[0] + cur->c[1] * 15] = 0;
        cur = cur->next;
    }
}

void update_game(int board[255], Snake *s)
{
    // Clear tail
    Node *old_tail;
    memcpy(old_tail, s->end, sizeof(Node));
    s->end->next->prev = NULL;
    free(s->end);

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
}

int is_valid(int board[255], Snake *s)
{
    Node *p = s->beg;
    while (p)
    {
        // check if OOB
        if (p->c[0] < 0 || p->c[0] >= WIDTH || p->c[1] < 0 || p->c[1] >= HEIGHT)
            return 0;

        // check if hits itself
        int *arr = calloc(HEIGHT * WIDTH, sizeof(int));
        int square = p->c[0] + WIDTH * p->c[1];
        if (arr[square]++)
            return 0;

        p = p->next;
    }
    return 1;
}

void render(int board[TOTAL_SQUARES])
{
    char *square = "[]";
    char *space = "  ";
    char *apple = "@@";
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (board[x + WIDTH * y])
                printf(square);
            else
                printf(space);
        }
        printf("\n");
    }
}

void wait(int time)
{
    Sleep(time);
}

void start_game(int board[TOTAL_SQUARES], Snake *s)
{
    s->dir = right;

    s->beg->prev = NULL;
    s->beg->c[0] = 7;
    s->beg->c[1] = 7;

    s->end->c[0] = 6;
    s->end->c[1] = 7;
    s->end->next = NULL;

    s->end->prev = s->beg;
    s->beg->next = s->end;
}

int main()
{

    int board[HEIGHT * WIDTH] = {0};
    Node head;
    Node tail;
    Snake s = {.beg = &head, .end = &tail};

    start_game(board, &s);
    draw(board, &s);
    render(board);
    return 0;
}