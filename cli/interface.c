#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynarray.h"
#include "element_info.h"
#include "polymorphic_record.h"

static void* map_increment_gpa(const void* p) {
    const PersonRecord* rec = (const PersonRecord*)p;
    if (rec->type == RECORD_STUDENT) {
        PersonRecord* copy = (PersonRecord*)record_clone(p);
        copy->data.student.gpa += 0.1;
        if (copy->data.student.gpa > 5.0) copy->data.student.gpa = 5.0;
        return copy;
    }
    return record_clone(p);
}

static int filter_excellent(const void* p) {
    const PersonRecord* rec = (const PersonRecord*)p;
    return rec->type == RECORD_STUDENT && rec->data.student.gpa >= 4.5;
}

static int filter_senior(const void* p) {
    const PersonRecord* rec = (const PersonRecord*)p;
    return rec->type == RECORD_TEACHER && rec->data.teacher.experience_years >= 10;
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

static int has_duplicate_id(const DynArray* arr, int id) {
    for (size_t i = 0; i < array_size(arr); ++i) {
        const PersonRecord* rec = (const PersonRecord*)array_get(arr, i);
        if (rec && rec->id == id) return 1;
    }
    return 0;
}

static void print_array(const DynArray* arr, const char* title) {
    printf("\n=== %s (Size: %zu) ===\n", title, array_size(arr));
    for (size_t i = 0; i < array_size(arr); ++i) {
        void* p = array_get(arr, i);
        if (p) {
            char* str = arr->info->to_string(p);
            if (str) {
                printf("%s\n", str);
                free(str);
            }
        }
    }
}

static void load_demo(DynArray* arr) {
    PersonRecord* s1 = record_create_student("Ivanov", 20, 1, 4.8);
    PersonRecord* s2 = record_create_student("Petrov", 19, 2, 3.9);
    PersonRecord* s3 = record_create_student("Sidorov", 21, 3, 4.5);
    PersonRecord* t1 = record_create_teacher("Kozlov", 45, 4, 15, "Math");
    PersonRecord* t2 = record_create_teacher("Volkov", 38, 5, 5, "Physics");

    if (array_push(arr, s1) || array_push(arr, s2) || array_push(arr, s3) ||
        array_push(arr, t1) || array_push(arr, t2)) {
        printf("Error: Failed to load demo data.\n");
    } else {
        printf("Demo data loaded: 3 Students, 2 Teachers.\n");
    }
    record_destroy(s1); record_destroy(s2); record_destroy(s3);
    record_destroy(t1); record_destroy(t2);
}

void run_interface(void) {
    DynArray* people = array_create(element_info_person_record());
    if (!people) { printf("Error: Failed to create array.\n"); return; }
    load_demo(people);

    while (1) {
        printf("\n--- Variant 17: Polymorphic Dynamic Array ---\n");
        printf("1. Add Student\n2. Add Teacher\n3. Show All\n");
        printf("4. Map: Increment GPA (Students only)\n");
        printf("5. Where: Excellent Students (GPA >= 4.5)\n");
        printf("6. Where: Senior Teachers (Exp >= 10)\n");
        printf("7. Concat: Merge with another array\n");
        printf("8. Clear\n9. Reload Demo\n0. Exit\nChoice: ");

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
                if (read_int("Age: ", 16, 100, &age) || read_int("ID: ", 1, 9999, &id) ||
                    read_double("GPA: ", 0.0, 5.0, &gpa)) { printf("Invalid input.\n"); break; }
                if (has_duplicate_id(people, id)) { printf("Error: ID %d already exists.\n", id); break; }
                PersonRecord* s = record_create_student(name, age, id, gpa);
                if (!s) { printf("Memory error.\n"); break; }
                if (array_push(people, s)) printf("Error adding student.\n");
                else printf("Student added.\n");
                record_destroy(s);
                break;
            }
            case 2: {
                char name[128], subj[64]; int age, id, exp;
                printf("Name: "); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
                if (read_int("Age: ", 16, 100, &age) || read_int("ID: ", 1, 9999, &id) ||
                    read_int("Experience: ", 0, 50, &exp)) { printf("Invalid input.\n"); break; }
                printf("Subject: "); fgets(subj, sizeof(subj), stdin); subj[strcspn(subj, "\n")] = 0;
                if (has_duplicate_id(people, id)) { printf("Error: ID %d already exists.\n", id); break; }
                PersonRecord* t = record_create_teacher(name, age, id, exp, subj);
                if (!t) { printf("Memory error.\n"); break; }
                if (array_push(people, t)) printf("Error adding teacher.\n");
                else printf("Teacher added.\n");
                record_destroy(t);
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
                DynArray* second = array_create(element_info_person_record());
                if (!second) { printf("Memory error.\n"); break; }
                DynArray* merged = array_concat(people, second);
                if (!merged) printf("Concat failed.\n");
                else {
                    printf("Concat successful. Size: %zu (unchanged).\n", array_size(merged));
                    array_destroy(merged);
                }
                array_destroy(second);
                break;
            }
            case 8: array_clear(people); printf("Array cleared.\n"); break;
            case 9: array_clear(people); load_demo(people); break;
            case 0: array_destroy(people); printf("Exit.\n"); return;
            default: printf("Invalid option.\n");
        }
    }
}