/*
	BITS F464 - MACHINE LEARNING
	(Assignment 1: ML_A1.c)

	Task: Implementation of Candidate Elimination Algorithm

	@authors:
		 Ankit Anand (2015A7PS0145H)
		 Raunak Ritesh (2015A7PS0160H)
		 Samkit Jain (2015A7PS0102H)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* CONSTANTS */
#define SIZE 101
#define NUMATTRS 16
#define NONE -1
#define MAX 1000
#define ANY -2
#define NUM_TYPES 7

/* STRUCTURE DEFINITIONS */
typedef struct hypothesis{
	int attrValues[NUMATTRS];
}hypothesis;

typedef struct hypothesisSet{
	hypothesis* hyp;
	int size;
}hypothesisSet;

typedef struct trainingExample{
	int attrValues[NUMATTRS];
	int value;
	char name[20];
}trainingExample;

typedef struct trainingSet{
	trainingExample* examples;
}trainingSet;

typedef struct versionSpace{
	hypothesisSet G, S;
}versionSpace;

/**
	Initializes hypothesis with most Specific boundary of version space
	
	@return the most specific hypothesis (contains all NONE values)
*/
hypothesis initializeS()
{
	hypothesis S;
	for(int i=0; i<NUMATTRS; i++){
		S.attrValues[i] = NONE;
	}
	return S;
}

/** 
	Initialize hypothesis with most General boundary of version space 
	
	@return the most specific hypothesis (contains all NONE values)
*/
hypothesis initializeG()
{
	hypothesis G;
	for(int i=0; i<NUMATTRS; i++){
		G.attrValues[i] = ANY;
	}
	return G;
}

/** 
	Checks if a hypothesis and an instance are consistent with each other

	@param h: given hypothesis
		   te: given training Example
		   posValue: value of the positive class considered in one v/s all technique
	
	@return 1 if consistent, 0 otherwise
*/
int isConsistent(hypothesis h, trainingExample te, int posValue)
{
	for(int i=0; i<NUMATTRS; i++){
		if(h.attrValues[i] == NONE){
			if(te.value != posValue)
				return 1;
			else
				return 0;
		}
		else if(h.attrValues[i] == ANY)
			continue;
		else if(h.attrValues[i] != te.attrValues[i]){
			if(te.value != posValue)
				return 1;
			else
				return 0;
		}
	}
	if(te.value == posValue)
		return 1;
	else
		return 0;
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
int classify(versionSpace vs[], trainingExample te)
{

	return 0;
}

/**
	Splits the training examples into 80:20 ratio of training and test set of data respectively

	@param allData: consists of all the provided training examples
		   trainingSet: pointer to the trainSet to be filtered
		   testSet: pointer to the testSet to be filtered
		   numExamples: total number of training examples
		   numTrainingExamples: number of examples included in training set
*/
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

/**
	Extracts all the data from file and returns the set of training examples
	
	@param size: size of input training examples
	@return a training set of all input examples
*/
trainingSet extractData(int size)
{
	FILE* fp = fopen("zoo-data.txt", "r");
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

/**
	Prints all the training examples in the training set

	@param set: the training set to be printed
		   numTrainingExamples: number of examples in training set
*/
void printTrainingSet(trainingSet set, int numTrainingExamples)
{
	for(int i=0; i<numTrainingExamples; i++){
		printf("%s: ", set.examples[i].name);
		for(int j=0; j<NUMATTRS; j++){
			printf("%d ", set.examples[i].attrValues[j]);
		}
		printf("value = %d\n", set.examples[i].value);
	}
}

/**
	Prints a hypothesis

	@param: hypothesis to be printed
*/
void printHypothesis(hypothesis h)
{
	printf("hypothesis: <");
	for(int i=0; i<NUMATTRS; i++){
		if(h.attrValues[i] == NONE)
			printf("ϕ,");
		else if(h.attrValues[i] == ANY)
			printf("?,");
		else
			printf("%d,",h.attrValues[i]);
	}
	printf(">");
}

/**
	Check if two hypotheses are equal

	@param h1: the first hypothesis
		   h2: the second hypothesis
	@return 1 if both are equal, 0 otherwise
*/
int equal(hypothesis h1, hypothesis h2)
{
	for(int i=0; i<NUMATTRS; i++){
		if(h1.attrValues[i] != h2.attrValues[i])
			return 0;
	}
	return 1;
}

/**
	Check if h is part of a hypothesisSet

	@param hs: the hypothesis set to be used
		   h: the hypothesis to be searched
	@return 1 if h if found in the set, 0 otherwise
*/
int find(hypothesisSet hs, hypothesis h)
{
	for(int i=0; i<hs.size; i++){
		if(equal(hs.hyp[i], h)){
			return i;
		}
	}
	return -1;
}

/**
	Prints all the hypotheses in the hypothesis set
	
	@param hs: the hypothesis set to be printed
*/
void printHypothesisSet(hypothesisSet hs)
{
	printf("SIZE = %d\n", hs.size);
	for(int i=0; i<hs.size; i++){
		printf("\t\t");
		printHypothesis(hs.hyp[i]);
		printf("\n");
	}
}

/**
	Prints a training example
	
	@param te: the training example to be printed
*/
void printTrainingExample(trainingExample te) {
	printf("TE: <");
	for(int i=0; i<NUMATTRS; i++){
		printf("%d,",te.attrValues[i]);
	}
	printf(">  value: %d\n",te.value);
}

/**
	Adds a hypothesis to a hypothesis Set if not already present
	
	@param hs: the pointer to the set of the hypotheses
		   h: the hypothesis to be added the set
*/
void addToHypothesisSet(hypothesisSet *hs,hypothesis h){
    for(int i=0;i<(*hs).size;i++){
        if(equal((*hs).hyp[i], h))
            return;
    }
    (*hs).hyp[(*hs).size] = h;
    (*hs).size++;
}

/** Removes a hypothesis from a hypothesis set 

	@param hs: the pointer to the set of the hypotheses
		   j: the index of the hypothesis in the given set	
*/
void removeFromHypothesisSet(hypothesisSet *hs, int j){
    (*hs).hyp[j]=(*hs).hyp[(*hs).size-1];
    (*hs).size--;
}

/**
	Checks if the first hypothesis is more general than the other

	@param h1: the first hypothesis
		   h2: the second hypothesis
	@return 1 is h1 is more general, 0 otherwise
*/
int isMoreGeneral(hypothesis h1, hypothesis h2)
{
	int more = 1;
	for(int i=0; i<NUMATTRS; i++){
		if(h1.attrValues[i] != h2.attrValues[i]){
			if(h2.attrValues[i] == NONE)
				continue;
			else if(h1.attrValues[i] != ANY)
				more = 0;
		}
	}
	return more;
}

/**
	Checks if the first hypothesis is more specific than the other
	
	@param h1: the first hypothesis
		   h2: the second hypothesis
	@return 1 is h1 is more specific, 0 otherwise
*/
int isMoreSpecific(hypothesis h1, hypothesis h2)
{
	int more = 1;
	for(int i=0; i<NUMATTRS; i++){
		if(h1.attrValues[i] != h2.attrValues[i]){
			if(i == 12){
				if(h2.attrValues[i] != ANY && h1.attrValues[i] != NONE)
						more = 0;
			}
			else if(h2.attrValues[i] == ANY || h1.attrValues[i] == NONE)
					continue;
			else
				more = 0;
		}
	}
	return more;
}

/**
	Checks if there is a more general hypothesis present in the hypothesis set

	@param hs: pointer to the set of hypotheses
		   h: the hypothesis to be checked
	@return 1 is there exists a more general hypothesis, 0 otherwise
*/
int checkForMoreGeneral(hypothesisSet *hs, hypothesis h)
{
	for(int i=0; i<(*hs).size; i++){
		if(equal((*hs).hyp[i], h))
			continue;
		if(isMoreGeneral((*hs).hyp[i], h))
			return 1;
	}
	return 0;
}

/**
	Checks if there is a more specific hypothesis present in the hypothesis set

	@param hs: pointer to the set of hypotheses
		   h: the hypothesis to be checked
	@return 1 is there exists a more specific hypothesis, 0 otherwise
*/
int checkForMoreSpecific(hypothesisSet* hs, hypothesis h)
{
	for(int i=0; i<(*hs).size; i++){
		if(equal((*hs).hyp[i], h))
			continue;
		if(isMoreSpecific((*hs).hyp[i], h))
			return 1;
	}
	return 0;
}

/**
	Returns the unique minimal generalization, h, of s such that h matches
    the given positive instance and some member of G is more general (or
    equally general) than h.

	@param S: pointer to the specific boundary of hypotheses
		   G: pointer to the general boundary of hypotheses
		   h: current hypothesis
		   te: current instance of training data set
*/
void getMinGeneralization(hypothesisSet *S, hypothesisSet* G, hypothesis h, trainingExample te)
{
	for(int i=0; i<NUMATTRS; i++){
		if(h.attrValues[i] == NONE)
			h.attrValues[i] = te.attrValues[i];
		else if(te.attrValues[i] == NONE || h.attrValues[i] == ANY)
			continue;
		else{
			if(h.attrValues[i] != te.attrValues[i])
				h.attrValues[i] = ANY;
		}
	}
	if(checkForMoreGeneral(G, h)){
		addToHypothesisSet(S,h);
	}
}

/**
	Specializes just enough
	
	@param G: pointer to the general boundary of hypotheses
		   S: pointer to the specific boundary of hypotheses
		   h: current hypothesis
		   te: current instance of training data set
*/
void getMinSpecialization(hypothesisSet* G, hypothesisSet* S, hypothesis h, trainingExample te)
{
	int values[] = {0,2,4,5,6,8};
	for(int i=0; i<NUMATTRS; i++){
		if(h.attrValues[i] == NONE)
			continue;
		else if(i != 12){
			if(h.attrValues[i] == ANY){
				h.attrValues[i] = 1-te.attrValues[i];
				
				if(checkForMoreSpecific(S, h)){
					addToHypothesisSet(G,h);
				}
				h.attrValues[i] = ANY;
			}
			else if(h.attrValues[i] == te.attrValues[i]){
				h.attrValues[i] = NONE;
				if(checkForMoreSpecific(S, h)){
					addToHypothesisSet(G,h);
				}
				h.attrValues[i] = te.attrValues[i];
			}
		}
		else if(i == 12){
			if(h.attrValues[i] == ANY){
				for(int j=0; j<6; j++){
					if(te.attrValues[i] != values[j]){
						h.attrValues[i] = values[j];
						if(checkForMoreSpecific(S, h)){
							addToHypothesisSet(G,h);
						}
						h.attrValues[i] = ANY;
					}
				}
			}
			else if(h.attrValues[i] == te.attrValues[i]){
				h.attrValues[i] = NONE;
				if(checkForMoreSpecific(S, h)){
					addToHypothesisSet(G,h);
				}
				h.attrValues[i] = te.attrValues[i];
			}
		}
	}
}

/**
	The main algorithm for calculating Version Space using Candidate Elimination
	
	@param ts: the training set of examples
		   numTrainingExamples: number of training examples
		   posValue: the class for which version space is to be determined
	@return the version space for the current class
*/
versionSpace solve(trainingSet ts, int numTrainingExamples, int posValue)
{
	versionSpace vs;
	hypothesisSet G, S;
	G.hyp = (hypothesis*)malloc(sizeof(hypothesis)*MAX);
	S.hyp = (hypothesis*)malloc(sizeof(hypothesis)*MAX);
	G.hyp[0] = initializeG();
	S.hyp[0] = initializeS();
	G.size = S.size = 1;

	for(int i=0; i<numTrainingExamples; i++){
		if(ts.examples[i].value == posValue){
			for(int j=0; j<G.size; j++){
				if(!isConsistent(G.hyp[j], ts.examples[i], posValue)){
					G.hyp[j] = G.hyp[G.size-1];
					G.size -= 1;
					j--;
				}
			}
			for(int j=0; j<S.size; j++){
				if(!isConsistent(S.hyp[j], ts.examples[i], posValue)){
					hypothesis htemp = S.hyp[j];
					S.hyp[j] = S.hyp[S.size-1];
					S.size -= 1;
					j--;
					getMinGeneralization(&S, &G, htemp, ts.examples[i]);
					for(int k=0; k<S.size; k++){
						if(checkForMoreSpecific(&S,S.hyp[k])){
							S.hyp[k] = S.hyp[S.size-1];
							S.size -= 1;
							k--;
						}
					}
				}
			}
		}
		else{
			for(int j=0; j<S.size; j++){
				if(!isConsistent(S.hyp[j], ts.examples[i], posValue)){
					S.hyp[j] = S.hyp[S.size-1];
					S.size -= 1;
					j--;
				}
			}
			for(int j=0; j<G.size; j++){
				//printf("here j = %d\n",j);
				if(checkForMoreGeneral(&G,G.hyp[j])){
					G.hyp[j] = G.hyp[G.size-1];
					G.size -= 1;
					j--;
					continue;
				}
				if(!isConsistent(G.hyp[j], ts.examples[i], posValue)){
					hypothesis htemp = G.hyp[j];
					removeFromHypothesisSet(&G,j);
					j--;
					getMinSpecialization(&G, &S, htemp, ts.examples[i]);
				}
			}
		}
	}
	vs.G = G;
	vs.S = S;
	return vs;
}

int main()
{
	int numExamples, numTrainingExamples, numTestExamples;
	numExamples = SIZE;
	numTrainingExamples = (int)((double)numExamples);
	numTestExamples = numExamples - numTrainingExamples;
	trainingSet set = extractData(SIZE);
	trainingSet trainSet = set, testSet;
	versionSpace vs[NUM_TYPES];
	for(int i=1; i<=7; i++){
		vs[i-1] = solve(trainSet, numTrainingExamples, i);
		printf("CLASS %d: \n",i);
		if(vs[i-1].S.size == 0 || vs[i-1].G.size == 0){
			printf("Concept cannot be learnt\n\n");
			continue;
		}
		printf("\tSpecific Boundary\n\t\t");
		printHypothesisSet(vs[i-1].S);
		printf("\tGeneral Boundary\n\t\t");
		printHypothesisSet(vs[i-1].G);
		printf("\n");
	}
	return 0;
}
