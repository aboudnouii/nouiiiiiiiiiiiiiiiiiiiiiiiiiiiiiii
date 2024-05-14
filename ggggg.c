#include <stdio.h>
#include <stdlib.h>

typedef struct memory {
    char P_H;//state
    int bit;//1-0 bit map
    int name;//name of process
    int S;//start index
    int L;// size
} memory;

typedef struct QWT {
    int name;
    int size;
    int status;
} QWT;

void memory_I(memory M[], int n) {
    for (int i = 0; i < n; i++) {
        M[i].P_H = 'H';// HOLE
        M[i].bit = 0;
        M[i].S = i;
        M[i].L = 1;
        M[i].name = -1;
    }
}

void display(memory M[], int n) {
    int i, j, L;
    for (i = 0; i < n; i++) {
        j = i;
        L = 0;
        while (j < n && M[i].name == M[j].name && M[j].P_H == M[i].P_H) {
            L += M[j].L;
            j++;
        }
        printf("|%c|%d|%d|->", M[i].P_H, M[i].S, L);
        i = j - 1;
    }
    printf("\n");
}

void fill_QWT(QWT Q[], int n) {
    for (int i = 0; i < n; i++) {
        Q[i].name = i;
        printf("Size of process %d: ", i + 1);
        scanf("%d", &Q[i].size);
        Q[i].status = 0;
    }
}
void display_QWT(QWT Q[], int n) {
    printf("Process\tSize\n");
    for (int i = 0; i < n; i++) {
        if (Q[i].status == 1) {
            printf("%d\t%d\t%d\t%d\n", Q[i].name + 1, Q[i].size);
        }
    }
}

void deallocat(memory M[], QWT Q[], int n, int num) {
    int j;
    display_QWT(Q,num);
    printf("Which process do you want to deallocate?: ");
    scanf("%d", &j);
    j = j - 1;
    if (Q[j].status == 1) {
        Q[j].status = 2; // Reset process status
        for (int i = 0; i < n; i++) {
            if (M[i].name == Q[j].name) {
                M[i].P_H = 'H';
                M[i].bit = 0;
                M[i].S = i;
                M[i].L = 1;
                M[i].name = -1;
            }
        }
        printf("Process %d deallocated successfully.\n", j + 1);
    } else {
        printf("Error: Process %d cannot be deallocated.\n", j + 1);
    }
}

void display_bit(memory M[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d", M[i].bit);
    }
    printf("\n");
}

int find_best(memory M[], int n, int size) {
    int min = n + 1;
    int c = 0;
    int best_start = -1;
    int i = 0;
    while (i < n) {
        if (M[i].bit == 0) {
            int j = i;
            c = 0;
            while (j < n && M[j].bit == 0) {
                c++;
                j++;
            }
            if (c < min && c >= size) {
                min = c;
                best_start = i;
            }
            i = j;
        } else {
            i++;
        }
    }
    return best_start;
}

void best(memory M[], QWT Q[], int unit, int n, int num) {
    for (int i = 0; i < num; i++) {
        if (Q[i].status == 0) {
            int start = find_best(M, n, Q[i].size / unit);
            if (start != -1) {
                Q[i].status = 1;
                for (int j = start; j < start + Q[i].size / unit; j++) {
                    M[j].P_H = 'P';
                    M[j].bit = 1;
                    M[j].S = j;
                    M[j].L = 1;
                    M[j].name = Q[i].name;
                }
            } else {
                printf("Error: There is not enough space for process %d whose size is %d.\n", i + 1, Q[i].size);
            }
        }
    }
}

void clear() {
    system("@cls||clear");
    printf("\n");
}

int main() {
    int size, unit, n, num, choice, b = 0, a = 0, c = 0;
    printf("Enter size of memory: ");
    scanf("%d", &size);
    printf("Enter size of unit: ");
    scanf("%d", &unit);
    n = size / unit;
    printf("%d\n", n);
    memory M[n];
    memory_I(M, n);
    printf("Enter number of processes: ");
    scanf("%d", &num);
    QWT Q[num];
    while (b == 0) {
        printf("1: Enter the data\n");
        printf("2: Allocate\n");
        printf("3: Deallocate\n");
        printf("4: Display\n");
        printf("5: Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                clear();
                fill_QWT(Q, num);
                clear();
                a = 1;
                break;
            case 2:
                            clear();
                            best(M, Q, unit, n, num);
                    c = 1;
                    printf("Allocation successful\n");
                break;
            case 3:
                clear();
                if (c == 1) {
                    deallocat(M, Q, n, num);
                } else {
                    printf("Allocate first\n");
                }
                break;
            case 4:
                clear();
                printf("1: Memory\n");
                printf("2: Bit map\n");
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        clear();
                        display(M, n);
                        break;
                    case 2:
                        clear();
                        display_bit(M, n);
                        break;
                }
                break;
            case 5:
                b = 1;
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
    return 0;
}
