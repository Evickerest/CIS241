#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *get_date_data(char *date);

int main() {
    FILE *fp;
    char date[50];
    int month;
    int day;
    int year;
    puts("Enter the date you want to get data for.\n  Format: M/D/Y - Do Not Include Leading Zeros\n  Valid Date Range: 7/6/10 - 10/4/19");
    scanf("%d/%d/%d", &month, &day, &year);
    sprintf(date, "%d/%d/%d", month, day, year);

    get_date_data(date);
    return 0;
}

int *get_date_data(char *date) {
    FILE *fp;
    char string[400];
    char *tknPtr;
    char *endPtr;
    int i;
    char date2[50];
    int month;
    int day;
    int year;
    
    puts("\n");
    fp = fopen("./spxpc.csv", "r");
    for (i = 0; i < 2332; i++) {
        fgets(string, 399, (FILE*)fp);
        tknPtr = strtok(string, ",");
        if ( strstr(string, date) != NULL ) {
            printf("The Stock Data for the Date: %s\n", tknPtr);
            tknPtr = strtok(NULL, ",");
            printf("  The Put/Call Ratio is: %s\n", tknPtr);
            if ( strtod(tknPtr, &endPtr) < 0.4 ) {
                printf("    The Stock is Dangerously Bearish: %s\n", tknPtr);
            }
            else {
                if ( strtod(tknPtr, &endPtr) < 0.7 ) {
                    printf("    The Stock is Bearish: %s\n", tknPtr);
                }
                else {
                    if ( strtod(tknPtr, &endPtr) > 1.2 ) {
                        printf("    The Stock is Very Bullish: %s\n", tknPtr);
                    }
                    else {
                        if ( strtod(tknPtr, &endPtr) > 1 ) {
                            printf("    The Stock is Bullish: %s\n", tknPtr);
                        }
                        else {
                            printf("    The Stock is Normal: %s\n", tknPtr);
                        }
                    }
                }
            }
            tknPtr = strtok(NULL, ",");
            printf("  The SPX Put Volume is: %s\n", tknPtr);
            if ( strtod(tknPtr, &endPtr) > 700000 ) {
                printf("    The Stock Has Very High Put: %s\n", tknPtr);
            }
            else {
                if ( strtod(tknPtr, &endPtr) > 500000 ) {
                    printf("    The Stock Has High Put: %s\n", tknPtr);
                }
                else {
                    if ( strtod(tknPtr, &endPtr) > 300000 ) {
                        printf("    The Stock Has Normal Put: %s\n", tknPtr);
                    }
                    else {
                        if ( strtod(tknPtr, &endPtr) > 100000 ) {
                            printf("    The Stock Has Low Put: %s\n", tknPtr);
                        }
                        else {
                            printf("    The Stock Has Very Low: %s\n", tknPtr);
                        }
                    }
                }
            }
            tknPtr = strtok(NULL, ",");
            printf("  The SPX Call Volume is: %s\n", tknPtr);
            if ( strtod(tknPtr, &endPtr) > 700000 ) {
                printf("    The Stock Has Very High Call: %s\n", tknPtr);
            }
            else {
                if ( strtod(tknPtr, &endPtr) > 500000 ) {
                    printf("    The Stock Has High Call: %s\n", tknPtr);
                }
                else {
                    if ( strtod(tknPtr, &endPtr) > 300000 ) {
                        printf("    The Stock Has Normal Call: %s\n", tknPtr);
                    }
                    else {
                        if ( strtod(tknPtr, &endPtr) > 100000 ) {
                            printf("    The Stock Has Low Call: %s\n", tknPtr);
                        }
                        else {
                            printf("    The Stock Has Very Call: %s\n", tknPtr);
                        }
                    }
                }
            }
            tknPtr = strtok(NULL, ",");
            printf("  The Total SPX Options Volume is: %s", tknPtr);
            if ( strtod(tknPtr, &endPtr) > 1000000 ) {
                printf("    The Stock Has Very High Volume: %s\n", tknPtr);
            }
            else {
                if ( strtod(tknPtr, &endPtr) > 700000 ) {
                    printf("    The Stock Has High Volume: %s\n", tknPtr);
                }
                else {
                    if ( strtod(tknPtr, &endPtr) > 500000 ) {
                        printf("    The Stock Has Normal Volume: %s\n", tknPtr);
                    }
                    else {
                        if ( strtod(tknPtr, &endPtr) > 300000 ) {
                            printf("    The Stock Has  Low Volume: %s\n", tknPtr);
                        }
                        else {
                            printf("    The Stock Has Very Low: %s\n", tknPtr);
                        }
                    }
                }
            }
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    printf("The date (%s) doesn't have an entry.\n", date);
    puts("Enter another date you want to get data for.\n  Format: M/D/Y - Do Not Include Leading Zeros\n  Valid Date Range: 7/6/10 - 10/4/19");
    scanf("%d/%d/%d", &month, &day, &year);
    sprintf(date2, "%d/%d/%d", month, day, year);
    get_date_data(date2);
    return 0;
}
