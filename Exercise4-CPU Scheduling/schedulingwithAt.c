#include <stdio.h>
#include <limits.h>

struct P {
    int id, at, bt, wt, tat, pr, rt;
} p[20];

int n;

/* Display Result */
void result(struct P a[], int g[], int st[], int en[], int gc, int priority)
{
    int i;
    float aw = 0, at = 0;

    printf("\nGantt Chart:\n|");

    for(i = 0; i < gc; i++)
        printf(" P%d |", g[i]);

    printf("\n%d", st[0]);

    for(i = 0; i < gc; i++)
        printf("   %d", en[i]);

    printf("\n\nProcess\tAT\tBT");

    if(priority)
        printf("\tPR");

    printf("\tWT\tTAT\n");

    for(i = 0; i < n; i++)
    {
        aw += a[i].wt;
        at += a[i].tat;

        printf("P%d\t%d\t%d",
               a[i].id,
               a[i].at,
               a[i].bt);

        if(priority)
            printf("\t%d", a[i].pr);

        printf("\t%d\t%d\n",
               a[i].wt,
               a[i].tat);
    }

    printf("\nAverage WT = %.2f", aw / n);
    printf("\nAverage TAT = %.2f\n", at / n);
}


/* FCFS */
void fcfs()
{
    struct P a[20], t;
    int i, j, time = 0;
    int g[20], st[20], en[20];

    for(i = 0; i < n; i++)
        a[i] = p[i];

    /* Sort according to Arrival Time */
    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(a[j].at > a[j + 1].at)
            {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }

    for(i = 0; i < n; i++)
    {
        if(time < a[i].at)
            time = a[i].at;

        g[i] = a[i].id;
        st[i] = time;

        a[i].wt = time - a[i].at;

        time += a[i].bt;

        en[i] = time;

        a[i].tat = time - a[i].at;
    }

    result(a, g, st, en, n, 0);
}


/* SJF - Non Preemptive */
void sjf()
{
    struct P a[20];
    int d[20] = {0};
    int g[20], st[20], en[20];

    int i, time = 0, c = 0;
    int idx, mb, gc = 0;

    for(i = 0; i < n; i++)
        a[i] = p[i];

    while(c < n)
    {
        idx = -1;
        mb = INT_MAX;

        for(i = 0; i < n; i++)
        {
            if(!d[i] &&
               a[i].at <= time &&
               a[i].bt < mb)
            {
                mb = a[i].bt;
                idx = i;
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        g[gc] = a[idx.id;
        st[gc] = time;
        a[idx].wt = time - a[idx].at;
        time += a[idx].bt;
        en[gc++] = time;
        a[idx].tat = time - a[idx].at;
        d[idx] = 1;
        c++;
    }
    result(a, g, st, en, gc, 0);
}
/* SRTF - Preemptive SJF */
void srtf()
{
    struct P a[20];
    int g[2000], st[2000], en[2000];
    int raw[2000];
    int i, time = 0, c = 0;
    int rc = 0, gc = 0;
    int idx, mr;
    for(i = 0; i < n; i++)
    {
        a[i] = p[i];
        a[i].rt = a[i].bt;
    }
    while(c < n)
    {
        idx = -1;
        mr = INT_MAX;
        for(i = 0; i < n; i++)
        {
            if(a[i].at <= time &&
               a[i].rt > 0 &&
               a[i].rt < mr)
            {
                mr = a[i].rt;
                idx = i;
            }
        }
        if(idx == -1)
        {
            time++;
            continue;
        }
        raw[rc++] = a[idx].id;
        a[idx].rt--;
        time++;
        if(a[idx].rt == 0)
        {
            a[idx].tat = time - a[idx].at;
            a[idx].wt = a[idx].tat - a[idx].bt;
            c++;
        }
    }
    /* Create Gantt Chart */
    time = 0;
    for(i = 0; i < rc;)
    {
        int id = raw[i];
        int start = time;
        while(i < rc && raw[i] == id)
        {
            i++;
            time++;
        }
        g[gc] = id;
        st[gc] = start;
        en[gc++] = time;
    }
    result(a, g, st, en, gc, 0);
}
/* Non Preemptive Priority */
void priority()
{
    struct P a[20];
    int d[20] = {0};
    int g[20], st[20], en[20];
    int i, time = 0, c = 0;
    int idx, bp, gc = 0;
    for(i = 0; i < n; i++)
        a[i] = p[i];
    while(c < n)
    {
        idx = -1;
        bp = INT_MAX;
        for(i = 0; i < n; i++)
        {
            if(!d[i] &&
               a[i].at <= time &&
               a[i].pr < bp)
            {
                bp = a[i].pr;
                idx = i;
            }
        }
        if(idx == -1)
        {
            time++;
            continue;
        }
        g[gc] = a[idx].id;
        st[gc] = time;
        a[idx].wt = time - a[idx].at;
        time += a[idx].bt;
        en[gc++] = time;

        a[idx].tat = time - a[idx].at;
        d[idx] = 1;
        c++;
    }
    result(a, g, st, en, gc, 1);
}

/* Preemptive Priority */
void ppriority()
{
    struct P a[20];
    int g[2000], st[2000], en[2000];
    int raw[2000];
    int i, time = 0, c = 0;
    int rc = 0, gc = 0;
    int idx, bp;
    for(i = 0; i < n; i++)
    {
        a[i] = p[i];
        a[i].rt = a[i].bt;
    }
    while(c < n)
    {
        idx = -1;
        bp = INT_MAX;
        for(i = 0; i < n; i++)
        {
            if(a[i].at <= time &&
               a[i].rt > 0 &&
               a[i].pr < bp)
            {
                bp = a[i].pr;
                idx = i;
            }
        }
        if(idx == -1)
        {
            time++;
            continue;
        }
        raw[rc++] = a[idx].id;
        a[idx].rt--;
        time++;
        if(a[idx].rt == 0)
        {
            a[idx].tat = time - a[idx].at;
            a[idx].wt = a[idx].tat - a[idx].bt;

            c++;
        }
    }
    /* Create Gantt Chart */
    time = 0;
    for(i = 0; i < rc;)
    {
        int id = raw[i];
        int start = time;
        while(i < rc && raw[i] == id)
        {
            i++;
            time++;
        }
        g[gc] = id;
        st[gc] = start;
        en[gc++] = time;
    }
    result(a, g, st, en, gc, 1);
}
/* Round Robin */
void rr()
{
    struct P a[20];
    int q[2000];
    int in[20] = {0};
    int done[20] = {0};
    int g[2000], st[2000], en[2000];
    int tq;
    int front = 0, rear = 0;
    int time = 0, c = 0, gc = 0;
    int i, idx, ex;
    for(i = 0; i < n; i++)
    {
        a[i] = p[i];
        a[i].rt = a[i].bt;
    }
    printf("\nEnter Time Quantum: ");
    scanf("%d", &tq);
    while(c < n)
    {
        /* Add arrived processes */
        for(i = 0; i < n; i++)
        {
            if(a[i].at <= time &&
               !in[i] &&
               !done[i])
            {
                q[rear++] = i;
                in[i] = 1;
            }
        }
        if(front == rear)
        {
            time++;
            continue;
        }
        idx = q[front++];
        in[idx] = 0;
        g[gc] = a[idx].id;
        st[gc] = time;
        if(a[idx].rt < tq)
            ex = a[idx].rt;
        else
            ex = tq;
        a[idx].rt -= ex;
        time += ex;
        en[gc++] = time;
        /* Add newly arrived processes */
        for(i = 0; i < n; i++)
        {
            if(a[i].at <= time &&
               !in[i] &&
               !done[i])
            {
                q[rear++] = i;
                in[i] = 1;
            }
        }
        if(a[idx].rt > 0)
        {
            q[rear++] = idx;
            in[idx] = 1;
        }
        else
        {
            done[idx] = 1;
            c++;
            a[idx].tat = time - a[idx].at;
            a[idx].wt = a[idx].tat - a[idx].bt;
        }
    }
    result(a, g, st, en, gc, 0);
}
/* MAIN */
int main()
{
    int i, ch;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    /* Get all process details */
    printf("\nEnter Process Details\n");
    for(i = 0; i < n; i++)
    {
        p[i].id = i + 1;
        printf("\nP%d\n", i + 1);
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].at);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].bt);
        printf("Enter Priority: ");
        scanf("%d", &p[i].pr);
    }

    /* Display Input Table */
    printf("\n\n========== INPUT PROCESS TABLE ==========\n");

    printf("Process\tAT\tBT\tPriority\n");

    for(i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\n",
               p[i].id,
               p[i].at,
               p[i].bt,
               p[i].pr);
    }

    printf("=========================================\n");
    /* Scheduling Menu */
    do
    {
        printf("\n\n========== CPU SCHEDULING ==========\n");
        printf("1. FCFS\n");
        printf("2. SJF\n");
        printf("3. SRTF\n");
        printf("4. Priority\n");
        printf("5. Preemptive Priority\n");
        printf("6. Round Robin\n");
        printf("7. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &ch);
        switch(ch)
        {
            case 1:
                fcfs();
                break;
            case 2:
                sjf();
                break;
            case 3:
                srtf();
                break;
            case 4:
                priority();
                break;
            case 5:
                ppriority();
                break;
            case 6:
                rr();
                break;
            case 7:
                printf("\nProgram terminated.\n");
                break;
            default:
                printf("\nInvalid Choice!\n");
        }
    } while(ch != 7);

    return 0;
}
