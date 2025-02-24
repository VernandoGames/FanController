#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "mcp2515/mcp2515.h"

// mcp2515 library:
// https://github.com/adamczykpiotr/pico-mcp2515

const float ac_press_high_cutin = 300;
const float ac_press_high_cutout = 270;
const float engine_temp_high_cutin = 212;
const float engine_temp_high_cutout = 210;
const float engine_temp_low_cutin = 120;
const float ac_press_low_cutin = 120;
bool ac_press_high_output = false;
bool engine_temp_high_output = false;

MCP2515 can0;
struct can_frame rx;

int main()
{
    stdio_init_all();

    //Initialize CAN interface
    can0.reset();
    can0.setBitrate(CAN_1000KBPS, MCP_16MHZ);
    can0.setNormalMode();

	// pico_led_init();

	// Pin 0 - Low Output
	// Pin 1 - High Output

	// Pin 28 - ADC 2 (Engine Coolant Temp)
	// Pin 27 - ADC 1 (AC Pressure Transducer)
	// Conversions:
	// Coolant Temp: 1V = 55.833
	// AC Press: 1V = 46.65
	// Scratch that.. Prob gonna have to talk on CAN
	// bus to get information from pcm. Most reliable.
	adc_init();
	adc_gpio_init(28);
	adc_gpio_init(27);
	gpio_init(0);
	gpio_set_dir(0, GPIO_OUT);
	gpio_init(1);
	gpio_set_dir(1, GPIO_OUT);
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

	long last_led_update = get_absolute_time();

    while (true) {
		gpio_put(0, false);
		sleep_ms(10);
		long currentTime = get_absolute_time();
		if (currentTime - last_led_update > 100000) {
			last_led_update = currentTime;
			//printf("%s", currentTime);
			gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
		}
		
		bool high_enable = false;
		bool low_enable = false;
		const float engine_temp = 100; // engine_temp_v * 55.833;
		const float ac_press = 100; //ac_press_v * 46.65;

		// Handle high cut-ins / cut-outs
		if (ac_press > ac_press_high_cutin) {
			ac_press_high_output = true;
		} else if (ac_press < ac_press_high_cutout && ac_press_high_output)
		{
			ac_press_high_output = false;
		}
		if (engine_temp > engine_temp_high_cutin) {
			engine_temp_high_output = true;
		} else if (engine_temp < engine_temp_high_cutout && engine_temp_high_output) {
			engine_temp_high_output = false;
		}

		// Handle low cut ins
		if (ac_press > ac_press_low_cutin) low_enable = true;
		if (engine_temp > engine_temp_low_cutin) low_enable = true;

		gpio_put(0, !low_enable);
		gpio_put(1, !high_enable);
		
		if (can0.readMessage(&rx) == MCP2515::ERROR_OK) {
			printf("New frame from ID: %10x\n", rx.can_id);
		}
		//sleep_ms(1000);
    }
}
