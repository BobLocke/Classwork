#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//#define __STDC_FORMAT_MACROS

// SIZES IN BITS
#define NUM_ROUNDS 16
#define BLOCK_SIZE 64

char keystr[16];
uint64_t key;
char textBuf[8];
uint64_t cyphBuf;
uint8_t subkey[16][12];
int debug = 0;

typedef struct BLOCK {
	uint16_t w[4];
} BLOCK; 

typedef struct FPAIR {
	uint16_t f[2];
} FPAIR;

uint16_t ftable[256] = {0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,
0xe7,0x2d,0x4d,0x8a,0xce,0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,
0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,
0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,
0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,0x3e,0x5b,0xa5,
0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,
0x0c,0xef,0xbc,0x72,0x75,0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,
0x08,0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,0xa6,0xbb,0xac,
0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46};


void printblock(BLOCK testblock){
	printf("%04x%04x%04x%04x", testblock.w[0], testblock.w[1], testblock.w[2], testblock.w[3]);
	if (debug) printf("\n");
	return;
	}

void printtext(BLOCK testblock){
	printf("%c%c%c%c%c%c%c%c", (testblock.w[0] >> 8), testblock.w[0], (testblock.w[1] >> 8), testblock.w[1], (testblock.w[2] >> 8), testblock.w[2], (testblock.w[3] >> 8), testblock.w[3]); 
	if (debug) printf("\n");
	return;
}

uint8_t keySchedulerE(int x){
	key = (key << 1) | (key >> (BLOCK_SIZE - 1));
	uint64_t tempkey = key;
	uint8_t k[8];
	for (int i = 7; i >= 0; i--){
		k[i] = tempkey & 0xFF;
		tempkey = (tempkey >> 8);
	}
	return k[x % 8];
}

void generateSubKeys(){
	for(int round = 0; round < NUM_ROUNDS; round++){
		for(int i = 0; i < 12; i++){
				subkey[round][i] = keySchedulerE(4 * round + (i% 4));
		}
	}
	return;
}

BLOCK whitening(BLOCK block){
	uint16_t k0 = (key >> (BLOCK_SIZE - 16));
	uint16_t k1 = (key >> (BLOCK_SIZE - 32));
	uint16_t k2 = (key >> (BLOCK_SIZE - 48));
	uint16_t k3 = key;
	BLOCK output;
	output.w[0] = block.w[0]^k0;
	output.w[1] = block.w[1]^k1;
	output.w[2] = block.w[2]^k2;
	output.w[3] = block.w[3]^k3;
	return output;
}

uint16_t G(uint16_t input, int round, int callno){ // on second call, callno = 4
	uint8_t g1, g2, g3, g4, g5, g6;
	g1 =  input >> 8;
	g2 = input;
	g3 = ftable[g2^subkey[round][0 + callno]]^g1;
	g4 = ftable[g3^subkey[round][1 + callno]]^g2;
	g5 = ftable[g4^subkey[round][2 + callno]]^g3;
	g6 = ftable[g5^subkey[round][3 + callno]]^g4;
	if (debug) printf("g1: %x g2: %x g3: %x g4: %x g5: %x g6: %x\n", g1, g2, g3, g4, g5, g6);
	return (uint16_t) (g5 << 8) + g6;
}

FPAIR F(uint16_t r0, uint16_t r1, int round){
	uint16_t t0, t1;
	FPAIR output;
	t0 = G(r0, round, 0);
	t1 = G(r1, round, 4);
	if (debug) printf("t0: %x\tt1: %x\n", t0, t1);
	output.f[0] = (t0 + 2 * t1 + (uint16_t) ((subkey[round][8] << 8) + subkey[round][9])) % 65536;
	output.f[1] = (t1 + 2 * t0 + ((subkey[round][10] << 8) + subkey[round][11])) % 65536;
	if (debug) printf("f0: %x\tf1: %x\n", output.f[0], output.f[1]);
	return output;
}

BLOCK encrypt(BLOCK block){
	BLOCK output;
	output = whitening(block);
	if (debug) printf("after whitening: ");
	if (debug) printblock(output);

	for(int round = 0; round < 16; round++){
		BLOCK temp = output;
		FPAIR f0f1 = F(temp.w[0], temp.w[1], round);
		output.w[0] = (((temp.w[2]^f0f1.f[0]) >> 1) ^ ((temp.w[2]^f0f1.f[0]) << 15));
		output.w[1] = ((temp.w[3] << 1) ^ (temp.w[3] >> 15))^f0f1.f[1];
		output.w[2] = temp.w[0];
		output.w[3] = temp.w[1];
		if (debug) printf("after round %i: ", round + 1);
		if (debug) if (debug) printblock(output);
		if (debug) printf("-----------------------\n");
	}
	BLOCK final;
	final.w[0] = output.w[2];
	final.w[1] = output.w[3];
	final.w[2] = output.w[0];
	final.w[3] = output.w[1];
	final = whitening(final);
	return final;
}

BLOCK decrypt(BLOCK block){
	BLOCK output;
	output = whitening(block);
	if (debug) printf("after whitening: ");
	if (debug) printblock(output);

	for(int round = 15; round >= 0; round--){
		BLOCK temp = output;
		FPAIR f0f1 = F(temp.w[0], temp.w[1], round);
		output.w[0] = ((temp.w[2] << 1) ^ (temp.w[2] >> 15)^f0f1.f[0]);
		output.w[1] = (((temp.w[3]^f0f1.f[1]) >> 1) ^ ((temp.w[3]^f0f1.f[1]) << 15));
		output.w[2] = temp.w[0];
		output.w[3] = temp.w[1];
		if (debug) printf("after round %i: ", 16 - round);
		if (debug) printblock(output);
		if (debug) printf("-----------------------\n");
	}
	BLOCK final;
	final.w[0] = output.w[2];
	final.w[1] = output.w[3];
	final.w[2] = output.w[0];
	final.w[3] = output.w[1];
	final = whitening(final);
	return final;
}

BLOCK textToBlock(char* input){
	BLOCK output;
	int j = 0;
	for(int i = 0; i < 8; i++){
		uint16_t temp = input[i] << 8;
		//fprintf(stderr, "textToBlock: input:%s\n", input);
		//fprintf(stderr, "textToBlock: iter:%i temp:%x inputshift:%x\n", i, temp, input[i] << 8);
		i++;
		output.w[j] = temp + input[i];
		j++;
	}
	return output;
}

BLOCK hexToBlock(uint64_t input){
	BLOCK output;
	output.w[0] = (input >> (BLOCK_SIZE - 16));
	output.w[1] = (input >> (BLOCK_SIZE - 32));
	output.w[2] = (input >> (BLOCK_SIZE - 48));
	output.w[3] = input;
		
	return output;
}

void wipetextBuffer(){
	for (int i = 0; i < 8; i++){
		textBuf[i] = '0';
	}
	return;
}

void wipekeyBuffer(){
	for (int i = 0; i < 16; i++){
		keystr[i] = '0';
	}
	return;
}

int main(int argc, char* argv[]){

	if ((argc < 3) && ((argv[3][0] == 'd') || (argv[3][0] == 'e'))){
		printf("Missing arguments. Please use \"./WSUcrypt <plaintext> <key> e\" for encrypt, and \"./WSUcrypt <cyphertext> <key> d\" for decrypt.\n");
		return -1;
	}
	char mode = argv[3][0];

	
	FILE *keytxt;
	keytxt = fopen(argv[2], "r");
	if (!keytxt){
			fprintf(stderr, "Error: key.txt not found.\n");
		return -1; 
		}
	fread(keystr, 16, 1, keytxt);
	fclose(keytxt);
	sscanf(keystr, "%" SCNx64, &key); 
	generateSubKeys();

	
	BLOCK nextblock;
	FILE *plaintext;
	plaintext = fopen(argv[1], "r");
	if (!plaintext){
		fprintf(stderr, "Error: plaintext.txt not found.\n");
		return -1; 
	}
	while(fscanf(plaintext, "qzxzx") != EOF){
		
		if (mode == 'e'){
			fread(textBuf, 8, 1, plaintext);
			//fprintf(stderr, "textBuf: %s\n", textBuf);
			nextblock = textToBlock(textBuf);
			printblock(encrypt(nextblock));
			wipetextBuffer();
		} else {

			fread(keystr, 16, 1, plaintext);
			sscanf(keystr, "%" SCNx64, &cyphBuf); 
			nextblock = hexToBlock(cyphBuf);
			printtext(decrypt(nextblock));
			wipekeyBuffer();
		}
	}

	fclose(plaintext);


	// ############## TEST PRINTS #############

	// ### PRINT SUBKEYS ###

	/*
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 11; j++){
			printf("%x  ", subkey[i][j]);
		}
		printf("%x\n", subkey[i][11]);
	}*/

	// ### TEST WHITEN ###

	/*
	BLOCK etext;
	etext.w[0] = 0x0123;
	etext.w[1] = 0x4567;
	etext.w[2] = 0x89ab;
	etext.w[3] = 0xcdef;

	BLOCK dtext;
	dtext.w[0] = 0x84f0;
	dtext.w[1] = 0xece4;
	dtext.w[2] = 0x2428;
	dtext.w[3] = 0x2f79;
    */



    //#### TEST ENCRYPT/DECRYPT AND PRINTING ###
    /*
	BLOCK dtext, etext;

	etext = textToBlock("bobisbob");

	printblock(etext);
	dtext = encrypt(etext);
	printf("encrypted: ");
	printblock(dtext);
	BLOCK final = decrypt(dtext);
	printblock(final);
	printf("\n\n");
	printtext(final);
	*/

	//printf("%s\n", keystr);
	//rintf("%" PRIu64 "\n", key);
	//printf("\n\n%i\n", keySchedulerE(0, 0));
	//printf("\n");
	return 0;
}