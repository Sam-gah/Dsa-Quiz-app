#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define constants
#define MAX_QUIZZES 10
#define MAX_QUESTIONS 10
#define MAX_OPTIONS 4
#define MAX_TITLE_LENGTH 50
#define MAX_DESC_LENGTH 100
#define MAX_QUESTION_LENGTH 100
#define MAX_OPTION_LENGTH 50

// Define quiz and question structs
typedef struct {
    char question_text[MAX_QUESTION_LENGTH];
    char options[MAX_OPTIONS][MAX_OPTION_LENGTH];
    int correct_answer;
} Question;

typedef struct {
    char title[MAX_TITLE_LENGTH];
    char description[MAX_DESC_LENGTH];
    int num_questions;
    Question questions[MAX_QUESTIONS];
} Quiz;

// Function prototypes
void start_quiz(Quiz quiz);
void start_quiz_timer(Quiz quiz, int time_limit);
void create_quiz();
void save_quiz(Quiz quiz);
Quiz load_quiz();
int menu();

int main() {
    // Seed the random number generator
    srand(time(NULL));
    
    // Show the menu and prompt the user for input
    int choice = menu();
    
    // Loop until the user chooses to exit
    while (choice != 4) {
        switch (choice) {
            case 1:
                // Load a pre-made quiz
                printf("Loading quiz...\n");
                Quiz quiz = load_quiz();
                start_quiz(quiz);
                break;
            case 2:
                // Create a new quiz
                create_quiz();
                break;
            case 3:
                // Load a saved quiz
                printf("Loading saved quiz...\n");
                Quiz saved_quiz = load_quiz();
                start_quiz(saved_quiz);
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
        // Show the menu again
        choice = menu();
    }
    
    return 0;
}

// Function to display the main menu and get the user's choice
int menu() {
    int choice;
    printf("\n");
    printf("==========\n");
    printf(" MAIN MENU\n");
    printf("==========\n");
    printf("1. Load pre-made quiz\n");
    printf("2. Create new quiz\n");
    printf("3. Load saved quiz\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

// Function to start a quiz with pre-made questions
void start_quiz(Quiz quiz) {
    printf("Starting quiz '%s': %s\n", quiz.title, quiz.description);
    for (int i = 0; i < quiz.num_questions; i++) {
        printf("Question %d: %s\n", i+1, quiz.questions[i].question_text);
        for (int j = 0; j < MAX_OPTIONS; j++) {
            printf("%d. %s\n", j+1, quiz.questions[i].options[j]);
        }
        int answer;
        printf("Enter your answer (1-%d): ", MAX_OPTIONS);
        scanf("%d", &answer);
        if (answer == quiz.questions[i].correct_answer) {
            printf("Correct!\n");
        } else {
            printf("Incorrect.\n");
        }
    }
    printf("Quiz complete.\n");
}

// Function to start a quiz with a timer
// Function to start a quiz with a timer
void start_quiz_timer(Quiz quiz, int time_limit) {
    int score = 0;
    printf("Starting quiz '%s': %s\n", quiz.title, quiz.description);
    time_t start_time = time(NULL);
    time_t current_time = start_time;
    int remaining_time = time_limit;
    for (int i = 0; i < quiz.num_questions; i++) {
        printf("Question %d: %s\n", i+1, quiz.questions[i].question_text);
        for (int j = 0; j < MAX_OPTIONS; j++) {
            printf("%d. %s\n", j+1, quiz.questions[i].options[j]);
        }
        int answer;
        printf("Enter your answer (1-%d): ", MAX_OPTIONS);
        scanf("%d", &answer);
        if (answer == quiz.questions[i].correct_answer) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Incorrect.\n");
        }
        current_time = time(NULL);
        remaining_time = time_limit - (int)difftime(current_time, start_time);
        if (remaining_time <= 0) {
            printf("Time's up!\n");
            break;
        } else {
            printf("Time remaining: %d seconds\n", remaining_time);
        }
    }
    printf("Quiz complete. Final score: %d/%d\n", score, quiz.num_questions);
}

// Function to create a new quiz
void create_quiz() {
    // Initialize a new Quiz struct
    Quiz quiz;
    quiz.num_questions = 0;
    
    // Get the title and description from the user
    printf("Enter quiz title: ");
    scanf(" %[^\n]", quiz.title);
    printf("Enter quiz description: ");
    scanf(" %[^\n]", quiz.description);
    
    // Loop to add questions
    char response = 'y';
    while (response == 'y' && quiz.num_questions < MAX_QUESTIONS) {
        Question question;
        printf("Enter question text: ");
        scanf(" %[^\n]", question.question_text);
        for (int i = 0; i < MAX_OPTIONS; i++) {
            printf("Enter option %d: ", i+1);
            scanf(" %[^\n]", question.options[i]);
        }
        printf("Enter correct answer (1-%d): ", MAX_OPTIONS);
        scanf("%d", &question.correct_answer);
        quiz.questions[quiz.num_questions] = question;
        quiz.num_questions++;
        if (quiz.num_questions < MAX_QUESTIONS) {
            printf("Add another question? (y/n): ");
            scanf(" %c", &response);
        }
    }
    // Save the quiz
    save_quiz(quiz);
    printf("Quiz saved.\n");
}

// Function to save a quiz to a file
void save_quiz(Quiz quiz) {
    char filename[MAX_TITLE_LENGTH + 4]; // +4 for .txt extension
    sprintf(filename, "%s.txt", quiz.title);
    FILE *file = fopen(filename, "w");
    fprintf(file, "%s\n%s\n", quiz.title, quiz.description);
    for (int i = 0; i < quiz.num_questions; i++) {
        fprintf(file, "%s\n", quiz.questions[i].question_text);
        for (int j = 0; j < MAX_OPTIONS; j++) {
            fprintf(file, "%s\n", quiz.questions[i].options[j]);
        }
        fprintf(file, "%d\n", quiz.questions[i].correct_answer);
    }
}
  
Quiz load_quiz(char *filename) {
    Quiz quiz;
    FILE *file = fopen(filename, "r");
    fscanf(file, " %[^\n]\n", quiz.title);
    fscanf(file, " %[^\n]\n", quiz.description);
    quiz.num_questions = 0;
    while (!feof(file) && quiz.num_questions < MAX_QUESTIONS) {
        Question question;
        fscanf(file, " %[^\n]\n", question.question_text);
        for (int i = 0; i < MAX_OPTIONS; i++) {
            fscanf(file, " %[^\n]\n", question.options[i]);
        }
        fscanf(file, " %d\n", &question.correct_answer);
        quiz.questions[quiz.num_questions] = question;
        quiz.num_questions++;
    }
    fclose(file);
    return quiz;
}

