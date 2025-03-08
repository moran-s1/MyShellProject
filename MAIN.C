#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_INPUT 1024

/**
 * מציג הודעת פתיחה בעת הפעלת ה-Shell
 */
void welcomeMessage() {
    printf("\n==============================\n");
    printf(" Welcome to myShell \n");
    printf("==============================\n\n");
}

/**
 * מציג את שם המשתמש, שם המחשב והנתיב הנוכחי לפני כל קלט
 */
void getLocation() {
    char cwd[PATH_MAX];  // משתנה לשמירת הנתיב הנוכחי
    char hostname[HOST_NAME_MAX];  // שם המחשב
    char *username = getlogin();  // שם המשתמש המחובר

    getcwd(cwd, sizeof(cwd));  // מקבל את הנתיב הנוכחי
    gethostname(hostname, sizeof(hostname));  // מקבל את שם המחשב

    printf("\033[1;34m%s@%s:\033[1;32m%s\033[0m$ ", username, hostname, cwd);
}

/**
 * מקבל קלט מהמשתמש ומנקה את התו '\n' בסופו
 */
void getInput(char *input) {
    getLocation();  // מציג את המיקום הנוכחי לפני קלט
    fgets(input, MAX_INPUT, stdin);
    input[strcspn(input, "\n")] = 0;  // מסיר את תו ה-Newline
}

/**
 * מחלק את הפקודה שהמשתמש הכניס למילים (ארגומנטים)
 */
void splitArgument(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " ");  // מחלק את הפקודה לפי רווחים
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");  // ממשיך לפצל
    }
}

/**
 * משנה תיקייה (פקודת `cd`), תומך בנתיבים עם רווחים
 */
void cd(char **args) {
    if (args[1] == NULL) {
        printf("Usage: cd <directory>\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd failed");
        }
    }
}

/**
 * מעתיק קובץ (פקודת `cp`), קורא וכותב בתים שלמים
 */
void cp(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf("Usage: cp <source> <destination>\n");
        return;
    }

    FILE *src = fopen(args[1], "r");
    FILE *dest = fopen(args[2], "w");

    if (src == NULL || dest == NULL) {
        printf("File error: unable to open file.\n");
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
}

/**
 * מבצע יציאה מה-Shell עם הודעת יציאה מסודרת
 */
void logout() {
    printf("Logging out...\n");
    exit(0);
}

/**
 * הפונקציה הראשית - מקבלת פקודות מהמשתמש ומעבדת אותן
 */
int main() {
    char input[MAX_INPUT];  // מחרוזת לקלט המשתמש
    char *args[64];  // מערך לשמירת הפרמטרים לאחר הפיצול

    welcomeMessage();  // מציג מסך פתיחה

    while (1) {
        getInput(input);  // מקבל קלט מהמשתמש
        splitArgument(input, args);  // מחלק את הקלט לארגומנטים

        // אם המשתמש רוצה לצאת - מפעיל את logout()
        if (strcmp(args[0], "exit") == 0) {
            logout();
        }

        // פקודת שינוי תיקייה
        if (strcmp(args[0], "cd") == 0) {
            cd(args);
            continue;
        }

        // פקודת העתקת קובץ
        if (strcmp(args[0], "cp") == 0) {
            cp(args);
            continue;
        }

        // אם הפקודה לא זוהתה
        printf("Command not found: %s\n", args[0]);
    }

    return 0;
}
