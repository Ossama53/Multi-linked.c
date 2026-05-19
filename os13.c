#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct user user;
typedef struct follow_node follow_node;

struct user
{
    int uid;
    char *name;
    int age;
    user *next;
    user *previous;
};

struct follow_node
{
    int uid;
    follow_node *next;
};

typedef struct
{
    int uid;
    follow_node *following;
} follow_list;

user *head = NULL;
follow_list *graph = NULL;
int user_count = 0;

user *c_user(int uid, char *name, int age)
{
    user *u = malloc(sizeof(user));
    u->uid = uid;
    u->age = age;
    u->name = malloc(strlen(name) + 1);
    strcpy(u->name, name);
    u->next = u->previous = NULL;
    return u;
}

user *find_user(int uid)
{
    user *ptr = head;
    while (ptr != NULL && ptr->uid != uid)
        ptr = ptr->next;
    return ptr;
}

void last(user *u)
{
    if (head == NULL)
    {
        head = u;
        return;
    }
    user *ptr = head;
    while (ptr->next != NULL)
        ptr = ptr->next;
    ptr->next = u;
    u->previous = ptr;
}

void init_graph()
{
    user *ptr = head;
    while (ptr != NULL)
    {
        user_count++;
        ptr = ptr->next;
    }

    graph = malloc(user_count * sizeof(follow_list));

    int i = 0;
    ptr = head;
    while (ptr != NULL)
    {
        graph[i].uid = ptr->uid;
        graph[i].following = NULL;
        i++;
        ptr = ptr->next;
    }
}

int find_index(int uid)
{
    for (int i = 0; i < user_count; i++)
        if (graph[i].uid == uid)
            return i;
    return -1;
}

void follow(int a_uid, int b_uid)
{
    int a = find_index(a_uid);
    if (a == -1 || a_uid == b_uid)
        return;

    follow_node *n = malloc(sizeof(follow_node));
    n->uid = b_uid;
    n->next = graph[a].following;
    graph[a].following = n;
}

void unfollow(int a_uid, int b_uid)
{
    int a = find_index(a_uid);
    if (a == -1)
        return;

    follow_node *cur = graph[a].following, *prev = NULL;
    while (cur != NULL && cur->uid != b_uid)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL)
        return;

    if (prev == NULL)
        graph[a].following = cur->next;
    else
        prev->next = cur->next;
    free(cur);
}

void p_user(int uid)
{
    user *u = find_user(uid);
    int a = find_index(uid);

    printf("user info\n");
    printf("uid=%d  name=%s  age=%d\n", u->uid, u->name, u->age);

    printf("following: ");
    follow_node *ptr = graph[a].following;
    if (ptr == NULL)
        printf("nobody");
    while (ptr != NULL)
    {
        printf("%s ", find_user(ptr->uid)->name);
        ptr = ptr->next;
    }

    printf("\nfollowers: ");
    int found = 0;
    for (int i = 0; i < user_count; i++)
    {
        follow_node *p = graph[i].following;
        while (p != NULL)
        {
            if (p->uid == uid)
            {
                printf("%s ", find_user(graph[i].uid)->name);
                found = 1;
            }
            p = p->next;
        }
    }
    if (!found)
        printf("nobody");
    printf("\n");
}

void print_allu()
{
    user *ptr = head;
    while (ptr != NULL)
    {
        p_user(ptr->uid);
        ptr = ptr->next;
    }
}

int main()
{
    last(c_user(5, "omar", 22));
    last(c_user(6, "osama", 29));
    last(c_user(7, "awadh", 31));
    last(c_user(8, "Abdullah", 24));
    last(c_user(9, "yasser", 38));
    last(c_user(10, "nasir", 26));

    init_graph();

    printf("irregular block\n");
    user *ptr = head;
    while (ptr != NULL)
    {
        printf("uid=%-2d  address=%p\n", ptr->uid, (void *)ptr);
        ptr = ptr->next;
    }
    printf("\n");

    follow(5, 6);
    follow(6, 5);
    follow(5, 7);
    follow(7, 9);
    follow(8, 5);
    follow(10, 6);

    printf("before\n");
    print_allu();

    unfollow(6, 5);
    unfollow(5, 7);

    printf("after\n");
    print_allu();

    return 0;
}
