#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void get_date_data(char *data);
int *get_data(char *date, int entries);
void readDateFromUser(char *date);
void getDataFromDate(char *date, char*dateData);

int main() {
    char date[50], data[400];
  
    do {
        readDateFromUser(date); 
        getDataFromDate(date, data);
    }
    while( strcmp("none", data) == 0 ); 
    // if no data ask user for date again
   
    get_date_data(data);
    return 0;
}


void readDateFromUser(char *date){
    int m, d, y;

    puts("Enter the date you want to get data for.\n  Format: M/D/Y - Do Not Include Leading Zeros\n  Valid Date Range: 7/6/10 - 10/4/19");
    
    scanf("%d/%d/%d", &m, &d, &y);
    sprintf(date, "%d/%d/%d", m, d, y);
}


void getDataFromDate(char * date, char * dateData){
    FILE *fp;
    unsigned short int dataFound = 0; //saves memory
    fp = fopen("./spxpc.csv", "r");

    // loop through until match is found
    while( fgets(dateData, 399, (FILE*)fp) != NULL ){
        if( strstr(dateData, date)){
            dataFound = 1;
            break;
        } 
    } 
    if( dataFound == 0){ // if no match was ever found
        printf("\nNo entry found for %s\n\n", date);
        strcpy(dateData, "none");
    }
    fclose(fp);
}


void getStockQuality(double ratio, char *quality){
    if( ratio >= 2 ){ // bearish
        strcpy(quality,"dangerously bearish");
    } else if( ratio >= 1.75){
        strcpy(quality,"very bearish");
    } else if( ratio >= 1.3){
        strcpy(quality,"moderately bearish");
    } else if( ratio >= 1){
        strcpy(quality,"normal");
    } else if( ratio >= 0.9){
        strcpy(quality,"moderately bullish");
    } else {
        strcpy(quality,"very bullish");
    }
}

void getVolumeQuality(double volume, char *quality){
    if( volume >= 8e5 ){ 
        strcpy(quality,"very high");
    } else if( volume >= 7e5){
        strcpy(quality,"high");
    } else if( volume >= 6e5){
        strcpy(quality,"moderately high");
    } else if( volume >= 5e5){
        strcpy(quality,"normal");
    } else if( volume >= 4e5){
        strcpy(quality,"less than normal");
    } else if( volume >= 3e5){
        strcpy(quality,"moderately low");
    } else if( volume >= 2e5){
        strcpy(quality,"low");
    } else {
        strcpy(quality,"very low");
    }
}

void printVolumeQualityMessage(char *message, double volume ){
    char quality[30];
    getVolumeQuality(volume, quality);
    printf("\n\t\t%s %s.\n\n", message, quality);
}

void printStockQualityMessage(char *message, double volume ){
    char quality[30];
    getStockQuality(volume, quality);
    printf("\n\t\t%s %s.\n\n", message, quality);
}


void get_date_data(char*data) {
    char *tknPtr, *endPtr;
  
    // Stock Data
    tknPtr = strtok(data, ",");
    printf("\n\nThe Stock Data for the Date: %s\n", tknPtr);

    // Put/Call Ratio
    tknPtr = strtok(NULL, ",");
    printf("\tThe Put/Call ratio is: %s", tknPtr);
    printStockQualityMessage("The Stock is", strtod(tknPtr, &endPtr));

    // Put
    tknPtr = strtok(NULL, ",");
    printf("\tThe SPX Put Volume is: %s", tknPtr);
    printVolumeQualityMessage("The SPX Put Volume is", strtod(tknPtr, &endPtr));

    // Call
    tknPtr = strtok(NULL, ",");
    printf("\tThe SPX Call Volume is: %s", tknPtr);
    printVolumeQualityMessage("The SPX Call Volume is", strtod(tknPtr, &endPtr));

    // Options Volume
    tknPtr = strtok(NULL, ",");
    printf("\tThe SPX Options Volume is: %s", tknPtr);
    printVolumeQualityMessage("The SPX Options Volume is", (strtod(tknPtr, &endPtr) / 2)); // averaged out

}

int *get_data(char *date, int entries) {
    FILE *fp;
    char string[400];
    char *tknPtr;
    char *endPtr;
    int i;
    char date2[50];
    int month, day, year;
    float sum, init, final;
    int count;
    
    count = 0;
    sum = 0;
    
    fp = fopen("./spxpc.csv", "r");
    for (i = 0; i < 2332; i++) {
        fgets(string, 399, (FILE*)fp);
        tknPtr = strtok(string, ",");
        if ( strstr(string, date) != NULL ) {
            printf("Start Date: %s\n", tknPtr);
            tknPtr = strtok(NULL, ",");
            sum += strtod(tknPtr, &endPtr);
            init = sum;
            count += 1;
        }
        else {
            if (count > 0) {
                tknPtr = strtok(NULL, ",");
                final = strtod(tknPtr, &endPtr);
                sum += final;
                count += 1;
                if ( count == entries ) {
                    printf("Avg P/C Ratio: %f\n", sum / count);
                    printf("Initial P/C Ratio: %f\n", init);
                    printf("Final P/C Ratio: %f\n", final);
                    if (init + 0.1 < final) {
                        printf("Woohoo! The stock is doing very well after %d entries.\n", count);
                    }
                    else {
                        if (init + 0.03 < final) {
                            printf("Woops! The stock is doing well after %d entries.\n", count);
                        }
                        else {
                            if (init + 0.03 > final) {
                                printf("The stock is a little worse after %d entries.\n", count);
                            }
                            else {
                                printf("The stock is doing a lot worse after %d entries.\n", count);
                            }
                        }
                    }
                    fclose(fp);
                    printf("\n");
                    return 0;
                }
                else {
                    if (i >= 2331) {
		        printf("Not enough dates for a full %d-day analysis. Only read %d entries.\n", entries, count);
		        printf("Avg P/C Ratio: %f\n", sum / count);
		        printf("Initial P/C Ratio: %f\n", init);
		        printf("Final P/C Ratio: %f\n\n", final);
		   }
                }
            }
        }
    }
    fclose(fp);
    return 0;
}
