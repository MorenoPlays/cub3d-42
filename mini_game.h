#ifndef MINI_GAME_H
#define MINI_GAME_H
#define size_bloco 50
#define player_size 10
#define largura 1024
#define altura 700
#define PI 3.141592654
#include "minilibx-linux/mlx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_player
{
    float x;
    float y;
    float angulo;

    int up;
    int down;
    int left;
    int right;

    int rl;
    int rr;
} t_player;

typedef struct s_img
{
    void *img;
    int bpp;
    int size_line;
    int endian;
}t_img;

typedef struct s_game
{
    void    *game;
    void    *janela;
    int     max_map_x;
    int     max_map_y;
    char **map;
    char *data;
    t_player *player;
    t_img *imag;
}t_game ;
#endif