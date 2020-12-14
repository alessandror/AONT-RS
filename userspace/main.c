#include <stdio.h>
#include <stdlib.h>
#include "cauchy_rs.h"
#include "aont.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/random.h>
#include <kcapi.h>

#define FILE_SIZE 4194304 
#define BLOCK_SIZE 4096

int read_file(uint8_t *buf, size_t data_size, char* path){
    int fd;
    int ret = 0;
    
    fd = open(path, O_RDONLY);
    ret = read(fd, buf, data_size);
    close(fd);
    return ret;
}

int write_file(uint8_t *buf, size_t data_size, char* path){
    int fd;
    int ret = 0;

    fd = open(path, O_CREAT|O_WRONLY|O_TRUNC, 0);
    ret = write(fd, buf, data_size);
    close(fd);
    return ret;
}

int main(){
    /*char* input_file[] = {"/home/austen/Documents/io-cs111-s19.pdf"};
    char* output_file[] = {"/home/austen/Documents/io-cs111-s19-encoded.txt"};
    char* output_encrypted_file[] = {"/home/austen/Documents/io-cs111-s19-encrypted.txt"};

    size_t data_blocks = 2;
    size_t parity_blocks = 3;
    size_t data_length = BLOCK_SIZE;
    uint8_t **shares = malloc(sizeof(uint8_t*) * (data_blocks + parity_blocks));
    int i = 0, j = 0, ret = 0;
    struct timespec timespec1, timespec2;
    uint8_t erasures[0] = {};
    uint8_t num_erasures = 0;
    uint8_t key[32];
    uint8_t total_shares = 0;
    size_t share_size = get_share_size(data_length, data_blocks);
    uint8_t *read_buffer = malloc(FILE_SIZE);
    uint8_t *write_buffer = malloc((data_blocks + parity_blocks) * share_size * (FILE_SIZE / BLOCK_SIZE));

    uint8_t *encrypt_buffer = malloc(FILE_SIZE);
    printf("Share size %ld\n", share_size);

    for(i = 0; i < data_blocks + parity_blocks; i++) shares[i] = malloc(share_size);


    read_file(read_buffer, FILE_SIZE, input_file[0]);
    
    for(i = 0; i < FILE_SIZE/BLOCK_SIZE; i++) {
	printf("Encoding %d\n", i);
        encode_aont_package(&read_buffer[i * BLOCK_SIZE], data_length, shares, data_blocks, parity_blocks);
	for(j = 0; j < data_blocks + parity_blocks; j++){
	    printf("memcpy %d\n", j);
            printf("share number %d\n", total_shares);
	    memcpy(&write_buffer[total_shares * share_size], shares[j], share_size);
	    total_shares++;
	}
    }

    write_file(write_buffer, (data_blocks + parity_blocks)* share_size * (FILE_SIZE / BLOCK_SIZE), output_file[0]);


    ret = getrandom(key, sizeof(key), 0);
    uint8_t iv[32];
    memset(iv, 0, 32);
    uint8_t *ciphertext = malloc(BLOCK_SIZE);
    for(i = 0; i < FILE_SIZE / BLOCK_SIZE; i++){
        kcapi_cipher_enc_aes_cbc(key, 32, &read_buffer[i*BLOCK_SIZE], BLOCK_SIZE, iv, ciphertext, BLOCK_SIZE);
	memcpy(&write_buffer[i * BLOCK_SIZE], ciphertext, BLOCK_SIZE);
    }

    write_file(write_buffer, FILE_SIZE, output_encrypted_file[0]);

    free(read_buffer);
    free(write_buffer);
    free(encrypt_buffer);
    free(shares);
    free(ciphertext);*/

    uint8_t input[4096];
    uint8_t output[4096];
    size_t share_size = get_share_size(4096, 2);
    size_t data_blocks = 2;
    size_t parity_blocks = 3;
    uint8_t erasures[0] = {};
    uint8_t num_erasures = 0;
    int ret;
    int i;
    uint8_t **shares = malloc(sizeof(uint8_t*) * (data_blocks + parity_blocks)); 
    if(shares == NULL){
        return -1;
    }

    for(i = 0; i < data_blocks + parity_blocks; i++){
        shares[i] = malloc(share_size);
    }

    ret = getrandom(input, 4096, 0);

    encode_aont_package(input, 4096, shares, data_blocks, parity_blocks);

    decode_aont_package(output, 4096, shares, data_blocks, parity_blocks, erasures, num_erasures);

    for(i = 0; i < 4096; i++){
        if(input[i] != output[i]){
            printf("Input mismatch %d\n", i);
	}
    }
    return ret;
}
