#include "gpio_test.h"

static uint8_t is_tft_pin(const GPIO* gpio){
	return (gpio == &PA4 || gpio == &PA5 || gpio == &PA6 ||gpio == &PA7 ||gpio == &PA8);
}

//Start the test
void gpio_test(){

}
