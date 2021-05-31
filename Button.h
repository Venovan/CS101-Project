#ifndef _PLAYBUTTON_H_
#define _PLAYBUTTON_H_

class Button{
       public:
     float x, y, w, h;
     short R, G, B;
     Rectangle button;
     Text txt;
     string mssg;
     bool is_hidden;


     Button(string argmssg, float argx, float argy, float argw, float argh, short argR, short argG, short argB, bool arghide): mssg(argmssg), x(argx), y(argy), w(argw), h(argh), R(argR), G(argG), B(argB), is_hidden(arghide){
     if (!is_hidden){
                  txt={x, y, mssg};
                  button={x, y, w, h};
                  button.setColor(COLOR(R, G, B));
               }
     else{
            txt={x, y, ""};}}

    void toggle_hide_Button(){
                            if (is_hidden){txt.setMessage(mssg);
                                           button={x, y, w, h};
                                           button.setColor(COLOR(R, G, B));
                                            }
                            else {
                            button ={};
                            txt.setMessage("");
                                }

                             is_hidden=!is_hidden;
                              }


    bool if_clicked(float m, float n){             //check if clicked on button
                                    if(fabs(m-x)<w/2 && fabs(n-y)<h/2){
                                                                        return true;
                                                                        }
                                     }

    void Button_change_color(short r, short g, short b){
                                                  R=r; G=g; B=b;
                                                  button.setColor(COLOR(R, G, B));
                                                  }
               };


#endif

