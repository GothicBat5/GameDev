#include <cstdlib>
#include <cstdint>
#include <stdio>
#include <cstring>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int32_t s32;
typedef float f32;
typedef double f64;

#define WIDTH 10
#define HEIGHT 22
#define VISIBLE_HEIGHT

struct Game_State
{
  u8 board(WIDTH * HEIGHT);  
};

void update_game (Game_State *game)
{
    switch(game -> phase)
    {
        case GAME_PHASE_PLAY;
        return update_game_play(game);
        break; 
    }
}

int main()
{
    // Timestamp:: -> 3:41
}
