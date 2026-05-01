#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynarray.h"
#include "element_info.h"
#include "student.h"
#include "teacher.h"

/* === Вспомогательные функции для операций (чистый C) === */
static void* map_increment_gpa(const void* p) {
    if (((const Person*)p)->age >= 30) return person_clone(p);
    Student* s = student_clone(p);
    s->gpa += 0.1; if(s->gpa > 5.0) s->gpa = 5.0;
    return s;
}

static int filter_excellent(const void* p) {
    return ((const Person*)p)->age < 30 && ((const Student*)p)->gpa >= 4.5;
}

static int filter_senior(const void* p) {
    return ((const Person*)p)->age >= 30 && ((const Teacher*)p)->experience_years >= 10;
}

/* === Утилиты ввода === */
static int read_int(const char* prompt, int min, int max, int* out) {
    char buf[64];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    char* end;
    long val = strtol(buf, &end, 10);
    if (end == buf || *end != '\n' || val < min || val > max) return -1;
    *out = (int)val;
    return 0;
}

static int read_double(const char* prompt, double min, double max, double* out) {
    char buf[64];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    char* end;
    double val = strtod(buf, &end);
    if (end == buf || *end != '\n' || val < min || val > max) return -1;
    *out = val;
    return 0;
}

/* === Вывод === */
static void print_array(const DynArray* arr, const char* title) {
    printf("\n=== %s (Size: %zu) ===\n", title, array_size(arr));
    for (size_t i = 0; i < array_size(arr); ++i) {
        void* p = array_get(arr, i);
        if (p) person_print(p);
    }
}

static void load_demo(DynArray* arr) {
    Student s1 = {{"Ivanov", 20, 1}, 4.8};
    Student s2 = {{"Petrov", 19, 2}, 3.9};
    Student s3 = {{"Sidorov", 21, 3}, 4.5};
    Teacher t1 = {{"Kozlov", 45, 4}, 15, "Math"};
    Teacher t2 = {{"Volkov", 38, 5}, 5, "Physics"};
    
    if (array_push(arr, &s1) != 0 || array_push(arr, &s2) != 0 || 
        array_push(arr, &s3) != 0 || array_push(arr, &t1) != 0 || 
        array_push(arr, &t2) != 0) {
        printf("Error: Failed to load demo data.\n");
    } else {
        printf("Demo data loaded: 3 Students, 2 Teachers.\n");
    }
}

/* === Главный интерфейс === */
void run_interface(void) {
    DynArray* people = array_create(element_info_person());
    if (!people) { printf("Error: Failed to create array.\n"); return; }
    load_demo(people);

    while (1) {
        printf("\n--- Variant 17: Polymorphic Dynamic Array ---\n");
        printf("1. Add Student\n2. Add Teacher\n3. Show All\n");
        printf("4. Map: Increment GPA (Students)\n");
        printf("5. Where: Excellent Students (GPA >= 4.5)\n");
        printf("6. Where: Senior Teachers (Exp >= 10)\n");
        printf("7. Concat: Merge Arrays\n8. Clear\n9. Reload Demo\n0. Exit\nChoice: ");
        
        int choice;
        if (read_int("", 0, 9, &choice) != 0) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                char name[128]; int age, id; double gpa;
                printf("Name: "); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
                if (read_int("Age: ", 16, 100, &age) != 0 || 
                    read_int("ID: ", 1, 9999, &id) != 0 || 
                    read_double("GPA: ", 0.0, 5.0, &gpa) != 0) {
                    printf("Invalid input.\n"); break;
                }
                Student s = {{name, age, id}, gpa};
                if (array_push(people, &s) != 0) printf("Error adding student.\n");
                else printf("Student added.\n");
                break;
            }
            case 2: {
                char name[128], subj[64]; int age, id, exp;
                printf("Name: "); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
                if (read_int("Age: ", 16, 100, &age) != 0 || 
                    read_int("ID: ", 1, 9999, &id) != 0 || 
                    read_int("Experience: ", 0, 50, &exp) != 0) {
                    printf("Invalid input.\n"); break;
                }
                printf("Subject: "); fgets(subj, sizeof(subj), stdin); subj[strcspn(subj, "\n")] = 0;
                Teacher t = {{name, age, id}, exp, subj};
                if (array_push(people, &t) != 0) printf("Error adding teacher.\n");
                else printf("Teacher added.\n");
                break;
            }
            case 3: print_array(people, "All Records"); break;
            case 4: {
                DynArray* mapped = array_map(people, map_increment_gpa);
                if (!mapped) printf("Map failed.\n");
                else { print_array(mapped, "Mapped (GPA +0.1)"); array_destroy(mapped); }
                break;
            }
            case 5: {
                DynArray* res = array_where(people, filter_excellent);
                if (!res) printf("Where failed.\n");
                else { print_array(res, "Excellent Students"); array_destroy(res); }
                break;
            }
            case 6: {
                DynArray* res = array_where(people, filter_senior);
                if (!res) printf("Where failed.\n");
                else { print_array(res, "Senior Teachers"); array_destroy(res); }
                break;
            }
            case 7: {
                DynArray* empty = array_create(element_info_person());
                DynArray* concat = array_concat(people, empty);
                if (!concat) printf("Concat failed.\n");
                else {
                    printf("Concat successful. New size: %zu\n", array_size(concat));
                    array_destroy(concat);
                }
                array_destroy(empty);
                break;
            }
            case 8: array_clear(people); printf("Array cleared.\n"); break;
            case 9: array_clear(people); load_demo(people); break;
            case 0: array_destroy(people); printf("Exit.\n"); return;
            default: printf("Invalid option.\n");
        }
    }
}