#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <assert.h>

const int MAX_SYMB = 100;

struct Node
{
    char* str;
    int len;
    Node* left;
    Node* right;
    bool existed;
};

enum AkError
{
    AK_NO_ERROR,
    AK_ERROR_CALLOC,
    AK_NULL_PTR_FILE,
    AK_ERROR_FSEEK,
    AK_ERROR_STAT,
    AK_INCORRECT_ANSWER,
    AK_FGETS_ERROR,
    AK_PROC_ANSWER_ERROR,
    AK_ERROR_FREAD,
    AK_ERROR_READ
};

enum AkError insert_branch       (Node* node, char* new_object, char* sign);
enum AkError create_node         (Node** node);
enum AkError process_riddle      (Node* current_node);
enum AkError object_search       (Node** current_node);
void         ak_tree_print       (Node* node, FILE* file, int* n_space);
void         set_log_file        (FILE* file);
int          again               (void);
enum AkError read_tree           (FILE* file, const char* NAME, Node** root, char** buffer);
enum AkError my_fread            (size_t size, FILE *fp, char** buffer_ptr);
int          skip_space          (char* line);
enum AkError create_tree         (char* buffer, Node** cur_node, int* pos);
void         is_that_true        (Node** current_node);
void         printf_str          (FILE* file, Node* node, int n_space);
int          ask_and_proc_answer (const char* str);
enum AkError replace_node        (Node** node, char* str, int len);
void         printing_branches   (Node* node, FILE* file, int* n_space);
void         print_error         (enum AkError error);
const char*  get_error           (enum AkError error);
void         tree_dtor           (Node* root);

