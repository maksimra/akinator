#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <stdbool.h>

const size_t MAX_SYMB = 100;

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
    AK_ERROR_STAT
};

enum AkError insert_branch  (Node* node, char* new_object, char* sign);
enum AkError create_node    (Node** node);
void         process_riddle (Node* current_node);
void         down_the_tree  (Node** current_node);
void         ak_tree_print  (Node* node, FILE* file);
void         set_log_file   (FILE* file);
int          again          (void);
enum AkError read_tree      (FILE* file, const char* NAME, Node** root);
enum AkError my_fread       (size_t size, FILE *fp, char** buffer_ptr);
void         skip_space     (char** line);
void         create_tree    (char* buffer, Node** cur_node, size_t* pos);
