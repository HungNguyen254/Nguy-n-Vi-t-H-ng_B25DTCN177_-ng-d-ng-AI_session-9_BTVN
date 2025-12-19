#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_QUESTIONS 10
#define SELECTED_QUESTIONS 5
#define OPTION_COUNT 4

typedef struct {
    char question[200];
    char options[OPTION_COUNT][100];
    int correct_answer;
} Question;

/* ======================= TIEN ICH ======================= */

int char_to_index(char choice) {
    switch(choice) {
        case 'A': case 'a': return 0;
        case 'B': case 'b': return 1;
        case 'C': case 'c': return 2;
        case 'D': case 'd': return 3;
        default: return -1;
    }
}

int get_user_answer(void) {
    char c;
    int idx;
    do {
        scanf(" %c", &c);
        idx = char_to_index(c);
        if(idx == -1)
            printf("Lua chon khong hop le! Nhap A/B/C/D: ");
    } while(idx == -1);
    return idx;
}

/* ======================= HIEN THI ======================= */

void display_question(Question q, int num) {
    printf("\n=== CAU HOI %d ===\n", num);
    printf("%s\n\n", q.question);
    printf("A. %s\n", q.options[0]);
    printf("B. %s\n", q.options[1]);
    printf("C. %s\n", q.options[2]);
    printf("D. %s\n", q.options[3]);
    printf("Lua chon cua ban (A/B/C/D): ");
}

void display_result(char name[], int score, int total) {
    float point = (float)score * 10 / total;
    printf("\n========================================\n");
    printf("Nguoi choi: %s\n", name);
    printf("Dung: %d/%d\n", score, total);
    printf("Diem: %.1f/10\n", point);

    if(score == total) printf("Xuat sac!\n");
    else if(score >= total * 0.8) printf("Rat tot!\n");
    else if(score >= total * 0.6) printf("Kha tot!\n");
    else printf("Can co gang hon!\n");

    printf("========================================\n");
}

/* ======================= FILE ======================= */

void save_score(char name[], int score, int total) {
    FILE *f = fopen("diem.txt", "a");
    if(!f) return;

    time_t now = time(NULL);
    char t[50];
    strftime(t, sizeof(t), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(f, "Nguoi choi: %s\n", name);
    fprintf(f, "Thoi gian: %s\n", t);
    fprintf(f, "Ket qua: %d/%d\n", score, total);
    fprintf(f, "---------------------------\n");

    fclose(f);
}

/* ======================= RANDOM ======================= */

void select_random_questions(Question src[], Question dst[], int total, int n) {
    int idx[TOTAL_QUESTIONS];
    for(int i = 0; i < total; i++) idx[i] = i;

    for(int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = idx[i];
        idx[i] = idx[j];
        idx[j] = tmp;
    }

    for(int i = 0; i < n; i++)
        dst[i] = src[idx[i]];
}

/* ======================= KHOI TAO ======================= */

void init_questions(Question q[]) {
    strcpy(q[0].question, "Ngon ngu C ra doi nam nao?");
    strcpy(q[0].options[0], "1969");
    strcpy(q[0].options[1], "1972");
    strcpy(q[0].options[2], "1980");
    strcpy(q[0].options[3], "1990");
    q[0].correct_answer = 1;

    strcpy(q[1].question, "Ham nao chay dau tien?");
    strcpy(q[1].options[0], "start()");
    strcpy(q[1].options[1], "main()");
    strcpy(q[1].options[2], "init()");
    strcpy(q[1].options[3], "run()");
    q[1].correct_answer = 1;

    /* ?? Týõng t? cho các câu c?n l?i */
}

/* ======================= INPUT ======================= */

void input_player_name(char name[]) {
    printf("Nhap ten cua ban: ");
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0;
}

/* ======================= GAME ======================= */

int play_quiz(Question questions[], int count) {
    int score = 0;
    for(int i = 0; i < count; i++) {
        display_question(questions[i], i + 1);
        int ans = get_user_answer();
        if(ans == questions[i].correct_answer) {
            printf("? Dung!\n");
            score++;
        } else {
            printf("? Sai! Dap an: %c\n",
                   'A' + questions[i].correct_answer);
        }
    }
    return score;
}

/* ======================= MAIN ======================= */

int main(void) {
    srand(time(NULL));

    char name[100];
    Question all[TOTAL_QUESTIONS];
    Question selected[SELECTED_QUESTIONS];

    init_questions(all);
    input_player_name(name);

    select_random_questions(all, selected,
                            TOTAL_QUESTIONS,
                            SELECTED_QUESTIONS);

    int score = play_quiz(selected, SELECTED_QUESTIONS);

    display_result(name, score, SELECTED_QUESTIONS);
    save_score(name, score, SELECTED_QUESTIONS);

    return 0;
}

