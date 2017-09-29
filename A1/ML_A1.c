#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 101
#define NUMATTRS 16

typedef struct hypothesis{
	int attrValues[NUMATTRS];
}hypothesis;

typedef struct hypothesisSet{
	hypothesis* next;
}hypothesisSet;

typedef struct trainingExample{
	int attrValues[NUMATTRS];
	int value;
	char name[20];
}trainingExample;

typedef struct trainingSet{
	trainingExample* examples;
}trainingSet;

/* Initialize Specific boundary of version space */
void initializeS()
{
	;
}

/* Initialize General boundary of version space */
void initializeG()
{
	;
}

/* Returns 1(true) if the example matches any of the hypothesis in G, otherwise 0(false) */
int isGMatch()
{
	return 0;
}

/* Returns true(1) if hypothesis and instance are consistent */
int isConsistent()
{
	return 0;
}

/* Returns 1(true) if h1 is more general than h2 */
int isMoreGeneral()
{
	return 0;
}

/* Returns 1(true) if h1 is more specific than h2 */
int isMoreSpecific()
{
	return 0;
}

/* Returns the unique minimal generalization, h, of s such that h matches
   the given positive instance and some member of G is more general (or
   equally general) than h. */
void getMinGeneralization()
{

}

/* Specialize just enough */
void getMinSpecialization()
{

}


int conductVoting()
{
    return 0;
}

void enumerateVersionSpace()
{
	;
}

/* Given a new example, classifies it into one of the output classes and returns the output class' value */
int classify()
{
	return 0;
}

/* Splits the training examples into 60:40 ratio of training and test set of data respectively */
void splitTrainingAndTestData(trainingSet allData, trainingSet *trainSet, trainingSet *testSet, int numExamples, int numTrainingExamples)
{
	int taken[numExamples], count = 0, idxTrain = 0, idxTest = 0;
	time_t t;
	(*trainSet).examples = (trainingExample*)malloc(sizeof(trainingExample)*numTrainingExamples);
	(*testSet).examples = (trainingExample*)malloc(sizeof(trainingExample)*(numExamples-numTrainingExamples));
	for(int i=0; i<numExamples; i++) taken[i] = 0;
	srand((unsigned) time(&t));
	do{
		int randomNum = rand() % numExamples;
		if(!taken[randomNum]){
			taken[randomNum] = 1;
			count++;
		}
	}while(count != numTrainingExamples);
	for(int i=0; i<numExamples; i++){
		if(taken[i]) (*trainSet).examples[idxTrain++] = allData.examples[i];
		else (*testSet).examples[idxTest++] = allData.examples[i];
	}
}

/* Extracts all the data from file and returns the set of training examples */
trainingSet extractData(int size)
{
	FILE* fp = fopen("zoo.data.txt", "r");
	char str[20], ch;
	int it, value, attrNum, flag, exampleIndex, count;
	trainingExample instance;
	trainingSet set;
	set.examples = (trainingExample*)malloc(sizeof(trainingExample)*size);
	if(fp == NULL){
		perror("Error in opening file\n");
		return set;
	}
	else{
		exampleIndex = it = flag = count = attrNum = 0;
		do{
			ch = fgetc(fp);
			if(ch == ',' || ch == '\n'){
				str[it] = '\0';
				if(!flag){
					instance.name[it] = '\0';
				}
				else{
					value = atoi(str);
					if(count < NUMATTRS)
						instance.attrValues[count++] = value;
					else
						instance.value = value;
				}
				it = 0;
			}
			else{
				str[it] = ch;
				if(!flag) instance.name[it] = ch;
				it++;
			}
			if(ch == '\n'){
				flag = 0;
				set.examples[exampleIndex++] = instance;
                count = 0;
			}
			if(!flag && ch == ',')flag = 1;
			if(feof(fp))break;
		}while(1);
	}
	return set;
}

void printSet(trainingSet set, int numTrainingExamples)
{
	for(int i=0; i<numTrainingExamples; i++){
		printf("%s: ", set.examples[i].name);
		for(int j=0; j<NUMATTRS; j++){
			printf("%d ", set.examples[i].attrValues[j]);
		}
		printf("value = %d\n", set.examples[i].value);
	}
}

int main()
{
	int numExamples, numTrainingExamples, numTestExamples;
	numExamples = SIZE;
	numTrainingExamples = (int)((double)numExamples*0.6);
	numTestExamples = numExamples - numTrainingExamples;
	trainingSet set = extractData(SIZE), trainSet, testSet;
	splitTrainingAndTestData(set, &trainSet, &testSet, numExamples, numTrainingExamples);
	printSet(trainSet, numTrainingExamples);
	return 0;
}
