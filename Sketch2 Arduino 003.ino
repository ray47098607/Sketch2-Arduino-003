/*
 Name:		Sketch2_Arduino_003.ino
 Created:	2020/12/26 下午 05:35:56
 Author:	AS-CP6230
*/

/*
Arduino generic menu system
Rui Azevedo - ruihfazevedo(@rrob@)gmail.com
output : Serial
input : Analog Joystick + Serial
*/
#include<Arduino.h>
#include <menu.h>
#include <menuIO/liquidCrystalOut.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#include <menuIO/analogAxisIn.h>
#include <menuIO/keyIn.h>
#include <menuIO/chainStream.h>

using namespace Menu;

// LCD /////////////////////////////////////////
#include <Wire.h>
#include <LiquidCrystal.h>
#define RS 22
#define E 24
#define D4 26
#define D5 28
#define D6 30
#define D7 32

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);  //建立 LCD 物件




#ifdef ESP8266
#define JOY_X 2
#define JOY_Y 3
#define JOY_BTN 4
#else
#define JOY_X A0
#define JOY_Y A1
#define JOY_BTN 40
#endif

//analog joystick parameters
// uint8_t pin,
// uint8_t sensivity=5,
// bool inv=false,
// int center=512,
// int inertia=6,
// int bufferSz=1,
// navCmds posCmd=upCmd,
// navCmds negCmd=downCmd

analogAxis<JOY_Y, 10, false> ay;

int test = 500;

#define LEDPIN LED_BUILTIN
#define MAX_DEPTH 2

unsigned int timeOn = 10;
unsigned int timeOff = 90;


MENU(mainMenu, "LED01 menu", Menu::doNothing, Menu::noEvent, Menu::noStyle
	, FIELD(timeOn, "On", "ms", 0, 1000, 10, 1, Menu::doNothing, Menu::noEvent, Menu::noStyle)
	, FIELD(timeOff, "Off", "ms", 0, 10000, 10, 1, Menu::doNothing, Menu::noEvent, Menu::noStyle)
	, FIELD(GLED,"001","m",0,100,10,1, Menu::doNothing, Menu::noEvent, Menu::noStyle)
	,EXIT("<back")
);


//joystick button pin
#define joyBtn 40

keyMap btnsMap[] = { {-joyBtn,defaultNavCodes[enterCmd].ch} };//negative pin numbers use internal pull-up, this is on when low
keyIn<1> btns(btnsMap);// 1 is the number of keys

serialIn menuSerialIn(Serial);

MENU_INPUTS(in, &ay, &btns, &menuSerialIn);



MENU_OUTPUTS(out, MAX_DEPTH
	, SERIAL_OUT(Serial)
	, LIQUIDCRYSTAL_OUT(lcd, { 10,0,16,2 })//must have 2 items at least
);

NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

bool blink(int timeOn, int timeOff) {
	return millis() % (unsigned long)(timeOn + timeOff) < (unsigned long)timeOn;
}

void setup() {

	Serial.begin(115200);
	while (!Serial);
	btns.begin();
	Serial.println("Menu 4.x joysick example");
}

void loop() {
	nav.poll();
	// while(menuSerialIn.available())
	//   Serial.write(menuSerialIn.read());
	delay(100);
	digitalWrite(LEDPIN, blink(timeOn, timeOff));
}

