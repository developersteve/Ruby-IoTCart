#include <FileIO.h>
#include <Bridge.h>
#include <Process.h>
#include <Adafruit_Thermal.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

String msg1 = "APIDays Sydney";
String msg2 = "";
String msg3 = "";
String msg4 = "@Braintree_dev";

int printer_RX_Pin = 6;
int printer_TX_Pin = 5;

Adafruit_Thermal printer(printer_RX_Pin, printer_TX_Pin);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Bridge.begin();
  FileSystem.begin();
  printer.begin();
  lcd.begin(20, 4);
  pinMode(13,OUTPUT);
}

void loop() {

  analogWrite(13, 20);
  startmsg();

  char Tid[10];
  String content = "";
  char character;
  Process p;

  p.runShellCommand("/usr/bin/sqlite3 /www/proc/YunPHP/print.db \"select trans from print where ind = 0 limit 1;\"");

  while (p.running());

  while (p.available()>0) {
    char c = p.read();
    if(c != '\r'){
      content.concat(c);
    }
  }

  if (content != "") {
    content.trim();

    p.runShellCommand("/usr/bin/sqlite3 /www/proc/YunPHP/print.db \"update print set ind = 1 where trans = '"+content+"';\"");

    char path[ ] = "/www/proc/YunPHP/files/";

    content.toCharArray(Tid, 10);

    strcat(path, Tid);

    if(FileSystem.exists(path)){

        String text;
        File file = FileSystem.open(path, FILE_READ);
        while (file.available() > 0){
          char d = file.read();
          text += d;
        }
        file.close();

        int textLen = text.length();

        if(textLen>0)
        {
          printing(text);
          text = "";
        }


    }

  }

  Serial.flush();
}

void startmsg()
{
  lcd.setCursor(0, 0);
  lcd.print(msg1);
  lcd.setCursor(0, 1);
  lcd.print(msg2);
  lcd.setCursor(0, 2);
  lcd.print(msg3);
  lcd.setCursor(0, 3);
  lcd.print(msg4);
}

void blinker(int blinkloop)
{
  while(blinkloop>0)
  {
    lcd.noDisplay();
    delay(500);
    lcd.display();
    delay(500);
    blinkloop--;
  }
}

void printing(String text){

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("********************");
  lcd.setCursor(0, 1);
  lcd.print("      INCOMING      ");
  lcd.setCursor(0, 2);
  lcd.print("      PRINTING      ");
  lcd.setCursor(0, 3);
  lcd.print("********************");
  blinker(3);

  printer.justify('C');
  printer.println(msg1);
  printer.feed(2);
  printer.justify('L');
  printer.println(text);
  printer.feed(3);

  delay(1000);

  lcd.clear();
  startmsg();

}

