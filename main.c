#define _GNU_SOURCE

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048

struct string_write_result {
    char *data;
    unsigned long size;
};

struct recurrence {
    char *word;
    int ntimes;
};

struct recurrence_wrap {
    struct recurrence *words;
    int num_words;
};

static struct recurrence_wrap w;
static struct string_write_result string;
 
/* Callback function for curl */
static size_t write_data_buffer(void *ptr, size_t size, size_t nmemb, void *stream) {
    string.data = realloc(string.data, sizeof(char) * (string.size + (size * nmemb) + 1));
    memset(string.data + string.size, 0, (size * nmemb) + 1);
    memcpy(string.data + string.size, ptr, size * nmemb);
    string.size += size * nmemb;
    return size * nmemb;
}

static void download_page(const char *url) {
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_buffer);
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
}

static void count_recurrences(const char *word) {
    w.num_words++;
    w.words = realloc(w.words, w.num_words * sizeof(struct recurrence));
    
    int i = w.num_words - 1;
    w.words[i].word = strdup(word);
    w.words[i].ntimes = 0;
    
    char *tmp = string.data;
    while ((tmp = strcasestr(tmp, word))) {
        w.words[i].ntimes++;
        tmp += strlen(word);
    }
}

static void print_recurrences(void) {
    for (int i = 0; i < w.num_words; i++) {
        printf("Word '%s' appears %d times.\n", w.words[i].word, w.words[i].ntimes);
    }
}

static void free_recurrences(void) {
    for (int i = 0; i < w.num_words; i++) {
        if (w.words[i].word) {
            free(w.words[i].word);
        }
    }
    if (w.words) {
        free(w.words);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Usage: %s list of words to be searched\n", argv[0]);
        return 1;
    }
    download_page("http://www.liberoquotidiano.it/");
    
    if (string.data) {
        for (int i = 1; i < argc; i++) {
            count_recurrences(argv[i]);
        }
        
        print_recurrences();
        free_recurrences();
        free(string.data);
    }
    return 0;
}
