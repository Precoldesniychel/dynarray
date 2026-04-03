#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/student.h"
#include "../include/teacher.h"
#include "../include/dynarray.h"

static void print_menu() {
    printf("\n========== Menu ==========\n");
    printf("1. Add student\n");
    printf("2. Add teacher\n");
    printf("3. Show all\n");
    printf("4. Filter: excellent students (GPA >= 4.5)\n");
    printf("5. Filter: experienced teachers (>=10 years)\n");
    printf("6. Concatenate lists\n");
    printf("7. Run unit tests\n");
    printf("8. Demo: Load sample data (15 students + 5 teachers)\n");
    printf("9. Clear single element by index\n");
    printf("10. Clear all elements\n");
    printf("11. Remove element by index\n");
    printf("0. Exit\n");
    printf("==========================\n");
    printf("Choice: ");
}

static PersonID read_id() {
    PersonID id = {0, 0};
    printf("Passport series: ");
    if (scanf("%d", &id.series) != 1) {
        id.series = 0;
    }
    printf("Passport number: ");
    if (scanf("%d", &id.number) != 1) {
        id.number = 0;
    }
    return id;
}

static void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int read_int(const char* prompt, int* value) {
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        clear_input_buffer();
        return -1;
    }
    clear_input_buffer();
    return 0;
}

static int read_double(const char* prompt, double* value) {
    printf("%s", prompt);
    if (scanf("%lf", value) != 1) {
        clear_input_buffer();
        return -1;
    }
    clear_input_buffer();
    return 0;
}

static int read_string(const char* prompt, char* buffer, size_t size) {
    printf("%s", prompt);
    if (scanf("%s", buffer) != 1) {
        clear_input_buffer();
        return -1;
    }
    clear_input_buffer();
    return 0;
}

static void add_student(DynArray* arr) {
    if (!arr) {
        printf("Error: Array is NULL\n");
        return;
    }
    
    PersonID id = read_id();
    char first[50], middle[50], last[50], group[20];
    int course; 
    double grade;
    
    if (read_string("First name: ", first, sizeof(first)) < 0) return;
    if (read_string("Middle name: ", middle, sizeof(middle)) < 0) return;
    if (read_string("Last name: ", last, sizeof(last)) < 0) return;
    if (read_string("Group: ", group, sizeof(group)) < 0) return;
    if (read_int("Course: ", &course) < 0) return;
    if (read_double("Average grade (GPA): ", &grade) < 0) return;
    
    if (grade < 0.0 || grade > 5.0) {
        printf("Error: GPA must be between 0.0 and 5.0\n");
        return;
    }
    if (course < 1 || course > 6) {
        printf("Error: Course must be between 1 and 6\n");
        return;
    }
    
    Student* s = student_create(id, first, middle, last, 0, group, course, grade);
    if (!s) {
        printf("Error: Failed to create student\n");
        return;
    }
    if (array_push(arr, &s) < 0) {
        printf("Error: Failed to add student to array\n");
        student_destroy(s);
    } else {
        printf("Student added successfully!\n");
    }
}

static void add_teacher(DynArray* arr) {
    if (!arr) {
        printf("Error: Array is NULL\n");
        return;
    }
    
    PersonID id = read_id();
    char first[50], middle[50], last[50], dept[50], pos[50];
    int exp; 
    double salary;
    
    if (read_string("First name: ", first, sizeof(first)) < 0) return;
    if (read_string("Middle name: ", middle, sizeof(middle)) < 0) return;
    if (read_string("Last name: ", last, sizeof(last)) < 0) return;
    if (read_string("Department: ", dept, sizeof(dept)) < 0) return;
    if (read_string("Position: ", pos, sizeof(pos)) < 0) return;
    if (read_int("Experience (years): ", &exp) < 0) return;
    if (read_double("Salary: ", &salary) < 0) return;
    
    if (exp < 0 || exp > 60) {
        printf("Error: Experience must be between 0 and 60 years\n");
        return;
    }
    if (salary < 0) {
        printf("Error: Salary cannot be negative\n");
        return;
    }
    
    Teacher* t = teacher_create(id, first, middle, last, 0, dept, pos, exp, salary);
    if (!t) {
        printf("Error: Failed to create teacher\n");
        return;
    }
    if (array_push(arr, &t) < 0) {
        printf("Error: Failed to add teacher to array\n");
        teacher_destroy(t);
    } else {
        printf("Teacher added successfully!\n");
    }
}

static int _filter_excellent(const void* elem) {
    const Person* p = *(const Person* const*)elem;
    return (p->type == PERSON_STUDENT) && student_is_excellent((Student*)p);
}

static int _filter_senior(const void* elem) {
    const Person* p = *(const Person* const*)elem;
    return (p->type == PERSON_TEACHER) && teacher_is_senior((Teacher*)p);
}

static void demo_load_sample_data(DynArray* arr) {
    if (!arr) {
        printf("Error: Array is NULL\n");
        return;
    }
    
    printf("Loading sample data...\n");
    
    const char* student_first[] = {"John", "Jane", "Bob", "Alice", "Charlie", 
                                   "Diana", "Edward", "Fiona", "George", "Helen",
                                   "Ivan", "Julia", "Kevin", "Laura", "Mike"};
    const char* student_last[] = {"Smith", "Johnson", "Williams", "Brown", "Jones",
                                  "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
                                  "Petrov", "Sidorov", "Ivanov", "Kozlov", "Wilson"};
    const char* groups[] = {"507", "508", "509", "510", "511"};
    
    for (int i = 0; i < 15; i++) {
        PersonID id = {1000 + i, 200000 + i};
        double gpa = 3.0 + (i % 10) * 0.2;
        int course = 1 + (i % 4);
        const char* group = groups[i % 5];
        
        Student* s = student_create(id, student_first[i], "", student_last[i], 0, 
                                    group, course, gpa);
        if (s && array_push(arr, &s) < 0) {
            student_destroy(s);
        }
    }
    
    const char* teacher_first[] = {"Alexander", "Maria", "Dmitry", "Elena", "Sergey"};
    const char* teacher_last[] = {"Popov", "Sokolov", "Lebedev", "Kozlov", "Novikov"};
    const char* departments[] = {"Mathematics", "Physics", "Computer Science", "Chemistry", "Biology"};
    const char* positions[] = {"Professor", "Associate Professor", "Senior Lecturer", "Lecturer", "Assistant"};
    
    for (int i = 0; i < 5; i++) {
        PersonID id = {2000 + i, 300000 + i};
        int exp = 5 + i * 3;
        double salary = 50000 + i * 10000;
        
        Teacher* t = teacher_create(id, teacher_first[i], "", teacher_last[i], 0,
                                    departments[i], positions[i], exp, salary);
        if (t && array_push(arr, &t) < 0) {
            teacher_destroy(t);
        }
    }
    
    printf("Sample data loaded: 15 students and 5 teachers added!\n");
}

static void clear_element_by_index(DynArray* arr) {
    if (!arr) {
        printf("Error: Array is NULL\n");
        return;
    }
    
    size_t size = array_size(arr);
    if (size == 0) {
        printf("Array is empty!\n");
        return;
    }
    
    printf("Current array size: %zu\n", size);
    int index;
    if (read_int("Enter index to clear (0 to clear pointer only, 1 to destroy element): ", &index) < 0) {
        return;
    }
    
    int mode;
    if (read_int("Mode (0=clear pointer only, 1=destroy element): ", &mode) < 0) {
        return;
    }
    
    if (index < 0 || (size_t)index >= size) {
        printf("Error: Index out of range!\n");
        return;
    }
    
    void* elem_ptr = array_get(arr, index);
    if (!elem_ptr) {
        printf("Error: Element is NULL\n");
        return;
    }
    
    void* elem = *(void**)elem_ptr;
    if (!elem) {
        printf("Error: Element pointer is NULL\n");
        return;
    }
    
    if (mode == 1) {
        person_destroy((Person*)elem);
        *(void**)elem_ptr = NULL;
        printf("Element at index %d destroyed and cleared!\n", index);
    } else {
        *(void**)elem_ptr = NULL;
        printf("Element at index %d cleared (pointer set to NULL)!\n", index);
    }
}

static void clear_all_elements(DynArray* arr) {
    if (!arr) {
        printf("Error: Array is NULL\n");
        return;
    }
    
    printf("Clear all menu:\n");
    printf("1. Clear pointers only (keep array structure)\n");
    printf("2. Destroy all elements and clear array\n");
    printf("Choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    
    if (choice == 1) {
        for (size_t i = 0; i < arr->size; i++) {
            void* elem_ptr = (char*)arr->data + i * arr->info->element_size;
            *(void**)elem_ptr = NULL;
        }
        printf("All pointers cleared (%zu elements)!\n", arr->size);
    } else if (choice == 2) {
        array_clear(arr);
        printf("All elements destroyed and array cleared!\n");
    } else {
        printf("Invalid choice!\n");
    }
}

static void remove_element_by_index(DynArray* arr) {
    if (!arr) {
        printf("Error: Array is NULL\n");
        return;
    }
    
    size_t size = array_size(arr);
    if (size == 0) {
        printf("Array is empty!\n");
        return;
    }
    
    printf("Current array size: %zu\n", size);
    int index;
    if (read_int("Enter index to remove: ", &index) < 0) {
        return;
    }
    
    if (index < 0 || (size_t)index >= size) {
        printf("Error: Index out of range!\n");
        return;
    }
    
    if (array_remove_at(arr, index) == 0) {
        printf("Element at index %d removed successfully!\n", index);
    } else {
        printf("Error: Failed to remove element!\n");
    }
}

void run_interface() {
    DynArray* people = array_create(16, get_person_ptr_info());
    if (!people) {
        printf("Error: Failed to create array!\n");
        return;
    }
    
    int choice;
    
    do {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid choice! Please enter a number.\n");
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1: 
                add_student(people); 
                break;
            case 2: 
                add_teacher(people); 
                break;
            case 3: 
                if (array_size(people) == 0) {
                    printf("Array is empty!\n");
                } else {
                    array_print(people, stdout);
                }
                break;
            case 4: {
                if (array_size(people) == 0) {
                    printf("Array is empty!\n");
                    break;
                }
                DynArray* excellent = array_where(people, _filter_excellent);
                if (excellent && array_size(excellent) > 0) {
                    printf("\n=== Excellent Students ===\n");
                    array_print(excellent, stdout);
                } else {
                    printf("No excellent students found!\n");
                }
                if (excellent) array_destroy(excellent);
                break;
            }
            case 5: {
                if (array_size(people) == 0) {
                    printf("Array is empty!\n");
                    break;
                }
                DynArray* seniors = array_where(people, _filter_senior);
                if (seniors && array_size(seniors) > 0) {
                    printf("\n=== Experienced Teachers ===\n");
                    array_print(seniors, stdout);
                } else {
                    printf("No experienced teachers found!\n");
                }
                if (seniors) array_destroy(seniors);
                break;
            }
            case 6: {
                if (array_size(people) == 0) {
                    printf("Array is empty! Nothing to concatenate.\n");
                    break;
                }
                DynArray* copy = array_create(8, get_person_ptr_info());
                if (!copy) {
                    printf("Error: Failed to create copy array!\n");
                    break;
                }
                for (size_t i = 0; i < array_size(people); i++) {
                    void* elem = array_get(people, i);
                    array_push(copy, elem);
                }
                DynArray* combined = array_concat(people, copy);
                if (combined) {
                    printf("\n=== Concatenation Result ===\n");
                    array_print(combined, stdout);
                    array_destroy(combined);
                }
                array_destroy(copy);
                break;
            }
            case 7: {
                printf("\nRunning unit tests...\n");
                #ifdef _WIN32
                int ret = system("tests.exe");
                #else
                int ret = system("./tests");
                #endif
                if (ret != 0) {
                    printf("Tests completed with exit code: %d\n", ret);
                }
                break;
            }
            case 8:
                demo_load_sample_data(people);
                break;
            case 9:
                clear_element_by_index(people);
                break;
            case 10:
                clear_all_elements(people);
                break;
            case 11:
                remove_element_by_index(people);
                break;
            case 0: 
                printf("Exiting. Goodbye!\n"); 
                break;
            default: 
                printf("Invalid choice! Please enter 0-11.\n"); 
                break;
        }
    } while (choice != 0);
    
    array_destroy(people);
    printf("All resources freed. Program terminated.\n");
}