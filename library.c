#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    char title[100];
    char author[100];
    char year[10];
};

// Decode URL-encoded input
void decode_input(const char src[], char dest[]) {
    int i = 0, j = 0;
    while (src[i] != '\0') {
        if (src[i] == '+') {
            dest[j++] = ' ';
        } else if (src[i] == '%' && src[i + 1] && src[i + 2]) {
            char hex[3] = {src[i + 1], src[i + 2], '\0'};
            dest[j++] = (char)strtol(hex, NULL, 16);
            i += 2;
        } else {
            dest[j++] = src[i];
        }
        i++;
    }
    dest[j] = '\0';
}

// Extract the value of a specific field from QUERY_STRING
void get_field_value(const char* query, const char* field, char* output) {
    char* start = strstr(query, field);
    if (start) {
        start += strlen(field) + 1; // Move past "field="

        int i = 0;
        while (start[i] != '&' && start[i] != '\0') {
            output[i] = start[i];
            i++;
        }
        output[i] = '\0';
    } else {
        output[0] = '\0'; // If not found, return empty string
    }
}

int main() {
    // Output required CGI headers
    printf("Content-type: text/html\n\n");

    char* query = getenv("QUERY_STRING");

    if (query == NULL || strlen(query) == 0) {
        printf("<h2>No data received.</h2>");
        return 1;
    }

    struct Book book;
    char raw_title[100], raw_author[100], raw_year[10];

    // Parse fields
    get_field_value(query, "title", raw_title);
    get_field_value(query, "author", raw_author);
    get_field_value(query, "year", raw_year);

    // Decode fields
    decode_input(raw_title, book.title);
    decode_input(raw_author, book.author);
    decode_input(raw_year, book.year);

    // Display result
    printf("<h2>Book Added:</h2>");
    printf("<p><strong>Title:</strong> %s</p>", book.title);
    printf("<p><strong>Author:</strong> %s</p>", book.author);
    printf("<p><strong>Year:</strong> %s</p>", book.year);

    return 0;
}
