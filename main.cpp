#include "akinator.h"

int main (const int argc, const char* argv[])
{
    enum AkError error = AK_NO_ERROR;
    const char* NAME = argv[1];
    FILE* file = fopen (NAME, "a+");
    set_log_file (file);
    Node* root = NULL;
    read_tree (file, NAME, &root);
    do
    {
        Node* current_node = root;
        while (current_node->left != NULL && current_node->right != NULL)
        {
            error = object_search (&current_node);
        }
        process_riddle (current_node);
    }
    while (again());
    ak_tree_print (root, file);
    return 0;
}
