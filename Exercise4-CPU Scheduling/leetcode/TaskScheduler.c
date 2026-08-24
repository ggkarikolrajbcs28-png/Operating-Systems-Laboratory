int leastInterval(char* tasks, int tasksSize, int n) {
    int frequencies[26] = {0};
    
    for (int i = 0; i < tasksSize; i++) {
        frequencies[tasks[i] - 'A']++;
    }

    int max_freq = 0;
    for (int i = 0; i < 26; i++) {
        if (frequencies[i] > max_freq) {
            max_freq = frequencies[i];
        }
    }

    int max_freq_count = 0;
    for (int i = 0; i < 26; i++) {
        if (frequencies[i] == max_freq) {
            max_freq_count++;
        }
    }

    int empty_slots_structure = (max_freq - 1) * (n + 1) + max_freq_count;

    return (tasksSize > empty_slots_structure) ? tasksSize : empty_slots_structure;
}
