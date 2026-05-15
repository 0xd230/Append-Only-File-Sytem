//to start off, after these months of working on this hellish piece of shit
//i have never written such dirty and risky code, and this project proved
//the lack of knowledge i have in this kind of things

//also, i will remake this one day, with at least a cleaner code

//you nkow...i just found a vid in tiktok...and now im thinking....
//wait nvm ill just say this inside the logs

//this is yo stupid ass a year in the future
//great fucking idea, "ill say it in the logs" he said before proceeding to forget and neglect this shit. Real genius move, dumbass
//you didnt even write shit in the logs

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

typedef enum {isfalse, istrue} boolean;

enum Type {
    TEXT = 0xAA,
    IMG = 0xAB,
    VID = 0xAC
};

struct Dirent {
    unsigned short name_size;
    char * name;
    enum Type type;
    unsigned long fsize;
};

struct Depos {
    char * deName;
    unsigned long pos;
};

const unsigned char deprefix = 0xFF;
boolean running = istrue;

char ** parsestr(char *);
void drawBorder(char);

void stdinFlush();
unsigned long len(char *);
boolean isStringSame(char *, char *);
int strContains(unsigned char *, unsigned char);
unsigned char * readline(char *, unsigned long);
unsigned long getLastLineAddr(char *);

boolean checkPass(char *, char *);
void inputPass(char * pass);
struct Dirent createde(char **, enum Type);
struct Dirent parsede(char *);
void pushFile(struct Dirent, char *, char *);
unsigned long countde(char * file);
struct Depos * listde(char *);
unsigned char * readcontentsde(char *, unsigned long);

int main(int argc, char ** argv) {
    char * file = "test", * pass = NULL;
    unsigned long size;

    getline(&pass, &size, stdin);
    printf("%d", isStringSame((char *)readline(file, 0), pass) ? 1 : 0);

    return 0;
}

char ** parsestr(char * str) {
    unsigned long size = len(str);
    unsigned long arrsize = 1;
    unsigned long strsize = 0;
    char ** arr;
    arr = malloc(arrsize * sizeof(char *));

    //ai used for help of reallocation and allocation

    for (int i = 0; i < size; i++) {
        if (str[i] == ' ') {
            arrsize++;
            arr = realloc(arr, arrsize * sizeof(char *));
        }
    }

    for (int i = 0; i < arrsize; i++) {
        arr[i] = malloc(1);
    }

    for (int i = 0, c = 0; i < arrsize;) {
        if (*str == ' ' || *str == 0x0) {
            c = 0;
            strsize = 0;
            i++;
            goto skip;
        }

        strsize++;
        arr[i] = realloc(arr[i], strsize + 1);
        arr[i][c - 1] = *str;
        arr[i][c] = 0x0;

        c++;
        skip:
            str++;
    }

    return arr;
}

void drawBorder(char c) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int width = w.ws_col;
    while (width-- > 0)
        printf("%c", c);
}

void stdinFlush() {
    int c;
    while ((c = getchar()) != 0xa && c != EOF);
}

//algorithm implemented from the standard C library
unsigned long len(char * str) {
    int i;
    for (i = 0; str[i] != 0x0; i++);
    return i;
}

boolean isStringSame(char * str1, char * str2) {
    if (len(str1) != len(str2))
        return isfalse;

    for (int i = 0; i < len(str1); i++) {
        if (str1[i] != str2[i])
            return isfalse;
    }
    return istrue;
}

int strContains(unsigned char * str, unsigned char sym) {
    int line = 0;

    if (str == NULL)
        return -1;

    for (int i = 0; i < len((char *) str); i++) {
        if (str[i] == sym)
            return line;
        line++;
    }

    return -1;
}

unsigned char * readline(char * file, unsigned long line) {
    FILE * f = fopen(file, "r");

    unsigned long size = 1;
    unsigned char * buff = malloc(size);
    unsigned long curr = 0;
    int c;
    int i = 0;
    int line_found = 0;

    while ((c = fgetc(f)) != EOF) {
        if (c == 0xa) {
            curr++;
            continue;
        }
        if (curr == line) {
            line_found = 1;
            if (i + 1 >= size) {
                size++;
                buff = realloc(buff, size);
            }
            buff[i] = c;
            i++;
        }
    }

    if (!line_found) { 
        return NULL;
    }

    buff[i] = 0x0;
    fclose(f);
    return buff;
}

unsigned long getLastLineAddr(char * file) {
    FILE * f = fopen(file, "r");
    int i = 0;
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (c == 0xa)
            i++;
    }

    fclose(f);
    return i;
}

boolean checkPass(char * file, char * str) {
    char * pass = (char * )readline(file, 0);

    while (*pass != 0x0) {
        if (*str != *pass)
            return isfalse;
        str++;
        pass++;
    }

    return istrue;
}

void inputPass(char * pass) {

}

struct Dirent createde(char ** buff, enum Type t) {
    struct Dirent de;
    char tmp[128];

    do {
        fgets(tmp, 127, stdin);
        
        if (tmp[0] == 0xa) { //save 1 bit for null terminator
            printf("Name cannot be blank!\n");
            continue;
        }
        
        if (len(tmp) > 63) {
            printf("Name too large!\n");
            stdinFlush();
            continue;
        } else {
            break;
        }
    } while (1);

    de.name_size = len(tmp) - 1;
    de.name = malloc(de.name_size);
    
    for (int i = 0; i < de.name_size && tmp[i] != 0xa; i++)
        de.name[i] = tmp[i];

    de.name[de.name_size] = 0x0;
    
    for (int i = 0; i < len(tmp); i++)
        tmp[i] = 0;

    de.type = t;

    de.fsize = 0;
    *buff = (char *) malloc(1);
    *buff[0] = 0x0;

    char * linep = NULL;
    unsigned long size = 0;
    unsigned linelen;
    while ((linelen = getline(&linep, &size, stdin))) {
        if (isStringSame(linep, "FILEEND\n")) {
            break;
        }
        de.fsize += linelen;
        *buff = realloc(*buff, de.fsize);

        for (int i = 0; linelen-- > 0; i++) {
            (*buff)[len(*buff)] = linep[i];
        }
    }

    return de;
}

struct Dirent parsede(char * str) {
    if ((unsigned char)*str == 10)
        str++;

    struct Dirent de = {0};

    if ((unsigned char)*str !=  deprefix) {
        printf("Line is not a Dirent!");
        return de;
    }

    de.name_size = 0;
    de.name = NULL;
    de.type = 0;
    de.fsize = 0;

    unsigned char * pstr = (unsigned char *)str;
    unsigned long _strlen = len(str);

    while (_strlen-- > 0) {
        if (*pstr == 32 || *pstr == 255)
            goto skip;

        if (de.name_size == 0) {
            de.name_size = *pstr;
            goto skip;
        }

        if (de.name == NULL) {
            de.name = malloc(de.name_size);
            for (int i = 0; i < de.name_size; i++) {
                de.name[i] = *pstr;
                pstr++;
            }
            goto skip;
        }

        
        if (de.type == 0) {
            de.type = *pstr;
            goto skip;
        }
        
        if (de.fsize == 0) {
            int size = 1;
            char * buff = malloc(size);
            buff[0] = 0x0;
            for (int i = 0; *pstr != 0; i++) {
                size++;
                buff = realloc(buff, size);
                buff[len(buff)] = *pstr;
                pstr++;
            }
            de.fsize = atoi(buff);
            goto skip;
        }

        skip:
            pstr++;
    }
    return de;
}

void pushFile(struct Dirent de, char * content, char * file) {
    FILE * f = fopen(file, "a");

    fprintf(f, "%c %c %s %c %lu\n%s", (char)deprefix, (char)de.name_size, de.name, (char)de.type, de.fsize, content);
    fclose(f);
}

unsigned long countde(char * file) {
    unsigned long count = 0;
    unsigned char * buff;
    unsigned long line = 0;
    while((buff = readline(file, line)) != NULL) {
        for (int i = 0; i < len((char *) buff); i++){
            if ((unsigned char) *buff == 255)
                count++;
            buff++;
        }

        line++;
    }

    return count;
}

struct Depos * listde(char * file) {
    unsigned char size = 0;
    unsigned char * buff;
    unsigned long decount = 0;
    FILE * f = fopen(file, "r");
    int line = 0;
    struct Dirent de = {0};
    struct Depos * depos = malloc(countde(file) * sizeof(struct Depos));
    unsigned long index = 0;

    while ((buff = readline(file, line)) != NULL) {
        if (strContains(buff, deprefix) >= 0) {
            de = parsede((char *)buff);
            //printf("%s : %lu\n", de.name, len((char *) de.name));
            //memory leaks, unable to fix, ai used
            depos[index].deName = malloc(len((char *) de.name));
            for (int i = 0; i < len((char *) de.name); i++) {
                depos[index].deName[i] = de.name[i];
            }
            depos[index].pos = line;
            index++;
        }
        
        line++;
    }

    fclose(f);
    return depos;
}

unsigned char * readcontentsde(char * file, unsigned long posde) {
    FILE * f = fopen(file, "r");
    unsigned char * tmp = NULL;
    
    if (strContains(readline(file, posde), deprefix) == -1)
        return NULL;
    
    unsigned char * buff = malloc(1);
    *buff = 0x0;
    unsigned long s = 0;
    while ((tmp = readline(file, posde + 1)) != NULL && strContains((unsigned char *)tmp, 0xff) == -1) {
        buff = realloc(buff, len((char *) buff) + len((char *)tmp) + 1);

        //ai used in this section, cause: afraid of memory leak and loop failure (Hot take: the AI didtn even help shit on this section)
        //the coment above was copy pasted into the AI prompt, it commented on it LMAOOO
        //i didnt like its atitude.

        //Your original comment is fair: AI didn't help shit here — 
        //because many AI tools don’t catch memory management bugs unless prompted clearly.
        
        //just see, ill make my own AI, this comment is its history

        for (int i = 0; i < len((char *)tmp); i++) {
            buff[len((char *)buff)] = tmp[i];
        }
        buff[len((char *)buff)] = 0x0;
        posde++;
    }

    fclose(f);
    return buff;
}