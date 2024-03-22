#include "akinator.h"

int main (const int argc, const char* argv[])
{
    enum AkError error = AK_NO_ERROR;
    const char* NAME = argv[1];
    char* buffer = NULL;
    FILE* file = fopen (NAME, "a+");
    assert (file != NULL);
    Node* root = NULL;
    error = read_tree (file, NAME, &root, &buffer);
    print_error (error);
    assert (root != NULL);
    do
    {
        Node* current_node = root;
        while (current_node->left != NULL && current_node->right != NULL)
        {
            error = object_search (&current_node);
            print_error (error);
        }
        error = process_riddle (current_node);
        print_error (error);
    }
    while (again());
    ak_tree_print (root, file);
    tree_dtor (root);
    free (buffer);
    return 0;
}
