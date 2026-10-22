#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

//defining structs
typedef struct node{
    char name[20];
    struct node *child;
    struct node *sibling;
}node;

typedef struct{
    char username[20];
    int password;
}credentials;

//defining functions
int welcome_screen(void);
int main_menu(void);
node* find_node(node *curr, const char *target);
int add_child(void);
int add_sibling(void);
void free_tree(node *curr);
int delete_member(void);
int find_path(node *curr, const char *target, node *path[], int depth);
int trace_lineage(void);
int display_tree(void);
void change_username(void);
int change_password(int n);
void load_credentials(void);
void save_credentials(void);
int quit(void);

node *root = NULL;

credentials user;

//defining main
int main(void){
    load_credentials();

    if (welcome_screen() == 0){
        system("cls");

        int mmreturn;

        do{
            mmreturn = main_menu();
            while (mmreturn > 8 || mmreturn < 1){
                printf("Please enter a valid choice.\n");
                mmreturn = main_menu();
            }

            if (mmreturn == 1){
                system("cls");
                add_child();
            }
            else if (mmreturn == 2){
                system("cls");
                add_sibling();
            }
            else if (mmreturn == 3){
                system("cls");
                delete_member();
            }
            else if (mmreturn == 4){
                system("cls");
                trace_lineage();
            }
            else if (mmreturn == 5){
                system("cls");
                display_tree();
            }
            else if (mmreturn == 6){
                system("cls");
                change_username();
            }
            else if (mmreturn == 7){
                system("cls");
                user.password = change_password(user.password);
                save_credentials();
            }
            else if (mmreturn == 7){
                system("cls");
                change_username();
            }
            else {
                quit();
            }
        } while (mmreturn != 7);
    }
}

//displaying welcome screen and authentication
int welcome_screen(void){
    char input_name[20];
    int input_password;
    int correct = 1;
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                  WELCOME TO LINEAGE CRAFT                                       | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    while (correct == 1){
        printf("|Enter username: ");
        scanf("%19s", input_name);
        printf("|-------------------------------------------------------------------------------------------------| \n");
        printf("|Enter password: ");
        scanf("%d", &input_password);

        if(strcmp(user.username, input_name) == 0 && user.password == input_password){
            printf("|=================================================================================================| \n");
            printf("|                                          WELCOME BACK!                                          | \n");
            printf("|=================================================================================================| \n");
            correct = 0;
            break;
        }
        else{
            printf("Please enter correct information\n");
        }
    }
    return correct;
}

//main menu and process selection
int main_menu(void){
    int choice;
    printf("|=================================================================================================| \n");
    printf("|                                            MAIN  MENU                                           | \n");
    printf("|=================================================================================================| \n");
    printf("|                                            1. Add Child                                         | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                            2. Add Sibling                                       | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                            3. Delete  Member                                    | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                            4. Trace lineage                                     | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                            5. Display tree                                      | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                            6. Change username                                   | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                            7. Change password                                   | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");
    printf("|                                            8. Quit                                              | \n");
    printf("|-------------------------------------------------------------------------------------------------| \n");

    printf("Enter your choice(1-8): ");
    scanf("%d", &choice);

    return choice;
}

//defining find_node which will help in implementing almost all the tree related functions.
node* find_node(node *curr, const char *target) {
    if (curr == NULL){
            return NULL;
    }

    if (strcmp(curr->name, target) == 0){
        return curr;
    }

    node *found = find_node(curr->child, target);

    if (found != NULL){
        return found;
    }
    return find_node(curr->sibling, target);
}

//defining add_child
int add_child(void) {
    char parent[20];
    char child_name[20];

    printf("Enter parent name: ");
    scanf("%19s", parent);

    printf("Enter child name: ");
    scanf("%19s", child_name);

    if (root == NULL){
        root = (node *)malloc(sizeof(node));
        strcpy(root->name, parent);

        node *child_node = (node *)malloc(sizeof(node));
        strcpy(child_node->name, child_name);

        root->sibling = NULL;

        child_node->child = NULL;
        child_node->sibling = NULL;

        root->child = child_node;

        printf("%s added as the root member and %s as their child.\n",parent, child_name);
        return 0;
    }

    node *parent_node = find_node(root, parent);

    if (parent_node == NULL){
        printf("Parent not found.\n");
        return 1;
    }

    node *new_node = (node *)malloc(sizeof(node));
    strcpy(new_node->name, child_name);
    new_node->child = NULL;
    new_node->sibling = NULL;

    if (parent_node->child == NULL){
        parent_node->child = new_node;
    }

    else{
        node *temp = parent_node->child;
        while (temp->sibling != NULL){
            temp = temp->sibling;
        }
        temp->sibling = new_node;
    }

    printf("%s added as a child of %s.\n", child_name, parent);
    return 0;
}

//defining add_sibling.
int add_sibling(void){
    char existing_member[20];
    char sibling_name[20];

    printf("Enter existing member name: ");
    scanf("%19s", existing_member);

    printf("Enter sibling name: ");
    scanf("%19s", sibling_name);

    if (root == NULL){
        printf("Tree is empty.\n");
        return 1;
    }

    if (strcmp(root->name, existing_member) == 0){
        printf("Root member cannot have a sibling.\n");
        return 1;
    }

    node *target_node = find_node(root, existing_member);

    if (target_node == NULL){
        printf("Member not found.\n");
        return 1;
    }

    node *new_node = (node *)malloc(sizeof(node));
    strcpy(new_node->name, sibling_name);
    new_node->child = NULL;
    new_node->sibling = target_node->sibling;
    target_node->sibling = new_node;

    printf("%s added as a sibling of %s.\n", sibling_name, existing_member);
    return 0;
}

//defining free_tree which will help in deleting branches or the tree as a whole when quitting.
void free_tree(node *curr){
    if (curr == NULL){
            return;
    }
    free_tree(curr->child);
    free_tree(curr->sibling);
    free(curr);
}

//defining deleting_member.
int delete_member(void){
    char name[20];

    printf("Enter member name to delete: ");
    scanf("%19s", name);

    if (root == NULL){
        printf("Tree is empty.\n");
        return 1;
    }

    node *target = find_node(root, name);

    if (target == NULL){
        printf("Member not found.\n");
        return 1;
    }

    if (target == root){
        free_tree(root);
        root = NULL;
        printf("Root member %s deleted. Tree is now empty.\n", name);
        return 0;
    }

    node *stack[1000];
    int top = 0;
    node **link = NULL;

    stack[top++] = root;

    while (top > 0 && link == NULL){
        node *curr = stack[--top];

        if (curr->child == target){
            link = &curr->child;
        }
        else if (curr->sibling == target){
            link = &curr->sibling;
        }
        else{
            if (curr->child != NULL){
                stack[top++] = curr->child;
            }
            if (curr->sibling != NULL){
                stack[top++] = curr->sibling;
            }
        }
    }

    if (link == NULL){
        printf("Member not found in tree.\n");
        return 1;
    }

    *link = target->sibling;
    target->sibling = NULL;
    free_tree(target);

    printf("%s and all their descendants deleted.\n", name);
    return 0;
}

int find_path(node *curr, const char *target, node *path[], int depth){
    if (curr == NULL) {
        return 0;
    }

    path[depth] = curr;

    if (strcmp(curr->name, target) == 0) {
        return depth + 1;
    }

    node *child = curr->child;
    while (child != NULL) {
        int length = find_path(child, target, path, depth + 1);
        if (length > 0) {
            return length;
        }
        child = child->sibling;
    }

    return 0;
}

//Tracing the lineage of particular members.
int trace_lineage(void){
    if (root == NULL) {
        printf("Tree is empty.\n");
        return 1;
    }

    char name[20];
    printf("Enter member name to trace lineage: ");
    scanf("%19s", name);

    node *path[100];
    int length = find_path(root, name, path, 0);

    if (length == 0) {
        printf("Member '%s' not found in the family tree.\n", name);
        return 1;
    }

    printf("\nLineage for %s:\n", name);
    printf("-------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < length; i++){
        if (i == 0) {
            printf("[Root Ancestor] %s\n", path[i]->name);
        }
        else if (i == length - 1){
            printf("[Target] %s\n", path[i]->name);
        }
        else{
            for (int j = 0; j < i; j++) printf("  ");
            printf("Generation %d: %s\n", i, path[i]->name);
        }
    }
    printf("-------------------------------------------------------------------------------------------------\n");

    return 0;
}

//Displays the entire tree
int display_tree(void){
    if (root == NULL) {
        printf("Tree is empty.\n");
        return 1;
    }

    printf("\n=================================================================================================\n");
    printf("                                         FAMILY TREE                                             \n");
    printf("=================================================================================================\n");

    typedef struct {
        node *node_ptr;
        int level;
    } StackItem;

    StackItem stack[1000];
    int top = 0;

    stack[top].node_ptr = root;
    stack[top].level = 0;
    top++;

    while (top > 0) {
        top--;
        node *curr = stack[top].node_ptr;
        int level = stack[top].level;

        for (int i = 0; i < level; i++) {
            printf("    ");
        }
        printf("|-- %s\n", curr->name);

        if (curr->sibling != NULL) {
            stack[top].node_ptr = curr->sibling;
            stack[top].level = level;
            top++;
        }

        if (curr->child != NULL) {
            stack[top].node_ptr = curr->child;
            stack[top].level = level + 1;
            top++;
        }
    }

    printf("=================================================================================================\n");

    return 0;
}

// File I/O functions for credentials
void load_credentials(void){
    FILE *file = fopen("credentials.txt", "r");
    if (file == NULL) {
        strcpy(user.username, "admin");
        user.password = 123;
        save_credentials();
        return;
    }
    fscanf(file, "%19s %d", user.username, &user.password);
    fclose(file);
}

void save_credentials(void){
    FILE *file = fopen("credentials.txt", "w");
    if (file != NULL) {
        fprintf(file, "%s %d\n", user.username, user.password);
        fclose(file);
    }
}

//Defining change_username.
void change_username(void) {
    int verify_pass;
    printf("Enter current password to verify: ");
    scanf("%d", &verify_pass);

    if (verify_pass != user.password) {
        printf("Incorrect password. Username change aborted.\n");
        return;
    }

    char new_username[20];
    printf("Enter new username: ");
    scanf("%19s", new_username);

    strcpy(user.username, new_username);
    save_credentials();
    printf("Username changed successfully!\n");
}

//Defining change_password.
int change_password(int n) {
    int verify;
    do {
        printf("Enter old password: ");
        scanf("%d", &verify);
        if (verify != n) {
            printf("Incorrect password. Try again.\n");
        }
    }while (verify != n);

    int new_pass;
    printf("Enter new password: ");
    scanf("%d", &new_pass);
    printf("Password changed successfully!\n");

    return new_pass;
}

//Defining quit which would free up all memory and safely close the program without any memory leaks(hopefully!).
int quit(void){
    free_tree(root);
    root = NULL;
    printf("GOODBYE!");
    return 0;
}