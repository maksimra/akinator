#include "akinator.h"

int main (void)
{
    FILE* file = fopen ("akinator.txt", "r+");
    set_log_file (file);
    Node* root = (Node*) calloc (1, sizeof (Node));
    root->str = (char*) calloc (1, MAX_SYMB);
    strncpy (root->str, "Unknown who", MAX_SYMB);
    //read_tree (file, root, "akinator.txt");
    do
    {
        Node* current_node = root;
        while (current_node->left != NULL && current_node->right != NULL)
        {
            down_the_tree (&current_node);
        }
        process_riddle (current_node);
    }
    while (again());
    ak_tree_print (root, file);
    return 0;
}
