#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void merge_sort(int start, int end);
void merge(int start, int middle, int end);
bool is_creating_cycle(int start, int current);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j && preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    const int mid = pair_count / 2;

    if (pair_count <= 1)
    {
        return;
    }
    merge_sort(0, pair_count - 1);
}

void merge_sort(int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int middle = (start + end) / 2;
    merge_sort(start, middle);
    merge_sort(middle + 1, end);
    merge(start, middle, end);
}

void merge(int start, int middle, int end)
{
    pair temp[end - start + 1];
    int left_index = start;
    int right_index = middle + 1;
    int pair_array_index = 0;

    while (left_index <= middle && right_index <= end)
    {
        int left_preferred = preferences[pairs[left_index].winner][pairs[left_index].loser] -
                             preferences[pairs[left_index].loser][pairs[left_index].winner];
        int right_preferred = preferences[pairs[right_index].winner][pairs[right_index].loser] -
                              preferences[pairs[right_index].loser][pairs[right_index].winner];

        if (left_preferred > right_preferred)
        {
            temp[pair_array_index++] = pairs[left_index++];
        }
        else
        {
            temp[pair_array_index++] = pairs[right_index++];
        }
    }

    while (left_index <= middle)
    {
        temp[pair_array_index++] = pairs[left_index++];
    }

    while (right_index <= end)
    {
        temp[pair_array_index++] = pairs[right_index++];
    }

    for (int i = start, j = 0; i <= end; i++, j++)
    {
        pairs[i] = temp[j];
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        if (!is_creating_cycle(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }
}

bool is_creating_cycle(int start, int current)
{
    if (current == start)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[current][i])
        {
            if (is_creating_cycle(start, i))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool has_edge = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                has_edge = true;
                break;
            }
        }

        if (!has_edge)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
