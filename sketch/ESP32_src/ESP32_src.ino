#include <ros.h>
#include <std_msgs/Empty.h>
//#include <std_msgs/String.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32MultiArray.h> //for sensor values
#include <WiFi.h>

// use first channel of 16 channels (started from zero)
#define LP 0
#define LM 1
#define RP 2
#define RM 3

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN_0 18 \\LP
#define LED_PIN_1 17 \\LM
#define LED_PIN_2 16 \\RP
#define LED_PIN_3 4 \\RM

const int dt = 5;
const int N = 36; //must be same as rosnode "head2control"
const int unit_forward_time = 10; //ms
const int unit_time = (int) (1450.0/(float)N * (90.0/65.0)); //ms
int loc = -1; //0~N-1


const char SSID[] = "write your SSID";
const char PASSWORD[] = "write your password";
IPAddress server(192, 168, 100, 40); //change to your IP address

const uint16_t serverPort = 11411;

WiFiClient client;

class WiFiHardware {
  public:
  WiFiHardware() {};

  void init() {
    client.connect(server, serverPort);
  }

  int read() {
    return client.read();
  }

  void write(uint8_t* data, int length) {
    for(int i=0; i<length; i++) client.write(data[i]);
  }

  unsigned long time(){
    return millis();
  }
};

ros::NodeHandle_<WiFiHardware> nh;
std_msgs::Empty dummy_msg;


int POWER_R = 255; 
int POWER_L = 255; 

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void reset(const std_msgs::Empty& msg){
  ledcAnalogWrite(LP, 0);
  ledcAnalogWrite(LM, 0);
  ledcAnalogWrite(RP, 0);
  ledcAnalogWrite(RM, 0);
  delay(dt);
}

void forward(const std_msgs::Empty& msg){
  ledcAnalogWrite(LP, POWER_L);
  ledcAnalogWrite(RP, POWER_R);
  //delay(dt);
  delay(unit_forward_time);
  reset(dummy_msg);
}

void back(const std_msgs::Empty& msg){
  ledcAnalogWrite(LM, POWER_L);
  ledcAnalogWrite(RM, POWER_R);
  delay(dt);
}

void TTL(const std_msgs::Empty& msg) //turn to left
{
  ledcAnalogWrite(LM,POWER_L);
  ledcAnalogWrite(RP,POWER_R);
}

void TTR(const std_msgs::Empty& msg) //turn to right
{
  ledcAnalogWrite(LP, POWER_L);
  ledcAnalogWrite(RM, POWER_R);
}

void cb(const std_msgs::Int8& msg)
{
  if(loc==-1) loc=msg.data; //for restart
  
  int diff = msg.data - loc;
  if(diff<-N/2)diff += N;
  if(diff>N/2) diff -= N;

  if(diff>0){
    TTL(dummy_msg);
    delay(unit_time*diff);
    reset(dummy_msg);
  }
  if(diff<0){
    TTR(dummy_msg);
    delay(unit_time*(-diff));
    reset(dummy_msg);
  }
  if(diff==0){
    reset(dummy_msg);
  }
  
  loc = msg.data;
}


ros::Subscriber<std_msgs::Empty> sub_f("forward", &forward);
ros::Subscriber<std_msgs::Empty> sub_b("back", &back);
ros::Subscriber<std_msgs::Empty> sub_l("TTL", &TTL);
ros::Subscriber<std_msgs::Empty> sub_r("TTR", &TTR);
ros::Subscriber<std_msgs::Empty> sub_s("stop", &reset);

ros::Subscriber<std_msgs::Int8> sub_n("location", &cb);

void setup(){
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }

  Serial.println(" connected");

  ledcSetup(LP, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LM, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(RP, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(RM, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN_0, LP);
  ledcAttachPin(LED_PIN_1, LM);
  ledcAttachPin(LED_PIN_2, RP);
  ledcAttachPin(LED_PIN_3, RM);
  
  nh.initNode();
  nh.subscribe(sub_n);

  nh.subscribe(sub_f);
  nh.subscribe(sub_b);
  nh.subscribe(sub_l);
  nh.subscribe(sub_r);
  nh.subscribe(sub_s);
  
  delay(10);
}


void loop(){  
  nh.spinOnce();
  delay(1);
}
