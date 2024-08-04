#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define height 400
#define width 400
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920
#define scale_h SCREEN_HEIGHT / height
#define scale_w SCREEN_WIDTH / width
// Material IDs
#define mat_id_p1 (short)-1
#define mat_id_empty 0
#define mat_id_sand  1
#define mat_id_water 2
#define mat_id_stone 3
#define mat_id_smoke  4

// Colors
#define mat_col_p1 (Color)RAYWHITE
#define mat_col_empty (Color){0, 0, 0, 255}
#define mat_col_sand  (Color){194, 178, 128, 255}
#define mat_col_water (Color){0, 119, 190, 255}
#define mat_col_stone (Color){169, 169, 169, 255}
#define mat_col_smoke (Color){50, 50, 50, 255}

typedef struct {
    short id;
    Color color;
    bool updated_this_frame;
} t_particle;

typedef struct {
    short radius;
    int x;
    int y;
    short mat_id;
} t_brush;



int InBounds(int x, int y);
int IsEmpty(int y, int x);
void UpdateSand(int y, int x);
void UpdateWater(int y, int x);
void InitBrush(t_brush *brush, t_particle **array);
void DrawFrame(t_particle **array);
void CalculateFrame(t_particle **array);
void ResetUpdateFlags(t_particle **array);


void UpdateWater(int y, int x);
void UpdateSand(int y, int x);
void UpdateStone(int y, int x);
void UpdateSmoke(int y, int x);


