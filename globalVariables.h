#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

//------------------------------------------

#define INPUT_NEURONS		16
#define HIDDEN_NEURONS		100
#define OUTPUT_NEURONS		26 // a-z

#define RELU                0
#define TANH                1
#define SIGMOID             2

//------------------------------------------

enum Symbol{LETTER_A=0,  LETTER_B=1,  LETTER_C=2,  LETTER_D=3,
            LETTER_E=4,  LETTER_F=5,  LETTER_G=6,  LETTER_H=7,
            LETTER_I=8,  LETTER_J=9,  LETTER_K=10, LETTER_L=11,
            LETTER_M=12, LETTER_N=13, LETTER_O=14, LETTER_P=15,
            LETTER_Q=16, LETTER_R=17, LETTER_S=18, LETTER_T=19,
            LETTER_U=20, LETTER_V=21, LETTER_W=22, LETTER_X=23,
            LETTER_Y=24, LETTER_Z=25}; //, UNKNOWN=27};

const char alpha_list[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

struct LetterStructure{
    Symbol symbol;
    int outputs[OUTPUT_NEURONS];
    int f[INPUT_NEURONS];
};

extern LetterStructure letters[20001];
extern LetterStructure testPattern;
extern LetterStructure test4000[4001];

//extern int NUMBER_OF_PATTERNS;
const int NUMBER_OF_PATTERNS = 20000;
const int NUMBER_OF_TRAINING_PATTERNS = 16000;
const int NUMBER_OF_TEST_PATTERNS = 4000;
extern bool patternsLoadedFromFile;
extern int MAX_EPOCHS;
extern double LEARNING_RATE;

extern int activation_function;
extern int isNormalise;
extern int STOP_RATE;
extern double MSE;
extern double PGC;
extern double err2;

#endif // GLOBALVARIABLES_H
