#include "mini_game.h"

void put_pixel(int x, int y, int color, t_game *game)
{
    char *index;

    if(x>= largura || y>= altura || x < 0 || y < 0)
        return ;
    index = game->data + (y * game->imag->size_line + x * (game->imag->bpp / 8));
    *(unsigned int *)index=color;
    // game->data[index+1]=(color >> 8) & 0xFF;
    // game->data[index+2]=(color & 0xFF);
}

void put_pixel_cub(int x, int y,int color, t_game *game)
{
    char *index;

    if(x >= largura || y>=altura || x < 0 || y < 0)
        return;
    index = game->data_cub + (y * game->imag_cub->size_line + x * (game->imag_cub->bpp/8));
    *(unsigned int *)index = color;
}

void    init_player(t_player *player)
{
    player->x=2*size_bloco;
    player->y=7*size_bloco;
    player->angulo=PI/2;
    player->up=0;
    player->down=0;
    player->left=0;
    player->right=0;
    player->rl=0;
    player->rr=0;
}
void    limpar_tela(t_game *game)
{
    int i;
    int j;

    i = 0;
    while (i<=altura)
    {
        j=0;
        while (j<=largura)
        {
            put_pixel(j, i ,0x000000, game);
            put_pixel_cub(j, i ,0x000000, game);
            j++;
        }
        i++;
    }
}

void    desenhar_player(t_game *mini_game, int size, int x, int y)
{
    int i;
    int j;

    i = 0;
    while (i<size)
    {
        j=0;
        while (j<size)
        {
           put_pixel(x, y, 0xfff111, mini_game);
            j++;
        }
        i++;
    }
    mlx_put_image_to_window(mini_game->game, mini_game->janela, mini_game->imag->img, x, y);
}

void    desenhar_quadrado(t_game *mini_game, int size, int x, int y)
{
    for (int i = 0; i <= size; i++)
    {
        put_pixel(x + i, y, 0xffff11, mini_game);          // Linha superior
        put_pixel(x + i, y + size, 0xffff11, mini_game);    // Linha inferior
        put_pixel(x, y + i, 0xffff11, mini_game);           // Linha esquerda
        put_pixel(x + size, y + i, 0xffff11, mini_game);    // Linha direita
    }
    
}

void    desenhar_mapa(char **map, t_game *mini_game )
{
    int i;
    int j;
    int salto_x;
    int salto_y;

    i = 0;
    salto_y=0;
    while (map[i])
    {
        j=0;
        salto_x=0;
        while (map[i][j])
        {
            if(map[i][j]=='1')
                desenhar_quadrado(mini_game, size_bloco, salto_x, salto_y);
            j++;
            salto_x+=size_bloco;
        }
        salto_y+=size_bloco;
        i++;
    }
}

int verify_wall(t_game *game, int x, int y)
{
    int ray_x;
    int ray_y;

    ray_x = (x/size_bloco);
    ray_y = (y/size_bloco);
    if(ray_x<0 || ray_y<0 || ray_x >= game->max_map_y)
        return (1);
    if(game->map[ray_x][ray_y]=='1')
        return (1);
    return(0);
}

void    desenhar_jogador(t_player *player, t_game *game)
{
    desenhar_quadrado(game,player_size,player->y, player->x );
}

void    ft_maximo(t_game *game)
{
    int i;

    i = 0;
    while (game->map[i])
    {
        i++;
    }
    game->max_map_y=i;
}


int    move_player(t_game *game)
{
    t_player *player = game->player;
    float speed = 0.5;
    double seno;
    double coseno;

    seno= sin(player->angulo);
    coseno= cos(player->angulo);
    if(game->player->rl==1)
        player->angulo-=0.01;
    if(game->player->rr==1)
        player->angulo+=0.01;

    if(player->angulo > PI * 2)
        player->angulo=0;
    if(player->angulo < 0)
        player->angulo=PI * 2;


    if(game->player->up==1)
    {
        printf("up\n");
        player->x+=coseno*speed;
        player->y+=seno*speed;
    }
    else if(game->player->down==1)
    {
        printf("down\n");
        player->x-=coseno*speed;
        player->y-=seno*speed;
    }
    else if(game->player->left==1)
    {
         printf("right\n");
         player->x+=seno*speed;
        player->y-=coseno*speed;
    }
    else if (game->player->right==1)
    {
        printf("left\n");
         player->x -= seno*speed;
        player->y += coseno*speed;
    }
    printf("player_x:%f\nplayer_y:%f\n", player->y, player->x);
    printf("angulo:%d\n", (int)(player->angulo*((180*2)/(PI * 2))));
    return (0);
}


void    paredes(int *distancia, t_game *game, double *ray, int *step, float x, int side)
{
    float distancia_da_parede;
    float altura_parede;
    float drawn[2];
    int color;

    if (side == 0) // Eixo X
        distancia_da_parede = fabs((distancia[0] - (game->player->x/size_bloco) + (1 - step[0]) / 2) / ray[0]);
    else // Eixo Y
        distancia_da_parede = fabs((distancia[1] - (game->player->y/size_bloco) + (1 - step[1]) / 2) / ray[1]);


    if(distancia_da_parede>0.1f)
        altura_parede=(altura)/distancia_da_parede;
    else
        altura_parede=altura;
    drawn[0]=(-altura_parede/2)+(altura/2);
    drawn[1]=(altura_parede/2)+(altura/2);
    if(side==0)
        color=0x00ff00;
    else
        color=0xffffff;
    while(drawn[0]<drawn[1])
    {
        put_pixel_cub(x, drawn[0],color, game);
        drawn[0]++;
    }
}
void    direcao(int *step , double *ray)
{
    
    if(ray[0]>0)
        step[0]=1;
    else
        step[0]=-1;
    if(ray[1]>0)
        step[1]=1;
    else
        step[1]=-1;
    
    //return(step);
}

void dda_1(int *side, float *distancia, int *distancia_inicial_player, int *step)
{
    if(distancia[2]<distancia[3])
    {
        distancia_inicial_player[0]+=step[0];
        distancia[2]+=distancia[0];
        *side=0;
    }
    else
    {
        distancia_inicial_player[1]+=step[1];
        distancia[3]+=distancia[1];
        *side=1;
    }
}
int dda(float *distancia, int *distancia_inicial_player, t_game *mini_game, double *ray, float x)
{
    int step[2];
    int hit=0;
    int side;

    direcao(step,ray);
    while (hit==0)
    {
        dda_1(&side, distancia, distancia_inicial_player, step);
        if(distancia_inicial_player[0]<0 || distancia_inicial_player[1]<0 || distancia_inicial_player[0] >= mini_game->max_map_y)
            hit=1;
        else if(mini_game->map[distancia_inicial_player[0]][distancia_inicial_player[1]]=='1')
            hit=1;
    }
    paredes(distancia_inicial_player, mini_game, ray, step, x, side);
    return (side);
}

int side_ray(t_game *mini_game, float angulo, float x)
{
    double ray[2];
    float distancia_parede[4];
    int distancia_inicial_do_player[2];

    ray[0]=cos(angulo);
    ray[1]=sin(angulo);
    distancia_inicial_do_player[0]=mini_game->player->x/size_bloco;
    distancia_inicial_do_player[1]=mini_game->player->y/size_bloco;
    if(ray[0]==0)
        distancia_parede[0]=1;
    else
        distancia_parede[0]=fabs(1/ray[0]);
    if(ray[1]==0)
        distancia_parede[1]=1;
    else
        distancia_parede[1]=fabs(1/ray[1]);
    if(ray[0]>0)
        distancia_parede[2]=(distancia_inicial_do_player[0] + 1.0 - (mini_game->player->x/size_bloco)) * distancia_parede[0];
    else
        distancia_parede[2]=((mini_game->player->x/size_bloco) - distancia_inicial_do_player[0]) * distancia_parede[0];
    if(ray[1]>0)
        distancia_parede[3]=(distancia_inicial_do_player[1] + 1.0  - (mini_game->player->y/size_bloco)) * distancia_parede[1];
    else
        distancia_parede[3]=((mini_game->player->y/size_bloco) - distancia_inicial_do_player[1]) * distancia_parede[1];
    return(dda(distancia_parede, distancia_inicial_do_player, mini_game, ray, x));
}
void angulo_visao(t_game *game, float angulo, int i)
{
    float x;
    float y;
    int color;

    if(side_ray(game, angulo, i)==1)
        color=0x00ff00;
    else
        color=0xffffff;
    x = game->player->x;
    y = game->player->y;
    while (verify_wall(game, x, y)!=1)
    {
        put_pixel(y, x, color,game);
        x+=cos(angulo);
        y+=sin(angulo);
    }
    
}
int loop_game(t_game *mini_game)
{
    int i;
    float ajuste_angulo;
    float start;
    float angulo;

    angulo=PI/3;
    limpar_tela(mini_game);
    move_player(mini_game);
    desenhar_mapa(mini_game->map, mini_game);
    desenhar_jogador( mini_game->player, mini_game);
    ajuste_angulo = (angulo) / largura;
    start = mini_game->player->angulo - (PI / 6);
    i = 0;
    while (i < largura)
    {
         angulo_visao(mini_game, start, i);
         start+=ajuste_angulo;
         i++;
    }
    mlx_put_image_to_window(mini_game->game, mini_game->janela, mini_game->imag->img, 0,0);
    mlx_put_image_to_window(mini_game->game, mini_game->janela_cub, mini_game->imag_cub->img, 0, 0);
     return (0);
}
int key_press(int key, t_game *game)
{
    t_player *player = game->player;

    if(key==65363)
        player->rl=1;
    if(key==65361)
        player->rr=1;
    if(key=='w')
        player->up=1;
    if(key=='s')
        player->down=1;
    if(key=='a')
        player->left=1;
    if (key=='d')
        player->right=1;
    return (0);
}

int key_press_relese(int key, t_game *game)
{
    t_player *player = game->player;

    if(key==65363)
        player->rl=0;
    if(key==65361)
        player->rr=0;
    if(key=='w')
        player->up=0;
    if(key=='s')
        player->down=0;
    if(key=='a')
        player->left=0;
    if (key=='d')
        player->right=0;
    return (0);
}

int main()
{
    char *map[]={
        "11111111111111111111",
        "10010001000000010001",
        "10001001000000010001",
        "10100000001000000001",
        "10000000010000000001",
        "10101000000000001001",
        "10010000010000000001",
        "11111111111111111111",
        NULL
    };
    t_game mini_game;
    t_player player;
    t_img img;
    t_img img_cub;
    init_player(&player);
    mini_game.game=mlx_init();
    mini_game.janela=mlx_new_window(mini_game.game,largura,altura,"mini_map");
    mini_game.janela_cub=mlx_new_window(mini_game.game,largura, altura,"cub3d");
    mini_game.player=&player;
    img.img=mlx_new_image(mini_game.game, largura, altura);
    img_cub.img=mlx_new_image(mini_game.game, largura, altura);
    mini_game.data=mlx_get_data_addr(img.img, &img.bpp, &img.size_line, &img.endian);
    mini_game.data_cub=mlx_get_data_addr(img_cub.img, &img_cub.bpp, &img_cub.size_line, &img_cub.endian);
    mini_game.map=map;
    ft_maximo(&mini_game);
    mini_game.imag=&img;
    mini_game.imag_cub=&img_cub;
    mlx_hook(mini_game.janela,2, 1L << 0,  &key_press, &mini_game);
    mlx_hook(mini_game.janela,3, 1L << 1,  &key_press_relese, &mini_game);
    mlx_loop_hook(mini_game.game, &loop_game, &mini_game);
    mlx_loop(mini_game.game);
}