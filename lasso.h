

#ifndef __LASSO_H__
#define __LASSO_H__

#define WINDOW_X 900
#define WINDOW_Y 700

#define STEP_TIME 0.1

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 50
#define MIN_RELEASE_SPEED 50
#define MAX_RELEASE_SPEED 400
#define INIT_RELEASE_SPEED 300


#define LASSO_G 30


#define LASSO_SIZE 40
#define COIN_SIZE 10
#define THICKNESS 10


double COIN_SPEED;
double COIN_ANGLE_DEG;
double COIN_START_X;
int LEVEL=2;
double COIN_G;


class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  int num_coins;
  int live = 1;
  string lives= "Lives: + + + + +";         //lives remain
  bool gameover=false, game_paused=true;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  int getNumCoins() { return num_coins; }
  void add_level();
  void cut_life();
  string lives_now() { return lives; }
  int getlive() { return live; }
  void change_highest(int);
  void resetscore();
  void game_over_change(bool x){ gameover = x; }           //changes the status of gameover
  bool game_over_status(){return gameover; }             //returns if game has over or not
  string getstrHighscore();                                   //TO GET THE HIGHEST SCORE TILL NOW
  bool pause_status();                                       //if game is paused or not
  bool flip_pause(){game_paused=!game_paused;}
}; // End class Lasso

#endif

