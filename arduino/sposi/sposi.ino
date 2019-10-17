

int ledOne = 15;   // LED1 connected to Analog Pin 1 (A1)
int ledTwo = 16;   // LED2 connected to Analog Pin 2 (A2)
int ledThree = 17; // LED3 connected to Analog Pin 3 (A3)
int ledFour = 18;  // LED4 connected to Analog Pin 4 (A4)
int ledFive = 19;  // LED5 connected to Analog Pin 5 (A5)

int ledMonitor = 14; // LEDSTART connected to Analog Pin 0 (A0)

int btnOne = 4;   // BUTTON1 connected to Digital Pin 4 (D4)
int btnTwo = 5;   // BUTTON2 connected to Digital Pin 5 (D5)
int btnThree = 6; // BUTTON3 connected to Digital Pin 6 (D6)
int btnFour = 7;  // BUTTON4 connected to Digital Pin 7 (D7)
int btnFive = 8;  // BUTTON5 connected to Digital Pin 8 (D8)

int btnStart = 2; // BUTTONSTART connected to Digital Pin 2 (D2)
int btnStop = 3;  // BUTTONSTOP connected to Digital Pin 3 (D3)

byte incomingByte = 0x0; // Serial read byte

boolean timerStarted = false;

int delayForLED = 20;             // IN MSECS
unsigned long timeLimit = 300000; // IN MSECS
unsigned long previousMillis = 0;

void setup()
{
  Serial.begin(9600);
  analogWrite(ledOne, 255);
  delay(delayForLED);
  analogWrite(ledOne, 0);
  delay(delayForLED);
  analogWrite(ledTwo, 255);
  delay(delayForLED);
  analogWrite(ledTwo, 0);
  delay(delayForLED);
  analogWrite(ledThree, 255);
  delay(delayForLED);
  analogWrite(ledThree, 0);
  delay(delayForLED);
  analogWrite(ledFour, 255);
  delay(delayForLED);
  analogWrite(ledFour, 0);
  delay(delayForLED);
  analogWrite(ledFive, 255);
  delay(delayForLED);
  analogWrite(ledFive, 0);
  delay(delayForLED);
  analogWrite(ledFour, 255);
  delay(delayForLED);
  analogWrite(ledFour, 0);
  delay(delayForLED);
  analogWrite(ledThree, 255);
  delay(delayForLED);
  analogWrite(ledThree, 0);
  delay(delayForLED);
  analogWrite(ledTwo, 255);
  delay(delayForLED);
  analogWrite(ledTwo, 0);
  delay(delayForLED);
  analogWrite(ledOne, 255);
  delay(delayForLED);
  analogWrite(ledOne, 0);
  // Serial.println("system:timer_limit:" + String(timeLimit));
  sendMessage("system", "time", int(timeLimit));
  // delay(10);
}

void loop()
{

  readSerial();

  if (digitalRead(btnStart) && !timerStarted)
  {
    startTimer();
    previousMillis = millis();
  }
  if (timerStarted)
  {
    if ((unsigned long)(millis() - previousMillis) < timeLimit)
    {
      checkButtons();
    }
    else
    {
      stopTimer();
      sendMessage("status", "timer", 3);
      // Serial.println("status:timer:passed");
      // delay(10);
    }
  }
}

void readSerial()
{
  if (Serial.available() > 0)
  {
    String incomingString = Serial.readString();
    if (incomingString.equals("start_timer\n"))
    {
      startTimer();
    }
    else if (incomingString.equals("stop_timer\n"))
    {
      stopTimer();
    }
    // else
    // {
    //   Serial.print("system:received:" + incomingString);
    //   delay(10);
    // }
  }
}

void checkButtons()
{
  if (digitalRead(btnOne))
  {
    stopTimer();
    analogWrite(ledOne, 255);
    sendMessage("status", "player", 1);
    sendMessage("status", "time", int((millis() - previousMillis)));
    // Serial.print("status:pressed:player1:" + (millis() - previousMillis));
    // delay(10);
  }
  else if (digitalRead(btnTwo))
  {
    stopTimer();
    analogWrite(ledTwo, 255);
    sendMessage("status", "player", 2);
    sendMessage("status", "time", int((millis() - previousMillis)));
    // Serial.print("status:pressed:player2:" + (millis() - previousMillis));
    // delay(10);
  }
  else if (digitalRead(btnThree))
  {
    stopTimer();
    analogWrite(ledThree, 255);
    sendMessage("status", "player", 3);
    sendMessage("status", "time", int((millis() - previousMillis)));
    // Serial.print("status:pressed:player3:" + (millis() - previousMillis));
    // delay(10);
  }
  else if (digitalRead(btnFour))
  {
    stopTimer();
    analogWrite(ledFour, 255);
    sendMessage("status", "player", 4);
    sendMessage("status", "time", int((millis() - previousMillis)));
    // Serial.print("status:pressed:player4:" + (millis() - previousMillis));
    // delay(10);
  }
  else if (digitalRead(btnFive))
  {
    stopTimer();
    analogWrite(ledFive, 255);
    sendMessage("status", "player", 5);
    sendMessage("status", "time", int((millis() - previousMillis)));
    // Serial.print("status:pressed:player5:" + (millis() - previousMillis));
    // delay(10);
  }
}

void startTimer()
{
  timerStarted = true;
  turnOffLights();
  analogWrite(ledMonitor, 255);
  sendMessage("status", "timer", 1);
  // Serial.println("status:timer:started");
  // delay(10);
}

void stopTimer()
{
  timerStarted = false;
  analogWrite(ledMonitor, 0);
  sendMessage("status", "timer", 2);
  // Serial.println("status:timer:stopped");
  // delay(10);
}

void turnOffLights()
{
  analogWrite(ledOne, 0);
  analogWrite(ledTwo, 0);
  analogWrite(ledThree, 0);
  analogWrite(ledFour, 0);
  analogWrite(ledFive, 0);
}

void sendMessage(String type, String parameter, int value)
{
  byte message[] = {0x00, 0x00, 0x00, 0x00, 0x00};

  // First Byte Check
  if (type.equals("system"))
  {
    message[0] = 0x01;
  }
  else if (type.equals("status"))
  {
    message[0] = 0x02;
  }

  // Second Byte Check
  if (parameter.equals("timer"))
  {
    message[1] = 0x01;
  }
  else if (parameter.equals("player"))
  {
    message[1] = 0x02;
  }
  else if (parameter.equals("time"))
  {
    message[1] = 0x03;
  }

  // Third Byte Check
  if (message[1] == 0x03)
  {
    message[2] = (byte)(value & 0xFF);
    message[3] = (byte)((value >> 2) & 0xFF);
    message[4] = (byte)((value >> 4) & 0xFF);
  }
  else
  {
    message[3] = byte(value);
  }

  Serial.write(message, sizeof(message));
}
