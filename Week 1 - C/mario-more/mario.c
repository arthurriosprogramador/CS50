#include <cs50.h>
#include <stdio.h>

void build_pyramid(int size);
void build_left_side(int rows, int index);
void build_right_side(int rows, int index);

int main(void)
{
    int n;
    do
    {
        n = get_int("Type a number between 1 and 8: ");
    }
    while (n < 1 || n > 8);

    build_pyramid(n);
}

void build_pyramid(int size)
{
    for (int i = 0; i < size; i++)
    {
        build_left_side(size, i);
        printf("  ");
        build_right_side(size, i);
        printf("\n");
    }
}

void build_left_side(int rows, int index)
{
    for (int i = 0; i < (rows - index - 1); i++)
    {
        printf(" ");
    }
    for (int j = 0; j < index + 1; j++)
    {
        printf("#");
    }
}

void build_right_side(int rows, int index)
{
    for (int i = 0; i < index + 1; i++)
    {
        printf("#");
    }
}
