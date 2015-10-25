
#include "ws2812_i2s.h"
#include <tgmath.h>

extern uint8_t last_leds [];
extern int last_led_count;
extern char light_mode;

int frame;

#undef fmod

//TODO use real implementation
double fmod(double a, double b) {
	/*while (a>b)
		a-=b;
	while (a<-b)
		a+=b;
	return a;*/
	return (a - b * ((int)(a/b)));
}


void WriteHSV( float hue, float sat, float value, uint8_t* out)
{

	float pr = 0;
	float pg = 0;
	float pb = 0;

	short ora = 0;
	short og = 0;
	short ob = 0;

	float ro = fmod( hue * 6, 6. );

	float avg = 0;

	ro = fmod( ro + 6 + 1, 6 ); //Hue was 60* off...

	if( ro < 1 ) //yellow->red
	{
		pr = 1;
		pg = 1. - ro;
	} else if( ro < 2 )
	{
		pr = 1;
		pb = ro - 1.;
	} else if( ro < 3 )
	{
		pr = 3. - ro;
		pb = 1;
	} else if( ro < 4 )
	{
		pb = 1;
		pg = ro - 3;
	} else if( ro < 5 )
	{
		pb = 5 - ro;
		pg = 1;
	} else
	{
		pg = 1;
		pr = ro - 5;
	}

	//Actually, above math is backwards, oops!
	pr *= value;
	pg *= value;
	pb *= value;

	avg += pr;
	avg += pg;
	avg += pb;

	pr = pr * sat + avg * (1.-sat);
	pg = pg * sat + avg * (1.-sat);
	pb = pb * sat + avg * (1.-sat);

	ora = pr * 255;
	og = pb * 255;
	ob = pg * 255;

	if( ora < 0 ) ora = 0;
	if( ora > 255 ) ora = 255;
	if( og < 0 ) og = 0;
	if( og > 255 ) og = 255;
	if( ob < 0 ) ob = 0;
	if( ob > 255 ) ob = 255;
	
	*out = ob;
	*(out+1) = og;
	*(out+2) = ora;

	//return (ob<<16) | (og<<8) | ora;
}


void pattern_update() {
	int arrsize = last_led_count*3;

	frame++;
	ws2812_push( last_leds, arrsize );
}

void pattern_tick(int led) {
	if (led>=last_led_count)
		return;
		
	uint8_t* addr = last_leds+led*3;
	
	
	switch (light_mode) {
		case 1: {
			int framemod = frame % last_led_count;
			addr[0] = led==framemod ? 0xff : 0x00;
			addr[1] = led==framemod+1 ? 0xff : 0x00;
			addr[2] = led==framemod+2 ? 0xff : 0x00;
			break;
		}
		case 2: WriteHSV( led*.05 + frame*.01, 1, 0.05, addr); break;
		case 3: WriteHSV( led*.001 - frame*.001, 1,  ((((led%256) - ((frame>>1)%256)+256)%256) ) /256.0*1.5-0.1, addr); break;
		
	}
}
