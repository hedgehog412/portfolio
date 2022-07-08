#include "body.h"
#include "text.h"
#include "color.h"
#include "scene.h"
#include "forces.h"
#include "list.h"
#include "sdl_wrapper.h"
#include "state.h"
#include "player.h"
#include "missiles.h"
#include "zappers.h"
#include "missiles.h"
#include "coins.h"
#include "laser.h"
#include "background.h"
#include "menu.h"
#include "time.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES

#define G 6.67E-11          // N m^2 / kg^2
#define M 6E24              // kg
#define g 15              // m / s^2
#define R (sqrt(G * M / g)) // m

#define WALL_COLOR ((rgb_color_t){0, 0, 1})

// screen size
#define MAX ((vector_t){2000, 1000})
#define MIN ((vector_t){0, 0})

// obtacle time
#define ZAPPER_TIME 1.3
#define DISTANCE_FREE -300
#define COIN_TIME 1
#define LASER_TIME 5
#define MISSILE_TIME 4
#define TIME_BTW_Z_L 0
#define LASER_DURATION 3
#define LASER_CHARGE_TIME 2

// obstacle speed
#define INIT_SPEED_Z 800
#define INIT_SPEED_C 800
#define INIT_SPEED_M 1400
#define ACC_Z 10
#define ACC_C 10
#define ACC_M 10

// Powerup variables
#define NUM_POWERUPS 2
#define POWERUP_START 15
#define COIN_MAGNET_DURATION 5
#define INVINCIBILITY_DURATION 5

Mix_Music * gMusic;

// Stage Types
enum {START_SCREEN, MAIN_GAME, END_SCREEN, END_TRANSITION};

//Powerup Types
enum {COIN_MAGNET, INVINCIBILITY};

// Defining Structs
typedef struct powerup{
    size_t powerup_type;
    bool powerup_on;
    double powerup_timer;
} powerup_t;

typedef struct state {
  scene_t *scene;
  int stage;
  double speed_z;
  double speed_c;
  double speed_m;
  double zapper_timer;
  double laser_timer;
  double laser_tick_timer;
  double missile_timer;
  double coin_timer;
  double game_timer;
  double game_score;
  double high_score;
  bool player_alive;
  bool power_activated;
  size_t coin_counter;
  size_t total_coins_counter;
  list_t *powerups;
} state_t;

// game-flow control functions
scene_t *game_scene_init();
scene_t *start_scene_init();
scene_t *end_scene_init(state_t *state);
void end_game(state_t *state);
void start_game(state_t *state);
void start_transition(state_t *state);
void end_transition(state_t *state);
void player_motion(char key, key_event_type_t type, double held_time, state_t *state);
void menu_screen_handeler(char key, key_event_type_t type, double held_time, state_t *state);
void main_game_emscripten_main(state_t *state);
void start_screen_emscripten_main(state_t *state);
void end_transition_emscripten_main(state_t *state);
void end_screen_emscripten_main(state_t *state);
int find_player(scene_t *scene);
int find_gravity(scene_t *scene);
int find_laser(scene_t *scene);
void gravity_body_generator(scene_t *scene);
void add_zappers(state_t *state);
void add_missiles(state_t *state);
void add_coins(state_t *state);
void player_boundary(body_t *player, body_t *gravity_body);
void off_screen(state_t *state);
void neg_gravity(body_t *player, body_t *gravity_body);
void laser_tick(state_t *state, double dt);
void coin_magnet_tick(state_t *state, powerup_t *magnet, double dt);
void invincibility_tick(state_t *state, powerup_t *invincibility, double dt);
int find_shield(scene_t *scene);
void ch_coin(body_t *player, body_t *coin, vector_t axis, void *aux);
void ch_power_coin(body_t *player, body_t *coin, vector_t axis, void *aux);
void ch_obstacle(body_t *player, body_t *obstacle, vector_t axis, void *aux);
powerup_t *powerup_init(size_t type);
list_t *powerup_list_init();
bool powerup_on(list_t *powerups);
void powerup(state_t *state, double dt);
powerup_t *find_powerup_on(list_t *powerups);
void update_highscore(double time);
double get_highscore();
scene_t *highscore_scene_init();

// Init
state_t *emscripten_init() {
  sdl_init(MIN, MAX);
  state_t *state = malloc(sizeof(state_t));
  assert(state != NULL);
  scene_t *scene = start_scene_init();
  state->scene = scene;
  state->stage = START_SCREEN;
  sdl_render_scene(state->scene);
  return state;
}

void emscripten_main(state_t *state) {
  switch (state->stage){
  case START_SCREEN:
    start_screen_emscripten_main(state);
    break;
  case END_SCREEN:
    end_screen_emscripten_main(state);
    break;
  case MAIN_GAME:
    main_game_emscripten_main(state); 
    break;
  case END_TRANSITION:
    end_transition_emscripten_main(state);
    break;
  default:
    break;
  }
}

void emscripten_free(state_t *state) {
  Mix_FreeMusic(gMusic);
  gMusic = NULL;
  scene_free(state->scene);
  list_free(state->powerups);
  free(state);
}

// main function for game scene, called every tick
void main_game_emscripten_main(state_t *state){
  double dt = time_since_last_tick();
  body_t *player = scene_get_body(state->scene, find_player(state->scene));
  body_t *gravity_body = scene_get_body(state->scene, find_gravity(state->scene));
  state->game_timer += dt;
  state->game_score += dt;
  if (state -> game_timer >= 4){
    state->zapper_timer += dt;
    state->laser_timer += dt;
    state->missile_timer += dt;
    state->coin_timer += dt;
  }
  if(state->laser_timer >= LASER_TIME){
      laser_tick(state, dt);
  }else{
    add_zappers(state);
  }
  add_missiles(state);
  add_coins(state);
  powerup(state, dt);
  player_boundary(player, gravity_body);
  off_screen(state);

  // harder component
  state->speed_c += dt*ACC_C;
  state->speed_m += dt*ACC_M;
  state->speed_z += dt*ACC_Z;

  // rudimentary score CHANGE HERE
  char s[50];
  sprintf(s,"%f", state->game_score);
  txt_set_txt(scene_get_texts(state->scene,0), s);
  
   char c[50];
   sprintf(c, "%zu", state->coin_counter);
   txt_set_txt(scene_get_texts(state->scene, 1), c);

   char hs2[50];
   sprintf(hs2, "%f", state->high_score);
   txt_set_txt(scene_get_texts(state->scene, 2), hs2);
  
  scene_tick(state->scene, dt);
  sdl_render_scene(state->scene);
  
  if (!(state->player_alive)){
    end_transition(state);
    return;
  }

 }

// main function for start scene, called every tick
void start_screen_emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  scene_tick(state->scene, dt);
  sdl_render_scene(state->scene);
}

void end_transition_emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  if (state->game_timer >= 8){
    end_game(state);
    return;
  }
  if (state->game_timer >= 5){
    vector_t transition_vel = {0,-500};
    body_set_velocity(scene_get_body(state->scene, scene_bodies(state->scene)-1), transition_vel);
  }
  body_t *player = scene_get_body(state->scene, find_player(state->scene));
  vector_t vel = body_get_velocity(player);
  if (body_get_centroid(player).y < 20 && vel.y<0){
    body_set_velocity(player, (vector_t){vel.x, -0.7*vel.y});
  }
  state->game_timer += dt;
  scene_tick(state->scene, dt);
  sdl_render_scene(state->scene);
}

// main function for end scene, called every tick
void end_screen_emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  scene_tick(state->scene, dt);

  char hs[50];
  sprintf(hs, "%f", state->game_score);
  txt_set_txt(scene_get_texts(state->scene, scene_text(state->scene)-1), hs);

  sdl_render_scene(state->scene);
}

// initialises the start screen scene
scene_t *start_scene_init() {
  scene_t *scene = scene_init();
  text_t *text1 = text_init("Press Space to Start", (vector_t) {MAX.x/2, 150}, BLACK, 1);
  scene_add_text(scene, text1);
  text_t *text2 = text_init("R E S C U E  M I S S I O N", (vector_t) {MAX.x/2, 7*MAX.y/8}, (rgb_color_t){1,1,0}, 3);
  scene_add_text(scene, text2);
  text_t *text3 = text_init("The princess is in danger! You must save her!", (vector_t) {MAX.x/2, 6.7*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text3);
  text_t *text4 = text_init("Obstacles will be along your way. Avoid the", (vector_t) {MAX.x/2, 5.6*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text4);
  text_t *text5 = text_init("zappers, missiles, and lasers and collect power ups", (vector_t) {MAX.x/2, 4.5*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text5);
  text_t *text6 = text_init("to go further! Coins will enhance your score.", (vector_t) {MAX.x/2, 3.4*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text6);
  gMusic = Mix_LoadMUS( "./assets/BGM_STR_MII_EDIT_ORCH_DRC.dspadpcm.wav" );
  if( gMusic == NULL ){
    printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
  }
  Mix_PlayMusic( gMusic, -1 );
  sdl_on_key((key_handler_t)menu_screen_handeler);
  scene_add_body(scene, start_screen_generator(MAX, MIN));
  return scene;
}

// initialises the end screen scene
scene_t *end_scene_init(state_t *state) {
  scene_t *scene = scene_init();
  text_t *text1 = text_init("Press Space to Restart", (vector_t) {MAX.x/2, 150}, BLACK, 1);
  scene_add_text(scene, text1);
  text_t *text2 = text_init("G A M E  O V E R !", (vector_t) {MAX.x/2, 7*MAX.y/8}, (rgb_color_t){1,1,0}, 3);
  scene_add_text(scene, text2);
  text_t *text3 = text_init("You failed the mission...", (vector_t) {MAX.x/2, 6.7*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text3);
  text_t *text4 = text_init("The princess remains locked up in her castle", (vector_t) {MAX.x/2, 5.6*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text4);
  text_t *text5 = text_init("Score:", (vector_t) {MAX.x/3, 4.5*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text5);
  text_t *text55 = text_init("0", (vector_t) {MAX.x/2, 4.5*MAX.y/10}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text55);

  

  sdl_on_key((key_handler_t)menu_screen_handeler);
  scene_add_body(scene, end_screen_generator(MAX, MIN));
  return scene;
}

// initialises the highscore screen scene
scene_t *highscore_scene_init() {
  scene_t *scene = scene_init();
  text_t *text1 = text_init("Press Space to Restart", (vector_t) {MAX.x/2, 150}, (rgb_color_t){0,0,0}, 1);
  scene_add_text(scene, text1);
  text_t *text2 = text_init("H I G H  S C O R E !", (vector_t) {MAX.x/2, 7*MAX.y/8}, (rgb_color_t){1,1,0}, 3);
  scene_add_text(scene, text2);
  text_t *text3 = text_init("You got closer than anyone else!", (vector_t) {MAX.x/2, 5*MAX.y/8}, (rgb_color_t){1,1,1}, 1);
  scene_add_text(scene, text3);
  text_t *text55 = text_init("0", (vector_t) {MAX.x/2, 3.5*MAX.y/8}, (rgb_color_t){1,1,0}, 3);
  scene_add_text(scene, text55);

  sdl_on_key((key_handler_t)menu_screen_handeler);
  scene_add_body(scene, end_screen_generator(MAX, MIN));
  return scene;
}

// initialises the game scene
scene_t *game_scene_init() {
  scene_t *scene = scene_init();
  sdl_on_key((key_handler_t)player_motion);
  background_init(scene, MAX, MIN);
  gravity_body_generator(scene);
  scene_add_body(scene, player_generator());
  create_newtonian_gravity(scene, G, scene_get_body(scene,find_player(scene)), scene_get_body(scene,find_gravity(scene)));
  
  // initialising score counter
  text_t *score = text_init("0",(vector_t){200,950},BLACK,1);
  scene_add_text(scene, score);

  // initialising coin counter
  text_t *coin = text_init("0", (vector_t){1900, 950}, BLACK, 1);
  scene_add_text(scene, coin);

  text_t *high_score = text_init("0", (vector_t){1000, 950}, (rgb_color_t){0,0,0}, 1);
  scene_add_text(scene, high_score);
  
  return scene;
}

// key handeler for player motion
void player_motion(char key, key_event_type_t type, double held_time,
                   state_t *state) {
  scene_t *scene = ((state_t *)state)->scene;
  body_t *player = scene_get_body(scene, find_player(scene));
  vector_t player_velocity =
      ((info_t *)body_get_info(player))->control_velocity;
  if (type == KEY_PRESSED) {
    switch (key) {
    case SPACE_BAR:
      body_set_velocity(player, player_velocity);
      break;
    default:
      body_set_velocity(player, VEC_ZERO);
      break;
    }
  } else if (type == KEY_RELEASED) {
    body_set_velocity(player, VEC_ZERO);
  }
}

// key handeler for start and end screens
void menu_screen_handeler(char key, key_event_type_t type, double held_time,
                   state_t *state) {
  if (type == KEY_PRESSED) {
    switch (key) {
    case SPACE_BAR:
      start_game(state);
      start_transition(state);
      break;
    default:
      break;
    }
  } else if (type == KEY_RELEASED) {
  }
}

// starts the game, initialising the player and other obstacles
void start_game(state_t *state){
  scene_t *old_scene = state->scene;
  state->scene = game_scene_init();
  scene_free(old_scene);
  state->player_alive = true;
  state->speed_c = INIT_SPEED_C;
  state->speed_m = INIT_SPEED_M;
  state->speed_z = INIT_SPEED_Z;
  state->zapper_timer = 0;
  state->laser_timer = 0;
  state->missile_timer = 0;
  state->laser_tick_timer = 0;
  state->coin_timer = 0;
  state->coin_counter = 0;
  state->total_coins_counter = 0;
  state->game_timer = 0;
  state->game_score = 0;
  state->power_activated = false;
  state->powerups = powerup_list_init();
  state->high_score = get_highscore();
  state->stage = MAIN_GAME;
}

// is called once the game is started
void start_transition(state_t *state){
  vector_t transition_vel = {0,500};
  body_t *rect = body_init(rectangle_vertices(MAX.x,MAX.y,0,vec_multiply(0.5,MAX)), INFINITY, color_init(123,46,46));
  body_set_velocity(rect,transition_vel);
  scene_add_body(state->scene, rect);
}

// is called once the player dies
void end_transition(state_t *state){

  double curtain_size = 10;
  double player_change_vel = 200;
  double player_drag = 10;
  double background_drag = 0.5;
  double obs_vel_decrease = 0.7;
  rgb_color_t *curtain_color = color_init(123,46,46);


  scene_remove_all_text(state->scene);
  
  state->game_timer = 0;
  state->stage = END_TRANSITION;
  sdl_on_key(NULL);
  body_t *player = scene_get_body(state->scene, find_player(state->scene));
  vector_t vel = body_get_velocity(player);
  for (size_t i = 0; i < scene_bodies(state->scene); i++){
    body_t *body = scene_get_body(state->scene, i);
    body_set_velocity(body,vec_multiply(obs_vel_decrease, body_get_velocity(scene_get_body(state->scene, i))));
    if (((info_t *)body_get_info(body))->type == BACKGROUND){
       create_drag(state->scene, background_drag, body);
    }
    
  }
  body_set_velocity(player, (vector_t){vel.x+player_change_vel,vel.y});
  create_drag(state->scene, player_drag, player);
  body_t *rect = body_init(rectangle_vertices(MAX.x,curtain_size*MAX.y,0,(vector_t){MAX.x/2,(curtain_size*MAX.y/2)+(MAX.y)}), INFINITY, curtain_color);
  scene_add_body(state->scene, rect);
}

// cleares the game screen and initialses the end screen
void end_game(state_t *state){
  scene_t *old_scene = state->scene;
  scene_free(old_scene);
  state->speed_c = INIT_SPEED_C;
  state->speed_m = INIT_SPEED_M;
  state->speed_z = INIT_SPEED_Z;
  state->zapper_timer = 0;
  state->laser_timer = 0;
  state->laser_tick_timer = 0;
  state->missile_timer = 0;
  state->coin_counter = 0;
  if(state->game_score > state->high_score){
      state->high_score = state->game_score;
      update_highscore(state->game_score);
      state->scene = highscore_scene_init();
  }
  else{
    state->scene = end_scene_init(state);
  }
  state->powerups = powerup_list_init();
  state->stage = END_SCREEN;
}

// initialises powerup_t with given type
powerup_t *powerup_init(size_t type){
    powerup_t *powerup = malloc(sizeof(powerup_t));
    powerup->powerup_timer = 0;
    powerup->powerup_on = false;
    powerup->powerup_type = type;
    return powerup;
}

// initialises powerups list in state
list_t *powerup_list_init(){
    list_t *powerups = list_init(2, (free_func_t) free);
    list_add(powerups, powerup_init(COIN_MAGNET));
    list_add(powerups, powerup_init(INVINCIBILITY));
    return powerups;
}

// Returns whether or not a powerup is on
bool powerup_on(list_t *powerups){
    for(size_t i = 0 ; i < list_size(powerups); i++){
        if(((powerup_t *)list_get(powerups, i))->powerup_on){
            return true;
        }
    }
    return false;
}

//Returns the powerup that is on
powerup_t *find_powerup_on(list_t *powerups){
    for(size_t i = 0 ; i < list_size(powerups); i++){
        powerup_t *powerup = (powerup_t *)list_get(powerups, i);
        if(powerup->powerup_on){
            return powerup;
        }
    }
    return NULL;
}

void powerup(state_t *state, double dt){
    if(state->power_activated == true && !powerup_on(state->powerups)){
        srand(time(NULL));
        size_t type = rand() % NUM_POWERUPS;
        powerup_t *powerup = list_get(state->powerups, type);
        switch(type){
            case COIN_MAGNET:
                coin_magnet_tick(state, powerup, dt);
                break;
            case INVINCIBILITY:
                invincibility_tick(state, powerup, dt);
                break;
            default:
                break;
        }
    }
    else if(powerup_on(state->powerups)){
        powerup_t *powerup = find_powerup_on(state->powerups);
        switch(powerup->powerup_type){
            case COIN_MAGNET:
                coin_magnet_tick(state, powerup, dt);
                break;
            case INVINCIBILITY:
                invincibility_tick(state, powerup, dt);
                break;
            default:
                break;
        }
    }
}

//generates coin magnet for given time interval if 10 coins are collected
void coin_magnet_tick(state_t *state, powerup_t *magnet, double dt){
    if(!magnet->powerup_on){
        magnet->powerup_on = true;
        body_t *player = scene_get_body(state->scene, find_player(state->scene));
        for(size_t i = 0; i < scene_bodies(state->scene); i++){
            body_t *coin = scene_get_body(state->scene, i);
            if(((info_t *)body_get_info(coin))->type == COIN){
                create_onesided_gravity(state->scene, 1e7, player, coin);
            }
        }
        text_t *mag = text_init("COIN MAGNET",(vector_t){1750,50},(rgb_color_t){1,1,0},1);
        scene_add_text(state->scene, mag);
    }
    if(magnet->powerup_on){
        magnet->powerup_timer += dt;
    }
    if(magnet->powerup_timer >= COIN_MAGNET_DURATION && magnet->powerup_on){
        scene_remove_text(state->scene, scene_text(state->scene)-1);
        magnet->powerup_timer = 0;
        magnet->powerup_on = false;
        state->power_activated = false;
    }
    
}

//generates invincibility shield
void invincibility_tick(state_t *state, powerup_t *invincibility, double dt){ 
    if(!invincibility->powerup_on){
        list_t *rectangle = rectangle_vertices(20, MAX.y - MIN.y, 0, (vector_t){PLAYER_INIT_POSITION.x + 200, MAX.y * 0.5});
        info_t *info = malloc(sizeof(info_t));
        info->type = SHIELD;
        body_t *shield = body_init_with_info(rectangle, 5, color_init(125, 245, 200), info, (free_func_t) free);
        for(size_t i = 0; i < scene_bodies(state->scene); i++){
            body_t *obstacle = scene_get_body(state->scene, i);
            info_t *info = (info_t *)body_get_info(obstacle);
            if(info->type == ZAPPER || info->type == MISSILE){
                create_semi_destructive_collision(state->scene, 1,shield, obstacle);
            }  
        }
        scene_add_body(state->scene, shield);
        state->laser_tick_timer = LASER_DURATION;
        state->laser_timer = 200;

        text_t *inv = text_init("INVINCIBILITY",(vector_t){1750,50},(rgb_color_t){0,0.9,1},1);
        scene_add_text(state->scene, inv);
        invincibility->powerup_on = true;
    }
    if(invincibility->powerup_on){
        invincibility->powerup_timer += dt;
    }
    if(invincibility->powerup_timer >= INVINCIBILITY_DURATION && invincibility->powerup_on){
        scene_remove_text(state->scene, scene_text(state->scene)-1);
        invincibility->powerup_on = false;
        invincibility->powerup_timer = 0;
        scene_remove_body(state->scene, find_shield(state->scene));
        state->power_activated = false;
    }
}

// generates zappers for given time interval
void add_zappers(state_t *state){
    if (state->zapper_timer >= ZAPPER_TIME) {
      scene_add_body(state->scene, zapper_generate(MAX, MIN, state->speed_z));
      create_collision(state->scene, scene_get_body(state->scene, find_player(state->scene)), 
      scene_get_body(state->scene, scene_bodies(state->scene)-1), ch_obstacle, state, NULL);
      rotate_zapper(scene_get_body(state->scene, scene_bodies(state->scene) - 1));
      state->zapper_timer = 0;
      if(((powerup_t *)list_get(state->powerups, INVINCIBILITY))->powerup_on){
          create_semi_destructive_collision(state->scene, 1, scene_get_body(state->scene, find_shield(state->scene)), 
          scene_get_body(state->scene, scene_bodies(state->scene) - 1));
      }
  }
}

//generates misiles for given time interval
void add_missiles(state_t *state){
    if (state->missile_timer >= MISSILE_TIME) {
      scene_add_body(state->scene, missile_generate(MAX, MIN, state->speed_m));
      create_collision(state->scene, scene_get_body(state->scene, find_player(state->scene)), 
      scene_get_body(state->scene, scene_bodies(state->scene)-1), ch_obstacle, state, NULL);
      state->missile_timer = 0;
      if(((powerup_t *)list_get(state->powerups, INVINCIBILITY))->powerup_on){
          create_semi_destructive_collision(state->scene, 1, scene_get_body(state->scene, find_shield(state->scene)), 
          scene_get_body(state->scene, scene_bodies(state->scene) - 1));
      }
  }
  
}

//generates coins for given time interval
void add_coins(state_t *state){
    if (state->coin_timer >= COIN_TIME) {
      srand(time(NULL));
      if (state->total_coins_counter % POWERUP_START == 0 && state->total_coins_counter > 0){
        scene_add_body(state->scene, coin_power_generate(MAX, MIN, state->speed_c));
        create_collision(state->scene, scene_get_body(state->scene, find_player(state->scene)), 
          scene_get_body(state->scene, scene_bodies(state->scene)-1), ch_power_coin, state, NULL);
      } else {
        scene_add_body(state->scene, coin_generate(MAX, MIN, state->speed_c));
        create_collision(state->scene, scene_get_body(state->scene, find_player(state->scene)), 
        scene_get_body(state->scene, scene_bodies(state->scene)-1), ch_coin, state, NULL);
        if(((powerup_t *)list_get(state->powerups, COIN_MAGNET))->powerup_on){
          create_onesided_gravity( state->scene, G, scene_get_body(state->scene, find_player(state->scene)),
        scene_get_body(state->scene, scene_bodies(state->scene) - 1));
        }
      }
      state->total_coins_counter += 1;
      state->coin_timer = 0;
  }
}

void ch_coin(body_t *player, body_t *coin, vector_t axis, void *aux){
    body_remove(coin);
    ((state_t *)aux)->coin_counter++;
    ((state_t *)aux)->game_score += COIN_POINTS;
}

void ch_power_coin(body_t *player, body_t *coin, vector_t axis, void *aux){
    body_remove(coin);
    ((state_t *)aux)->power_activated = true;
}

void ch_obstacle(body_t *player, body_t *obstacle, vector_t axis, void *aux){
    ((state_t *)aux)->player_alive = false;
}

// controls laser motion for a given time interval
void laser_tick(state_t *state, double dt){
    double empty_time = (SPAWN_X_POSITION_Z - MIN.x) / state->speed_z;
    state->laser_tick_timer += dt;
    if(state->laser_tick_timer >= empty_time && state->laser_timer < 100){
        scene_add_body(state->scene, laser_generator(MAX, MIN));
        state->laser_timer = 100;
        state->laser_tick_timer = 0;
    }
    if(state->laser_timer >= 100 && state->laser_timer < 200 && state->laser_tick_timer >= LASER_CHARGE_TIME){
        body_t *laser = scene_get_body(state->scene, find_laser(state->scene));
        body_add_polygon(laser, body_get_shape(laser), color_init(255, 0 , 0));
        create_collision(state->scene, scene_get_body(state->scene, find_player(state->scene)), 
        laser, ch_obstacle, state, NULL);
        state->laser_tick_timer = 0;
        state->laser_timer = 200;
    }
    if(state->laser_timer >= 200 && state->laser_tick_timer >= LASER_DURATION){
        if(find_laser(state->scene) != -1){
            scene_remove_body(state->scene, find_laser(state->scene));
        }
        state->laser_timer = 0;
        state->laser_tick_timer = 0; 
        state->zapper_timer = ZAPPER_TIME * 0.5;
    }
}

// stops player from leaving window
void player_boundary(body_t *player, body_t *gravity_body){
  if (body_get_centroid(player).y - HALF_LENGTH_P <= MIN.y) {
    body_set_centroid(player, (vector_t){body_get_centroid(player).x, HALF_LENGTH_P});
    neg_gravity(player, gravity_body);
  }
  if (body_get_centroid(player).y + HALF_LENGTH_P >= MAX.y) {
    body_set_centroid(player, (vector_t){body_get_centroid(player).x, MAX.y - HALF_LENGTH_P});
    body_set_velocity(player, VEC_ZERO);
  }
}

// Function that controlls what happens to objects off the screen
void off_screen(state_t *state){
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
    body_t *body = scene_get_body(state->scene, i);
    if (((info_t *)body_get_info(body))->type == BACKGROUND){
      background_wraparound(body);
    } 
    else if (body_get_centroid(body).x < DISTANCE_FREE){
      body_remove(scene_get_body(state->scene, i));
    } else if (body_get_centroid(body).y > 2*MAX.y) {
      body_remove(scene_get_body(state->scene, i));
    }
  }
}

// adds a body to the scene that mimics earth's gravity
void gravity_body_generator(scene_t *scene) {
  list_t *gravity_ball = rectangle_vertices(1, 1, 0, VEC_ZERO);
  info_t *info = malloc(sizeof(info_t));
  assert (info != NULL);
  info->type = GRAVITY;
  info->control_velocity = P_REGULAR_VELOCITY;
  body_t *body = body_init_with_info(gravity_ball, M, color_to_pointer(WALL_COLOR),
                                     info, free);
  vector_t gravity_center = {.x = 0, .y = -R/10};
  body_set_centroid(body, gravity_center);
  scene_add_body(scene, body);
}

// returns the index of the player body within the scene
int find_player(scene_t *scene) {
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    if (((info_t *)body_get_info(scene_get_body(scene, i)))->type ==
        PLAYER) {
      return i;
    }
  }
  return 0;
}

// returns the index of the gravity body within the scene
int find_gravity(scene_t *scene) {
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    if (((info_t *)body_get_info(scene_get_body(scene, i)))->type ==
        GRAVITY) {
      return i;
    }
  }
  return -1;
}

//returns the index of the shield body
int find_shield(scene_t *scene) {
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    if (((info_t *)body_get_info(scene_get_body(scene, i)))->type ==
        SHIELD) {
      return i;
    }
  }
  return -1;
}

// returns the index of the laser body within the scene
int find_laser(scene_t *scene) {
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    if (((info_t *)body_get_info(scene_get_body(scene, i)))->type ==
        LASER) {
      return i;
    }
  }
  return -1;
}

// updates highscore
void update_highscore(double new_high_score){
    FILE *file = fopen("/highscore.txt", "w");
    char buff[100];
    assert(file != NULL);
    sprintf(buff, "%f", new_high_score);
    fputs(buff, file);
    fclose(file);
}

//returns highscore
double get_highscore(){
    FILE *file = fopen("/highscore.txt", "r");
    double highscore = 0;
    char buff[100];
    char *endptr;
    if(file != NULL){
        fgets(buff, 100, file);
        highscore = strtod(buff, &endptr);
        fclose(file);
    }
    return highscore;
}

// creates a normal force on the player
void neg_gravity(body_t *player, body_t *gravity_body){
    double r = get_distance(body_get_centroid(player), body_get_centroid(gravity_body));
    vector_t gravity = vec_multiply(
        G * (body_get_mass(player) * body_get_mass(gravity_body)) / pow(r, 3),
        vec_subtract(body_get_centroid(player), body_get_centroid(gravity_body)));
    body_add_force(player, gravity);
}