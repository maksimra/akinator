#include "akinator.h"

static FILE* log_file = stderr;

void set_log_file (FILE* file)
{
    log_file = file;
}

void is_that_true (Node** current_node)
{
    printf ("%.*s?\n", (*current_node)->len, (*current_node)->str);
}

enum AkError object_search (Node** current_node)
{
    char answer[MAX_SYMB] = {};

    is_that_true (current_node);

    if (fgets (answer, MAX_SYMB, stdin) == NULL)
        return AK_FGETS_ERROR;

    if (strncmp (answer, "Yes\n", MAX_SYMB) == 0) // создать константу поменьше локальную
    {
        *current_node = (*current_node)->left;
    }
    else if (strncmp (answer, "No\n", MAX_SYMB) == 0)
    {
        *current_node = (*current_node)->right;
    }
    else
    {
        return AK_INCORRECT_ANSWER;
    }
}

void printf_str (FILE* file, Node* node)
{
    fprintf (file, "(\"%.*s\"",node->len, node->str);
    printf ("(\"%.*s\"",node->len, node->str);
}

void printing_branches (Node* node, FILE* file)
{
    if (node != NULL)
        ak_tree_print (node, file);
    else
        fprintf (file, "_");
}

void ak_tree_print (Node* node, FILE* file)
{
    printf_str (file, node);

    printing_branches (node->left, file);
    printing_branches (node->right, file);

    fprintf (file, ")");
}

int ask_and_proc_answer (const char* str)
{
    printf ("%s", str);

    char* answer = (char*) calloc (MAX_SYMB, sizeof (char)); // не через динамику

    if (answer == NULL)
        return -1;

    if (fgets (answer, MAX_SYMB, stdin) == NULL)
        return -1;

    if (strncmp (answer, "Yes\n", MAX_SYMB) == 0) // функция, которая обрабатывает да или нет
        return true;

    if (strncmp (answer, "No\n", MAX_SYMB) == 0)
        return false;

    return -1;
}

enum AkError process_riddle (Node* current_node)
{
    printf ("%.*s? ", current_node->len, current_node->str); // ассерты для входных параметров

    int answer = ask_and_proc_answer ("Did I guess right?\n");

    if (answer == -1)
        return AK_PROC_ANSWER_ERROR;

    if (answer == true)
    {
        printf ("See you soon\n"); // return + delete else
    }
    else
    {
        char* new_object = (char*) calloc (MAX_SYMB, sizeof (char));
        char* sign = (char*) calloc (MAX_SYMB, sizeof (char));

        if (new_object == NULL || sign == NULL)
            return AK_ERROR_CALLOC;

        printf ("Who is it?\n");

        if (fgets (new_object, MAX_SYMB, stdin) == NULL) // вручную занулить последний символ
            return AK_FGETS_ERROR;

        fprintf (log_file, "New_object = %s\n", new_object); // научиться переносить
        printf ("How is %.*s different from %.*s?\n", (int) (strlen (new_object) - 1), new_object, current_node->len, current_node->str);

        if (fgets (sign, MAX_SYMB, stdin) == NULL) // property
            return AK_FGETS_ERROR;

        fprintf (log_file, "New_sign = %s\n", sign);

        insert_branch (current_node, new_object, sign); // rename
    }
}

enum AkError insert_branch (Node* node, char* new_object, char* sign)
{
    AkError error = AK_NO_ERROR;

    char* prev_str = node->str;
    int prev_len  = node->len;

    node->str = sign;
    node->len = (int) (strlen (sign) - sizeof (char)); // для \n

    error = replace_node (&(node->right), prev_str, prev_len);

    if (error != AK_NO_ERROR)
        return error;

    error = replace_node (&(node->left), new_object, (int) (strlen (new_object) - sizeof (char)));

    printf ("node->str == %.*s\n", node->len, node->str);
    printf ("node->right->str == %.*s\n", node->right->len, node->right->str);
    printf ("node->left->str == %.*s\n", node->left->len, node->left->str);

    if (error != AK_NO_ERROR)
        return error;

    return AK_NO_ERROR;
}

enum AkError replace_node (Node** node, char* str, int len)
{
    AkError error = AK_NO_ERROR;
    *node = (Node*) calloc (1, sizeof (Node)); // исправить
    if (error != AK_NO_ERROR)
        return error;
    (*node)->str = str;
    (*node)->len = len;
    return AK_NO_ERROR;
}

AkError create_node (Node** node)
{
    Node* temp = (Node*) calloc (1, sizeof (Node)); // использовать в функции выше
    if (temp == NULL)
        return AK_ERROR_CALLOC;
    *node = temp;
    return AK_NO_ERROR;
}

int again (void)
{
    char answer[MAX_SYMB] = {};
    printf ("Again?\n");
    scanf ("%s", answer); // fgets - если теряет информацию - сообщать
    if (strncmp (answer, "Yes", MAX_SYMB) == 0)
    {
        return 1;
    }
    else
        return 0;
}

enum AkError read_tree (FILE* file, const char* NAME, Node** root)
{
    enum AkError error = AK_NO_ERROR;
    struct stat statbuf = {};
    char* buffer = NULL;

    if (stat (NAME, &statbuf))
        return AK_ERROR_STAT;

    error = my_fread (statbuf.st_size, file, &buffer); // тут хорошо функция зайдёт

    if (error != AK_NO_ERROR)
        return error;

    size_t pos = 0;
    Node* cur_node = NULL;
    create_tree (buffer, &cur_node, &pos);
    *root = cur_node;
}

void create_tree (char* buffer, Node** cur_node, size_t* pos)
{
    create_node (cur_node);
    char str[MAX_SYMB] = {};
    *pos += 2 * sizeof (char);
    char* pos_quotes = strchr (buffer + *pos, '\"'); // если после скобки не ковычка - ошибка
    (*cur_node)->str = buffer + *pos; // можно \0 ковычку, чтобы длиной не пользоваться так часто
    (*cur_node)->len = pos_quotes - (buffer + *pos);
    *pos = pos_quotes - buffer + sizeof (char);

    if (buffer[*pos] == '(') // проверяю, что текущий символ откр скобка - если нет - ошибка (или выходить из функции - тогда не нужны нижние подчеркивания)
        create_tree (buffer, &((*cur_node)->left), pos); //  потом искать ковычки

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
        create_tree (buffer, &((*cur_node)->right), pos);
        (*pos)++;
        return;
    }
}

void skip_space (char** line)
{
    for (size_t i = 0; isspace (**line); i++) // переделать
    {
        (*line)++;
    }
}

enum AkError my_fread (size_t size, FILE *fp, char** buffer_ptr) // заменить на обычный fread
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
    }

    if (fseek (fp, position, SEEK_SET) != 0)
        return AK_ERROR_FSEEK;

    return AK_NO_ERROR;
}
