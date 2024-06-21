#include "markov_chain.h"
//#include "tweets_generator.c"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TWEET 20
#define ZERO 0
#define ONE 1


/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}

int is_not_end_of_sentc_n(Node* node){
    if(!node){return ZERO;}
    char *str = node->data->data;
    size_t len = strlen(str);
    if(len ==0){return ZERO;}
    return str[len-1] !='.';//true 1, false 0
}

int is_not_end_of_sentc_m(MarkovNode * node){
    if(!node){return ZERO;}
    char *str = node->data;
    size_t len = strlen(str);
    if(len ==0){return ZERO;}
    return str[len-1] !='.';//true 1, false 0
}

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr){
    int i = 0;
    Node* cur = markov_chain->database->first;
    while(i < markov_chain->database->size){
        char* try_data = cur->data->data;
        if (!strcpy(try_data,data_ptr)){
            return cur;
        }
        cur = cur->next;
        i++;
    }
    return NULL;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr){
    Node* n_node = get_node_from_database(markov_chain,data_ptr);
    if (n_node != NULL){
        return n_node;
    }

    char* n_str = malloc(strlen(data_ptr)+1);
    if(n_str == NULL){
        return NULL;
    }
    MarkovNode* markov_node = malloc(sizeof(MarkovNode));
    if(markov_node == NULL){
        free(n_str);
        return NULL;
    }
    memccpy(n_str, data_ptr, (int)(strlen(data_ptr)+1),sizeof((strlen(data_ptr)+1)));
    *markov_node= (MarkovNode){n_str,NULL,0};
    int check = add(markov_chain->database,markov_node);
    if (check==1){//free?
        free(n_str);
        free(markov_node);
        return NULL;
    }
    return markov_chain->database->last;
}

int add_node_to_frequency_list(MarkovNode *first_node
        , MarkovNode *second_node){
    for(int i =0; i < first_node->size_freq_lst; i++){
        if(first_node->frequency_list[i].markov_node == second_node){
            first_node->frequency_list[i].markov_node += 1;
            return EXIT_SUCCESS;
        }
    }
    MarkovNodeFrequency* new_freq_lst = realloc(first_node->frequency_list,
              sizeof(MarkovNodeFrequency)*(first_node->size_freq_lst +1));
    if(new_freq_lst == NULL){
        return EXIT_FAILURE;
    }
    first_node->frequency_list = new_freq_lst;
    first_node->frequency_list[first_node->size_freq_lst]=
            (MarkovNodeFrequency){second_node,1};

//    first_node->frequency_list[first_node->size_freq_lst].markov_node = second_node;
//    first_node->frequency_list[first_node->size_freq_lst].frequency = 1;
    first_node->size_freq_lst +=1;
    return EXIT_SUCCESS;}


//    MarkovNodeFrequency * cur = first_node->frequency_list;
//    while(cur != NULL){
//        if(cur->markov_node == second_node){
//            cur->frequency +=1;
//            return EXIT_SUCCESS;
//        }
//        cur = cur->
//    }
//    MarkovNodeFrequency
//}




void free_database(MarkovChain ** ptr_chain){
    //derefrence the pointer to save it for after
    Node *cur = (*ptr_chain)->database->first;
    while (cur!=0){
        //save the pointer and next so we can free without losing it
        Node* tmp = cur;
        cur = cur->next;
        free(tmp->data->data);
        free(tmp->data->frequency_list);
        free(tmp->data);
        free(tmp);
    }
    free((*ptr_chain)->database);
    free(*ptr_chain);
}


MarkovNode* get_first_random_node(MarkovChain *markov_chain){
    while(ONE){
        int num= get_random_number(markov_chain->database->size);
        Node *cur_node = markov_chain->database->first;
        while (num !=ZERO){
        cur_node = cur_node->next;
        num--;}
        if (is_not_end_of_sentc_n(cur_node)){
            return cur_node->data;
        }
}}

MarkovNode* get_next_random_node(MarkovNode *cur_markov_node){
    if (!cur_markov_node || !cur_markov_node->frequency_list) {
        return NULL; // Handle NULL pointers or invalid data
    }
    int max_num =ZERO;
    for(int i =ZERO; i < cur_markov_node->size_freq_lst ;i++){
        max_num += cur_markov_node->frequency_list[i].frequency;
    }
    int rand_num = get_random_number(max_num);
    int cur_freq = ZERO;
    for(int i =ZERO; i < cur_markov_node->size_freq_lst ;i++){
        cur_freq += cur_markov_node->frequency_list[i].frequency;
        if(rand_num < cur_freq){
            return cur_markov_node->frequency_list[i].markov_node;
        }
    }
    return NULL;
}

void generate_tweet(MarkovNode *first_node, int max_length){
    MarkovNode *cur_node = first_node;
    while(max_length!=ZERO){
        MarkovNode * next_node =get_next_random_node(cur_node);
        if(!next_node){ break;}
        printf(" %s", next_node->data);
        if(!is_not_end_of_sentc_m(next_node)){
            break;
        }
        cur_node = next_node;
        max_length--;
    }
    printf("\n");
}