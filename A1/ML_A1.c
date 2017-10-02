#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 101
#define NUMATTRS 16
#define NONE -1
#define ANY -2
#define NUM_TYPES 7

typedef struct hypothesis{
	int attrValues[NUMATTRS];
}hypothesis;

typedef struct hypothesisSet{
	hypothesis hyp[100];
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

/* Initialize Specific boundary of version space */
hypothesis initializeS()
{
	hypothesis S;
	for(int i=0; i<NUMATTRS; i++){
		S.attrValues[i] = NONE;
	}
	return S;
}

/* Initialize General boundary of version space */
hypothesis initializeG()
{
	hypothesis G;
	for(int i=0; i<NUMATTRS; i++){
		G.attrValues[i] = ANY;
	}
	return G;
}

/* Returns true(1) if hypothesis and instance are consistent */
int isConsistent(hypothesis h, trainingExample te)
{
	for(int i=0; i<NUMATTRS; i++)
		if(h.attrValues[i] == NONE || h.attrValues[i] != ANY && h.attrValues[i] != te.attrValues[i])
	 		return 0;
	return 1;
}

/* Returns 1(true) if the example matches any of the hypothesis in G, otherwise 0(false) */
int isGMatch(hypothesisSet hypSet, trainingExample te)
{
	for(int i=0; i<hypSet.size; i++)
		if(isConsistent(hypSet.hyp[i], te))
			return 1;
	return 0;
}

/* Returns 1(true) if h1 is more general than h2 */
int isMoreGeneral(hypothesis h1, hypothesis h2)
{
	for(int i=0; i<NUMATTRS; i++){
		if(h1.attrValues[i] == NONE)
			continue;
		if(h1.attrValues[i] != h2.attrValues[i] && h1.attrValues[i] != ANY)
			return 0;
	}
	return 1;
}

/* Returns 1(true) if h1 is more specific than h2 */
int isMoreSpecific(hypothesis h1, hypothesis h2)
{
	for(int i=0; i<NUMATTRS; i++){
		if(h1.attrValues[i] == NONE)
			continue;
		if(h1.attrValues[i] != h2.attrValues[i] && h2.attrValues[i] != ANY)
			return 0;
	}
	return 1;
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

hypothesisSet removeNonMatching(hypothesisSet hs, trainingExample te)
{
	hypothesisSet hnew = hs;
	int j = 0;
	for(int i=0; i<hs.size; i++){
		if(isConsistent(hs.hyp[i], te)){
			hnew.hyp[j] = hs.hyp[i];
			j++;
		}
	}
	hnew.size = j;
	return hnew;
}

hypothesisSet removeMatching(hypothesisSet hs, trainingExample te)
{
	hypothesisSet hnew = hs;
	int j = 0;
	for(int i=0; i<hs.size; i++){
		if(!isConsistent(hs.hyp[i], te)){
			hnew.hyp[j] = hs.hyp[i];
			j++;
		}
	}
	hnew.size = j;
	return hnew;
}

int attrMatch(int attr1, int attr2)
{
	int match = 1;
	if(attr1 != attr2)
		if(attr1 != ANY && attr2 != ANY)
			match = 0;
	return match;
}

/* Returns the unique minimal generalization, h, of s such that h matches
   the given positive instance and some member of G is more general (or
   equally general) than h. */
hypothesis getMinGeneralization(hypothesis h, trainingExample te)
{
	for(int i=0; i<NUMATTRS; i++){
		if(h.attrValues[i] != te.attrValues[i] && !attrMatch(h.attrValues[i], te.attrValues[i])){
			if(h.attrValues[i] == NONE){
				h.attrValues[i] = te.attrValues[i];
			}
			else{
				h.attrValues[i] = ANY;
			}
		}
	}
	return h;
}

void printHypothesis(hypothesis h)
{
	printf("hypothesis: <");
	for(int i=0; i<NUMATTRS; i++){
		printf("%d,",h.attrValues[i]);
	}
	printf(">");
}

/* Specialize just enough */
hypothesisSet getMinSpecialization(hypothesis h, trainingExample te)
{
	hypothesisSet specializations;
	specializations.size = 0;
	int values[] = {0,2,4,5,6,8};
	for(int i=0; i<NUMATTRS; i++){
		if(h.attrValues[i] == ANY){
			hypothesis hnew;
			if(i == 12){
				for(int j=0; j<6; j++){
					if(te.attrValues[i] == values[j]){
						for(int k=0; k<6; k++){
							if(j != k){
								hnew = h;
								hnew.attrValues[i] = values[k];
								specializations.hyp[specializations.size] = hnew;
								specializations.size += 1;
							}
						}
						break;
					}
				}
			}
			else{
				hnew = h;
				if(te.attrValues[i] == 0){
					hnew.attrValues[i] = 1;
				}
				else{
					hnew.attrValues[i] = 0;
				}
				specializations.hyp[specializations.size] = hnew;
				specializations.size += 1;
				//printf("i = %d size = %d\n",i,specializations.size);
			}
		}
	}
	//printf("size = %d\n",specializations.size);
	return specializations;
}

int processGeneralization(hypothesis h, hypothesisSet G)
{
	if(G.size == 0)
		return 1;
	for(int i=0; i<G.size; i++){
		if(isMoreSpecific(h, G.hyp[i]))
			return 1;
	}
	return 0;
}

hypothesisSet processSpecializations(hypothesisSet specializations, hypothesisSet S)
{
	hypothesisSet validSpecializations;
	hypothesis h;
	for(int i=0; i<NUMATTRS; i++)h.attrValues[i] = NONE;
	for(int i=0; i<S.size; i++){
		for(int j=0; j<specializations.size; j++){
			if(isMoreGeneral(specializations.hyp[j], S.hyp[i])){
				validSpecializations.hyp[validSpecializations.size] = specializations.hyp[j];
				validSpecializations.size += 1;
			}
			//else if(S[i] == initializeS())validSpecializations.append(specializations[j])
		}
	}
	return validSpecializations;
}

/* Check if two hypotheses are equal */
int equal(hypothesis h1, hypothesis h2)
{
	for(int i=0; i<NUMATTRS; i++){
		if(h1.attrValues[i] != h2.attrValues[i])
			return 0;
	}
	return 1;
}

/* Check if h is part of a hypothesisSet */
int find(hypothesisSet hs, hypothesis h)
{
	for(int i=0; i<hs.size; i++){
		if(equal(hs.hyp[i], h)){
			return i;
		}
	}
	return -1;
}

/* Remove from G any h that is more specific than any other hypothesis in G */
hypothesisSet removeMoreSpecific(hypothesisSet G)
{
	hypothesisSet hnew;
	hnew.size = 0;
	for(int i=0; i<G.size; i++){
		for(int j=0; j<G.size; j++){
			if(i != j && isMoreSpecific(G.hyp[i], G.hyp[j])){
				int idx = find(hnew, G.hyp[i]);
				if(idx != -1){
					hnew.hyp[idx] = hnew.hyp[hnew.size-1];
					hnew.size -= 1;
				}
			}
		}
	}
	return hnew;
}

hypothesisSet add(hypothesisSet hs1, hypothesisSet hs2)
{
	for(int i=0; i<hs2.size; i++){
		if(find(hs1, hs2.hyp[i]) == -1){
			hs1.hyp[hs1.size] = hs2.hyp[i];
			hs1.size += 1;
		}
	}
	return hs1;
}

void printHypothesisSet(hypothesisSet hs)
{
	printf("SIZE = %d\n", hs.size);
	for(int i=0; i<hs.size; i++){
		printf("<");
		for(int j=0; j<NUMATTRS; j++){
			printf("%d,",hs.hyp[i].attrValues[j]);
		}
		printf(">\n");
	}
}

void  printTrainingExample(trainingExample te) {
	printf("TE: <");
	for(int i=0; i<NUMATTRS; i++){
		printf("%d,",te.attrValues[i]);
	}
	printf(">    value: %d\n",te.value);
}

/* The main algorith for calculating Version Space using Candidate Elimination */
versionSpace solve(trainingSet ts, int numTrainingExamples, int posValue)
{
	versionSpace vs;
	hypothesisSet G, S;
	G.hyp[0] = initializeG();
	S.hyp[0] = initializeS();
	G.size = S.size = 1;

	printHypothesisSet(S);
	printHypothesisSet(G);
	printf("\n");

	for(int i=0; i<numTrainingExamples; i++){
		if(ts.examples[i].value == posValue){
			printf("POSITIVE ");
			printTrainingExample(ts.examples[i]);
			G = removeNonMatching(G, ts.examples[i]);
			hypothesisSet Snew = S;
			for(int j=0; j<S.size; j++){
				if(!isConsistent(S.hyp[j], ts.examples[i])){
					Snew.hyp[j] = Snew.hyp[Snew.size-1];
					Snew.size -= 1;
					hypothesis general = getMinGeneralization(S.hyp[j], ts.examples[i]);
					if(processGeneralization(general, G)){
						Snew.hyp[Snew.size] = general;
						Snew.size += 1;
					}
				}
			}
			S = Snew;
			//S = removeMoreGeneral(S);
			printHypothesisSet(S);
			printHypothesisSet(G);
		}
		else{
			printf("NEGATIVE ");
			printTrainingExample(ts.examples[i]);
			S = removeNonMatching(S, ts.examples[i]);
			hypothesisSet Gnew = G;
			for(int j=0; j<G.size; j++){
				if(!isConsistent(G.hyp[j], ts.examples[i])){
					Gnew.hyp[j] = Gnew.hyp[Gnew.size-1];
					Gnew.size -= 1;
				}
				hypothesisSet specializations = getMinSpecialization(G.hyp[j], ts.examples[i]);
				printf("specializations: ");
				printf("%d => ",specializations.size);
				printHypothesisSet(specializations);
				specializations = processSpecializations(specializations, S);
				Gnew = add(Gnew, specializations);
			}
			G = Gnew;
			G = removeMoreSpecific(G);
			printHypothesisSet(S);
			printHypothesisSet(G);
		}
		if(S.size == G.size){
			int flag = 1;
			for(int i=0; i<S.size; i++){
				if(find(G, S.hyp[i]) == -1){
					flag = 0;
					break;
				}
			}
			if(flag)
				break;
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
	numTrainingExamples = (int)((double)numExamples*0.8);
	numTestExamples = numExamples - numTrainingExamples;
	trainingSet set = extractData(SIZE), trainSet, testSet;
	splitTrainingAndTestData(set, &trainSet, &testSet, numExamples, numTrainingExamples);
	printTrainingSet(trainSet, numTrainingExamples);
	versionSpace vs[NUM_TYPES];
	for(int i=3; i<=3; i++){
		vs[i-1] = solve(trainSet, numTrainingExamples, i);
		printf("Specific Boundary of %d\n",i);
		printHypothesisSet(vs[i-1].S);
		printf("General Boundary of %d\n",i);
		printHypothesisSet(vs[i-1].G);
		printf("\n");
	}
	return 0;
}
