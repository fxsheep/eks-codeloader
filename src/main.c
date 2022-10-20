#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "devmem.h"

int main(int argc, char **argv){
    FILE *fp;
    char buf[256];
    uint32_t loadaddr, hex, *progbuf, *progbuf_s;
    uint32_t *dsp_codemem;

    if(argc != 3){
        printf("Invalid args\n");
        return -1;
    }

    sscanf(argv[1], "%x", &loadaddr);
    printf("Load address: 0x%x\n", loadaddr);

    fp = fopen(argv[2], "r");

    if (!fp) {
        printf("Error opening file\n");
        return -1;
    }

    progbuf_s = malloc(0x400);
    progbuf = progbuf_s;
    if (!progbuf) {
        printf("Error allocing buffer\n");
        return -1;        
    }

    for (int i = 0; i < 0x400 / 4 - 1; i++){
        *(progbuf++) = 0x3CA03CA0; // fill code buffer with NOP
    }
    *progbuf = 0x9ABF011F; // SQ.brr{t} -0x3FC ; Jump back to start

    progbuf = progbuf_s; // reset pointer

    printf("### Code to load ###\n");
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        sscanf(buf, "%x", &hex);
        printf("%x\n", hex);
        if (hex < 0x10000) {
            hex = (hex << 16) | 0x3CA0; //padding with NOP
        }
        *(progbuf++) = hex;
        for (int i = 0; i < 4; i++) {
            progbuf++; //pad with 8 NOPs
        }
    }
    printf("### Program buffer ###\n");
    for (int i = 0; i < 0x400 / 4; i++){
        printf("%x\n", progbuf_s[i]);
    }

    dsp_codemem = devm_map(loadaddr, 0x400);
    if (dsp_codemem == NULL) {
        printf("Error mapping DSP code memory\n");
        return -1;        
    }   

    memcpy(dsp_codemem, progbuf_s, 0x400);

    devm_unmap(loadaddr, 0x400);
    fclose(fp);
    return 0;
}
