#include "robotcontrol.h"
#include <SoftwareSerial.h>

#define JOYSTICK_X			A0
#define JOYSTICK_Y			A1
#define DIAL				A2
#define BUTTON_1			2		// Joystick button
#define BUTTON_2			3		// Right button
#define BUTTON_3			4		// Top button
#define BUTTON_4			5		// Bottom button
#define BUTTON_5			6		// Left button

#define BUTTON_1_DOWN		0x01
#define BUTTON_2_DOWN		0x02
#define BUTTON_3_DOWN		0x04
#define BUTTON_4_DOWN		0x08
#define BUTTON_5_DOWN		0x10

#define APC220_RX			9
#define APC220_TX			8

#define MIN_UPDATE_INTERVAL	50

SoftwareSerial	apc220( APC220_RX, APC220_TX );
int				center_x, center_y;
byte			last_x = 0, last_y = 0, last_dial = 0, last_buttons = 0;
unsigned long	last_update = 0;

void setup()
{
	Serial.begin( 9600 );
	apc220.begin( 9600 );

	digitalWrite( BUTTON_1, HIGH );
	digitalWrite( BUTTON_2, HIGH );
	digitalWrite( BUTTON_3, HIGH );
	digitalWrite( BUTTON_4, HIGH );
	digitalWrite( BUTTON_5, HIGH );

	center_x	= analogRead( JOYSTICK_X );
	center_y	= analogRead( JOYSTICK_Y );
}

void loop()
{
	int raw_x, raw_y;
	byte joy_x, joy_y, dial, buttons;

	raw_x	= analogRead( JOYSTICK_X );
	raw_y	= analogRead( JOYSTICK_Y );

	if ( raw_x < center_x )	joy_x	= map( raw_x, 0, center_x, 0, 126 );
	else					joy_x	= map( raw_x, center_x, 1023, 127, 254 );

	if ( raw_y < center_y )	joy_y	= map( raw_y, 0, center_y, 0, 126 );
	else					joy_y	= map( raw_y, center_y, 1023, 127, 254 );

	dial	= map( analogRead( DIAL ), 0, 1023, 254, 0 );

	buttons	= 0;

	if ( digitalRead( BUTTON_1 ) == LOW )	buttons |= BUTTON_1_DOWN;
	if ( digitalRead( BUTTON_2 ) == LOW )	buttons |= BUTTON_2_DOWN;
	if ( digitalRead( BUTTON_3 ) == LOW )	buttons |= BUTTON_3_DOWN;
	if ( digitalRead( BUTTON_4 ) == LOW )	buttons |= BUTTON_4_DOWN;
	if ( digitalRead( BUTTON_5 ) == LOW )	buttons |= BUTTON_5_DOWN;

	if ( buttons == last_buttons )
	{
		if ( millis() - last_update < MIN_UPDATE_INTERVAL )				return;
		if ( joy_x == last_x && joy_y == last_y && dial == last_dial )	return;
	}

	apc220.write( 0xFF );
	apc220.write( last_x		= joy_x );
	apc220.write( last_y		= joy_y );
	apc220.write( last_dial		= dial );
	apc220.write( last_buttons	= buttons );

	last_update					= millis();
}
