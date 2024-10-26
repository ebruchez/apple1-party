#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "util.h"


#define KBD_DATA 0xd010
#define KBD_CTRL 0xd011
#define DSP_DATA 0xd012
#define DSP_CTRL 0xd013

#define CR 0xD
#define KBD_CR 0x8D

void a1_cputc(char c) {
    while ((*((char*)DSP_DATA) & 0x80) != 0)
        ;
    *((char*)DSP_DATA) = c;
}

char a1_cgetc() {
    while ((*((char*)KBD_CTRL) & 0x80) == 0)
        ;
    return *((char*)KBD_DATA);
}

void a1_cputs(const char *s) {
    while (*s != '\0') {
        a1_cputc(*s++);
    }
}

void a1_read_line(char *buf, int len) {
    uint8_t i;
    for (i = 0; i < len; i++) {
        const char c = a1_cgetc();
        a1_cputc(c);
        if (c == KBD_CR) {
            break;
        }
        buf[i] = c;
    }
    buf[i] = '\0';
}

#define DIGIT_WIDTH 5
#define DIGIT_HEIGHT 8
#define NUM_DIGITS 10

const unsigned char digits[NUM_DIGITS][DIGIT_HEIGHT] = {
    { 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
    { 0x03, 0x01, 0x71, 0x09, 0x07 }, // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E }, // 9
};

void print_digit(uint8_t digit) {
    uint8_t row, column;
    if (digit > 9) {
        a1_cputs("Invalid digit\r");
        return;
    }
    for (row = 0; row < DIGIT_HEIGHT; row++) {
        for (column = 0; column < DIGIT_WIDTH; column++) {
            if (digits[digit][column] & (1 << row)) {
                a1_cputc('#');
            } else {
                a1_cputc(' ');
            }
        }
        a1_cputc('\r');
    }
}

void print_big(const char *text) {
    uint8_t row, column;
    for (row = 0; row < DIGIT_HEIGHT; row++) {
        const char *p = text;
        char c;
        while (c = *p++) {
            if (c >= '0' && c <= '9') {
                const uint8_t digit = c - '0';
                for (column = 0; column < DIGIT_WIDTH; column++) {
                    if (digits[digit][column] & (1 << row)) {
                        a1_cputc('#');
                    } else {
                        a1_cputc(' ');
                    }
                }
                a1_cputs("  ");
            }
        }
        a1_cputc('\r');
    }
}

uint16_t a1_read_number() {
    uint16_t number = 0;
    uint8_t digits = 0;
    while(1) {
        const char c = a1_cgetc() & 0x7F;
        if (c >= '0' && c <= '9' && digits < 5) {
            a1_cputc(c);
            number = number * 10 + (c - '0');
            digits++;
        } else if (c == CR && digits > 0) {
            break;
        }
    }
    return number;
}