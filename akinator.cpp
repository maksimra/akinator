#include "akinator.h"

static FILE* log_file = stderr;

void set_log_file (FILE* file)
{
    log_file = file;
}

void down_the_tree (Node** current_node)
{
    char answer[MAX_SYMB];
    printf ("%.*s?\n", (*current_node)->len, (*current_node)->str);
    scanf ("%s", answer);
    if (strncmp (answer, "Yes", MAX_SYMB) == 0)
    {
        *current_node = (*current_node)->left;
    }
    else
    {
        *current_node = (*current_node)->right;
    }
}

void ak_tree_print (Node* node, FILE* file)
{
    fprintf (file, "(\"%.*s\"",node->len, node->str);
    if (node->left != NULL)
        ak_tree_print (node->left, file);
    else
        fprintf (file, "_");
    if (node->right != NULL)
        ak_tree_print (node->right, file);
    else
        fprintf (file, "_");
    fprintf (file, ")");
}

void process_riddle (Node* current_node)
{
    char* answer = (char*) calloc (MAX_SYMB, sizeof (char));
    printf ("%.*s? I am Guess?\n", current_node->len, current_node->str);
    scanf ("%s", answer);
    printf ("answer = %s\n", answer);
    if (strncmp (answer, "Yes", MAX_SYMB) == 0)
    {
        printf ("Good bye. I am to much busy for you\n");
    }
    else
    {
        char* new_object = (char*) calloc (MAX_SYMB, sizeof (char));
        printf ("Who is it?\n");
        scanf ("%s", new_object);
        printf ("How is %s different from %.*s?\n", new_object, current_node->len, current_node->str);
        scanf ("%s", answer);
        insert_branch (current_node, new_object, answer);
    }
}

AkError insert_branch (Node* node, char* new_object, char* sign)
{
    AkError error = AK_NO_ERROR;
    Node* right_node = (Node*) calloc (1, sizeof (Node));
    Node* left_node = (Node*) calloc (1, sizeof (Node));

    char* prev_str = node->str;
    node->str = sign;

    error = create_node (&right_node);
    if (error != AK_NO_ERROR)
        return error;
    node->right = right_node;
    right_node->str = prev_str;

    error = create_node (&left_node);
    if (error != AK_NO_ERROR)
        return error;
    node->left = left_node;
    left_node->str = new_object;
    printf ("new object == %s\n", node->left->str);
    return AK_NO_ERROR;
}


AkError create_node (Node** node)
{
    Node* temp = (Node*) calloc (1, sizeof (Node));
    if (temp == NULL)
        return AK_ERROR_CALLOC;
    *node = temp;
    return AK_NO_ERROR;
}

int again (void)
{
    char answer[MAX_SYMB] = {};
    printf ("Again?\n");
    scanf ("%s", answer);
    if (strncmp (answer, "Yes", MAX_SYMB) == 0)
    {
        return 1;
    }
    else
        return 0;
}

enum AkError read_tree (FILE* file, const char* NAME, Node* root)
{
    enum AkError error = AK_NO_ERROR;
    struct stat statbuf = {};
    char* buffer = NULL;

    if (stat (NAME, &statbuf))
        return AK_ERROR_STAT;

    error = my_fread (statbuf.st_size, file, &buffer);

    if (error != AK_NO_ERROR)
        return error;

    size_t pos = 0;
    Node* cur_node = root;
    create_tree (buffer, cur_node, &pos);
}

void create_tree (char* buffer, Node* cur_node, size_t* pos)
{
    create_node (&(cur_node));
    char str[MAX_SYMB] = {};
    *pos += 2 * sizeof (char);
    char* pos_quotes = strchr (buffer + *pos, '\"');
    cur_node->str = buffer + *pos;
    cur_node->len = pos_quotes - (buffer + *pos);
    *pos = pos_quotes - buffer + sizeof (char);

    if (buffer[*pos] == '(')
        create_tree (buffer, cur_node->left, pos);

    if (buffer[*pos] == '_')
        (*pos)++;

    if (buffer[*pos] == '_')
        (*pos)++;

    if (buffer[*pos] == ')')
    {
        (*pos)++;
        return;
    }
    if (buffer[*pos] == '(')
    {
        create_tree (buffer, cur_node->right, pos);
        (*pos)++;
        return;
    }
}

void skip_space (char** line)
{
    for (size_t i = 0; isspace (**line); i++)
    {
        (*line)++;
    }
}

enum AkError my_fread (size_t size, FILE *fp, char** buffer_ptr)
{
    if (fp == NULL)
        return AK_NULL_PTR_FILE;

    char* buffer = (char*) calloc (size + 1, sizeof (char));
    if (buffer == NULL)
        return AK_CALLOC_FAIL;

    *buffer_ptr = buffer;
    int position = ftell (fp);
    if (fseek (fp, 0, SEEK_SET) != 0)
        return AK_ERROR_FSEEK;
    int temp = 0;
    for (size_t i = 0; i < size && (temp = fgetc (fp)) != EOF; i++)
    {
        buffer[i] = (char) temp;
        continue;
    }
    if (fseek (fp, position, SEEK_SET) != 0)
        return AK_ERROR_FSEEK;

    return AK_NO_ERROR;
}
