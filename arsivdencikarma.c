#include "tarsau.h"

void arsivden_cikarma(char *arsiv_adi, char *hedef_dizin) {
    int fd = open(arsiv_adi, O_RDONLY);
    if (fd == -1) {
        printf("Arşiv dosyası uygunsuz veya bozuk\n"); 
        return;
    }

    char dosya_size_str[11];
    if (read(fd, dosya_size_str, 10) <= 0) {
        close(fd);
        return;
    }
    dosya_size_str[10] = '\0';
    int org_boyut = atoi(dosya_size_str); 

    char *org_bolumu = (char*)malloc(org_boyut + 1);
    read(fd, org_bolumu, org_boyut);
    org_bolumu[org_boyut] = '\0';

    struct stat st = {0};
    if (stat(hedef_dizin, &st) == -1) {
        mkdir(hedef_dizin, 0700); 
    }

    char *token = strtok(org_bolumu, "|");
    
    while (token != NULL) {
        char *kayit = strdup(token); 
        char *f_name = strtok(kayit, ",");
        char *f_mode = strtok(NULL, ",");
        char *f_size = strtok(NULL, ",");

        if (f_name && f_mode && f_size) {
            char yol[512];
            snprintf(yol, sizeof(yol), "%s/%s", hedef_dizin, f_name);

            int dosya_boyutu = atoi(f_size);
            int yeni_fd = open(yol, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            
            if (yeni_fd != -1) {
                char *tampon = (char*)malloc(dosya_boyutu);
                ssize_t okunan = read(fd, tampon, dosya_boyutu);
                if (okunan > 0) {
                    write(yeni_fd, tampon, okunan);
                }

                unsigned int mod = strtol(f_mode, NULL, 8);
                chmod(yol, mod); 

                close(yeni_fd);
                free(tampon);
            }
        }
        free(kayit);
        token = strtok(NULL, "|"); 
    }
    free(org_bolumu);
    close(fd);
    printf("%s dizinine dosyalar başarıyla açıldı.\n", hedef_dizin); 
}
