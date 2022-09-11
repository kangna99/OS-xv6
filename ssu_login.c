#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char userID[16][32];
char pwdID[16][32];
char id[32];
char pw[32];
int user_cnt;

//정상 리턴 1, EOF 리턴 0
int parser(int fd, char* buf) {
    char c;
    for(int i=0; i<32; i++) {
        if(read(fd, &c, sizeof(c)) <= 0) return 0;
        if(c == '\n' || c == ' ') break;
        buf[i] = c;
    }
    return 1;
}

void get_user_list() {
    int fd;

    if ((fd = open("list.txt", O_RDONLY)) < 0) {
        printf(1, "cannot open list.txt\n");
        exit();
    }

    for(int i=0; i<16; i++) {
        // list.txt에서 Username, Password 정보를 userID, pwdID에 저장
        parser(fd, userID[i]);
        int flag = parser(fd, pwdID[i]);
        user_cnt++;
        if(flag == 0) break;
    }
    close(fd);
}

// 비교해서 찾으면 0리턴, 못찾으면 -1리턴
int check_idpw() {
    // 입력받은 Username, Password와 list.txt 비교
    for(int i=0; i<user_cnt; i++) 
        if (!strcmp(id, userID[i]) && !strcmp(pw, pwdID[i])) return 0;
    return -1;
}

int main(int argc, char *argv[])
{
    int pid, wpid;
    memset(id, 0, sizeof(id));
    memset(pw, 0, sizeof(pw));
    
    get_user_list();
    printf(1, "Username: ");
    gets(id, sizeof(id));
    id[strlen(id)-1] = '\0';
    printf(1, "Password: ");
    gets(pw, sizeof(pw));
    pw[strlen(pw)-1] = '\0';

    if(check_idpw() < 0) exit();

    pid = fork();
    if(pid < 0) {
        printf(1, "init: fork failed\n");
        exit();
    }
    if(pid == 0) {
        exec("sh", argv);
        printf(1, "init: exec sh failed\n");
        exit();
    }
    while((wpid=wait()) >= 0 && wpid!=pid)
        printf(1, "zombie!\n");
}