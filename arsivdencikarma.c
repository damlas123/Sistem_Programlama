#include "tarsau.h"

void arsivden_cikarma(char *arsiv_adi, char *hedef_dizin) {

    /
    const char *ext = strrchr(arsiv_adi, '.');
    if (!ext || strcmp(ext, ".sau") != 0) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n");
        return;
    }

    int fd = open(arsiv_adi, O_RDONLY);
    if (fd == -1) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n");
        return;
    }

    char dosya_size_str[11];
    if (read(fd, dosya_size_str, 10) <= 0) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n");
        close(fd);
        return;
    }
    dosya_size_str[10] = '\0';
    long org_boyut = atol(dosya_size_str);


    char *org_bolumu = (char*)malloc(org_boyut + 1);
    if (!org_bolumu) {
        fprintf(stderr, "Hata: Bellek ayrılamadı.\n");
        close(fd);
        return;
    }
    read(fd, org_bolumu, org_boyut);
    org_bolumu[org_boyut] = '\0';

    // Hedef dizin yoksa oluştur
    struct stat st = {0};
    if (stat(hedef_dizin, &st) == -1) {
        mkdir(hedef_dizin, 0700);
    }

    char *saveptr_dis;
    char *token = strtok_r(org_bolumu, "|", &saveptr_dis);

    while (token != NULL) {
        char *kayit = strdup(token);
        if (!kayit) {
            token = strtok_r(NULL, "|", &saveptr_dis);
            continue;
        }

        char *saveptr_ic;
        char *f_name = strtok_r(kayit, ",", &saveptr_ic);
        char *f_mode = strtok_r(NULL, ",", &saveptr_ic);
        char *f_size = strtok_r(NULL, ",", &saveptr_ic);

        if (f_name && f_mode && f_size) {
            char yol[512];
            snprintf(yol, sizeof(yol), "%s/%s", hedef_dizin, f_name);

            long dosya_boyutu = atol(f_size);

            int yeni_fd = open(yol, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (yeni_fd != -1) {
                char *tampon = (char*)malloc(dosya_boyutu);
                if (tampon) {
                    ssize_t okunan = read(fd, tampon, dosya_boyutu);
                    if (okunan > 0) {
                        write(yeni_fd, tampon, okunan);
                    }
                    free(tampon);
                }

                unsigned int mod = (unsigned int)strtol(f_mode, NULL, 8);
                chmod(yol, mod);

                close(yeni_fd);
            }
        }

        free(kayit);
        token = strtok_r(NULL, "|", &saveptr_dis);
    }

    free(org_bolumu);
    close(fd);
    printf("%s dizinine dosyalar başarıyla açıldı.\n", hedef_dizin);
}