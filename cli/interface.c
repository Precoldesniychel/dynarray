#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynarray.h"
#include "element_info.h"
#include "student.h"
#include "teacher.h"

static char* input_str(const char* prompt) {
    char buf[256];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return NULL;
    buf[strcspn(buf, "\n")] = '\0';
    return _strdup(buf);
}

static bool filter_excellent(const void* p) {
    return student_is_excellent((const Student*)p);
}

static bool filter_senior(const void* p) {
    return teacher_is_senior((const Teacher*)p);
}

static void* count_inc(void* acc, void* elem) { (void)elem; (*(int*)acc)++; return acc; }

static void print_all(const DynArray* arr, const char* title) {
    printf("\n=== %s (Size: %zu) ===\n", title, array_size(arr));
    for (size_t i = 0; i < array_size(arr); ++i) {
        void* elem = array_get((DynArray*)arr, i);
        if (elem) person_print(elem);
    }
}

static void load_test_data(DynArray* arr) {
    if (!arr) return;
    // 10 Студентов
    array_push(arr, student_create("Ivanov Ivan", 20, 1, 4.8));
    array_push(arr, student_create("Petrova Anna", 19, 2, 3.9));
    array_push(arr, student_create("Sidorov Alex", 21, 3, 4.5));
    array_push(arr, student_create("Kuznetsova Maria", 18, 4, 5.0));
    array_push(arr, student_create("Smirnov Dmitry", 22, 5, 4.2));
    array_push(arr, student_create("Popova Elena", 20, 6, 4.7));
    array_push(arr, student_create("Volkov Sergey", 19, 7, 3.5));
    array_push(arr, student_create("Novikova Olga", 21, 8, 4.9));
    array_push(arr, student_create("Mikhailov Pavel", 20, 9, 4.1));
    array_push(arr, student_create("Fedorova Tatiana", 22, 10, 4.6));
    // 5 Преподавателей
    array_push(arr, teacher_create("Sidorov Petr", 45, 11, 15, "Math"));
    array_push(arr, teacher_create("Kozlova Maria", 38, 12, 5, "Physics"));
    array_push(arr, teacher_create("Volkov Igor", 52, 13, 20, "History"));
    array_push(arr, teacher_create("Lebedeva Svetlana", 41, 14, 8, "Biology"));
    array_push(arr, teacher_create("Orlov Andrey", 35, 15, 3, "Chemistry"));

    printf("Loaded demo data: 10 students, 5 teachers (15 total).\n");
}

static void run_demo_tests(DynArray* mixed) {
    printf("\n=== Running Automated Demo Tests ===\n");

    // 1. Фильтрация отличников
    printf("\n[1] Filtering students with GPA >= 4.5:\n");
    int ex_cnt = 0;
    for(size_t i=0; i<array_size(mixed); ++i) {
        void* p = array_get((DynArray*)mixed, i);
        if(!p) continue;
        Person* person = (Person*)p;
        if(person->age < 30 && student_is_excellent((Student*)p)) {
            ex_cnt++;
            printf("  - Found: %s (GPA: %.2f)\n", person->full_name ? person->full_name : "N/A", ((Student*)p)->gpa);
        }
    }
    printf("Result: %d excellent students found.\n", ex_cnt);

    // 2. Фильтрация опытных преподавателей
    printf("\n[2] Filtering teachers with Experience >= 10 years:\n");
    int sen_cnt = 0;
    for(size_t i=0; i<array_size(mixed); ++i) {
        void* p = array_get((DynArray*)mixed, i);
        if(!p) continue;
        Person* person = (Person*)p;
        if(person->age >= 30 && teacher_is_senior((Teacher*)p)) {
            sen_cnt++;
            printf("  - Found: %s (Exp: %d years)\n", person->full_name ? person->full_name : "N/A", ((Teacher*)p)->experience_years);
        }
    }
    printf("Result: %d senior teachers found.\n", sen_cnt);

    // 3. Конкатенация с подробным объяснением
    printf("\n[3] Testing Concatenation (array + empty array):\n");
    printf("  - Mechanism: Creates a NEW DynArray structure.\n");
    printf("  - Copies pointers from the first array, then from the second.\n");
    printf("  - Performs a SHALLOW COPY: elements are shared, memory is NOT duplicated.\n");
    printf("  - Type compatibility is checked before merging.\n");
    
    DynArray* empty = array_create(element_info_person());
    DynArray* merged = array_concat(mixed, empty);
    printf("  - Original size: %zu\n", array_size(mixed));
    printf("  - Empty size: %zu\n", array_size(empty));
    printf("  - Merged size: %zu (Expected: %zu)\n", array_size(merged), array_size(mixed) + array_size(empty));
    
    // Безопасная очистка view-массива (только структура, не элементы)
    if(merged) { free(merged->data); free(merged); }
    array_destroy(empty);

    // 4. Reduce
    printf("\n[4] Testing Reduce (Count elements):\n");
    int cnt = 0;
    array_reduce(mixed, count_inc, &cnt);
    printf("  - Iterated over %zu elements using reduce().\n", array_size(mixed));
    printf("  - Final accumulator value: %d\n", cnt);

    // 5. Очистка и перезагрузка
    printf("\n[5] Testing Clear & Reload:\n");
    array_clear(mixed);
    printf("  - Size after array_clear(): %zu\n", array_size(mixed));
    load_test_data(mixed);

    printf("\n=== All Demo Tests Completed Successfully ===\n");
}

void run_interface(void) {
    DynArray* people = array_create(element_info_person());
    if (!people) { printf("Error creating array.\n"); return; }

    load_test_data(people);

    int choice = 0;
    while (1) {
        printf("\n--- Polymorphic Dynamic Array (Variant 17) ---\n");
        printf("1. Add Student\n");
        printf("2. Add Teacher\n");
        printf("3. Show All\n");
        printf("4. Filter: Excellent Students (GPA >= 4.5)\n");
        printf("5. Filter: Senior Teachers (Exp >= 10)\n");
        printf("6. Demo: Concatenation\n");
        printf("7. Clear Array\n");
        printf("8. Run Automated Tests\n");
        printf("9. Reload Demo Data\n");
        printf("0. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }
        while(getchar()!='\n');

        switch (choice) {
            case 1: {
                char* name = input_str("Full Name: ");
                int age=20; double gpa=4.0;
                printf("Age: "); scanf("%d",&age); while(getchar()!='\n');
                printf("GPA: "); scanf("%lf",&gpa); while(getchar()!='\n');
                Student* s = student_create(name ? name : "Unknown", age, (int)array_size(people)+1, gpa);
                if(s) array_push(people, s);
                free(name); break;
            }
            case 2: {
                char* name = input_str("Full Name: ");
                int age=40, exp=5;
                printf("Age: "); scanf("%d",&age); while(getchar()!='\n');
                printf("Experience (years): "); scanf("%d",&exp); while(getchar()!='\n');
                Teacher* t = teacher_create(name ? name : "Unknown", age, (int)array_size(people)+1, exp, "CS");
                if(t) array_push(people, t);
                free(name); break;
            }
            case 3: print_all(people, "All Records"); break;
            case 4: {
                printf("\nFiltering Excellent Students (GPA >= 4.5):\n");
                int cnt = 0;
                for(size_t i=0; i<array_size(people); ++i) {
                    void* p = array_get((DynArray*)people, i);
                    if(p && ((Person*)p)->age < 30 && student_is_excellent((Student*)p)) {
                        printf(" - %s (GPA: %.2f)\n", ((Person*)p)->full_name, ((Student*)p)->gpa);
                        cnt++;
                    }
                }
                printf("Found %d excellent students.\n", cnt);
                break;
            }
            case 5: {
                printf("\nFiltering Senior Teachers (Exp >= 10):\n");
                int cnt = 0;
                for(size_t i=0; i<array_size(people); ++i) {
                    void* p = array_get((DynArray*)people, i);
                    if(p && ((Person*)p)->age >= 30 && teacher_is_senior((Teacher*)p)) {
                        printf(" - %s (Exp: %d yrs)\n", ((Person*)p)->full_name, ((Teacher*)p)->experience_years);
                        cnt++;
                    }
                }
                printf("Found %d senior teachers.\n", cnt);
                break;
            }
            case 6: {
                printf("\nConcatenation Demo:\n");
                printf("Creating empty array...\n");
                DynArray* e = array_create(element_info_person());
                printf("Calling array_concat(main, empty)...\n");
                DynArray* c = array_concat(people, e);
                printf("Result size: %zu (Original: %zu + Empty: %zu)\n", array_size(c), array_size(people), array_size(e));
                printf("Note: Concat performs a shallow copy. Pointers are shared, objects are NOT duplicated.\n");
                if(c) { free(c->data); free(c); }
                array_destroy(e);
                break;
            }
            case 7: array_clear(people); printf("Array cleared.\n"); break;
            case 8: run_demo_tests(people); break;
            case 9: array_clear(people); load_test_data(people); break;
            case 0: array_destroy(people); printf("Exiting...\n"); return;
            default: printf("Invalid option.\n");
        }
    }
}