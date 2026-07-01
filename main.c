#include <stdio.h>
#include <windows.h>

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

typedef struct ListNode ListNode;

typedef struct Snake
{
    int dir;
    ListNode *beg;
    ListNode *end;
} Snake;

// For a snake body, use a doubly linked list
typedef struct ListNode
{
    ListNode *prev;
    // Coords c;
    int c[2];
    ListNode *next;
} ListNode;

void run(int board[225], Snake *s);

void handle_input();

void draw(int board[225], Snake *s);
void erase(int board[225], Snake *s);
void update_game(int board[255], Snake *s);

void render(int board[225]);
void wait(int time); // in ms
void start_game(int board[225], Snake *s);

void run(int board[225], Snake *s)
{
    handle_input();
    update_game(board, s);
    render(board);
    wait(200);
}

void handle_input()
{
}

void draw(int board[225], Snake *s)
{
    ListNode *cur = s->beg;
    while (cur != NULL)
    {
        board[cur->c[0] + cur->c[1] * 15] = 1;
        cur = cur->next;
    }
}

void erase(int board[225], Snake *s)
{
    ListNode *cur = s->beg;
    while (cur != NULL)
    {
        board[cur->c[0] + cur->c[1] * 15] = 0;
        cur = cur->next;
    }
}

void update_game(int board[255], Snake *s)
{
    // Clear tail
    ListNode *old_tail;
    memcpy(old_tail, s->end, sizeof(ListNode));
    s->end->next->prev = NULL;
    free(s->end);

    // Move head
    ListNode *new = malloc(sizeof(ListNode));
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
    case right:
        ++new_coords[0];
    case left:
        --new_coords[0];
    default:
        printf("Invalid direction %i\n", s->dir);
        break;
    }

    new->c[0] = new_coords[0];
    new->c[1] = new_coords[1];

    s->beg->next = new;
    s->beg = s->beg->next;
}

void render(int board[225])
{
    for (int y = 0; y < 15; y++)
    {
        for (int x = 0; x < 15; x++)
        {
            if (board[x + 15 * y])
                printf("[]");
            else
                printf("  ");
        }
        printf("\n");
    }
}

void wait(int time)
{
    Sleep(time);
}

void start_game(int board[225], Snake *s)
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
    int board[225] = {0};
    ListNode head;
    ListNode tail;
    Snake s = {.beg = &head, .end = &tail};

    start_game(board, &s);
    draw(board, &s);
    render(board);
    return 0;
}