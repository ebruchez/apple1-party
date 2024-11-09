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

uint16_t count_entries() {
    uint16_t count = 0;
    Entry *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void delete_entry(uint16_t position) {
    if (position == 1) {
        Entry *temp = head;
        head = head->next;
        free(temp);
    } else {
        Entry *current = head;
        uint16_t i;
        for (i = 1; i < position - 1; i++) {
            current = current->next;
        }
        {
            Entry *temp = current->next;
            current->next = temp->next;
            free(temp);
        }
    }
}

void print_guests() {
    uint8_t position = 1;
    Entry *current = head;
    while (current != NULL) {
//        cprintf("%d. %s - %d guests\r", position, current->name, current->count);
        itoa(position, line_buffer, 10);
        a1_cputs(line_buffer);
        a1_cputs(". ");
        a1_cputs(current->name);
        a1_cputs(" - ");
        itoa(current->count, line_buffer, 10);
        a1_cputs(line_buffer);
        a1_cputs(" guests\r");
        current = current->next;
        position++;
    }
}

//size_t __heapmaxavail();
//size_t __heapmemavail();

int main () {

    a1_cputs("\r\r");
    print_big("2024");

    while(1) {
        a1_cputs("\rWelcome to the 2024 Chestnut Party!\r\rA = Check in\rD = Delete entry\r\r");

        switch (a1_cgetc() & 0x7F) {
            case 'A':
                a1_cputs("Enter your name: ");
                a1_read_line(line_buffer, LINE_BUFFER_LEN);
//                cprintf("\rWelcome, %s!\r\r", line_buffer);
                a1_cputs("\rWelcome, ");
                a1_cputs(line_buffer);
                a1_cputs("!\r\r");

                a1_cputs("Number of guests to check in: ");
                append(new_entry(strdup(line_buffer), a1_read_number()));

                break;
            case 'D':
                a1_cputs("Entry number to remove: ");
                {
                    uint16_t index = a1_read_number();
                    if (index <= count_entries())
                        delete_entry(index);
                    else
                        a1_cputs("\r\rInvalid entry number!\r");
                }
                break;
            default:
                continue;
                break;
        }

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
