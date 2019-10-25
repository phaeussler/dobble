#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

int not_in(uint16_t* n_words, int word, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(n_words[i] == word) return 0;
    }
    return 1;
}
void random_numbers(uint16_t* n_words)
{
    int cant = 0;
    int zeros = 0;
    srand(time(NULL));
    while(cant < 19)
    {
        uint16_t new_n = rand() % 1001;
        if(!new_n)
        {
            if(!zeros)
            {    
                n_words[cant] = new_n;
                zeros++;
                cant++;
            }
        }
        else if(not_in(n_words, new_n, 19))
        {
            n_words[cant] = new_n;
            cant++;
        }
    }
}

uint16_t* sep_words(uint16_t* n_words, int rep)
{
    uint16_t* new_list = calloc(9, sizeof(uint16_t));
    int cant = 0; 
    int zeros = 0;
    while(cant < 9)
    {
        int new_n = rand() % 18;
        if(!n_words[new_n] && new_n != rep)
        {
            if(!zeros)
            {    
                new_list[cant] = n_words[new_n];
                zeros++;
                cant++;
            }
        }
        else if(not_in(new_list, n_words[new_n], 9) && new_n != rep)
        {
            new_list[cant] = n_words[new_n];
            cant++;
        }
        // printf("cant: %d, new_n: %d\n", cant, new_n);
    }
    return new_list;
}

uint16_t* second_sep_words(uint16_t* n_words, uint16_t* sep_word, int rep)
{
    uint16_t* new_list = calloc(10, sizeof(uint16_t));
    int cant = 0;
    for(int i = 0; i < 19; i++){
        if(not_in(sep_word, n_words[i], 9))
        {
            new_list[cant] = n_words[i];
            cant++;
        }
    }
    return new_list;
}

void print_one_word(char* word, int size)
{
    int pos = rand() % (19 - size);
    int printed = 0;
    while(printed < 20)
    {
        if(pos == printed)
        {
            for(int i = 0; i < size; i++)
            {
                printf("%c ", word[i]);
            }
            printed += size;
        }
        else
        {
            printf("- ");
            printed++;
        }
        
    }
}

int add_words(uint16_t* card, char* payload, int* size, char** words, int cant, int curr, uint16_t* n_words, int rep)
{
    int pos_rep1 = -1;
    int pos;
    if(cant == 9) pos_rep1 = rand() % 8;
    for(int i = 0; i < 10; i++)
    {
        int mySize;
        char* word;
        if(cant == 10)
        {
            mySize = size[card[i]];
            word = words[card[i]];
        }
        else
        {   
            if(i < pos_rep1)
            {
                mySize = size[card[i]];
                word = words[card[i]];
            }
            else if(i == pos_rep1)
            {
                mySize = size[n_words[rep]];
                word = words[n_words[rep]];
            }
            else
            {
                mySize = size[card[i - 1]];
                word = words[card[i - 1]];
            }
            
        }
        pos = rand() % (19 - mySize);
        
        // payload[curr] = mySize;
        memcpy(&payload[curr], &mySize, 1);
        for(int j = 0; j < mySize; j++){
            payload[curr + j + 1] = word[j];
        }
        // payload[curr + mySize + 1] = pos;
        memcpy(&payload[curr + mySize + 1], &pos, 1);
        curr += mySize + 2;

    }

    return curr;
}

void send_words(int client_socket, uint16_t* n_words, char** words, int* size, int rep)
{
    
    uint16_t* first_card = sep_words(n_words, rep);
    uint16_t* second_card = second_sep_words(n_words, first_card, rep);
    int pos_rep1 = rand() % 10;
    char* word_to_print1;
    int size_to_print1;
    char* word_to_print2;
    int size_to_print2;
    uint8_t payloadSize = 0;
    // printf("              C A R T A 1                |                C A R T A 2              \n \n");
    for(int i = 0; i < 10; i++)
    {
        // if(i < pos_rep1)
        // {
        //     word_to_print1 = words[first_card[i]];
        //     size_to_print1 = size[first_card[i]];
        //     payloadSize += size_to_print1;
        // }
        // else if(i == pos_rep1)
        // {
        //     word_to_print1 = words[n_words[rep]];
        //     size_to_print1 = size[n_words[rep]];
        // }
        // else
        // {
        //     word_to_print1 = words[first_card[i - 1]];
        //     size_to_print1 = size[first_card[i - 1]];
        // }
        // word_to_print2 = words[second_card[i]];
        // size_to_print2 = size[second_card[i]];
        

        // print_one_word(word_to_print1, size_to_print1);
        // printf(" |  ");
        // print_one_word(word_to_print2, size_to_print2);
        // printf("\n");
        // if (i < 9) printf("- - - - - - - - - - - - - - - - - - - -  |  - - - - - - - - - - - - - - - - - - - -\n");
        payloadSize += size[second_card[i]];
        if(i < 9) payloadSize += size[first_card[i]];
        else payloadSize += size[n_words[rep]];
    }
    payloadSize += 40;
    unsigned char* sender = calloc(payloadSize + 1 + 1, sizeof(char));
    sender[0] = 9;
    sender[1] = payloadSize;
    int cant = 2;
    cant = add_words(first_card, sender, size, words, 9, cant, n_words, rep);
    add_words(second_card, sender, size, words, 10, cant, n_words, rep);
    send(client_socket, sender, 2+payloadSize, 0);
    // int curr = 0;
    // printf("%d ", sender[0]);
    // printf("%d \n", sender[1]);
    // curr += 2;
    // for(int i = 0; i < 20; i++){
    //     int a = sender[curr];
    //     printf("%d ", a);
    //     for(int j = 0; j < a; j++){
    //         printf("%c", sender[curr+1+j]);
    //     }
    //     printf(" %d\n", sender[curr +a+1]);
    //     curr += a+2;
    // }
    free(first_card);
    free(second_card);
    free(sender);
}

int main(int argc, char *argv[])
{
    char* objective_word[50];
    char** list = calloc(1001, sizeof(char*));
    for(int i = 0; i < 1001; i++){
        list[i] = calloc(21, sizeof(char));
    }
    int* size = calloc(1001, sizeof(int));
    FILE* test = fopen("palabras.txt", "r");
    int cant = 0;
    int aux_size;
    char aux_word[100];
    while(cant < 1001 && fscanf(test, "%d,%s", &size[cant], aux_word) != EOF)
    {
        strcpy(list[cant], aux_word);
        cant++;
    }
    fclose(test);
    
    uint16_t* words = calloc(19, sizeof(uint16_t));
    int rep = rand() % 18;

    random_numbers(words);

    send_words(1, words, list, size, rep);
    free(size);
    for(int i = 0; i < 1001; i++)
    {
        free(list[i]);
    }
    free(list);
    free(words);
}