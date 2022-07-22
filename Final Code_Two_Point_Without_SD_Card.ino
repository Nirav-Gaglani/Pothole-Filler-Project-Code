#include <HCSR04.h>

HCSR04 hc(8, 9); //initialisation class HCSR04 (trig pin , echo pin)

HCSR04 ic(17, 18); //initialising HCSR04 attached to monitor storage tank depth

int rec_mxF=0;
int rec_myF=0;
int rec_mxR=0;
int rec_myR=0;
int pos_mxs=0;
int pos_yfr=0;
int togo_y=0;
int togo_x=0;
int y_till=0;
int x_till=0;
int flag_x_till=0;
int fill_xlow[24]={0};
int fill_xhigh[24]={0};
int ystart=0;
int yend=0;
int xmid=0;
int ymid=0;

int ystartflag=0;
int xlow_flag=0;
int rec_Fflag=0;
int rec_Rflag=0;
int half_flag=0;
int dir_count=0;
int dcount[24]={0};//it records the number of target datapoints for each strip


float depth=0;
float store_depth[24]={0};
float length_strip=0;
float width_strip=1.6;
float volume=0;

float tank_w = 22.9; //in cm
float tank_l = 23.3;//in cm
//float tank_d = 20; //----------------Replace with the filled height-----------------//in cm
float mdep=0;
float tar_dep=0;
float ini_dep=0;
float final_dep=0;

const int dirPin_1 = 2;
const int stepPin_1 = 3;

const int dirPin_2 = 6;
const int stepPin_2 = 7;

const int dirPin_3 = 4;
const int stepPin_3 = 5;

const int dirPin_4 = 11;
const int stepPin_4 = 12; 


void setup() {

  Serial.begin(9600);
  
  pinMode(stepPin_1, OUTPUT);
  pinMode(dirPin_1, OUTPUT);

  pinMode(stepPin_2, OUTPUT);
  pinMode(dirPin_2, OUTPUT);

  pinMode(stepPin_3, OUTPUT);
  pinMode(dirPin_3, OUTPUT);

pos_mxs=0;

 for(int FR=0; FR<2; FR++){
  
  if(FR%2==0){
  digitalWrite(dirPin_1, LOW);
  digitalWrite(dirPin_2, HIGH); 
    }
  else{
  digitalWrite(dirPin_1, HIGH);
  digitalWrite(dirPin_2, LOW);
  rec_Rflag=1;
    }

  for(int my=0; my<12; my++){//----------------global or local ?????----------------

    if(pos_mxs==0){
         digitalWrite(dirPin_3, HIGH);
         Serial.println("Wrote dirPin_3, HIGH");
            }
    if(pos_mxs==4200){
         digitalWrite(dirPin_3, LOW);
         Serial.println("Wrote dirPin_3, LOW");
           }
    if(pos_mxs>0 && pos_mxs<4200){
      digitalWrite(stepPin_3, LOW);
      Serial.println("line *_*_*_*_*_*_*_*_*117*_*_*_*_*_*_*_*_*_*_");
      Serial.println("pos_mxs between 0 & 4200, wrote stepPin_3 LOW");
      Serial.println("Unplug Arduino with in 10 SECONDS");
      Serial.println("Change The Code Please:(");
      delay(10000);
      }
        
    if(rec_Fflag!=1 || rec_Rflag==1){
    for(int mx=0; mx<21; mx++){
      for(int mxf=0; mxf<200; mxf++){
          digitalWrite(stepPin_3, HIGH);
          delayMicroseconds(2000);
          digitalWrite(stepPin_3, LOW);
          delayMicroseconds(2000);
          if(my%2==0){
            pos_mxs++;
            }
           else{
              pos_mxs--;
              }
        }
        delay(60);
        depth = hc.dist();
        Serial.println(depth);
        if(rec_Fflag!=1){
         if(depth>22){
           rec_mxF=pos_mxs;
           rec_myF=pos_yfr;
           Serial.println("Recorded First *FORWARD* Depth Change Point");
           Serial.print(depth);
           Serial.print(" at ");
           Serial.print(rec_mxF);
           Serial.print(" ");
           Serial.println(rec_myF);
           rec_Fflag=1;
           }
          }
         if(rec_Rflag==1){
          if(depth>22){
            rec_mxR=pos_mxs;
            rec_myR=pos_yfr;
            Serial.println("Recorded First *REVERSE* Depth Change Point");
           Serial.print(depth);
           Serial.print(" at ");
           Serial.print(rec_mxR);
           Serial.print(" ");
           Serial.println(rec_myR);
            rec_Rflag=2;
            }
          }
      }
     }

      for(int jm = 0; jm < 200; jm++)
      {
        digitalWrite(stepPin_1, HIGH);
        digitalWrite(stepPin_2, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin_1, LOW);
        digitalWrite(stepPin_2, LOW);
        delayMicroseconds(2000);
      }
      if(FR%2==0){
      pos_yfr++;
      }
      else{
        pos_yfr--;
        }
    }

    if(pos_mxs==4200){
     digitalWrite(dirPin_3, LOW);
     Serial.println("MOTOR_3 at 4200 steps, Bringing MOTOR_3 to 0 steps");
      while(pos_mxs!=0){
          digitalWrite(stepPin_3, HIGH);
          delayMicroseconds(2000);
          digitalWrite(stepPin_3, LOW);
          delayMicroseconds(2000);
          pos_mxs--;
        }
        delay(10);
      }
    else{
      Serial.println("MOTOR_3 already at 0 steps");
      }
     if(pos_mxs>0 && pos_mxs<4200){
      Serial.println("*_*_*_*_*_*_*_*_line 190*_*_*_*_*_*_*_*_*");
      Serial.println("MOTOR_3 positon between 0 & 4200, NEED TO CHANGE THE CODE");
      Serial.println("Unplug Arduino within 10 SECONDS");
      delay(10000);
      }   
}

Serial.println("-----------Current POSITIONS-------------");
Serial.print("Y Position: ");
Serial.println(pos_yfr);
Serial.print("X Position: ");
Serial.println(pos_mxs);
Serial.println("*_*_*_*_*_*_*_Going again near the First FORWARD Depth Change Point_*_*_*_*_*_*_*");
Serial.println(" ");

//-------------------------Going back to First FORWARD Depth Change Point-----------------------------------------------------------------------
//-----here onwards consider yfr-->yhr && mxs-->ms---------

digitalWrite(dirPin_1, LOW);
digitalWrite(dirPin_2, HIGH);

pos_yfr=0;

if(rec_myF!=0){
  togo_y = ((rec_myF*2)-2);
  Serial.print(togo_y);
  Serial.println(" Half Revolutions to go to FORWARD Depth Change Point");
  }
else{
  togo_y=0;
  Serial.println("Depth change in 0th Strip");
  }
while(pos_yfr!=togo_y){
  for(int a=0; a<100; a++){
        digitalWrite(stepPin_1, HIGH);
        digitalWrite(stepPin_2, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin_1, LOW);
        digitalWrite(stepPin_2, LOW);
        delayMicroseconds(2000);  
    }
    pos_yfr++;
  }

if(rec_mxF!=0){
  togo_x = (rec_mxF-200);
  Serial.print(togo_x);
  Serial.println(" x steps to go to FIRST DEPTH CHANGE POINT");
  }
  else{
    togo_x = 0;
    }

pos_mxs=0;
digitalWrite(dirPin_3, HIGH);
while(pos_mxs!= togo_x){
          digitalWrite(stepPin_3, HIGH);
          delayMicroseconds(2000);
          digitalWrite(stepPin_3, LOW);
          delayMicroseconds(2000);
          pos_mxs++;
  }
  
y_till = ((rec_myR*2)+2);
if(y_till > 24){
  y_till = 24;
  }
  
x_till = (rec_mxR+200);
if(x_till > 4200){
  x_till = 4200;
  }
  
dir_count=0;

for(int ystrip = togo_y; ystrip<=y_till ; ystrip++){

xlow_flag=0;
  
  if(dir_count%2==0){
    digitalWrite(dirPin_3, HIGH);
    Serial.println("wrote MOTOR_3 HIGH line 281");
    }
    else{
      digitalWrite(dirPin_3, LOW);
      Serial.println("wrote MOTOR_3 LOW line 285");
      }
       
  for(int xto=1; xto<=210; xto++){
    if(ystrip == togo_y && half_flag==0){
      xto=(togo_x/20);
      half_flag=1;
      Serial.println("half loop started successfully");
      }

    if(ystrip == y_till && flag_x_till == 0){
      if(dir_count%2==0){
      xto = ((4200-x_till)/20);
      Serial.println("*_*_*_*_*_*_*_*_last strip_*_*_*_*_*_*_*_*_*_*");
      Serial.print("xto value increased to ");
      Serial.println(xto);
      flag_x_till = 1;
      }
      else{
      xto = (210-((4200-x_till)/20));
      Serial.println("*_*_*_*_*_*_*_*_last strip_*_*_*_*_*_*_*_*_*_*");
      Serial.print("xto value increased to  ");
      Serial.println(xto);
      flag_x_till = 1;
        }
      }
    
    for(int xtwen=0; xtwen<20; xtwen++){
       digitalWrite(stepPin_3, HIGH);
       delay(2);
       digitalWrite(stepPin_3, LOW);
       delay(2);
      if(dir_count%2==0){
        pos_mxs++;
       }
      else{
        pos_mxs--;
        }
    }
    
    depth = hc.dist();
    Serial.println(depth);

              if(depth>17.00){
                store_depth[ystrip]= store_depth[ystrip] + depth;
                dcount[ystrip]++;
                if(ystartflag==0){
                  ystart=ystrip;
                  ystartflag=1;
                  }
                yend = ystrip;
                if(xlow_flag==0){
                fill_xlow[ystrip] = pos_mxs;
                xlow_flag=1;
                }
                fill_xhigh[ystrip] = pos_mxs;
                    }
                  delay(60);
  }
  
  if(store_depth[ystrip]>0){
  store_depth[ystrip] = (store_depth[ystrip]/(dcount[ystrip]));
  }
  
  Serial.print("X range for ");
  Serial.print(ystrip);
  Serial.print("th strip is ");
  Serial.print(fill_xlow[ystrip]);
  Serial.print(" - ");
  Serial.println(fill_xhigh[ystrip]);
  
    for(int yf = 0; yf < 100; yf++)
      {
        digitalWrite(stepPin_1, HIGH);
        digitalWrite(stepPin_2, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin_1, LOW);
        digitalWrite(stepPin_2, LOW);
        delayMicroseconds(2000);
      }
      pos_yfr++;
      dir_count++;
 }
//______________________Bringing Back to Initial Position_________________________________
digitalWrite(dirPin_1, HIGH);
digitalWrite(dirPin_2, LOW);

while(pos_yfr != 0){
  for(int y_back=0; y_back<100; y_back++){
    digitalWrite(stepPin_1, HIGH);
    digitalWrite(stepPin_2, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin_1, LOW);
    digitalWrite(stepPin_2, LOW);
    delayMicroseconds(2000);
  }
  pos_yfr--;
  delay(10);
}

 //-----------------*_*_*_*_*_*_*_*_*_*_*_Calculating Volume_*_*_*_*_*_*_*_*_*_*_*-----------------------------

  
  for(int j=0; j<24; j++){
    Serial.print("Average Depth ");
    Serial.print(j);
    Serial.print(": ");
    Serial.println(store_depth[j]);
    length_strip = dcount[j]*0.32;
    volume = (volume + (store_depth[j]*length_strip*width_strip));
    Serial.print("Volume addition: ");
    Serial.println(volume);
    }

    Serial.println("-------------------------------------------------------------------------------");
    Serial.println("-------------------------------------------------------------------------------");
    Serial.println("-------------------------------------------------------------------------------");
    Serial.println(volume);

Serial.print("Pothole Y range is ");
Serial.print(ystart);
Serial.print(" - ");
Serial.println(yend);

digitalWrite(dirPin_3,LOW);
//-----------------------------X initial position--------------------
while(pos_mxs!=0){
  digitalWrite(stepPin_3, HIGH);
  delay(2);
  digitalWrite(stepPin_3, LOW);
  delay(2);
  pos_mxs--;
  }


//-------------filling point at center-------------------
ymid = ((ystart + yend)/2);
xmid = (((fill_xlow[ymid])+(fill_xhigh[ymid]))/2);
Serial.print("*_*_*_*_*_*_*_mid point is X-");
Serial.print(xmid);
Serial.print(" Y-");
Serial.println(ymid);

//---------------------Going to filling point-------------------------------
digitalWrite(dirPin_1, LOW);
digitalWrite(dirPin_2, HIGH);
digitalWrite(dirPin_3, HIGH);

while(pos_yfr!=ymid){
  for(int tmy=0; tmy<100; tmy++){
  digitalWrite(stepPin_1, HIGH);
  digitalWrite(stepPin_2, HIGH);
  delay(2);
  digitalWrite(stepPin_1, LOW);
  digitalWrite(stepPin_2, LOW);
  delay(2);
  }
  pos_yfr++;
  }
Serial.print("Y position at: ");
Serial.println(pos_yfr);

while(pos_mxs!=xmid){
  digitalWrite(stepPin_3, HIGH);
  delay(2);
  digitalWrite(stepPin_3, LOW);
  delay(2);
  pos_mxs++;
  }
Serial.print("X position at: ");
Serial.println(pos_mxs);

//---------------------Filling--------------------------------------------------

tar_dep = ((volume)/(tank_w*tank_l));
Serial.print("Target Depth is: ");
Serial.println(tar_dep);

delay(200);
ini_dep = ic.dist();
delay(150);
Serial.print("Current tank depth is: ");
Serial.println(ini_dep);
final_dep = ini_dep + tar_dep;
Serial.print("Final depth will be: ");
Serial.println(final_dep);

digitalWrite(dirPin_4, HIGH);

  delay(150);
  ini_dep = ic.dist();

  for(int tank_open=0; tank_open<17; tank_open++){
  digitalWrite(stepPin_4, HIGH);
  delay(10);
  digitalWrite(stepPin_4, LOW);
  delay(10);
  }
  
  while(ini_dep<=final_dep){
    delay(150);
    Serial.println("Material Outlet is Open");
    ini_dep = ic.dist();
    }
    
digitalWrite(dirPin_4, LOW);
for(int tank_open=0; tank_open<17; tank_open++){
  digitalWrite(stepPin_4, HIGH);
  delay(100);
  digitalWrite(stepPin_4, LOW);
  delay(100);
  }

Serial.println("Material Outlet Closed");
Serial.println("***___...***___***...Whole Process Completed...***___***...***___***");
        
}


void loop(){
  
  }
