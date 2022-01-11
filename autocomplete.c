
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

void delete_space(char *line){
    int j, i;
    i = 0;
    while( line[i] == ' '){
        i++;
    }
    for (j = 0; i < strlen(line); i++){
        line[j++] = line[i];
    }
    line[j] = '\0';
}

int comparator(const void *p, const void*q){
    struct term *term1 = (struct term *)p;
    struct term *term2 = (struct term *)q;
    char word1[200];
    char word2[200];
    strcpy(word1, term1->term);
    strcpy(word2, term2->term);
    int ret = strcmp(word1, word2);
    return ret;
}

void read_in_terms(struct term **terms, int *pnterms, char *filename){
    char line[200];
    int N;
    FILE *fp = fopen(filename, "r");
    fgets(line, sizeof(line), fp);
    N = atoi(line);
    *pnterms = N;
    *terms = malloc(N * sizeof(struct term)); 
    for(int i = 0; i < N; i++){
        fgets(line, sizeof(line), fp); 
        char weight[100];
        delete_space(line);
        int k = 0;
        while(line[k] != '\t'){
            weight[k] = line[k];
            k++;
        }
        weight[k] = '\0';
        k++;
        char word[200];
        int j;
        for(j = 0; k < (strlen(line) - 1); k++){ 
            word[j++] = line[k];
        }
        word[j] = '\0';
        char *endpt;
        double res;
        res = atof(weight);
        strcpy((*terms)[i].term, word);
        (*terms)[i].weight = res; 
    }
    qsort(*terms, N, sizeof(struct term), comparator);
}

int lowest_match(struct term *terms, int nterms, char *substr){
    int len = strlen(substr);
    int low = 0;
    int high = nterms;
    int result = -1;
    int mid;
    while( low <= high ){
        mid = low + (high - low)/2;
        char fterm[200];
        strcpy(fterm, terms[mid].term);
        char term[200];  
        int comp;
        if(strlen(fterm) <= len){
            comp = strcmp(substr, fterm);
        }
        else{
            int i;
            for (i = 0; i < len; i++){
                term[i] = fterm[i];
            }
            term[i] = '\0';
            
            comp = strcmp(substr, term);
        }
        
        if(comp == 0){
            result = mid;
            high = mid - 1;
        }
        else if(comp < 0){
            high = mid - 1;
        }
        else{
            low = mid + 1;
        }
    }
    return result;
}

int highest_match(struct term *terms, int nterms, char *substr){
    int len = strlen(substr);
    int low = 0;
    int high = nterms;
    int result = -1;
    int mid;
    while( low <= high ){
        mid = low + (high - low)/2;
        char fterm[200];
        strcpy(fterm, terms[mid].term);
        char term[200];
        int comp;
        if(strlen(fterm) <= len){
            comp = strcmp(substr, fterm);
        }
        else{
            int i; 
            for (i = 0; i < len; i++){
                term[i] = fterm[i];
            }
            term[i] = '\0';
            comp = strcmp(substr, term);
        }
    
        if(comp == 0){
            result = mid;
            low = mid + 1;
        }
        else if(comp < 0){
            high = mid - 1;
        }
        else{
            low = mid + 1;
        }
    }
    return result;
}

int comparator2(const void *p, const void*q){
    struct term *term1 = (struct term *)p;
    struct term *term2 = (struct term *)q;
    int weight1 = term1->weight;
    int weight2 = term2->weight;
    int ret = weight2 - weight1;
    return ret;
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){
    int low = lowest_match(terms, nterms, substr);
    int high = highest_match(terms, nterms, substr);
    if (low == -1){
        *answer = NULL;
        *n_answer = 0;
    }
    else{
        int N = (high - low) + 1;
        *answer = malloc(N*sizeof(struct term));
        int j = 0;
        for(int i = low; i <= high ; i++){ 
            strcpy((*answer)[j].term, terms[i].term);
            (*answer)[j].weight = terms[i].weight;
            j++;
        }
        qsort(*answer, N, sizeof(struct term), comparator2);
        *n_answer = N;
    }
      
}
