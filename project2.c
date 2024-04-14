#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printDataFromDate(char *data);
int *printDateFromDateRange(char *date, int entries);
void readDateFromUser(char *date);
void getDataFromDate(char *date, char*dateData);

int main() {
    char date[50], data[400];
    int entries;
  
    do {
        readDateFromUser(date); 
        getDataFromDate(date, data);
    }
    while( strcmp("none", data) == 0 ); 
    // if no data ask user for date again
   
    printDataFromDate(data); // Print out data from date

    printf("Enter number of entries (roughly days) starting from %s (inclusive) to compute the range averages.\n", date);
    scanf("%d", &entries);
    printDateFromDateRange(date, entries);

    return 0;
}


void readDateFromUser(char *date){ // Reads a date from the user
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


void getStockQuality(double ratio, char *quality){ // Determines what message to print for put/call ratio
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


void getVolumeQuality(double volume, char *quality){ // Determines what message to print for stock volume
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


void printVolumeQualityMessage(char *message, double volume ){ // Print the volume string message
    char quality[30];
    getVolumeQuality(volume, quality);
    printf("\n\t\t%s %s.\n\n", message, quality);
}


void printStockQualityMessage(char *message, double volume ){ // Print the put/call ratio message
    char quality[30];
    getStockQuality(volume, quality);
    printf("\n\t\t%s %s.\n\n", message, quality);
}


void printDataFromDate(char*data) { // Prints all information about the selected date
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

int *printDateFromDateRange(char *date, int entries) { // Gets data and analysis for put/call ratio
    FILE *fp;
    char *tknPtr,*endPtr,*endDate,string[400];
    int month, day, year, entryCount;
    float ratioSum, initRatio, finalRatio;
    
    entryCount = 0;
    ratioSum = 0;
    
    fp = fopen("./spxpc.csv", "r");
    for (int i = 0; i < 2332; i++) { // Loop through all the lines of the file
        fgets(string, 399, (FILE*)fp); // Stores the contents of a line
        tknPtr = strtok(string, ","); // Gets the first string token for the date

        if ( strstr(string, date) != NULL ) { // If the date of the token matches the desired date
            tknPtr = strtok(NULL, ","); // Get the put/call ratio
            ratioSum += strtod(tknPtr, &endPtr); // Add the put/call ratio to ratioSum
            initRatio = ratioSum; // Store the initial put/call ratio 
            entryCount += 1; // Increment count to allow for averaging later
            if (i >=2331) { // Prints if the date is the last one
                printf("\nThis is the last date! No future analysis possible.\n");
            }

        } else {
            if (entryCount > 0) { // Adds data from dates after the first match
                endDate = tknPtr; // The date at the end of the range
                tknPtr = strtok(NULL, ",");
                finalRatio = strtod(tknPtr, &endPtr); // The put/call ratio at the end of the range
                ratioSum += finalRatio; // Add the put/call ratio to the ratioSum
                entryCount += 1; // Increment count to allow for averaging later
                if ( entryCount >= entries ) { // Prints if there are enough entries for the full range analysis
                    printf("\nDate Range: %s to %s\n", date, endDate);
                    printf("\tAvg P/C Ratio: %f\n", ratioSum / entryCount);
                    printf("\tInitial P/C Ratio: %f\n", initRatio);
                    printf("\tFinal P/C Ratio: %f\n", finalRatio);

		    // prints differently based on how the intial put/call ratio compares with the final
                    if (initRatio + 0.1 < finalRatio) {
                        printf("Woohoo! The stock is doing very well after %d entries.\n", entryCount);
                    } else if (initRatio + 0.03 < finalRatio) {
                        printf("Woo! The stock is doing well after %d entries.\n", entryCount);
                    } else if (initRatio + 0.03 > finalRatio) {
                        printf("The stock is a little worse after %d entries.\n", entryCount);
                    } else {
                        printf("The stock is doing a lot worse after %d entries.\n", entryCount);
                    }
                    fclose(fp);
                    printf("\n");
                    return 0;
                } else {
                    if (i >= 2331) { // Prints if there aren't enough entries for the full range analysis
                        printf("\nNot enough dates for a full %d-day analysis. Only read %d entries.\n", entries, entryCount);
                        printf("\nDate Range: %s to %s\n", date, endDate);
                        printf("\tAvg P/C Ratio: %f\n", ratioSum / entryCount);
                        printf("\tInitial P/C Ratio: %f\n", initRatio);
                        printf("\tFinal P/C Ratio: %f\n\n", finalRatio);
		    }        
                }
            } 
        }
    }

    fclose(fp);
    return 0;
}
