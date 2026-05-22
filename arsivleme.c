#include "tarsau.h"
#include <libgen.h>   

int is_ascii_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    
    unsigned char buffer[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] > 127) {
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return 1;
}

int arsivle(int file_count, char *input_files[], char *output_file) {
    long total_size = 0;
    char records[32 * 512] = ""; 

    for (int i = 0; i < file_count; i++) {
        struct stat st;
        if (stat(input_files[i], &st) != 0) {
            fprintf(stderr, "%s dosyası bulunamadı!\n", input_files[i]);
            return 1;
        }

        if (!is_ascii_file(input_files[i])) {
            printf("%s giriş dosyasının formatı uyumsuzdur!\n", input_files[i]);
            return 0; 
        }

        total_size += st.st_size;
        if (total_size > MAX_TOTAL_SIZE) {
            fprintf(stderr, "Hata: Giriş dosyalarının toplam boyutu 200 MB'ı geçemez.\n");
            return 1;
        }

        char record[256];
        char *sadece_ad = basename(input_files[i]);  
        sprintf(record, "%s,%04o,%ld|", sadece_ad, st.st_mode & 0777, (long)st.st_size);
        strcat(records, record);
    }

  
    int header_size = strlen(records);
    char header_size_str[11];
    sprintf(header_size_str, "%010d", header_size); 

    FILE *out = fopen(output_file, "wb");
    if (!out) {
        perror("Çıktı dosyası oluşturulamadı");
        return 1;
    }

    fwrite(header_size_str, 1, 10, out);
    fwrite(records, 1, strlen(records), out);

    for (int i = 0; i < file_count; i++) {
        FILE *in = fopen(input_files[i], "rb");
        if (!in) {
            fprintf(stderr, "Hata: '%s' dosyası açılamadı.\n", input_files[i]);
            fclose(out);
            return 1;
        }
        unsigned char buffer[CHUNK_SIZE];
        size_t bytes;
        while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0) {
            fwrite(buffer, 1, bytes, out);
        }
        fclose(in);
    }

    fclose(out);
    printf("'%s' arşiv dosyası başarıyla oluşturuldu.\n", output_file);
    return 0;
}