#pragma once
#include "gpio.h"

void uart_init(volatile gpio* gpio_reg,volatile auxiliary_register_map* aux_reg_map);
void urt_send_char(volatile auxiliary_register_map* aux_reg_map, char c);
char urt_recive_char(volatile auxiliary_register_map* aux_reg_map);
void uart_send_string(volatile auxiliary_register_map* aux_reg_map,char *s);

