#include "projekti.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Adding a new calendar entry to the calendar.*/
void add_entry(const char *command, struct entry *calendar, int flag) {

    char description[50];
    int month, day, hour;

    int res = sscanf(command, "A %s %u %u %u", description, &month, &day, &hour);

    //checking for invalid commands
    if (res < 4 || res > 4) { 
        printf("Invalid add entry call: %s", command); //variable "command" contains \n
        return;
    }

    //checking that month, day and hour are sensible and then adding an entry to the calendar
    if (month > 12 || day > 31 || hour > 24) { 
        printf("Invalid parameter for month, day or hour.\n");
        return;
    }

    //checking if the date is already reserved
    struct entry *ptr = calendar;
    while(ptr != NULL) { 
        if (ptr->month == month && ptr->day == day && ptr->hour == hour) {
            printf("This hour has already been reserved. Choose another date.\n");
            return;
        }
        ptr = ptr->next;
    }
            
    struct entry *new = (struct entry *)malloc(sizeof(struct entry));
    struct entry *current = calendar;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new;
    new->next = NULL;
    int length = strlen(description) + 1;
    new->description = malloc(length * sizeof(char));
    memmove(new->description, description, length);
    new->month = month;
    new->day = day;
    new->hour = hour;
    
    //print only if called from main function (with command A)
    if (flag == 0) {
        printf("This entry was added to the calendar.\n");
    }

}

/* Deleting a calendar entry from the calendar.*/
void delete_entry(const char *command, struct entry *calendar) {

    int month, day, hour;

    int res = sscanf(command, "D %u %u %u", &month, &day, &hour);

    //checking for invalid commands
    if (res < 3 || res > 3) { 
        printf("Invalid delete entry call: %s\n", command);
        return;
    } 

    struct entry *current = calendar;
    struct entry *previous = calendar;
    //checking if there is such an entry to delete
    while(current != NULL) { 
        if (current->month == month && current->day == day && current->hour == hour) {
            break;
        }
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("There is no such entry to be deleted.\n");
        return;
    }

    //current points to the entry to be removed and previous to the previous entry 
    previous->next = current->next;
    free(current->description);
    free(current);
    printf("This entry was deleted from the calendar.\n");

}

/* Freeing all allocated memory. */
void cleanup(struct entry *calendar) {

    struct entry *current = calendar;
    while (current != NULL) { 
        struct entry *ptr = current->next;
        if (current->description != NULL) {
            free(current->description);
        }
        free(current);
        current = ptr;
    }
    calendar = NULL;

}

/* Compare two entries by date. */
int my_compare(const void *a, const void *b) {
    
    struct entry *first = (struct entry *)a;
    struct entry *second = (struct entry *)b;
    char date1[7];
    char date2[7];
    sprintf(date1, "%02d%02d%02d", first->month, first->day, first->hour);
    sprintf(date2, "%02d%02d%02d", second->month, second->day, second->hour);
    int res = strcmp(date1, date2);
    return res;
}

/* Print entries in sorted order. */
void print_entries(struct entry *calendar) {

    if (calendar->next == NULL) {
        printf("The calendar is empty.\n");
        return;
    }

    struct entry *current = calendar;
    int length = 0;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    length = length - 1; //forget the first null entry
    struct entry *table = (struct entry *)malloc(length * sizeof(struct entry));

    current = calendar->next; //forget the first null entry
    struct entry *pos = table;
    while (current != NULL) {
        memmove(pos, current, sizeof(struct entry));
        current = current->next;
        pos++;
    }

    //sort the table by date
    qsort(table, length, sizeof(struct entry), my_compare);

    printf("The entries in the calendar are:\n");
    for (int i = 0; i < length; i++) {
        printf("%s %02d.%02d. klo %02d\n", table[i].description, table[i].day, table[i].month, table[i].hour);
    }

    free(table);

}

/* Saving all entries in the calendar to a file with the given name. */
void write_calendar(const char *command, struct entry *calendar) {
    
    char filename[20];
    int res = sscanf(command, "W %s", filename);

    //checking for invalid commands
    if (res < 1 || res > 1) { 
        printf("Invalid write calendar into file call: %s", command); //variable "command" already contains \n
        return;
    }

    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Problem occured while opening a file.\n");
        return;
    }

    struct entry *current = calendar->next;
    while(current != NULL) {
        fprintf(f, "%s %d %d %d\n", current->description, current->month, current->day, current->hour);
        current = current->next;
    }

    printf("Calendar was saved into a file called %s.\n", filename);
    fclose(f);

}

struct entry* open_calendar(const char *command, struct entry *calendar, int flag) {

    char filename[20];
    int res = sscanf(command, "O %s", filename);

    //checking for invalid commands
    if (res < 1 || res > 1) { 
        printf("Invalid open calendar from file call: %s", command); //variable "command" contains \n
        return NULL;
    }

    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Problem occured while opening a file.\n");
        return NULL;
    }

    //freeing the existing calendar
    cleanup(calendar);

    struct entry *new_calendar = malloc(sizeof(struct entry));
    if (!new_calendar) {
        printf("Error with memory allocation.\n");
    }
    struct entry *current = new_calendar;
    current->description = NULL;
    current->month = 0;
    current->day = 0;
    current->hour = 0;
    current->next = NULL;

    char description[20];
    int month, day, hour;
    char add_command[80];

    while (fscanf(f, "%s %d %d %d", description, &month, &day, &hour) != EOF) {
        if (description == NULL && month == 0 && day == 0 && hour == 0) {
            break;
        }
        sprintf(add_command, "A %s %d %d %d", description, month, day, hour);
        add_entry(add_command, new_calendar, flag);
    }

    printf("Calendar was read from file %s.\n", filename);
    fclose(f);

    return new_calendar;
}   

/* The main function for using the programme. */
int main(void) {

    int continues = 1;
    struct entry *calendar = malloc(sizeof(struct entry));
    if (!calendar) {
        printf("Error with memory allocation.\n");
    }

    struct entry *current = calendar;
    current->description = NULL;
    current->month = 0;
    current->day = 0;
    current->hour = 0;
    current->next = NULL;

    //a flag to determine if add_entry was called from open_calendar or main function
    int flag = 0; 

    while(continues) {
        char buffer[80];
        char *ret = fgets(buffer, 80, stdin); //reading commands from the input stream
        if (ret == NULL) {
            continues = 0;
            printf("Nothing more to read.\n");
            break;
        }
        switch(ret[0]) {
            case 'A':
                flag = 0;
                add_entry(ret, calendar, flag);
                break;

            case 'D':
                delete_entry(ret, calendar);
                break;

            case 'L':
                print_entries(calendar);
                break;

            case 'W':
                write_calendar(ret, calendar);
                break;

            case 'O':
                flag = 1;
                calendar = open_calendar(ret, calendar, flag);
                break;

            case 'Q':
                printf("Quitting program.\n");
                continues = 0;
                break;

            default:
                printf("Unknown command.\n");
                break;
        }
    }

    cleanup(calendar);
}
