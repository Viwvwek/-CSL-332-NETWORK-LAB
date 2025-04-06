#include <stdio.h>
#include <stdlib.h>

int d[10][10], via[10][10];

int main() {
    int i, j, k, n, g[10][10];

    printf("\nEnter the number of nodes: ");
    scanf("%d", &n);

    printf("\n--- Enter the cost matrix (Use 999 for no direct path) ---\n");
    for (i = 0; i < n; i++) {
        printf("\nFor node %c:\n", i + 97);
        for (j = 0; j < n; j++) {
            printf("Cost from %c to %c: ", i + 97, j + 97);
            scanf("%d", &g[i][j]);
            if (g[i][j] != 999)
                d[i][j] = 1;
        }
    }

    // Display initial cost matrix
    printf("\n--- Initial Cost Matrix ---\n    ");
    for (i = 0; i < n; i++) {
        printf("%4c", i + 97);
    }
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c |", i + 97);
        for (j = 0; j < n; j++) {
            if (g[i][j] == 999)
                printf(" INF");
            else
                printf("%4d", g[i][j]);
        }
        printf("\n");
    }

    // Initialize via matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            via[i][j] = i;

    // Distance vector routing calculation
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (d[i][j] == 1) {
                for (k = 0; k < n; k++) {
                    if (g[i][j] + g[j][k] < g[i][k]) {
                        g[i][k] = g[i][j] + g[j][k];
                        via[i][k] = j;
                    }
                }
            }
        }
    }

    // Final output
    printf("\n--- Final Routing Table ---\n");
    for (i = 0; i < n; i++) {
        printf("\nRouting table for node %c:\n", i + 97);
        printf("Destination\tCost\tVia\n");
        for (j = 0; j < n; j++) {
            printf("    %c\t\t", j + 97);
            if (g[i][j] == 999)
                printf("INF\t-");
            else
                printf("%d\t%c", g[i][j], via[i][j] + 97);
            printf("\n");
        }
    }

    return 0;
}
