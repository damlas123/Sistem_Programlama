#include "tarsau.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Kullanım: %s -b ... veya %s -a ...\n", argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-b") == 0) {
        char *input_files[MAX_FILES];
        int file_count = 0;
        char *output_file = "a.sau"; 

        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                output_file = argv[++i];
            } else {
                if (file_count >= MAX_FILES) {
                    fprintf(stderr, "Hata: En fazla %d giriş dosyası olabilir.\n", MAX_FILES);
                    return 1;
                }
                input_files[file_count++] = argv[i];
            }
        }
        
        if (file_count == 0) {
            fprintf(stderr, "Hata: Arşivlenecek dosya belirtilmedi.\n");
            return 1;
        }
        
        return arsivle(file_count, input_files, output_file);

    } else if (strcmp(argv[1], "-a") == 0) {
        if (argc < 3 || argc > 4) {
            fprintf(stderr, "Kullanım: %s -a arsiv.sau [hedef_dizin]\n", argv[0]);
            return 1;
        }
        
        char *archive_file = argv[2];
        
        // DİKKAT: Senin kodunun çökmemesi için dizin verilmezse mevcut dizini ( . ) gönderiyoruz
        char *target_dir = (argc == 4) ? argv[3] : ".";
        
        arsivden_cikarma(archive_file, target_dir);
        
        return 0; // arsivden_cikarma fonksiyonun void olduğu için 0 döndürerek bitiriyoruz
        
    } else {
        fprintf(stderr, "Geçersiz parametre: %s\n", argv[1]);
        return 1;
    }

    return 0;
}