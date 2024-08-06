#include "particle.h"
#include <raylib.h>


t_particle **array;

int	ft_convert(int i, int new_min, int new_max, int old_min, int old_max)
{
	return ((new_max - new_min) *(i - old_min) / (old_max - old_min) + new_min);
}


t_player p1;

void    InitPlayer()
{
    p1.p_width = 10;
    p1.p_height = 10;
    p1.x = width / 2;
    p1.y = height / 2;
    p1.id = mat_id_p1;
    p1.vel_y = GRAVITY;
    p1.vel_x = 0;
    array[p1.y][p1.x].id = mat_id_p1;
}

bool IsOnGround()
{
    return !InBounds(p1.y + 1, p1.x) || !IsEmpty(p1.y + 1, p1.x);
}

void PlayerMove()
{

    if (IsKeyDown(KEY_D))
        p1.vel_x += ACCELERATION;
    if (IsKeyDown(KEY_A))
        p1.vel_x -= ACCELERATION;
    if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
        p1.vel_x *= FRICTION;

    if (p1.vel_x > 5) p1.vel_x = MAX_VEL;
    if (p1.vel_x < -5) p1.vel_x = -MAX_VEL;

    if (IsKeyPressed(KEY_W) && IsOnGround())
        p1.vel_y += JUMP;

    if (!IsOnGround())
        p1.vel_y +=GRAVITY;
    else if (!IsKeyPressed(KEY_W))
        p1.vel_y = 0;

    float new_x = p1.x + p1.vel_x;
    float new_y = p1.y + p1.vel_y;

    // Check boundaries horizontal
    if (!InBounds(new_y, new_x) || !IsEmpty(new_y, new_x) && !IsOnGround())
    {
        new_x = p1.x;
        p1.vel_x = 0;
    }
    // Check boundaries vertical
    if (!InBounds(new_y, new_x) || !IsEmpty(new_y, new_x))
    {
        new_y = p1.y + p1.vel_y / 2;
        p1.vel_y = 0;
    }
    //UpdatePosition
    if (InBounds(new_y, new_x) && IsEmpty(new_y, new_x))
    {
        array[p1.y][p1.x].id = mat_id_empty;
        p1.x = new_x;
        p1.y = new_y;
        array[p1.y][p1.x].id = mat_id_p1;
    }
}

int main()
{
    srand(time(NULL));
    t_brush brush;
    brush.mat_id = mat_id_sand;
    brush.radius = 4;

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

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FallingAutomata");
    InitPlayer();
    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        ResetUpdateFlags(array);
        InitBrush(&brush, array);
        BeginDrawing();
        DrawText(TextFormat("Vel_x = %d", p1.vel_x), SCREEN_WIDTH / 10, SCREEN_HEIGHT / 10, 50, RAYWHITE);
        PlayerMove();
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

    if (IsKeyDown(KEY_ONE))
        brush->mat_id = mat_id_sand;
    else if (IsKeyDown(KEY_TWO))
        brush->mat_id = mat_id_water;
    else if (IsKeyDown(KEY_THREE))
        brush->mat_id = mat_id_stone;
    else if (IsKeyDown(KEY_FOUR))
        brush->mat_id = mat_id_smoke;
    else if (IsKeyDown(KEY_FIVE))
        brush->mat_id = mat_id_empty;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        for (int dy = -brush->radius; dy <= brush->radius; dy++)
        {
            for (int dx = -brush->radius; dx <= brush->radius; dx++)
            {
                int x = ft_convert(brush->x + dx, 0, width, 0, SCREEN_WIDTH);
                int y = ft_convert(brush->y + dy, 0, height, 0, SCREEN_HEIGHT);
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
                case mat_id_p1:
                    array[y][x].color = mat_col_p1; break;
                case mat_id_empty:
                    array[y][x].color = mat_col_empty; break;
                case mat_id_sand:
                    array[y][x].color = mat_col_sand; break;
                case mat_id_water:
                    array[y][x].color = mat_col_water; break;
                case mat_id_stone:
                    array[y][x].color = mat_col_stone; break;
                case mat_id_smoke:
                    array[y][x].color = mat_col_smoke; break;
                default:
                    array[y][x].color = mat_col_empty; break;
            }
            //DrawPixel(x, y, array[y][x].color);
            DrawRectangle(x * scale_w, y *scale_h, scale_w * 10, scale_h * 10, array[y][x].color);
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
                case mat_id_p1:
                    break;
                case mat_id_sand:
                    UpdateSand(y, x); break;
                case mat_id_water:
                    UpdateWater(y, x); break;
                case mat_id_stone:
                    UpdateStone(y, x); break;
                case mat_id_smoke:
                    UpdateSmoke(y, x); break;
                default:
                    break;
            }
        }
    }
}
