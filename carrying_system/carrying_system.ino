#include <motor.h>
#include <interrupter.h>
#include <diode.h>

#include <SoftwareSerial.h>


void lift()
{
  boolean lifted = false;
  int error = 0;
  forward();
  delay(2000);
  while(checkDiode() == true)
  {
  }
  reverse();
  delay(10);
  stay();
  delay(5000);
  reverse();
  delay(4750);
  stay();
}

void drop()
{
  reverse();
  while(checkDiode() == true)
  {
  }
  delay(200);
  forward();
  delay(210);
  stay();
}

void up()
{
  reverse();
  delay(500);
  stay();
}

void down()
{
  forward();
  delay(500);
  stay();
}

void initialisation()
{
  forward();
  delay(1000);
  stay();
  delay(500);
  reverse();
  while(checkDiode() == true)
  {
  }
  forward();
  delay(6);
  stay();
}

void setup() {
  Serial.begin(115200);  
  initMotor(4,7);
  initDiode(A6);
  initSwitch(9);

}



void loop() {
  //////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////
  //       Read serial port                                       //
  //////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////
  String ligne;
  char line[1024] = {
    0      };
  int lineSize = 0; 
  if (Serial.available() > 0)
  {
    // read the incoming Line:
    lineSize = Serial.readBytesUntil('\n', line, 1024);
  }
  //////////////////////////////////////////////////////////////////
  //            Extact usefull information                        //
  //       Trame format  : AT*CmdName = parametre /n              //
  //////////////////////////////////////////////////////////////////
  
  if((line[3]=='L') && (line[4]=='I') && (line[5]=='F') && (line[6]=='T'))
  {
    Serial.write("AT*ACK=LIFT\n");
    lift();
    Serial.write("AT*ACK=LIFTED\n");
  }
  else if((line[3]=='D') && (line[4]=='R') && (line[5]=='O') && (line[6]=='P'))
  {
    Serial.write("AT*ACK=DROP\n");
    drop(); 
    Serial.write("AT*ACK=DROPED\n");
  }
  else if((line[3]=='I') && (line[4]=='N') && (line[5]=='I') && (line[6]=='T'))
  {
    Serial.write("AT*ACK=INIT\n");
    initialisation(); 
    Serial.write("AT*ACK=INITOK\n");
  }
  else if((line[3]=='U') && (line[4]=='P'))
  {
    Serial.write("AT*ACK=UP\n");
    up(); //AT*=CMD=ARG
    Serial.write("AT*ACK=UPED\n");
  }
  else if((line[3]=='D') && (line[4]=='O') && (line[5]=='W') && (line[6]=='N'))
  {
    Serial.write("AT*ACK=DOWN\n");  
    down(); 
    Serial.write("AT*ACK=DOWNED\n");
  }
  else if((line[3]=='D') && (line[4]=='I') && (line[5]=='O') && (line[6]=='D') && (line[7]=='E'))
  {
    Serial.write("AT*ACK=CHECKDIODE\n");
    if (checkDiode() == true)
    {
      Serial.write("AT*ACK=TRUE\n");
    }
    else
    {
      Serial.write("AT*ACK=FALSE\n");
    }
  }

}




