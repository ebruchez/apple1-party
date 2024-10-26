#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "util.h"

#define LINE_BUFFER_LEN 40

static char line_buffer[LINE_BUFFER_LEN];

typedef struct Entry {
    const char *name;
    uint16_t count;
    struct Entry *next;
} Entry;

static Entry *head = NULL;

Entry * new_entry(const char *name, uint16_t count) {
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->name = name;
    new_entry->count = count;
    new_entry->next = NULL;
    return new_entry;
}

void prepend(Entry *new_entry) {
    new_entry->next = head;
    head = new_entry;
}

void append(Entry *new_entry) {
    if (head == NULL) {
        head = new_entry;
    } else {
        Entry *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_entry;
    }
    new_entry->next = NULL;
}

uint16_t count_guests() {
    uint16_t count = 0;
    Entry *current = head;
    while (current != NULL) {
        count += current->count;
        current = current->next;
    }
    return count;
}

void print_guests() {
    uint8_t position = 1;
    Entry *current = head;
    while (current != NULL) {
        cprintf("%d. %s - %d guests\r", position, current->name, current->count);
        current = current->next;
        position++;
    }
}

//size_t __heapmaxavail();
//size_t __heapmemavail();

int main () {

    a1_cputc('\r');
    print_big("2024");

    while(1) {

//        cprintf("Max %d, avail %d\r\r", __heapmaxavail(), __heapmemavail());

        a1_cputs("\rWelcome to the Chestnut Party!\r\r");

        a1_cputs("Enter your name: ");
        a1_read_line(line_buffer, LINE_BUFFER_LEN);
        cprintf("\rWelcome, %s!\r\r", line_buffer);

        a1_cputs("How many guests are you checking in? ");
        append(new_entry(strdup(line_buffer), a1_read_number()));

        a1_cputc('\r');
        a1_cputc('\r');
        print_guests();
        a1_cputc('\r');

        itoa(count_guests(), line_buffer, 10);
        print_big(line_buffer);
        a1_cputc('\r');
    }

    return (0);
}
