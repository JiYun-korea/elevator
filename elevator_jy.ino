/*SW개발자 대회_엘리베이터*/

#include <Servo.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

Servo elservo; //서보모터를 elservo로 정의
SoftwareSerial elSerial(4,3); //블루투스 4번핀을 Rx, 3번핀을 Tx 설정
int Trig1 = 13;
int Echo1 = 12; //초음파센서1(1층) 핀설정
int Trig2 = 11;
int Echo2 = 10; //초음파센서2(2층) 핀설정
int Trig3 = 9;
int Echo3 = 8; //초음파센서3(3층) 핀설정
int led_1 = 7;
int led_2 = 6;
int led_3 = 5; //층별 LED 핀 설정
int flor = 1; //층 선언
unsigned char data = 0; //블루투스로 받는 데이터값 지정

    NewPing sonar1(Trig1, Echo1, 10);
    NewPing sonar2(Trig2, Echo2, 10);//NewPing헤더파일을 이용한 초음파센서의 거리 측정
    NewPing sonar3(Trig3, Echo3, 10); //최대 측정 가능 거리 10cm
    
void setup() {
    Serial.begin(9600); //시리얼 모니터 시작
    Serial.write("Hello elevator\n");
    elSerial.begin(9600); //블루투스 시리얼 통신 시작
    elservo.attach(2); 
}

void loop() {
    
    delay(50);
    
    float distance1 = sonar1.ping_cm();
    float distance2 = sonar2.ping_cm();
    float distance3 = sonar3.ping_cm(); //각 초음파센서의 거리를 distance변수에 저장
    
    
    if(elSerial.available()) //블루투스를 한다면 원하는 층값을 받기
    {
      data = elSerial.read();
      
      if(data=='7')  //앱에서 특정 버튼을 누르면 아두이노로 7을 보내도록 설정해둠
      {
        elSerial.write("현재 층수는 : ");
        elSerial.print(flor);
        elSerial.write("층"); //현재 층수를 블루투스에 전송
      }
      else 
      {
        Serial.write("미리 눌려진 층 : ");
      elSerial.write(data);
      elSerial.write("층을 눌렀습니다.\n");
      }
    }
      
    if((distance1 > 1 && distance1 < 9) || data =='1') //1층일때
    {
      digitalWrite(led_1,HIGH);
      Serial.write("1층\n");
      
      if(flor==2)
      {
        for(int i=90; i>0; i--)
        {
          elservo.write(i);
          delay(10);
        }
      }
      
      if(flor==3)
      {
        for(int i=180; i>0; i--)
        {
          elservo.write(i);
          delay(10);
        }
      }
      flor=1;
      digitalWrite(led_1,LOW);
      delay(2000);
    }
    
    
    else if((distance2 > 1 && distance2 < 9) || data =='2') //2층일때
    {
      digitalWrite(led_2,HIGH);
      Serial.write("2층\n");
      
      if(flor==1)
      {
        for(int i=0; i<90; i++)
        {
          elservo.write(i);
          delay(10);
        }
      }
      
      if(flor==3)
      {
        for(int i=180; i>90; i--)
        {
          elservo.write(i);
          delay(10);
        }
      }
      flor=2;
      digitalWrite(led_2,LOW);
      delay(2000);
    }
    
    else if((distance3 > 1 && distance3 < 9) || data == '3') //3층일때
    {
      digitalWrite(led_3,HIGH);
      Serial.write("3층\n");
      
      if(flor==1)
      {
        for(int i=0; i<180; i++)
        {
          elservo.write(i);
          delay(10);
        }
      }
      
      if(flor==2)
      {
        for(int i=90; i<180; i++)
        {
          elservo.write(i);
          delay(10);
        }
      }
      flor=3;
      digitalWrite(led_3,LOW);
      delay(2000);
    }
    data = 0;
}
