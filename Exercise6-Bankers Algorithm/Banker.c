[24bcs044@mepcolinux ex6]$cat ban2.c
#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 20
#define MAX_RESOURCES 20

int n, m;
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];

/* Calculate Need = Max - Allocation */
void calculateNeed() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

/* Display Need Matrix */
void printNeedMatrix() {
    printf("\nNEED MATRIX (Max - Allocation)\n\n");

    printf("Process\t");
    for (int j = 0; j < m; j++)
        printf("R%d\t", j + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("T%d\t", i + 1);

        for (int j = 0; j < m; j++)
            printf("%d\t", need[i][j]);

        printf("\n");
    }
}

/*
   Safety Algorithm
   Returns true if system is safe
*/
bool isSafeState(bool display) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safeSequence[MAX_PROCESSES];
    int count = 0;

    /* Initially Work = Available */
    for (int j = 0; j < m; j++)
        work[j] = available[j];

    if (display) {
        printf("\nInitial WORK = ( ");
        for (int j = 0; j < m; j++)
            printf("%d ", work[j]);
        printf(")\n");
    }

    while (count < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {

            if (!finish[i]) {
                bool canFinish = true;

                /* Check if Need <= Work */
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }

                if (canFinish) {

                    if (display) {
                        printf("\nT%d can finish because Need <= Work\n", i + 1);

                        printf("Work before T%d finishes: ( ", i + 1);
                        for (int j = 0; j < m; j++)
                            printf("%d ", work[j]);
                        printf(")\n");
                    }

                    /*
                       When process finishes,
                       it releases its allocated resources
                    */
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }

                    if (display) {
                        printf("T%d releases its Allocation.\n", i + 1);

                        printf("New Work: ( ");
                        for (int j = 0; j < m; j++)
                            printf("%d ", work[j]);
                        printf(")\n");
                    }

                    safeSequence[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        /* No remaining process can finish */
        if (!found) {
            if (display)
                printf("\nSYSTEM IS UNSAFE! No safe sequence exists.\n");

            return false;
        }
    }

    if (display) {
        printf("\n====================================\n");
        printf("SYSTEM IS IN A SAFE STATE\n");
        printf("Safe Sequence: ");

        for (int i = 0; i < n; i++) {
            printf("T%d", safeSequence[i] + 1);

            if (i != n - 1)
                printf(" -> ");
        }

        printf("\n====================================\n");
    }

    return true;
}

/* Handle a resource request */
void resourceRequest(int process) {
    int request[MAX_RESOURCES];

    printf("\nEnter resource request for T%d:\n", process + 1);

    for (int j = 0; j < m; j++) {
        printf("Resource R%d: ", j + 1);
        scanf("%d", &request[j]);
    }

    /* Check Request <= Need */
    for (int j = 0; j < m; j++) {
        if (request[j] > need[process][j]) {
            printf("\nREQUEST DENIED!\n");
            printf("The request exceeds the maximum remaining NEED.\n");
            return;
        }
    }

    /* Check Request <= Available */
    for (int j = 0; j < m; j++) {
        if (request[j] > available[j]) {
            printf("\nREQUEST CANNOT BE GRANTED NOW.\n");
            printf("The requested resources are not currently available.\n");
            return;
        }
    }

    /*
       Temporarily allocate resources
    */
    for (int j = 0; j < m; j++) {
        available[j] -= request[j];
        allocation[process][j] += request[j];
        need[process][j] -= request[j];
    }

    printf("\nChecking safety after temporarily granting request...\n");

    /* Run Safety Algorithm */
    if (isSafeState(false)) {

        printf("\nREQUEST ACCEPTED!\n");
        printf("The request was granted and the system remains SAFE.\n");

        isSafeState(true);

    } else {

        /* Rollback changes */
        for (int j = 0; j < m; j++) {
            available[j] += request[j];
            allocation[process][j] -= request[j];
            need[process][j] += request[j];
        }

        printf("\nREQUEST DENIED!\n");
        printf("Granting this request would make the system UNSAFE.\n");
    }
}

int main() {
    int choice;
    char workGiven;

    printf("****************************************\n");
    printf("       BANKER'S ALGORITHM \n");
    printf("****************************************\n");

    /* Number of processes and resources */
    printf("\nEnter number of teams/processes: ");
    scanf("%d", &n);

    printf("Enter number of resource types: ");
    scanf("%d", &m);

    if (n > MAX_PROCESSES || m > MAX_RESOURCES || n <= 0 || m <= 0) {
        printf("Invalid number of processes or resources!\n");
        return 1;
    }

    /* Allocation Matrix */
    printf("\n--- ENTER ALLOCATION MATRIX ---\n");
    for (int i = 0; i < n; i++) {
        printf("\nFor T%d:\n", i + 1);

        for (int j = 0; j < m; j++) {
            printf("Allocation of R%d: ", j + 1);
            scanf("%d", &allocation[i][j]);
        }
    }

    /* Max Matrix */
    printf("\n--- ENTER MAX MATRIX ---\n");
    for (int i = 0; i < n; i++) {
        printf("\nFor T%d:\n", i + 1);

        for (int j = 0; j < m; j++) {
            printf("Maximum demand of R%d: ", j + 1);
            scanf("%d", &maximum[i][j]);

            if (maximum[i][j] < allocation[i][j]) {
                printf("ERROR: Max cannot be less than Allocation!\n");
                return 1;
            }
        }
    }

    /* Calculate Need */
    calculateNeed();

    /*
       Ask whether Work / Available is already given
    */
    printf("\nIs WORK / AVAILABLE vector given? (y/n): ");
    scanf(" %c", &workGiven);

    if (workGiven == 'y' || workGiven == 'Y') {

        printf("\nEnter WORK / AVAILABLE vector:\n");

        for (int j = 0; j < m; j++) {
            printf("R%d: ", j + 1);
            scanf("%d", &available[j]);
        }

    } else {

        int total[MAX_RESOURCES] = {0};
        int allocatedSum[MAX_RESOURCES] = {0};

        printf("\nWORK is not given.\n");
        printf("Enter TOTAL resources to calculate WORK.\n");

        for (int j = 0; j < m; j++) {
            printf("Total R%d: ", j + 1);
            scanf("%d", &total[j]);
        }

        /* Calculate sum of allocated resources */
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                allocatedSum[j] += allocation[i][j];
            }
        }

        /* Available = Total - Allocated */
        for (int j = 0; j < m; j++) {
            available[j] = total[j] - allocatedSum[j];

            if (available[j] < 0) {
                printf("ERROR: Total resources cannot be less than allocated resources!\n");
                return 1;
            }
        }

        printf("\nCalculated WORK / AVAILABLE = ( ");
        for (int j = 0; j < m; j++)
            printf("%d ", available[j]);
        printf(")\n");
    }

    /* Display Need */
    printNeedMatrix();

    /* Initial Banker's Algorithm check */
    printf("\n\n===== INITIAL SAFETY CHECK =====\n");
    bool initialSafe = isSafeState(true);

    if (!initialSafe) {
        printf("\nThe initial state is unsafe. Resource requests cannot be safely processed.\n");
        return 0;
    }

    /*
       Keep accepting resource requests
    */
    while (1) {

        printf("\n====================================\n");
        printf("         RESOURCE REQUEST MENU\n");
        printf("====================================\n");
        printf("1. Request Resources\n");
        printf("2. Display Current State\n");
        printf("3. Run Safety Check Again\n");
        printf("4. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1: {
                int team;

                printf("\nWhich team is requesting resources? (1 to %d): ", n);
                scanf("%d", &team);

                if (team < 1 || team > n) {
                    printf("Invalid team number!\n");
                } else {
                    resourceRequest(team - 1);
                }

                break;
            }

            case 2:
                printf("\nCURRENT AVAILABLE / WORK = ( ");
                for (int j = 0; j < m; j++)
                    printf("%d ", available[j]);
                printf(")\n");

                printNeedMatrix();
                break;

            case 3:
                isSafeState(true);
                break;

            case 4:
                printf("\nExiting Banker's Algorithm Simulator. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}


[24bcs044@mepcolinux ex6]$

