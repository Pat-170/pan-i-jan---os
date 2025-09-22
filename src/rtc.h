#pragma once
#include <stdint.h>

/* Czy RTC jest w BCD */
int rtc_is_bcd(void);

/* Odczyt z rejestru CMOS (0x00 – sekundy) */
uint8_t rtc_read_reg(uint8_t reg);

/* Odczyt aktualnej sekundy (0..59) – stabilny (czeka na koniec update) */
uint8_t rtc_read_seconds(void);

/* Poczekaj do następnej sekundy i zwróć jej wartość */
uint8_t rtc_wait_next_second(uint8_t cur_sec);

