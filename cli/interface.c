#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynarray.h"
#include "element_info.h"
#include "student.h"
#include "teacher.h"


static void* map_increment_gpa(const void* p) {
    const Student* st = (const Student*)p;
    if (st->gpa >= 0.0 && st->gpa <= 5.0) {
        Student* s = student_clone(p);
        s->gpa += 0.1;
        if (s->gpa > 5.0) s->gpa = 5.0;
        return s;
    }
    return teacher_clone(p);
}


static int filter_excellent_student(const void* p) {
    const Student* s = (const Student*)p;
    return s->gpa >= 4.5;
}


static int filter_senior_teacher(const void* p) {
    const Teacher* t = (const Teacher*)p;
    return t->experience_years >= 10;
}


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


static void print_array(const DynArray* arr, const char* title) {
    printf("\n=== %s (Size: %zu) ===\n", title, array_size(arr));
    for (size_t i = 0; i < array_size(arr); ++i) {
        void* p = array_get(arr, i);
        if (p) arr->info->print(p);
    }
}


static void load_demo(DynArray* students, DynArray* teachers) {
    Student s1 = {{"Ivanov", 20, 1}, 4.8};
    Student s2 = {{"Petrov", 19, 2}, 3.9};
    Student s3 = {{"Sidorov", 21, 3}, 4.5};
    Teacher t1 = {{"Kozlov", 45, 4}, 15, "Math"};
    Teacher t2 = {{"Volkov", 38, 5}, 5, "Physics"};

    if (array_push(students, &s1) != 0 || array_push(students, &s2) != 0 ||
        array_push(students, &s3) != 0 ||
        array_push(teachers, &t1) != 0 || array_push(teachers, &t2) != 0) {
        printf("Error: Failed to load demo data.\n");
    } else {
        printf("Demo data loaded: 3 Students, 2 Teachers.\n");
    }
}

void run_interface(void) {
    DynArray* students = array_create(element_info_student());
    DynArray* teachers = array_create(element_info_teacher());
    if (!students || !teachers) {
        printf("Error: Failed to create arrays.\n");
        return;
    }
    load_demo(students, teachers);

    while (1) {
        printf("\n--- Variant 17: Polymorphic Dynamic Array ---\n");
        printf("1. Add Student\n2. Add Teacher\n3. Show All\n");
        printf("4. Map: Increment GPA (Students only)\n");
        printf("5. Where: Excellent Students (GPA >= 4.5)\n");
        printf("6. Where: Senior Teachers (Exp >= 10)\n");
        printf("7. Concat: Merge two arrays (same type)\n");
        printf("8. Clear All\n9. Reload Demo\n0. Exit\nChoice: ");

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
                Student* s = student_create(name, age, id, gpa);
                if (!s) { printf("Memory error.\n"); break; }
                if (array_push(students, s) != 0) printf("Error adding student.\n");
                else printf("Student added.\n");
                student_destroy(s);
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
                Teacher* t = teacher_create(name, age, id, exp, subj);
                if (!t) { printf("Memory error.\n"); break; }
                if (array_push(teachers, t) != 0) printf("Error adding teacher.\n");
                else printf("Teacher added.\n");
                teacher_destroy(t);
                break;
            }
            case 3:
                print_array(students, "Students");
                print_array(teachers, "Teachers");
                break;
            case 4: {
                DynArray* mapped = array_map(students, map_increment_gpa);
                if (!mapped) printf("Map failed.\n");
                else {
                    print_array(mapped, "Students after GPA increment");
                    array_destroy(mapped);
                }
                break;
            }
            case 5: {
                DynArray* res = array_where(students, filter_excellent_student);
                if (!res) printf("Where failed.\n");
                else {
                    print_array(res, "Excellent Students (GPA >= 4.5)");
                    array_destroy(res);
                }
                break;
            }
            case 6: {
                DynArray* res = array_where(teachers, filter_senior_teacher);
                if (!res) printf("Where failed.\n");
                else {
                    print_array(res, "Senior Teachers (Exp >= 10)");
                    array_destroy(res);
                }
                break;
            }
            case 7: {
                printf("Which type to concat? (1 - Students, 2 - Teachers): ");
                int type_choice;
                if (read_int("", 1, 2, &type_choice) != 0) {
                    printf("Invalid input.\n"); break;
                }
                DynArray* main_arr = (type_choice == 1) ? students : teachers;
                DynArray* second = array_create(main_arr->info);
                if (!second) { printf("Memory error.\n"); break; }
                DynArray* merged = array_concat(main_arr, second);
                if (!merged) {
                    printf("Concat failed.\n");
                } else {
                    printf("Concat successful. Size unchanged (%zu).\n", array_size(merged));
                    array_destroy(merged);
                }
                array_destroy(second);
                break;
            }
            case 8:
                array_clear(students);
                array_clear(teachers);
                printf("Both arrays cleared.\n");
                break;
            case 9:
                array_clear(students);
                array_clear(teachers);
                load_demo(students, teachers);
                break;
            case 0:
                array_destroy(students);
                array_destroy(teachers);
                printf("Exit.\n");
                return;
            default:
                printf("Invalid option.\n");
        }
    }
}