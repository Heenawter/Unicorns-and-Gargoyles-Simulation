
#include <iostream>
#include <list>
#include <algorithm> // for using copy (library function)

void generate_permutations(int *list, int start, int end, std::list<int*>& all_perms);

int main() {
    int test[] = {1, 2, 3, 4, 5};

    std::list<int*> all_perms;
    std::cout << "before" << std::endl;
    generate_permutations(test, 0, 4, all_perms);
    std::cout << "after" << std::endl;

    std::cout << all_perms.size() << std::endl;

    for(std::list<int*>::iterator it = all_perms.begin(); it != all_perms.end(); ++it) {
        for (int i = 0; i < 5; i++) 
            std::cout << *(*it + i);
        std::cout << std::endl;
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
        int* new_perm = new int[5];
        // std::cout << "new perm: " << new_perm << std::endl;
        // std::cout << *list << *(list + 1) << *(list + 2) << *(list + 3) << *(list + 4) << std::endl;
        std::copy(list, list + end + 1, new_perm);
        // std::cout << *new_perm << *(new_perm + 1) << *(new_perm + 2) << *(new_perm + 3) << *(new_perm + 4) << std::endl;

        // std::copy(list, list + end, new_perm);
        // for(i = 0; i < 5; i++) {
        //     std::cout << *(new_perm + i);
        // }
        // std::cout << std::endl;
        all_perms.push_back(new_perm);
    } else {
        for (i = start; i <= end; i++) {
            swap(list + start, list + i);
            generate_permutations(list, start + 1, end, all_perms);
            swap(list + start, list + i);
        }
    }
}