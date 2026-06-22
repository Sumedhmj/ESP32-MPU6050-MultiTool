//Libraries
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define MENU_BUTTON 2
#define SELECT_BUTTON 5

//Menu Selection variables
int menuindex=0;
int selectbutton=0;
int premenubtn=0;
enum SCREEN{
  MAIN_MENU,
  ACCEL_SCR,
  GYRO_SCR,
  BUBBLE_SCR,
  VIBRATOR_SCR
};
SCREEN currentscreen=MAIN_MENU;

//Vibratometer Variables
float peak=0;
float avg=0;
float prevMagnitude=0;
float vibration=0;
float vibrationSum = 0;
int vibrationCount = 0;
float graphData[128];

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

sensors_event_t accel;
sensors_event_t gyro;
sensors_event_t temp;

//Menu and Screen Functions
void drawMAIN_MENU();
void drawACCEL_SCR();
void drawGYRO_SCR();
void drawBUBBLE_SCR();
void drawVIBRATOR_SCR();


void setup() {

pinMode(MENU_BUTTON,INPUT);
pinMode(SELECT_BUTTON,INPUT_PULLUP);
Serial.begin(115200);
delay(1000);
  
//MPU6050 Setup
Wire.begin(21, 22);
Serial.println("Initializing MPU6050...");
if (!mpu.begin()) {
Serial.println("MPU6050 not found. Check wiring and I2C address.");
while (1) {
delay(10);
}
}
Serial.println("MPU6050 connected successfully.");

//Define MPU6050 Parameters
mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
mpu.setGyroRange(MPU6050_RANGE_500_DEG);
mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
Serial.println("MPU6050 configured.");
Serial.println();

//OLED SSD1306 Setup
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
Serial.println(F("SSD1306 allocation failed"));
for(;;);
}
Serial.println("SSD1306 allocation successful");
Serial.println("OLED configured");
}


void loop() {

mpu.getEvent(&accel, &gyro, &temp);

int currentmenubtn;
currentmenubtn=digitalRead(MENU_BUTTON);
selectbutton=digitalRead(SELECT_BUTTON);
if(currentmenubtn>premenubtn && currentscreen==MAIN_MENU){
  menuindex++;
  if(menuindex>=5){
    menuindex=0;
  }
}
premenubtn=currentmenubtn;

if (selectbutton==0){
  switch(menuindex){
    case 1:currentscreen=ACCEL_SCR;
    break;  
    case 2:currentscreen=GYRO_SCR;
    break; 
    case 3:currentscreen=BUBBLE_SCR;
    break; 
    case 4:currentscreen = VIBRATOR_SCR;
    peak = 0;
    avg = 0;
    vibrationSum = 0;
    vibrationCount = 0;
    prevMagnitude = 0;
    for(int i=0;i<128;i++){
        graphData[i] = 0;
    }
    break; 
    default:currentscreen=MAIN_MENU;
    break;
  }
}

display.clearDisplay();

switch(currentscreen){
  case MAIN_MENU: drawMAIN_MENU();
  break;
  case ACCEL_SCR: drawACCEL_SCR();
  break;
  case GYRO_SCR: drawGYRO_SCR();
  break;
  case BUBBLE_SCR: drawBUBBLE_SCR();
  break;
  case VIBRATOR_SCR: drawVIBRATOR_SCR();
  break;
}
if(menuindex>0 && currentmenubtn){
  currentscreen=MAIN_MENU;
}

display.setTextColor(WHITE);
display.setTextSize(1);
display.display();
delay(50);
}

//Main Menu Screen
void drawMAIN_MENU(){
  display.setCursor(6,0);
  display.println("MPU MENU:");
  display.drawLine(6,12,127,12,WHITE);
  display.setCursor(6,18);
  display.println("ACCELEROMETER");
  display.setCursor(6,28);
  display.println("GYRO");
  display.setCursor(6,38);
  display.println("BUBBLE LEVEL");
  display.setCursor(6,48);
  display.println("VIBROMETER");
  int cursorY=0;
  if(menuindex>0){
    cursorY=8+(menuindex*10);
    display.setCursor(0,cursorY);
    display.print(">");
  }
  peak=0;
  vibration=0;
}  

//Accelerometer Screen
void drawACCEL_SCR(){
    display.setCursor(0,10);
    display.print("X : ");
    display.println(accel.acceleration.x);
    display.print("y : ");
    display.println(accel.acceleration.y);
    display.print("Z : ");
    display.println(accel.acceleration.z);
    delay(300);
}

//Gyroscope Screen
void drawGYRO_SCR(){
    display.setCursor(0,10);
    display.print("X : ");
    display.println(gyro.gyro.x);
    display.print("y : ");
    display.println(gyro.gyro.y);
    display.print("Z : ");
    display.println(gyro.gyro.z);
    delay(300);
}

//Bubble Level Screen
void drawBUBBLE_SCR(){
    display.drawCircle(64,32, 31, WHITE);
    display.drawLine(64,29,64,35,WHITE);
    display.drawLine(61,32,67,32,WHITE);
    int bubbleX = map(accel.acceleration.y,-10,10,36,92);
    int bubbleY = map(accel.acceleration.z,-10,10,60,4);
    display.fillCircle(bubbleX,bubbleY,3, WHITE);
}

//Vibratometer Screen
void drawVIBRATOR_SCR(){
  
    float modifiedX=(accel.acceleration.x)-9.8; //To eliminate acceleration due to Gravity
    float currentMagnitude=sqrt((modifiedX*modifiedX)+(accel.acceleration.y*accel.acceleration.y)+(accel.acceleration.z*accel.acceleration.z));
    
    vibration=abs(currentMagnitude-prevMagnitude);
    prevMagnitude=currentMagnitude;

    vibrationSum += vibration;
    vibrationCount++;
    avg = vibrationSum / vibrationCount;
    
    if(vibration>peak){
      peak=vibration;
    }

    display.setCursor(0,0);
    display.print("PEAK:");
    display.print(peak);
    display.print("  ");
    display.print("AVG:");
    display.println(avg);
    display.drawLine(0,10,127,10,WHITE);

    //Graphical Representation of Vibration values
    for(int i=0; i<127; i++){
        graphData[i] = graphData[i+1];
    }
    graphData[127] = vibration;
    for(int x=0; x<127; x++){
    float displayVib1 = graphData[x] * 10;
    float displayVib2 = graphData[x+1] * 10;

    int y1 = map((int)displayVib1, 0, 50, 63, 12);
    int y2 = map((int)displayVib2, 0, 50, 63, 12);
    y1 = constrain(y1, 12, 63);
    y2 = constrain(y2, 12, 63);
    display.drawLine(x, y1,x+1, y2,WHITE);
    }
}
