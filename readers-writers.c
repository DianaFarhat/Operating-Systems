#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SPECIAL_READERS 3

sem_t order, wrt, mutex;
int readcount = 0;
int n; // Number of times readers and writers execute
int readerCount = 0; // Counter for the number of times readers executed
int writerCount = 0; // Counter for the number of times writers executed

void *specialReader(void *arg) {
    int id = *(int *)arg;
    int count = 0;

    while (count < n) {
        //Enter
        sem_wait(&order);
        printf("SpecialReader %d: Enter\n", id);
        sem_wait(&mutex);
        readcount++;
        if (readcount == 1)
            sem_wait(&wrt);
        sem_post(&mutex);
        sem_post(&order);

        // Execution
        printf("SpecialReader %d: Execute\n", id);
        
        //Exit Section
        sem_wait(&mutex);
        readcount--;
        if (readcount == 0)
            sem_post(&wrt);
        sem_post(&mutex);

        printf("SpecialReader %d: Exit\n", id);

        count++;
    }

    return NULL;
}

void *specialWriter(void *arg) {
    int id = *(int *)arg;
    int count = 0;

    while (count < n) {
        // Enter Section
        sem_wait(&order);
        printf("SpecialWriter %d: Enter\n", id);
        sem_wait(&wrt);
        sem_post(&order);

        // Critical Section
        printf("SpecialWriter %d: Execute\n", id);
        
        //Exit Section
        printf("SpecialWriter %d: Exit\n", id);

        sem_post(&wrt);

        count++;
    }

    return NULL;
}

int main() {
    int numSpecialReaders = 5; // Number of special readers
    int numSpecialWriters = 5; // Number of special writers
    int n_values[] = {10, 20, 50, 100, 500, 1000}; // Values of n for benchmarking
    int num_n_values = sizeof(n_values) / sizeof(n_values[0]); // Number of n values to benchmark

    // Initialize semaphores
    sem_init(&order, 0, 1);
    sem_init(&wrt, 0, 1);
    sem_init(&mutex, 0, 1);

    // Create output file
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    pthread_t specialReaders[numSpecialReaders];
    pthread_t specialWriters[numSpecialWriters];
    int specialReaderIds[numSpecialReaders];
    int specialWriterIds[numSpecialWriters];

    for (int k = 0; k < num_n_values; k++) {
        n = n_values[k];

        // Reset counters
        readerCount = 0;
        writerCount = 0;

        // Create special reader threads
        for (int i = 0; i < numSpecialReaders; i++) {
            specialReaderIds[i] = i + 1;
            pthread_create(&specialReaders[i], NULL, specialReader, &specialReaderIds[i]);
        }

        // Create special writer threads
        for (int i = 0; i < numSpecialWriters; i++) {
            specialWriterIds[i] = i + 1;
            pthread_create(&specialWriters[i], NULL, specialWriter, &specialWriterIds[i]);
        }

        // Wait for special reader threads to finish
        for (int i = 0; i < numSpecialReaders; i++) {
            pthread_join(specialReaders[i], NULL);
        }

        // Wait for special writer threads to finish
        for (int i = 0; i < numSpecialWriters; i++) {
            pthread_join(specialWriters[i], NULL);
        }

        // Write benchmark results to output file
        fprintf(outputFile, "#%d\n", n);
        fprintf(outputFile, "nb of readers: %d\n", numSpecialReaders);
        fprintf(outputFile, "nb of writers: %d\n", numSpecialWriters);
        for (int i = 0; i < numSpecialReaders; i++) {
            fprintf(outputFile, "%d\n", readerCount);
        }
        for (int i = 0; i < numSpecialWriters; i++) {
            fprintf(outputFile, "%d\n", writerCount);
        }
    }

    // Close output file
    fclose(outputFile);


    // Destroy semaphores
    sem_destroy(&order);
    sem_destroy(&wrt);
    sem_destroy(&mutex);

    return 0;
}
