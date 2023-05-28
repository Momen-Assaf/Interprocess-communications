#include "local.h"

void removeExtraSpaces(char *buffer) {
    int i, j;
    int spaceCount = 0;
    int len = strlen(buffer);

    // Remove spaces at the end of the line
    for (i = len - 1; i >= 0; i--) {
        if (buffer[i] == ' ')
            buffer[i] = '\0';
        else
            break;
    }

    // Remove consecutive extra spaces within the line
    for (i = 0; i < len; i++) {
        if (buffer[i] == ' ') {
            spaceCount++;
            if (spaceCount > 1) {
                for (j = i; j < len - 1; j++)
                    buffer[j] = buffer[j + 1];
                buffer[j] = '\0';
                len--;
                i--;
            }
        } else {
            spaceCount = 0;
        }
    }
}

int countColumns(char *buffer) {
    int count = 0;
    int len = strlen(buffer);
    int i;

    for (i = 0; i < len; i++) {
        if (buffer[i] == ' ' && buffer[i + 1] != ' ')
            count++;
    }

    return count + 1;  // Adding 1 for the last column
}

void padLine(char *buffer, int targetColumns) {
    int currentColumns = countColumns(buffer);

    while (currentColumns < targetColumns) {
        strcat(buffer, " fakeTxT");
        currentColumns++;
    }
}

int main(int argc, char** argv) {
    
    //char *test = "HI!";
    //create shared memory
    //int shm_id = shmget((key_t)sh_Key, 4096, 0666|IPC_CREAT);
    //void *sh_mem = shmat(shm_id,NULL,0);
    //strcpy(sh_mem,test);

    FILE *inputFile, *outputFile;
    char buffer[BUFFER_SIZE];
    int highestColumnCount = 0;
    int lineCount = 0;
    int i;

    // Open the input file
    inputFile = fopen("sender.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening the input file");
        return EXIT_FAILURE;
    }

    // Open the output file
    outputFile = fopen("senderEdit.txt", "w");
    if (outputFile == NULL) {
        perror("Error creating the output file");
        fclose(inputFile);
        return EXIT_FAILURE;
    }

    // Read input file line by line
    while (fgets(buffer, BUFFER_SIZE, inputFile) != NULL) {
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        // Remove extra spaces and trailing spaces
        removeExtraSpaces(buffer);

        // Determine the number of columns
        int columnCount = countColumns(buffer);
        if (columnCount > highestColumnCount)
            highestColumnCount = columnCount;

        // Write the line to the output file without padding
        //fprintf(outputFile, "%s\n", buffer);

        lineCount++;
    }

    // Rewind the input file
    rewind(inputFile);

    // Pad the lines with "fakeTxT" to match the highest column count
    for (i = 0; i < lineCount; i++) {
        fgets(buffer, BUFFER_SIZE, inputFile);
        buffer[strcspn(buffer, "\n")] = '\0';

        // Remove extra spaces and trailing spaces from the updated buffer
        removeExtraSpaces(buffer);

        padLine(buffer, highestColumnCount);

        // Write the modified line to the output file with newline character
        fprintf(outputFile, "%s\n", buffer);
    }

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File processing completed successfully.\n");
    printf("Highest number of columns: %d\n", highestColumnCount);

    /*pid_t parent_id = getpid();
    pid_t children_id[highestColumnCount];
    for (int c = 0; c < highestColumnCount; c++)
    {
        children_id[i] = getpid();
        if( children_id[i] == -1){
            perror("No Memory\n");
            exit(-1);           
        }
        else if ( children_id[i] == 0){
            printf("encoding...\n");
            ///code...
        }
        else{
            printf("Iam sender proccess\n");
            ///code...
            sleep(1);
            for (int k = 0 ; k < highestColumnCount ; k++){
                kill(children_id[k],SIGTERM);
                printf("Pid:%d killed\n",children_id[k]);
            }  
        } 
    }
    
    */
   printf("Iam sender proccess\n");
    return EXIT_SUCCESS;
}
