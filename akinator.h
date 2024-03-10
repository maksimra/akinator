#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>

const size_t MAX_SYMB = 100;

struct Node
{
    char* str;
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
};

enum AkError insert_branch  (Node* node, char* new_object, char* sign);
enum AkError create_node    (Node** node);
void         process_riddle (Node* current_node);
void         down_the_tree  (Node** current_node);
void         ak_tree_print  (Node* node, FILE* file);
void         set_log_file   (FILE* file);
int          again          (void);
void         read_tree      (FILE* file, Node* root);
enum AkError my_fread       (size_t size, FILE *fp, char** buffer_ptr);
void         read_tree      (FILE* file, Node* root, const char* file_name);
void         skip_space     (char** line);
