#ifndef TARSAU_H
#define TARSAU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> // open, O_RDONLY, O_WRONLY vb. için gerekli

#define MAX_FILES 32
#define MAX_TOTAL_SIZE (200 * 1024 * 1024) // 200 MB
#define CHUNK_SIZE 4096

// Fonksiyon Prototipleri
int is_ascii_file(const char *filename);
int arsivle(int file_count, char *input_files[], char *output_file);

void arsivden_cikarma(char *arsiv_adi, char *hedef_dizin); 

#endif