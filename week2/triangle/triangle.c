// Checks whether the 3 sides inputted by the user can mathematically be a triangle or not

# include <cs50.h>
# include <stdio.h>

int valid_triangle(int a, int b, int c);

int main(void)
{
    int sides[3];

    for (int i = 0; i < 3; i++)
    {
        do
        {
            sides[i] = get_int("Input side %i of the triangle (positive integer): ", i + 1);
        }
        while (sides[i] <= 0);
    }

    if (valid_triangle(sides[0], sides[1], sides[2]))
    {
        printf("This is a valid triangle!\n");
    }
    else
    {
        printf("This is not a valid triangle!\n");
    }
}

int valid_triangle(int a, int b, int c)
{
    if ((a + b > c) && (a + c > b) && (b + c > a))
    {
        return 1;
    }
    return 0;
}