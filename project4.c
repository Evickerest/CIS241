#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printDateData(char *data);
int *printDateFromRange(char *date, int entries);
void readDateFromUser(char *date);
void getDataFromDate(char *date, char*dateData);

int main() {
    char date[50], data[400];
    int entries, extraEntires = -1;

    // main loop
    while( extraEntires == -1){
        do {
            readDateFromUser(date); 
            getDataFromDate(date, data);
        }
        // if no data ask user for date again
        while( strcmp("none", data) == 0 ); 
    
        printDateData(data); // Print out data from date

        printf("\n\nEnter number of entries (roughly days) starting from %s (inclusive) to compute the range averages.\n", date);
        scanf("%d", &entries);
        printDateFromRange(date, entries);

        printf("\n\nEnter another number of entries from %s. (0 to quit or -1 to enter new start Date)\n", date);
        scanf("%d", &extraEntires );

        // if user enters 0, loop will quit
        // if user enters -1, main loop will begin again
        while( extraEntires > 0 ){
            printDateFromRange(date, extraEntires);
            printf("\n\nEnter another number of entries from %s. (0 to quit or -1 to enter new start Date)\n", date);
            scanf("%d", &extraEntires );
        };
    }

    return 0;
}


void readDateFromUser(char *date){ // Reads a date from the user
    int m, d, y;

    puts("\nEnter the date you want to get data for.\n  Format: M/D/Y - Do Not Include Leading Zeros\n  Valid Date Range: 7/6/10 - 10/4/19");
    
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
    printf("\n\t\t%s %s\n", message, quality);
}


void printStockQualityMessage(char *message, double volume ){ // Print the put/call ratio message
    char quality[30];
    getStockQuality(volume, quality);
    printf("\n\t\t%s %s\n", message, quality);
}


void printDateData(char*data) { // Prints all information about the selected date
    char *tknPtr, *endPtr;
  
    // Stock Data
    tknPtr = strtok(data, ",");
    printf("\n\nThe Stock Data for the Date: %s\n", tknPtr);

    // Put/Call Ratio
    tknPtr = strtok(NULL, ",");
    printf("\tThe Put/Call ratio is: %s\t", tknPtr);
    printStockQualityMessage("The Stock is", strtod(tknPtr, &endPtr));

    // Put
    tknPtr = strtok(NULL, ",");
    printf("\tThe SPX Put Volume is: %s\t", tknPtr);
    printVolumeQualityMessage("The SPX Put Volume is", strtod(tknPtr, &endPtr));

    // Call
    tknPtr = strtok(NULL, ",");
    printf("\tThe SPX Call Volume is: %s\t", tknPtr);
    printVolumeQualityMessage("The SPX Call Volume is", strtod(tknPtr, &endPtr));

    // Options Volume
    tknPtr = strtok(NULL, ",");
    tknPtr[strcspn(tknPtr,"\n")] = 0;  //removes trailing whitespace

    printf("\tThe SPX Options Volume is: %s\t", tknPtr);
    printVolumeQualityMessage("The SPX Options Volume is", (strtod(tknPtr, &endPtr) / 2)); // averaged out
}

void printRangeDate(char *firstData, char *endData){
    char *firstDate, *endDate, 
         *firstRatio, *endRatio,
         *firstPutVolume, *endPutVolume,
         *firstCallVolume, *endCallVolume,
         *firstVolume, *endVolume,
         *endPtr, *endPtr2;
    double difference = 0;

    firstDate = strtok(firstData, ",");
    firstRatio = strtok(NULL, ",");
    firstPutVolume = strtok(NULL, ",");
    firstCallVolume = strtok(NULL, ",");
    firstVolume = strtok(NULL, ",");
    firstVolume[strcspn(firstVolume,"\n")] = 0; //removes trailing whitespace

    endDate = strtok(endData, ",");
    endRatio = strtok(NULL, ",");
    endPutVolume = strtok(NULL, ",");
    endCallVolume = strtok(NULL, ",");
    endVolume = strtok(NULL, ",");
    endVolume[strcspn(endVolume,"\n")] = 0;  //removes trailing whitespace

    printf("\n\n\t\t -- %s -- \t -- %s -- \t -- Difference --\n", firstDate, endDate);

    // Put/Call Ratio
    difference = strtod(endRatio, &endPtr)-strtod(firstRatio, &endPtr2);
    printf("Put/Call Ratio \t| %s \t\t| %s \t\t| %f\n",firstRatio, endRatio, difference);

    // Put Volume
    difference = strtod(endPutVolume, &endPtr)-strtod(firstPutVolume, &endPtr2);
    printf("Put Volume \t| %s \t| %s \t| %f\n",firstPutVolume, endPutVolume, difference);

    // Call Volume
    difference = strtod(endCallVolume, &endPtr)-strtod(firstCallVolume, &endPtr2);
    printf("Call Volume \t| %s \t| %s \t| %f\n",firstCallVolume, endCallVolume, difference);

    // Total Volume
    difference = strtod(endVolume, &endPtr)-strtod(firstVolume, &endPtr2);
    printf("Volume \t\t| %s \t| %s \t| %f\n",firstVolume, endVolume, difference);

    printf("\nStock data for %s:", endDate);
    printStockQualityMessage("The Stock is", strtod(endRatio, &endPtr));
    printVolumeQualityMessage("The SPX Put Volume is", strtod(endPutVolume, &endPtr));
    printVolumeQualityMessage("The SPX Call Volume is", strtod(endCallVolume, &endPtr));
    printVolumeQualityMessage("The SPX Options Volume is", (strtod(endVolume, &endPtr) / 2)); // averaged out



}

int *printDateFromRange(char *date, int entries) { // Gets data and analysis for put/call ratio
    FILE *fp;
    char *tknPtr,*endPtr,*endDate, string[400], data1[400], data2[400];
    int month, day, year, entryCount;
    float sumRatio, initRatio, finalRatio;
    
    entryCount = 0;
    sumRatio = 0;
    
    fp = fopen("./spxpc.csv", "r");
    for (int i = 0; i < 2332; i++) { // Loop through all the lines of the file
        fgets(string, 399, (FILE*)fp); // Stores the contents of a line
        tknPtr = strtok(string, ","); // Gets the first string token for the date
        if ( strstr(string, date) != NULL ) { // If the date of the token matches the desired date
            endDate = tknPtr; // The date at the end of the range
            tknPtr = strtok(NULL, ","); // Get the put/call ratio
            sumRatio += strtod(tknPtr, &endPtr); // Add the put/call ratio to sumRatio
            initRatio = sumRatio; // Store the initial put/call ratio
            finalRatio = sumRatio; // The put/call ratio at this moment
            entryCount += 1; // Increment entryCount to allow for averaging later
            if (i >=2331) { // Prints if the date is the last one
                printf("\nThis is the last date! No future analysis possible.\n");
            }

        } else {
            if (entryCount > 0) { // Adds data from dates after the first match
                endDate = tknPtr; // The date at the end of the range
                tknPtr = strtok(NULL, ",");
                finalRatio = strtod(tknPtr, &endPtr); // The put/call ratio at the end of the range
                sumRatio += finalRatio; // Add the put/call ratio to the sumRatio
                entryCount += 1; // Increment entryCount to allow for averaging later
                if ( entryCount >= entries ) { // Prints if there are enough entries for the full range analysis
                    printf("\nDate Range: %s to %s\n", date, endDate);
                    printf("\tAvg P/C Ratio: %f\n", sumRatio / entryCount);
                    printf("\tInitial P/C Ratio: %f\n", initRatio);
                    printf("\tFinal P/C Ratio: %f\n", finalRatio);
                    i = 2332;
                } else {
                    if (i >= 2331) { // Prints if there aren't enough entries for the full range analysis
                        printf("\nNot enough dates for a full %d-day analysis. Only read %d entries.\n", entries, entryCount);
                        printf("\nDate Range: %s to %s\n", date, endDate);
                        printf("\tAvg P/C Ratio: %f\n", sumRatio / entryCount);
                        printf("\tInitial P/C Ratio: %f\n", initRatio);
                        printf("\tFinal P/C Ratio: %f\n", finalRatio);
		    }
                }
            }
        }
    }

    // Prints differently based on how the intial put/call ratio compares with the final
    if (initRatio < finalRatio) {
        printf("The stock is doing worse after %d entries.\n", entryCount);
        printf("The put/call ratio has increased after %d entries.\n", entryCount);
        printf("Recommendation: Consider buying more puts.\n");
    } else if (initRatio > finalRatio) {
        printf("Woohoo! The stock is doing very well after %d entries.\n", entryCount);
        printf("The put/call ratio has decreased after %d entries.\n", entryCount);
        printf("Recommendation: Consider buying more calls.\n");
    } else {
        printf("The stock is doing similarly after %d entries.\n", entryCount);
        printf("The put/call ratio has remained the same after %d entries.\n", entryCount);
        printf("No specific recommendation for buying more puts or calls at this time.\n");
    }
   
    fclose(fp);

    getDataFromDate(date, data1);
    getDataFromDate(endDate, data2); // Get data about the endDate
    printRangeDate(data1, data2);

    return 0;
}
