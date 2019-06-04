
#include <iostream>
#include <list>
#include <algorithm> // for using copy (library function)
#include "Deck.h"

const int PERM_COUNT = 11;

void generate_permutations(int *list, int start, int end, std::list<int*>& all_perms);

int main() {
    int test[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    std::list<int*> all_perms;
    std::cout << "before" << std::endl;
    generate_permutations(test, 0, PERM_COUNT - 1, all_perms);
    std::cout << "after" << std::endl;

    std::cout << all_perms.size() << std::endl;

    // for(std::list<int*>::iterator it = all_perms.begin(); it != all_perms.end(); ++it) {
    //     for (int i = 0; i < PERM_COUNT; i++)
    //         std::cout << *(*it + i);
    //     std::cout << " ";
    // }

    for(std::list<int*>::iterator it = all_perms.begin(); it != all_perms.end(); ++it) {
        delete *it;
    }

    return 0;
}

void swap(int *x, int *y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void generate_permutations(int *list, int start, int end, std::list<int*>& all_perms) {
    int i;

    if(start == end) {
        int *new_perm = new int[PERM_COUNT];
        std::copy(list, list + end + 1, new_perm);
        all_perms.push_back(new_perm);
    } else {
        for (i = start; i <= end; i++) {
            swap(list + start, list + i);
            generate_permutations(list, start + 1, end, all_perms);
            swap(list + start, list + i);
        }
    }
}