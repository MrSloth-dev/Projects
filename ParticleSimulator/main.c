#include "particle.h"


t_particle **array;

int main()
{
    srand(time(NULL));
    t_brush brush;
    brush.mat_id = mat_id_sand;
    brush.radius = 2;

    array = malloc(sizeof(t_particle*) * height);
    for (int k = 0; k < height; k++)
        array[k] = malloc(sizeof(t_particle) * width);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            array[y][x].id = mat_id_empty;
            array[y][x].color = mat_col_empty;
            array[y][x].updated_this_frame = false;
        }
    }

    InitWindow(width, height, "FallingAutomata");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        ResetUpdateFlags(array);
        InitBrush(&brush, array);
        BeginDrawing();
        CalculateFrame(array);
        DrawFrame(array);
        EndDrawing();
    }

    for (int k = 0; k < height; k++)
        free(array[k]);
    free(array);

    CloseWindow();
    return 0;
}

void ResetUpdateFlags(t_particle **array)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            array[y][x].updated_this_frame = false;
        }
    }
}

void InitBrush(t_brush *brush, t_particle **array)
{
    brush->x = GetMouseX();
    brush->y = GetMouseY();

    if (IsKeyPressed(KEY_ONE))
        brush->mat_id = mat_id_sand;
    else if (IsKeyPressed(KEY_TWO))
        brush->mat_id = mat_id_water;
    else if (IsKeyPressed(KEY_THREE))
        brush->mat_id = mat_id_stone;
    else if (IsKeyPressed(KEY_FOUR))
        brush->mat_id = mat_id_fire;
    else if (IsKeyPressed(KEY_FIVE))
        brush->mat_id = mat_id_empty;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        for (int dy = -brush->radius; dy <= brush->radius; dy++)
        {
            for (int dx = -brush->radius; dx <= brush->radius; dx++)
            {
                int x = brush->x + dx;
                int y = brush->y + dy;
                if (InBounds(y, x) && (dx * dx + dy * dy <= brush->radius * brush->radius))
                    array[y][x].id = brush->mat_id;
            }
        }
    }
}

int InBounds(int y, int x)
{
    return (x >= 0 && y >= 0 && x < width && y < height);
}

int IsEmpty(int y, int x)
{
    return (array[y][x].id == mat_id_empty);
}



void DrawFrame(t_particle **array)
{
    ClearBackground(mat_col_empty);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            switch (array[y][x].id) {
                case mat_id_empty:
                    array[y][x].color = mat_col_empty; break;
                case mat_id_sand:
                    array[y][x].color = mat_col_sand; break;
                case mat_id_water:
                    array[y][x].color = mat_col_water; break;
                case mat_id_stone:
                    array[y][x].color = mat_col_stone; break;
                case mat_id_fire:
                    array[y][x].color = mat_col_fire; break;
                default:
                    array[y][x].color = mat_col_empty; break;
            }
            DrawPixel(x, y, array[y][x].color);
        }
    }
}

void CalculateFrame(t_particle **array)
{
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            switch (array[y][x].id) {
                case mat_id_empty:
                    break;
                case mat_id_sand:
                    UpdateSand(y, x); break;
                case mat_id_water:
                    UpdateWater(y, x); break;
                case mat_id_stone:
                    UpdateStone(y, x); break;
                case mat_id_fire:
                    UpdateFire(y, x); break;
                default:
                    break;
            }
        }
    }
}
