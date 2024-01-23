#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 200

/*声明两个结构体*/
typedef struct {
    char word[MAX_WORD_LEN];
    char translation[MAX_WORD_LEN];
} Word;

typedef struct {
    char word[MAX_WORD_LEN];
    char correct_answer[MAX_WORD_LEN];
} WrongAnswer;

/*声明变量*/
Word words[MAX_WORDS];
WrongAnswer wrong_answers[MAX_WORDS];
int total_words = 0;
int total_wrong_answers = 0;

/*载入文件函数*/
void load_words(const char *filename) {
    char full_filename[MAX_WORD_LEN + 4];
    sprintf(full_filename, "%s.txt", filename);
	//FILE是文件类型，类似 int bool. *file 代表文件指针变量. fopen是标准库中的函数，用来打开文件，第一个参数是路径，第二个参数是打开模式。fopen会返回一个文件指针，这里用file接受。如果打开错误，返回NULL
    FILE *file = fopen(full_filename, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(1);//表示非正常退出，正常用0
    }

    char line[MAX_WORD_LEN * 2];//line储存每一行内容
    while (fgets(line, sizeof(line), file)) {
        char *word = strtok(line, " ");
        char *translation = strtok(NULL, "\n");
        if (word && translation) {
            strncpy(words[total_words].word, word, MAX_WORD_LEN);
            strncpy(words[total_words].translation, translation, MAX_WORD_LEN);
            total_words++;
        }
    }

    fclose(file);
}

/*洗牌算法*/
void shuffle(int *array, int n) {
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

/*生成错误选项*/
void generate_wrong_options(int index, int *wrong_options) {
    do {
        for (int j = 0; j < 3; j++) {
            wrong_options[j] = rand() % total_words;
        }
    } while (wrong_options[0] == wrong_options[1] || wrong_options[0] == wrong_options[2] || wrong_options[1] == wrong_options[2] || wrong_options[0] == index || wrong_options[1] == index || wrong_options[2] == index);
}

/*清屏*/
void clear_screen() {
    printf("\033[H\033[J");
}

/*测验主函数*/
void quiz() {
    char input[MAX_WORD_LEN];

    for (int i = 0; i < total_words; i++) {
        clear_screen();

        int index = i;
        int wrong_options[3];
        generate_wrong_options(index, wrong_options);

        char *options[4] = {words[index].translation, words[wrong_options[0]].translation, words[wrong_options[1]].translation, words[wrong_options[2]].translation};
        int options_order[4] = {0, 1, 2, 3};
        shuffle(options_order, 4);

        printf("单词: %s\n", words[index].word);
        for (int j = 0; j < 4; j++) {
            printf("%d: %s\n", j + 1, options[options_order[j]]);
        }
        printf("enter : ");
        scanf("%s", input);

        if (strcmp(input, "q") == 0) {
            printf("退出\n");
            return;
        } else if (options_order[atoi(input) - 1] != 0) {
            strncpy(wrong_answers[total_wrong_answers].word, words[index].word, MAX_WORD_LEN);
            strncpy(wrong_answers[total_wrong_answers].correct_answer, words[index].translation, MAX_WORD_LEN);
            total_wrong_answers++;
        }
    }

    printf("所有单词都已经过完，以下是你答错的题目：\n");
    for (int i = 0; i < total_wrong_answers; i++) {
        printf("单词: %s, 正确答案: %s\n", wrong_answers[i].word, wrong_answers[i].correct_answer);
    }

    printf("退出\n");
}

int main() {
    srand(time(NULL));  // 初始化随机数生成器
    char filename[MAX_WORD_LEN];
    printf("请输入文件名: ");
    scanf("%s", filename);
    load_words(filename);
    quiz();
    return 0;
}
