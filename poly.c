#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

asm(".intel_syntax noprefix;");
#define PUSH ".byte 0x50;"
#define POP  ".byte 0x58;"
#define MOV  ".byte 0xB8;"
#define NOP  ".byte 0x90;"

#define ADD  ".byte 0x01;"
#define AND  ".byte 0x21;"
#define XOR  ".byte 0x31;"
#define OR   ".byte 0x09;"
#define SBB  ".byte 0x19;"
#define SUB  ".byte 0x29;"

const unsigned char prefixes[] = { 0x01,0x21,0x31,0x09,0x19,0x29,0 };

#define JUNK asm volatile(PUSH NOP NOP NOP NOP NOP NOP NOP NOP POP)
#define JUNKLEN 8

unsigned char *code;
int codelen;

void readcode(const char *filename) {
  FILE *fp = fopen(filename, "rb");    JUNK;
  fseek(fp, 0L, SEEK_END);             JUNK;
  codelen = ftell(fp);
  code = malloc(codelen);              JUNK;
  fseek(fp, 0L, SEEK_SET);
  fread(code, codelen, 1, fp);         JUNK;
}


void writecode(const char *filename) {
  FILE *fp;
  int lastoffset = strlen(filename)-1;
  char lastchar = filename[lastoffset];
  char *newfilename = strdup(filename);  JUNK;
  lastchar = '0'+(isdigit(lastchar)?(lastchar-'0'+1)%10:0);
  newfilename[lastoffset] = lastchar;
  fp = fopen(newfilename, "wb");         JUNK;
  fwrite(code, codelen, 1, fp);          JUNK;
  fclose(fp);
  free(newfilename);
}


int writeinstruction(unsigned reg, int offset, int space) {
  if (space < 2) {
    code[offset] = 0x90;                         JUNK;
    return 1;
  }
  else if (space < 5 || rand()%2 == 0) {
    code[offset] = prefixes[rand()%6];          JUNK;
    code[offset+1] = 0xC0 + rand()%8*8 + reg;   JUNK;
    return 2;
  }
  else {
    code[offset] = 0xB8+reg;                     JUNK;
    *(short*)(code+offset+1) = rand();
    *(short*)(code+offset+3) = rand();          JUNK;
    return 5;
  }
}

int readinstruction(unsigned reg, int offset) {
  unsigned int c1 = code[offset];
  if (c1 == 0x90){
      return 1;
  
  }
    JUNK;
  if (c1 == 0xB8+reg) {
    return 5;          }           JUNK;
  if (strchr(prefixes,c1)) {
    unsigned c2 = code[offset+1]; JUNK;
    if (c2 >= 0xC0 && c2 <= 0xFF && (c2&7) == reg)
      return 2;                   JUNK;
  }                               JUNK;
  return 0;
}



void replacejunk(void) {
  int i, j, inc, space;
  srand(time(NULL));                                 JUNK;

  for (i = 0; i < codelen-JUNKLEN-2; i++) {
    unsigned int start = code[i];
    unsigned int end = code[i+JUNKLEN+1];
    unsigned int reg = start-0x50;
    
    if (start != 0x50) continue; JUNK;
    //if (start < PUSH || start >= PUSH+8) continue;   JUNK;
    if (end != 0x58+reg) continue;                    JUNK;
    if (reg == 4) continue; /* register 4 is ESP */

    j = 0;                                           JUNK;
    while ((inc = readinstruction(reg,i+1+j))) j += inc;
    if (j != JUNKLEN) continue;                      JUNK;



    reg = rand()%7;                                  JUNK;
    reg += (reg == 4);
    code[i] = 0x50+reg;                              JUNK;
    code[i+JUNKLEN+1] = 0x58+reg;                     JUNK;

    space = JUNKLEN;
    j = 0;                                           JUNK;
    while (space) {
      inc = writeinstruction(reg,i+1+j,space);       JUNK;
      j += inc;
      space -= inc;                                  JUNK;
    }

    printf("%d\n",i);                                JUNK;
  }
}


int main(int argc, char* argv[]) {

  readcode(argv[0]);     JUNK;
  replacejunk();         JUNK;
  writecode(argv[0]);    JUNK;

  return 0;
}







