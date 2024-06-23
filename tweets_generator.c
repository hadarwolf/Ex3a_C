#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"

#define DELIMITERS " \n\t\r"
#include "markov_chain.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  ZERO 0
#define ONE 1
#define THREE 3
#define FOUR 4
#define FIVE 5
#define TEN 10
#define MAX_LEN 20
#define MAX_SENT 1000

bool is_delimiter(char ch) {
    return (strchr(DELIMITERS, ch) != NULL);
}

int count_words_in_line(const char *line) {
    int word_count = ZERO;
    bool in_word = false;
    char ch;

    while ((ch = *line++) != '\0') {
        if (is_delimiter(ch)) {
            if (in_word) {
                word_count++;
                in_word = false;
            }
        } else {
            in_word = true;
        }
    }

    if (in_word) {
        word_count++;
    }

    return word_count;
}

int count_words_in_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
//        printf("Error: The file '%s' was not found.\n", file_path);
        return EXIT_FAILURE;
    }

    int word_count = 0;
    char line[MAX_SENT];

    while (fgets(line, sizeof(line), file) != NULL) {
        word_count += count_words_in_line(line);
    }

    fclose(file);
    return word_count;
}


int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain){
    char line[MAX_SENT];
    Node *prev_node = NULL;
    int sentc_end = ZERO;
    while(fgets(line, MAX_SENT, fp) != NULL){
        char *tok = strtok(line, DELIMITERS);
        while(tok != NULL && words_to_read >ZERO){
            Node *cur_node = add_to_database(markov_chain,tok);

            if(is_not_end_of_sentc_n(cur_node)){
                if(prev_node != NULL && !sentc_end){
                    add_node_to_frequency_list
                    (prev_node->data,cur_node->data);
                }
                sentc_end = ZERO;
            }
            else{sentc_end = ONE;
                add_node_to_frequency_list
                        (prev_node->data,cur_node->data);
            }
            prev_node = cur_node;
            words_to_read--;
            tok = strtok(NULL,DELIMITERS);
        }
    }

    return EXIT_SUCCESS;
}

//seed,num_tweet,path, (opt)num_words

int main(int argc, char *argv[]){
    if(!(argc ==FIVE || argc ==FOUR )){
        printf(NUM_ARGS_ERROR);
        return EXIT_FAILURE;}

    size_t seed = strtol(argv[1],NULL,TEN);
    srand(seed);
//    printf("%s", argv[3]);
    FILE *fp = fopen(argv[3],"r");
    if(fp==NULL){ printf(FILE_PATH_ERROR);
        return EXIT_FAILURE;}

    LinkedList database = {NULL,NULL,ZERO};
    MarkovChain markov_chain ={.database = &database};
    MarkovChain *ptr_chain =&markov_chain;
    if (argc == FIVE){
    long word_num= strtol(argv[4],NULL,TEN);
    fill_database(fp,(int)word_num,&markov_chain);}
    else{ long word_num = count_words_in_file(argv[4]);
        fill_database(fp,(int)word_num,&markov_chain);}
    int i =1;
    long tweet_num = strtol(argv[2],NULL,TEN);
    while(tweet_num!= 0){
        printf("Tweet %d",i);
        MarkovNode *first_node =get_first_random_node(&markov_chain);
        generate_tweet(first_node, MAX_LEN);
        i++;
        tweet_num--;
    }

    free_database(&ptr_chain);
    fclose(fp);
    return EXIT_SUCCESS;
    }








