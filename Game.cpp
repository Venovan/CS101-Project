#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "MovingObject.h"
#include "coin.h"
#include "Button.h"
#include "lasso.h"

using namespace simplecpp;

void MovingObject::nextStep(double t) {
  if(paused) { return; }

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}





int rand_int(int MIN, int MAX){
               if(MAX>MIN){
               return std::rand()%(MAX-MIN)+MIN;
                          }
               else cout<<"swap the arguement.";
                              }



//coin function defined form here

void Coin::initCoin() {
  coin_start_x =  rand_int((WINDOW_X+3*PLAY_X_START)/4, WINDOW_X);
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR(255, 255, 0));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin() {
  COIN_START_X=rand_int((WINDOW_X+3*PLAY_X_START)/4, WINDOW_X);               //give random value of x coordinate for launch
  COIN_SPEED = rand_int(200, 300);                                            //gives a random speed
  if (COIN_START_X <(WINDOW_X+PLAY_X_START)/2){                                   //ensure coin dont launches away from the screen
                                         COIN_ANGLE_DEG = rand_int(30, 90);
                                         }
  else COIN_ANGLE_DEG = rand_int(90, 160);
  COIN_G = rand_int(50, 100);                                                 //selects a random value of gravity
  isbomb = false;                                                           //whenever coin resets, it start as a gold coin even when
  this->coin_circle.setColor(COLOR("yellow"));                              //it was bomb previously
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(COIN_START_X, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::try_bomb(){
                if (!this->isPaused()){
                         if (rand_int(0, 100)<LEVEL){
                                           if (isbomb){
                                                     coin_circle.setColor(COLOR(255, 255, 0));
                                                     isbomb=false;
                                                     return;
                                                      }
                                           else{
                                                coin_circle.setColor(COLOR(51, 51, 51));
                                                isbomb=true;
                                                return;}
                                                }
                         return;
                            }
                                             }


//coin function definitons end here


//lasso function defined from here
void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(0, 128, 0));          //color of outer ring green at 10
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE-THICKNESS);
  lasso_loop.setColor(COLOR(0, 255, 0));      //inner circle light green at 4
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(153, 204, 0));              //color of line after firing green 43
  lasso_band.setColor(COLOR(51, 51, 0));               //color of lasso  at 52 in chart
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE-THICKNESS);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
                       if (the_coin->isbomb){                        //check if coin was a bomb or not
                                    if (num_coins>0) num_coins--;   //cut coin only when they are positive so dont go negative
                                    cut_life();                       // but do cut one life
                                    the_coin->resetCoin();
                                    the_coin = NULL;  }
                      else{
                           num_coins++;
                           the_coin->resetCoin();
                           the_coin =NULL;
                           }
                        }
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_SIZE);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_SIZE-THICKNESS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
}


void Lasso::resetscore(){
                         num_coins = 0;
                         lives= "Lives: + + + + +";
                                                    }




void Lasso::cut_life(){
                    lives=lives.substr(0, lives.size()-2);
                    live--;
                    if (live<=0){
                                gameover = true;
                                  }

                    }



void Lasso::add_level(){LEVEL+=2;}

void Lasso::change_highest(int score){
                    fstream score_file;
                    score_file.open("Highscore.txt", ios::out);
                    score_file<<score;
                    score_file.close();
                    return;
                    }
string Lasso::getstrHighscore(){
                            string maxscore;
                            fstream file;
                            file.open("Highscore.txt", ios::in | ios::app);
                            file>>maxscore;
                            if (maxscore==""){file.close();
                                         file.open("Highscore.txt", ios::app);
                                         file<<"0";
                                         maxscore="0";}
                            file.close();
                            return maxscore;
                            }


bool Lasso::pause_status(){
                        return game_paused;
                         }





int main(){
  initCanvas("Lasso", WINDOW_X, WINDOW_Y);

  bool blinker=true;            // game is not over; blinker haelp  in blinking GAMEOVER
  std::srand((int)time(0));
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 3;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // of lasso in m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // of lasso in degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  int int_hs=0;
  bool paused = true;
  bool rtheta = true;

  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);




  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  string highscore("Highest: ");
  Text for_throw(PLAY_X_START/2, 20, "t : throw");                 //to throw lasso
  Text for_yank(PLAY_X_START/2, 40, "y : yank");                   //to yank lasso
  Text for_grab(PLAY_X_START/2, 60, "l : grab");                  //to grab the coin
  Text for_play_again(PLAY_X_START/2, 80, "r : restart");                      //to restart the game after   GAMEOVER
  Text for_play_pause(PLAY_X_START/2, 100, "p : play/pause");
  Text for_angle_change(PLAY_X_START/2, 120, "a / z: angle");
  Text for_speed_change(PLAY_X_START/2, 140, "n / m: speed");
  Text notice1(PLAY_X_START/2, PLAY_Y_HEIGHT-160, "Notice:");
  Text notice2(PLAY_X_START/2, PLAY_Y_HEIGHT-140, "ensure caps");
  Text notice3(PLAY_X_START/2, PLAY_Y_HEIGHT-120, "lock is off");


  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  Text Highscore(PLAY_X_START+50, PLAY_Y_HEIGHT+80, (highscore+lasso.getstrHighscore()));               //prints the highest score on screen
  Text live(PLAY_X_START+200, PLAY_Y_HEIGHT+20, lasso.lives_now());                                 //prints num of lives
  Text GAMEOVER((PLAY_X_START+WINDOW_X)/2, PLAY_Y_HEIGHT/2, "");                               //prints game over after loosing all lives
  Text paused_status_game(PLAY_X_START+600, PLAY_Y_HEIGHT+20, "Press 'p' or the button to play.");

  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  paused = true; rtheta = true;
  COIN_SPEED = rand_int(100, 250);
  COIN_ANGLE_DEG = rand_int(90, 160);
  double coin_ax = 0;
  COIN_G = rand_int(50, 100);
  Button playButton("PLAY", (WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2, 60, 30, 255, 0, 0, false);


  Coin coin(COIN_SPEED, COIN_ANGLE_DEG, coin_ax, COIN_G, paused, rtheta);


  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 1;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit


for(;;) {                 //infinte loop could also have used while(true)
    if((runTime > 0) && (currTime > runTime)) { break; }

   XEvent e;          //built class XEvent not use nextevent to get rid of waitning
   if(checkEvent(e)){
                  if (mouseButtonPressEvent(e)){
                                      float lastx=e.xbutton.x, lasty=e.xbutton.y;
                                      if(lasso.pause_status() && playButton.if_clicked(lastx, lasty)){
                                                                                             playButton.toggle_hide_Button(); //hideplay
                                                                                             //unhide pause
                                                                                             lasso.flip_pause();  //resume game
                                                                                             paused_status_game.setMessage("Press 'p' to pause.");


                                                                                                         }


                                      }
               else if (keyPressEvent(e)){
                   char c = charFromEvent(e);
                   msg[msg.length()-1] = c;
                   charPressed.setMessage(msg);
                   if (c=='p'){
                                if (lasso.pause_status()){
                                                        lasso.flip_pause();
                                                        playButton.toggle_hide_Button();
                                                        paused_status_game.setMessage("Press 'p' to pause.");
                                                          }
                                else{
                                    lasso.flip_pause();
                                    playButton.toggle_hide_Button();
                                    paused_status_game.setMessage("Press 'p' or the button to play.");}
                                }

                   else if (c=='q'){
                               exit(false);}
                   else if (c=='r'){

                                    coin.resetCoin();
                                    lasso.resetscore();
                                    if (lasso.game_over_status()){

                                                                   coin.unpause();
                                                                   lasso.game_over_change(false);
                                                                   GAMEOVER.setMessage("");
                                                                   }
                                     }

                   if (!lasso.pause_status()){
                                                if (c=='t'){
                                                      lasso.unpause();}
                                                else if(c=='y'){
                                                                lasso.yank();}
                                                else if(c=='l'){
                                                                lasso.loopit();
                                                                lasso.check_for_coin(&coin);
                                                                wait(STEP_TIME*5); }
                                                else if (c=='a'){
                                                                 if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);}}
                                                else if(c=='z'){
                                                                 if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }}
                                                else if (c=='m'){
                                                                 if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP);}}
                                                else if(c=='n'){
                                                                 if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP);}}
                                                }



                                       }
                                    }



if (!lasso.pause_status()){
         coin.try_bomb();       //random chance to become a bomb according to LEVEL


                    if (lasso.game_over_status()){
                          lasso.pause();
                          coin.pause();    //pause coin while lasso already pause
                          if (blinker){
                                        GAMEOVER.setMessage("GAME OVER");
                                        wait(0.5);
                                        blinker= !blinker;
                                        }
                          else {GAMEOVER.setMessage("");
                                wait(0.5);
                                 blinker= !blinker;
                                 }
                          }
            lasso.nextStep(stepTime);
            coin.nextStep(stepTime);


            if(coin.isPaused()) {
                              if((currTime-last_coin_jump_end) >= COIN_GAP) {
                                                                            coin.unpause();
                                                                              }
                                 }

            if(coin.getYPos() > PLAY_Y_HEIGHT) {
                                                coin.resetCoin();
                                                last_coin_jump_end = currTime;
                                                 }


            if(coin.getXPos() < PLAY_X_START || coin.getXPos() > WINDOW_X) {
                                                                           coin.flip_vx();
                                                                           }


            if (coin.getYPos()< 0){
                                   coin.flip_vy();
                                     }





            sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
            coinScore.setMessage(coinScoreStr);
            live.setMessage(lasso.lives_now());

            stringstream x(lasso.getstrHighscore());               //we need highscore in integer format also
            x>>int_hs;
            if (lasso.getNumCoins()>int_hs){
                                             lasso.change_highest(lasso.getNumCoins());
                                             Highscore.setMessage(highscore+lasso.getstrHighscore());
                                             }




    stepCount++;
    currTime += stepTime;
    wait(0);}}
   // End for(;;)

  wait(1);}
// End main_program

