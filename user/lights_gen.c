
#include "ws2812_i2s.h"

extern uint8_t last_leds [];
extern int last_led_count;
extern char light_mode;

int frame;

void update_lights() {
	int arrsize = last_led_count*3;

	int idx = 0;
	for (int led=0;led<last_led_count;led++) {
		last_leds[idx] = led==frame ? 0xff : 0x00;
		last_leds[idx+1] = led==frame+1 ? 0xff : 0x00;
		last_leds[idx+2] = led==frame+2 ? 0xff : 0x00;
		idx+=3;
	}
	frame++;
	if (frame>last_led_count) frame=0;
	ws2812_push( last_leds, arrsize );
}
