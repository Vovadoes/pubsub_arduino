/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <Gyver433.h>

#include <ros.h>

#define SV610_TX 6

// Gyver433_RX<2, 100> rx;
Gyver433_TX<SV610_TX> tx; // указали пин

class NewHardware : public ArduinoHardware
{
public:
  NewHardware() : ArduinoHardware(&Serial1, 115200){};
};

ros::NodeHandle_<NewHardware> nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";

unsigned long long millis_pub = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  // attachInterrupt(0, isr, CHANGE); // прерывание пина радио по CHANGE
  nh.initNode();
  nh.advertise(chatter);
}

char data[] = "Hello from #xx"; // строка для отправки
byte count = 0;                 // счётчик для отправки

void loop()
{
  if (abs(millis() - millis_pub) >= 500)
  {
    str_msg.data = hello;
    chatter.publish(&str_msg);
    nh.spinOnce();
    Serial.println(hello);
    millis_pub = millis();
  }
  if (millis() > 2100)
  {
    // добавляем счётчик в строку
    data[12] = (count / 10) + '0';
    data[13] = (count % 10) + '0';
    if (++count >= 100)
      count = 0;
    tx.sendData(data);
    Serial.println(data);
    delay(100);
  }
  // if (rx.gotData())
  // {                                   // если успешно принято больше 0
  //   Serial.write(rx.buffer, rx.size); // выводим
  //   Serial.println();
  // }
}

// спец. тикер вызывается в прерывании
// void isr()
// {
//   rx.tickISR();
// }