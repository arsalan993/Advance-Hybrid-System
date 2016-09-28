#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

int a;
RTC_DS1307 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int chipSelect = 53;
int term;
int S_GRID = 22;
int S_CC = 24;
int S_AC_charger = 26;
int S_inverter = 28;
int rem;
int Minimum_Battery_level;
int Min_Grid_Voltage ;
int Max_Grid_Voltage;
int Battery_Critical_point ;
int Battery_losses;
int inverter_losses;
int extra_batt = 5;
float battery_level;
float load_voltage ;
float load_current;
int load_consumption;
float grid_voltage;
float grid_current;
float grid_power;
float DC_CC_voltage;
float DC_CC_current;
int CC_power ;
int losses;
int batterybuff1;
int batterybuff2;
int temp ;
int temp2;
int tempo;
int onetemphr;
int onetempmin;
int twotemphr;
int twotempmin;
int Loadshedinghr[10];
int Loadshedingmin[10];
int TLoadshedinghr[10];
int TLoadshedingmin[10];
int batterybuff[10];
int Tbatterybuff[10];
int Loadshedinghr1[10];
int Loadshedingmin1[10];
int TLoadshedinghr1[10];
int TLoadshedingmin1[10];
int batterybuff0[10];
int Tbatterybuff0[10];
int timediff;
int count ;
int timediff1;
int timediff2;
int differ1;
int differ2;
int battdiffer;
int count2;

DateTime future;
DateTime now;

void setup()
{ future = NULL;
  Serial.begin(9600);
  Minimum_Battery_level = 50;
  Min_Grid_Voltage = 210;
  Max_Grid_Voltage = 250;
  Battery_Critical_point = 75;
  Battery_losses = 10;
  inverter_losses = 15;

  SPI.begin();
  Wire.begin();
  rtc.begin();
  if (!(rtc.isrunning()))
  {
    Serial.println("RTC not Running");
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  lcd.begin(20, 4);
  /*****************************************/
  pinMode(S_GRID,           OUTPUT);
  pinMode(S_CC,             OUTPUT);
  pinMode(S_AC_charger,     OUTPUT);
  pinMode(S_inverter,       OUTPUT);
  term  = 0;

  int A1 = 0;
  int A2 = 0;
  int A3 = 0;
  int A5 = 0;
  int A6 = 0;
  int A4 = 0;

  SD.begin(chipSelect);
  pinMode(53, OUTPUT);
  temp = 0;
  tempo = 0;
  temp2 = 0;
  batterybuff1 = 0;
  batterybuff2 = 0;
  temp = 0;
  tempo = 0;
  onetemphr = 0;
  onetempmin = 0;
  twotemphr = 0;
  twotempmin = 0;
  Loadshedinghr[10] = 0;
  Loadshedingmin[10] = 0;
  TLoadshedinghr[10] = 0;
  TLoadshedingmin[10] = 0;
  Loadshedinghr1[10] = 0;
  Loadshedingmin1[10] = 0;
  TLoadshedinghr1[10] = 0;
  TLoadshedingmin1[10] = 0;
  batterybuff[10] = 0;
  Tbatterybuff[10] = 0;
  batterybuff0[10] = 0;
  Tbatterybuff0[10] = 0;
  timediff = 0;
  count = 0;
  timediff1 = 0;
  timediff2 = 0;
  differ1 = 0;
  differ2 = 0;
  battdiffer = 0;
  rem = 1;
}
void loop()
{
  DateTime now = rtc.now();
  battery_level = analogRead(A2);
  battery_level = (battery_level / 1023) * 100;
  load_voltage = analogRead(A4);
  load_current = analogRead(A3);
  load_voltage = (load_voltage / 1023) * 250;
  load_current = (load_current / 1023) * 2;
  load_consumption = load_voltage * load_current;
  grid_voltage = analogRead(A5);
  grid_voltage = (grid_voltage / 1023) * 250;
  DC_CC_voltage = analogRead(A1);
  DC_CC_current = analogRead(A6);
  DC_CC_voltage = (DC_CC_voltage / 1023) * 15;
  DC_CC_current = (DC_CC_current / 1023) * 33;
  CC_power  = DC_CC_voltage * DC_CC_current;

  losses = Battery_losses + inverter_losses ;

  if (!(grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage) && (temp == 0))
  {
    onetemphr = int(now.hour());
    onetempmin = int(now.minute());
    twotemphr = now.hour();
    twotempmin = now.minute();
    batterybuff1 = battery_level;
    batterybuff2 = battery_level;
    temp = 1;
  }
  if ((grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage) && (temp == 1))
  {
    twotemphr = now.hour();
    twotempmin = now.minute();
    batterybuff2 = battery_level;
    temp = 0;
    temp2 = 1;
  }
  if (twotemphr > onetemphr)
  {
    timediff1 = (twotemphr - onetemphr) * 60;
  }
  else if (twotemphr < onetemphr)
  {
    timediff1 = (onetemphr - twotemphr) * 60;
  }
  if (twotempmin > onetempmin)
  {
    timediff2 = twotempmin - onetempmin;
  }
  else if (onetempmin > twotempmin)
  {
    timediff2 = onetempmin - twotempmin;
  }
  timediff = timediff1 + timediff2;
  if ((timediff >= 1) && (temp2 == 1) )
  {
    Loadshedinghr[count] = int(onetemphr);
    Loadshedinghr1[count] = int(twotemphr);
    Loadshedingmin[count] = int(onetempmin);
    Loadshedingmin1[count] = int(twotempmin);
    batterybuff[count] = batterybuff1;
    batterybuff0[count] = batterybuff2;
    temp2 = 0;
    onetemphr = 0;
    twotemphr = 0;
    onetempmin = 0;
    twotempmin = 0;
    count++;
  }
  count2 = 0;
  if ((now.hour() == 24 ) && now.minute() == 59 && now.second() == 57 && tempo == 0)
  {
    DateTime future (now.unixtime() + 86400L + 1);
    for (int k = 0; k <= count ; k++) {
      TLoadshedinghr[count] = Loadshedinghr[count];
      TLoadshedinghr1[count] =  Loadshedinghr1[count];
      TLoadshedingmin[count] = Loadshedingmin[count];
      TLoadshedingmin1[count] = Loadshedingmin1[count];
      Tbatterybuff[count] = batterybuff[count];
      Tbatterybuff0[count] = batterybuff0[count];
    }
    tempo = 1;
  }
  if (now.hour() == 00 && now.minute() == 00 && tempo == 1)
  {
    tempo = 0;
  }
  if (now.day() == future.day())
  {
    for (count2 = 0; count2 <= count ; count2++)
    {
      if ( TLoadshedinghr1[count2] > TLoadshedinghr[count2])
      {
        differ1 = TLoadshedinghr1[count2] - TLoadshedinghr[count2];
      }
      else
      {
        differ1 = TLoadshedinghr[count2] - TLoadshedinghr1[count2];
      }
      if ( TLoadshedingmin1[count2] > TLoadshedingmin[count2])
      {
        differ2 = TLoadshedingmin1[count2] - TLoadshedingmin[count2];
      }
      else
      {
        differ2 = TLoadshedingmin[count2] - TLoadshedingmin1[count2];
      }
    }
    if (differ1 > 0)
    {
      differ2 = 0;
    }

    battdiffer = Tbatterybuff[count2] - Tbatterybuff0[count2];
    if ((battery_level - (battdiffer + extra_batt)) > Battery_Critical_point)
    {
      goto unannounced;
    }
    else
    {
      if ((now.hour() >= (TLoadshedinghr[count2] - differ1)) && (now.minute() >= TLoadshedingmin[count2] - differ2) && (now.hour() <= (TLoadshedinghr1[count2] - differ1)) && (now.minute() <= TLoadshedingmin1[count2] - differ2))
      {
        if ((CC_power - losses) > load_consumption && battery_level < 100  && (grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
        {
          digitalWrite(S_GRID, LOW);
          digitalWrite(S_AC_charger, HIGH);
          digitalWrite(S_CC, LOW);
          digitalWrite(S_inverter, HIGH);
          goto announced;
        }
        else if ((CC_power - losses) < load_consumption && battery_level < 100  && (grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
        {
          digitalWrite(S_GRID, HIGH);
          digitalWrite(S_AC_charger, HIGH);
          digitalWrite(S_CC, LOW);
          digitalWrite(S_inverter, LOW);
          goto announced;
        }
        else {
          goto unannounced;
        }
      }
    }
    goto unannounced;
  }
unannounced :
  if ((CC_power - losses) > load_consumption && battery_level > Battery_Critical_point  && !(grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
  {
    digitalWrite(S_GRID, LOW);
    digitalWrite(S_AC_charger, LOW);
    digitalWrite(S_CC, HIGH);
    digitalWrite(S_inverter, HIGH);
  }
  else if ((CC_power - losses) < load_consumption && (grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
  {
    if (battery_level > Minimum_Battery_level)
    {
      digitalWrite(S_GRID, HIGH);
      digitalWrite(S_AC_charger, LOW);
      digitalWrite(S_CC, HIGH);
      digitalWrite(S_inverter, LOW);
    }
    else  if (battery_level <= Minimum_Battery_level)
    {
      digitalWrite(S_GRID, HIGH);
      digitalWrite(S_AC_charger, HIGH);
      digitalWrite(S_CC, LOW);
      digitalWrite(S_inverter, LOW);
    }
  }
  else if ((CC_power - losses) > load_consumption && battery_level < Battery_Critical_point  && (grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
  {
    digitalWrite(S_GRID, HIGH);
    digitalWrite(S_AC_charger, LOW);
    digitalWrite(S_CC, HIGH);
    digitalWrite(S_inverter, LOW);
  }
  else if ((CC_power - losses) < load_consumption && battery_level < Battery_Critical_point  && (grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
  {

    if (battery_level <= Minimum_Battery_level)
    {
      digitalWrite(S_GRID, HIGH);
      digitalWrite(S_AC_charger, HIGH);
      digitalWrite(S_CC, LOW);
      digitalWrite(S_inverter, LOW);
    }
    else if (battery_level > Minimum_Battery_level)
    {
      digitalWrite(S_GRID, HIGH);
      digitalWrite(S_AC_charger, LOW);
      digitalWrite(S_CC, HIGH);
      digitalWrite(S_inverter, LOW);
    }
  }

  else if ((CC_power - losses)  > load_consumption && battery_level < Battery_Critical_point  && !(grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
  {
    digitalWrite(S_GRID, LOW);
    digitalWrite(S_AC_charger, LOW);
    digitalWrite(S_CC, HIGH);
    digitalWrite(S_inverter, HIGH);
  }
  else if ((CC_power - losses) < load_consumption && battery_level < Battery_Critical_point  && !(grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
  {
    digitalWrite(S_GRID, LOW);
    digitalWrite(S_AC_charger, LOW);
    digitalWrite(S_CC, HIGH);
    digitalWrite(S_inverter, LOW);
  }
  else if ((CC_power - losses) < load_consumption && battery_level > Battery_Critical_point  && !(grid_voltage > Min_Grid_Voltage && grid_voltage < Max_Grid_Voltage))
  {
    digitalWrite(S_GRID, LOW);
    digitalWrite(S_AC_charger, LOW);
    digitalWrite(S_CC, HIGH);
    digitalWrite(S_inverter, HIGH);
  }
  goto announced;
announced :
  lcd.setCursor(0, 0);
  lcd.print("Load :");
  lcd.setCursor(14, 0);
  lcd.print(load_consumption);
  lcd.setCursor(0, 1);
  lcd.print("Solar Power");
  lcd.setCursor(14, 1);
  lcd.print(CC_power );
  lcd.setCursor(0, 2);
  lcd.print("Battery Level");
  lcd.setCursor(14, 2);
  lcd.print(battery_level);
  lcd.setCursor(0, 3);
  lcd.print("GRID Voltage");
  lcd.setCursor(14, 3);
  lcd.print(grid_voltage);

  // make a string for assembling the data to log:
  String dataString2 = "";
  String dataString6 = "";
  String dataString8 = "";
  String dataString10 = "";
  String Year = "";
  String Month = "";
  String Days = "";
  String Hours = "";
  String Minute = "";
  String dataString11 = "";
  int A =  now.year(); int B =  now.month(); int C =  now.day(); int D =  now.hour(); int E =  now.minute();
  dataString2 += String(CC_power );
  dataString6 += String(grid_voltage);
  dataString8 += String(battery_level);
  dataString10 += String(load_consumption);
  Year += String(A, DEC);
  Month += String(B, DEC);
  Days += String(C, DEC);
  Hours += String(D, DEC);
  Minute += String(E, DEC);
  dataString11 += String(" ");
  /////////////////////////////////////
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(Year);
    dataFile.print(":");
    dataFile.print(Month);
    dataFile.print(":");
    dataFile.print(Days);
    dataFile.print("  ");
    dataFile.print(Hours);
    dataFile.print(":");
    dataFile.print(Minute);
    dataFile.print(",");
    dataFile.print(dataString8);
    dataFile.print(",");
    dataFile.print(dataString6);
    dataFile.print(",");
    dataFile.print(dataString2);
    dataFile.print(",");
    dataFile.print(dataString10);

    dataFile.println(dataString11);
    dataFile.close();
    // print to the serial port too:
  }
  SD.begin(chipSelect);
  for (int j = 0; j <= count - 1; j++)
  {
    Serial.print(Loadshedinghr[j]);
    Serial.print(":");
    Serial.print(Loadshedingmin[j]);
    Serial.print(" to ");
    Serial.print(Loadshedinghr1[j]);
    Serial.print(":");
    Serial.println(Loadshedingmin1[j]);
  }
  Serial.print("Starting Hour: ");
  Serial.println(onetemphr);
  Serial.print("Starting Min : ");
  Serial.println(onetempmin);
  timediff1 = 0;
  timediff2 = 0;
  timediff = 0;
}

