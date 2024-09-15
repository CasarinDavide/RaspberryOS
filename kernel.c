
#define ADDRESS_START 	          0x00000000 
#define ADDRESS_START_PERIPHERALS 0x3F000000 
#define ADDRESS_END_PERIPHERALS   0x3FFFFFFF
#define ADDRESS_AUX_REGISTRER_MAP 0x3F215000 
#define ADDRESS_GPIO_REGISTRERS 0x3F200000

#include "utilities_func.h"
#include "types.h"
#include "mini_uart.h"
#include "gpio.h"

volatile auxiliary_register_map* aux_reg_map = (auxiliary_register_map*) ADDRESS_AUX_REGISTRER_MAP;
volatile gpio* gpio_reg = (gpio*) ADDRESS_GPIO_REGISTRERS;


int prova(int n)
{
    n = n  + 1;
    return n;
}

void load_os_func() {

    prova(50);
    uart_init(gpio_reg,aux_reg_map);
    while (1)
    {
        uart_send_string(aux_reg_map,"TEST DI PROVA \0");
    }
}


