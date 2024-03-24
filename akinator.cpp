#include "akinator.h"

static FILE* log_file = stderr;
const static char n_answ = 4;

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
    const int n_answer = 4;

    is_that_true (current_node);

    if (fgets (answer, MAX_SYMB, stdin) == NULL)
        return AK_FGETS_ERROR;

    if (strlen (answer) > n_answ)
            printf ("Некорректный ответ. Часть информации утеряна.\n");

    if (strncmp (answer, "Yes\n", n_answ) == 0) // создать константу поменьше локальную
    {
        *current_node = (*current_node)->left;
    }
    else if (strncmp (answer, "No\n", n_answ) == 0)
    {
        *current_node = (*current_node)->right;
    }
    else
    {
        return AK_INCORRECT_ANSWER;
    }

    return AK_NO_ERROR;
}

void printf_str (FILE* file, Node* node, int n_space)
{
    fprintf (file, "%*c(\"%.*s\"\n", n_space, ' ', node->len, node->str);
}

void printing_branches (Node* node, FILE* file, int* n_space)
{
    if (node != NULL)
    {
        ak_tree_print (node, file, n_space);
    }
    else
    {
        fprintf (file, "_");
        (*n_space) -= 4;
    }
}

void print_start (FILE* file)
{
    fprintf (file, "graph G {\n");
}

void print_end (FILE* file)
{
    fprintf (file, "}");
}

void print_filling (Node* node, FILE* file)
{
    draw_left (node, file);
    draw_right (node, file);
}

void graphviz (Node* node, FILE* file)
{
    assert (node != NULL);
    assert (file != NULL);

    print_start (file);

    print_filling (node, file);

    print_end (file);
}

void draw_right (Node* node, FILE* file)
{
    if (node->right != NULL)
    {
        char* color = NULL;
        if (node->right->left == NULL && node->right->right == NULL)
            color = "red";
        else
            color = "green";
        fprintf (file, "\"%.*s\"--\"%.*s\"[label=\"Нет\", color = \"%s\"];\n", node->len, node->str, node->right->len, node->right->str, color);
        print_filling (node->right, file);
    }
}

void draw_left (Node* node, FILE* file)
{
    if (node->left != NULL)
    {
        char* color = NULL;
        if (node->left->left == NULL && node->left->right == NULL)
            color = "red";
        else
            color = "green";
        fprintf (file, "\"%.*s\"--\"%.*s\"[label=\"Да\", color = \"%s\"];\n", node->len, node->str, node->left->len, node->left->str, color);
        print_filling (node->left, file);
    }
}

void ak_tree_print (Node* node, FILE* file, int* n_space)
{
    printf_str (file, node, *n_space);
    (*n_space) += 4;

    printing_branches (node->left, file, n_space);
    printing_branches (node->right, file, n_space);

    fprintf (file, ")\n");
}

int ask_and_proc_answer (const char* str)
{
    printf ("%s", str);

    char answer[MAX_SYMB] = {};

    if (fgets (answer, MAX_SYMB, stdin) == NULL)
        return -1;

    if (strncmp (answer, "Yes\n", n_answ) == 0) // функция, которая обрабатывает да или нет
        return true;

    if (strncmp (answer, "No\n", n_answ) == 0)
        return false;

    return -1;
}

enum AkError process_riddle (Node* current_node)
{
    assert (current_node != NULL);

    printf ("%.*s? ", current_node->len, current_node->str);

    int answer = ask_and_proc_answer ("Did I guess right?\n");

    if (answer == -1)
        return AK_PROC_ANSWER_ERROR;

    if (answer == true)
    {
        printf ("See you soon\n");
        return AK_NO_ERROR;
    }

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

    return AK_NO_ERROR;
}

enum AkError insert_branch (Node* node, char* new_object, char* sign)
{
    AkError error = AK_NO_ERROR;

    assert (node != NULL && new_object != NULL && sign != NULL);

    char* prev_str = node->str;
    int prev_len  = node->len;

    node->existed = false;
    node->str = sign;
    node->len = strlen (sign) - sizeof (char); // для \n

    error = replace_node (&(node->right), prev_str, prev_len);
    node->right->existed = true;

    if (error != AK_NO_ERROR)
        return error;

    error = replace_node (&(node->left), new_object, (int) (strlen (new_object) - sizeof (char)));
    node->left->existed = false;

    if (error != AK_NO_ERROR)
        return error;

    return AK_NO_ERROR;
}

enum AkError replace_node (Node** node, char* str, int len)
{
    assert (str != NULL);

    AkError error = AK_NO_ERROR;
    *node = (Node*) calloc (1, sizeof (Node));

    if (*node == NULL)
        return AK_ERROR_CALLOC;

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

enum AkError read_tree (FILE* file, const char* NAME, Node** root, char** buffer)
{
    enum AkError error = AK_NO_ERROR;
    struct stat statbuf = {};

    assert (file != NULL);

    if (stat (NAME, &statbuf))
        return AK_ERROR_STAT;

    *buffer = (char*) calloc (statbuf.st_size + sizeof (char), sizeof (char));

    size_t size = 0;

    size = fread (*buffer, sizeof (char), statbuf.st_size, file); // тут хорошо функция зайдёт
    (*buffer)[statbuf.st_size] = '\0';
    assert (size == statbuf.st_size);
    if (size != statbuf.st_size)
        return AK_ERROR_FREAD;

    if (error != AK_NO_ERROR)
        return error;

    int pos = 0;
    Node* cur_node = NULL;
    error = create_tree ((*buffer), &cur_node, &pos);

    if (error != AK_NO_ERROR)
        return error;

    *root = cur_node;
    return AK_NO_ERROR;
}

enum AkError create_tree (char* buffer, Node** cur_node, int* pos)
{
    assert (buffer != NULL);

    AkError error = AK_NO_ERROR;

    create_node (cur_node);
    char str[MAX_SYMB] = {};

    *pos += skip_space (buffer + *pos);

    if (buffer[*pos] != '(')
    {
        return AK_ERROR_READ;
    }

    (*pos)++;

    if (buffer[*pos] != '\"')
        return AK_ERROR_READ;

    (*pos)++;

    char* pos_quotes = strchr (buffer + *pos, '\"');

    if (pos_quotes == NULL)
        return AK_ERROR_READ;

    (*cur_node)->str = buffer + *pos;
    (*cur_node)->len = pos_quotes - (buffer + *pos);
    (*cur_node)->existed = true;
    *pos = pos_quotes - buffer + sizeof (char);

    *pos += skip_space (buffer + *pos);

    if (buffer[*pos] == '(')
    {
        error = create_tree (buffer, &((*cur_node)->left), pos);
        if (error != AK_NO_ERROR)
        {
            return AK_ERROR_READ;
        }
    }

    if (buffer[*pos] == '_')
        (*pos)++;

    if (buffer[*pos] == '_')
        (*pos)++;


    if (buffer[*pos] == ')')
    {
        (*pos)++;
        *pos += skip_space (buffer + *pos);
        return error;
    }
    if (buffer[*pos] == '(')
    {
        error = create_tree (buffer, &((*cur_node)->right), pos);
        (*pos)++;
        *pos += skip_space (buffer + *pos);
        return error;
    }
    return error;
}

int skip_space (char* line)
{
    int n_space = 0;
    while (isspace (*(line + n_space)))
    {
        n_space++;
    }
    return n_space;
}

void tree_dtor (Node* root)
{
    assert (root != NULL);

    if (root->left != NULL)
        tree_dtor (root->left);

    if (root->right != NULL)
        tree_dtor (root->right);

    if (root->existed == false)
        free (root->str);
    free (root);

}

const char* get_error (enum AkError error)
{
    switch (error)
    {
        case AK_NO_ERROR:
            return "Ak: Ошибок в работе функций не выявлено.";
        case AK_NULL_PTR_FILE:
            return "Ak: Передан нулевой указатель на файл.";
        case AK_ERROR_FSEEK:
            return "Ak: Ошибка в работе функции fseek.";
        case AK_FGETS_ERROR:
            return "Ak: Ошибка в работе функции fgets.";
        case AK_ERROR_STAT:
            return "Ak: Ошибка в работе функции stat.";
        case AK_INCORRECT_ANSWER:
            return "Ak: Введён некорректный ответ.";
        case AK_ERROR_CALLOC:
            return "Ak: Ошибка в выделении памяти (calloc).";
        case AK_PROC_ANSWER_ERROR:
            return "Ak: Ошибка обработки ответа.";
        case AK_ERROR_FREAD:
            return "Ak: Ошибка чтения из файла.";
        default:
            return "Ak: Куда делся мой enum ошибок?";
    }
}

void print_error (enum AkError error)
{
    fprintf (log_file, "%s\n", get_error (error));
}
