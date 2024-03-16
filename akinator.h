#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <stdbool.h>

const int MAX_SYMB = 100;

struct Node
{
    char* str;
    int len;
    Node* left;
    Node* right;
};

enum AkError
{
    AK_NO_ERROR,
    AK_ERROR_CALLOC,
    AK_NULL_PTR_FILE,
    AK_CALLOC_FAIL,
    AK_ERROR_FSEEK,
    AK_ERROR_STAT,
    AK_INCORRECT_ANSWER
};

enum AkError insert_branch  (Node* node, char* new_object, char* sign);
enum AkError create_node    (Node** node);
enum AkError         process_riddle (Node* current_node);
enum AkError ask_questions  (Node** current_node);
void         ak_tree_print  (Node* node, FILE* file);
void         set_log_file   (FILE* file);
int          again          (void);
enum AkError read_tree      (FILE* file, const char* NAME, Node** root);
enum AkError my_fread       (size_t size, FILE *fp, char** buffer_ptr);
void         skip_space     (char** line);
void         create_tree    (char* buffer, Node** cur_node, size_t* pos);
void         is_that_true   (Node** current_node);
void         printf_str     (FILE* file, Node* node);
