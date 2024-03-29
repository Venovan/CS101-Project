#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {

  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }
  void step(double t){coin_circle.move(getvx()*t, getvy()*t);}
  bool isbomb=false;            //if coin is bomb or not
  void initCoin();
  void resetCoin();
  void try_bomb();

};
#endif
