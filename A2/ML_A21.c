#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define NUMATTRS 15
#define LASTIDX 14
#define SIZE 100
#define MAX1 100000
#define POSLEAF 1
#define NEGLEAF -1
#define INTERNALNODE 0
#define LOW -100

char hash[20][SIZE][SIZE]={0};
int map[SIZE][SIZE]={0}, M[SIZE]={0}, totalNodes=0;

typedef struct example{
	int attrValues[NUMATTRS];
}example;

typedef struct setOfExamples{
	int positive, negative;
	int size;
	example *eset;
}setOfExamples;

typedef struct DTNode{
	int visited[NUMATTRS];
	int currentAttribute, nodeType;
	setOfExamples eSet;
    struct DTNode* children;
}DTNode;

/* finds value corresponding to the training instance */
int findValue(example instance)
{
	return (instance.attrValues[LASTIDX] > 0)?instance.attrValues[LASTIDX]:-1;
}

/* Inserts an example into the set of examples */
void insertIntoSet(setOfExamples *se, example instance)
{
	(*se).eset[(*se).size] = instance;
	(*se).size += 1;
	if(findValue(instance) == 1) (*se).positive += 1;
	else (*se).negative += 1;
}

/* Compares equality of two strings */
int checkEquality(char* str1, char* str2)
{
	return (strcmp(str1,str2) == 0);
}

int parseInt(char* num, int length)
{
	return atoi(num);
}

/* Converts a string to an integer */
int stringtoint(char *brr,int l){
    int i,a=0,f=-1;
    for(i=0;i<l;i++){
        if(brr[i]>='0'&&brr[i]<='9'){
            a=a*10+(brr[i]-'0');
            f=1;
        }
    }
    if(f<0){
        return f;
    }
    return a;
}

/* conversion */
void convert(int a, char *brr,int l,example *ex1){
    a++;
    char temp1[2] = {0};
    char temp2[1] = {0};
        temp1[0]=' ';
        temp1[1]='?';
        temp2[0]='?';
    if(checkEquality(brr,temp1)==1 || checkEquality(brr,temp2)==1){
        (*ex1).attrValues[a-1]=1;
        return;
    }
    if(a==15){
        if(brr[1]=='<'){
            (*ex1).attrValues[a-1]=0;
        }
        else (*ex1).attrValues[a-1]=1;
        return;
    }
    if(a!=2 && a!=4 && a!=6 && a!=7 && a!=8 && a!=9 && a!=10 && a!=14 && a!=15){
        if(1){
            (*ex1).attrValues[a-1]=stringtoint(brr,l);
            M[a-1]=3;
        }
        return ;
    }
    a--;
    int i,n,j,flag;
    n=0;
    for(i=0;i<SIZE;i++){
        if(map[a][i]!=-1){
            n++;
        }
    }
    flag=0;
    for(i=0;i<n;i++){
        if(checkEquality(brr,hash[a][i])==1){
            (*ex1).attrValues[a]=map[a][i];
            flag=1;
            break;
        }
    }
    if(flag==0){
        for(j=0;j<l;j++){
            hash[a][n][j]=brr[j];
        }
        hash[a][n][j]=0;
        map[a][n]=n+1;
        M[a]=n+2;
        (*ex1).attrValues[a]=map[a][n];
    }
}

/* converts string to an example */
void examplify(char *string, example* instance)
{
	char part[1000] = {0};
	int length = strlen(string), num = 0, strIdx = 0;
	for(int i=0; i<length; i++){
		if(string[i] == ','){
			part[strIdx] = '\0';
			convert(num, part, strIdx, instance);
			num += 1;
			strIdx = 0;
		}
		else
			part[strIdx++] = string[i];
	}
	part[strIdx] = '\0';
	convert(num, part, strIdx, instance);
	num += 1;
	//instance -> value = findValue(*instance);
}

/* relative sorting */
void sort(int *arr,int *brr,int low,int high){
    int key=arr[low],i,temp;
    int a=low,b=high;
    if(low>=high){
        return;
    }
    for(i=low;i<=high;i++){
        if(arr[i]<key){
            temp=arr[i];
            arr[i]=arr[low];
            arr[low]=temp;
            temp=brr[i];
            brr[i]=brr[low];
            brr[low]=temp;
            low++;
        }
        else{
            temp=arr[i];
            arr[i]=arr[high];
            arr[high]=temp;
            temp=brr[i];
            brr[i]=brr[high];
            brr[high]=temp;
            high--;
        }
    }
    sort(arr,brr,a,low-1);
    sort(arr,brr,high+1,b);
}

/* calculates entropy */
double calculateEntropy(int pos, int neg)
{
	double val1 = (double)pos/(double)(pos+neg);
	double val2 = (double)neg/(double)(pos+neg);
	double entropy = val1*log(val1) + val2*log(val2);
	if(entropy < 0) entropy *= -1;
	return entropy;
}

/* finds threshold */
int findTS(setOfExamples *se, int attrIdx)
{
	int result, sz = (*se).size, x1, x2, y1, y2;
	int a[sz], b[sz];
	double gain = 0, maximum = MAX1;
	for(int i=0; i<sz; i++){
		a[i] = (*se).eset[i].attrValues[attrIdx];
		b[i] = findValue((*se).eset[i]);
	}
	sort(a,b,0,sz-1);
	x1 = y1 = result = 0;
	x2 = (*se).negative;
	y2 = (*se).positive;
	for(int i=0; i<sz-1; i++){
		if(b[i] == 1){y1++;y2--;}
		else{x1++; x2--;}
		i += 1;
		gain = (((double)i/(double)sz)*calculateEntropy(x1,y1))
			+(((double)(sz-i)/(double)sz)*calculateEntropy(x2,y2));
		if(gain < 0) gain *= -1;
		if(gain < maximum){
			maximum = gain;
			result = a[i-1];
		}
		i -= 1;
	}
	return result;
}

/* function to calculate information gain */
double calculateIG(setOfExamples se, int attrIdx)
{
	double incr, splitInfoIncr, result, gain, splitInfo, temp;
	gain = splitInfo = temp = 0;
	result = calculateEntropy(se.positive, se.negative);
	int pos[SIZE]={0}, neg[SIZE]={0};
	for(int i=0; i<se.size; i++){
		if(findValue(se.eset[i]) > 0) pos[se.eset[i].attrValues[attrIdx]] += 1;
		else neg[se.eset[i].attrValues[attrIdx]] += 1;
	}
	for(int i=1; i<M[attrIdx]; i++){
		double sum = pos[i] + neg[i];
		temp = ((double)sum)/(double)se.size;
		if(sum == 0) continue;
		incr = temp * calculateEntropy(pos[i], neg[i]);
		if(incr < 0) incr *= -1;
		gain += incr;
		splitInfoIncr = temp * log(temp);
		if(splitInfoIncr < 0) splitInfoIncr *= -1;
		splitInfo += splitInfoIncr;
	}
	if(result < 0) result *= -1;
	if(gain < 0) gain *= -1;
	gain = result - gain;
	gain /= splitInfo;
	if(gain < 0) gain *= -1;
	return gain;
}

/* builds the decision tree recursively */
void buildDT(DTNode *curr){
    if((*curr).eSet.size==(*curr).eSet.positive){
        (*curr).nodeType = 1;
        return;
    }
    if((*curr).eSet.size==(*curr).eSet.negative){
        (*curr).nodeType = -1;
        return;
    }
    double ent=0,ig=0,max=-100;
    int i,j,att;
    att=-1;
    for(i=0;i<14;i++){
        if((*curr).visited[i]==0){
            ig=calculateIG((*curr).eSet,i);
            if(ig>max){
                max=ig;
                att=i;
            }
        }
    }
    if(att==-1){
        (*curr).nodeType = 1;
        if((*curr).eSet.positive<(*curr).eSet.negative){
            (*curr).nodeType = -1;
        }
        return;
    }
    (*curr).currentAttribute = att;
    (*curr).children = (DTNode*)malloc(M[att]*sizeof(DTNode));
    totalNodes=totalNodes+M[att];
    for(i=0;i<M[att];i++){
        (*curr).children[i].eSet.size =0;
        (*curr).children[i].eSet.positive =0;
        (*curr).children[i].eSet.negative =0;
        (*curr).children[i].eSet.eset = (example *)malloc(((*curr).eSet.size)*sizeof(example));
        (*curr).children[i].currentAttribute = -1;
        (*curr).children[i].nodeType = 0;
        for(j=0;j<14;j++){
            (*curr).children[i].visited[j]=(*curr).visited[j];
        }
        (*curr).children[i].visited[(*curr).currentAttribute]=1;
    }
    for(i=0;i<(*curr).eSet.size;i++){
        insertIntoSet(
            &((*curr).children[(*curr).eSet.eset[i].attrValues[(*curr).currentAttribute]].eSet),(*curr).eSet.eset[i]
        );
    }
    for(i=0;i<M[att];i++)
        buildDT(&((*curr).children[i]));
}

/* get decision tree node's type */
int getDTNodeType(example instance, DTNode* node)
{
	if((*node).nodeType != INTERNALNODE)
		return (*node).nodeType;
	else
		return getDTNodeType(instance, &((*node).children[(instance).attrValues[(*node).currentAttribute]]));
}

/* deletes and returns an example from the set of examples */
example deleteExample(setOfExamples* se)
{
	(*se).size -= 1;
	int sz = (*se).size;
	if(findValue((*se).eset[sz]) == 1)
		(*se).positive -= 1;
	else
		(*se).negative -= 1;
	return (*se).eset[sz];
}

/* calculates accuracy */
double calculateAccuracy(DTNode* node, setOfExamples* se)
{
	double right = 0, tot = (*se).size;
	for(int i=0; i<(*se).size; i++){
		int type = getDTNodeType((*se).eset[i], node);
		if(type == findValue((*se).eset[i])) right += 1;
	}
	return right/tot;
}

/* applies pruning to the decision tree */
double prune(DTNode* node1, DTNode* node2, setOfExamples* se, double acc)
{
	int num = M[(*node2).currentAttribute];
	if((*node2).nodeType != 0) return acc;
	for(int i=0; i<num; i++) acc = prune(node1, &((*node2).children[i]), se, acc);
	if((*node2).eSet.positive >= (*node2).eSet.negative) (*node2).nodeType = POSLEAF;
	else (*node2).nodeType = NEGLEAF;
	double tmp = calculateAccuracy(node1, se);
	if(tmp >= acc){acc = tmp; return tmp;}
	else (*node2).nodeType = INTERNALNODE;
	return acc;
}

/* calculate number of nodes */
int numNodes(DTNode* node)
{
	if((*node).nodeType != INTERNALNODE) return 1;
	int num = M[(*node).currentAttribute];
	int result = 1;
	for(int i=0; i<num; i++) result += numNodes(&((*node).children[i]));
	return result;
}

/* initialize a set of examples */
void init(setOfExamples* se)
{
	(*se).eset = (example*)malloc(MAX1*sizeof(example));
	(*se).size = (*se).positive = (*se).negative = 0;
}

/* takes an input from command line: to be changed for file input */
void input(setOfExamples *se){
    int i,flag;
    flag=0;
    while(1){
        char arr[10000]={0};
        flag=scanf(" %[^\n]s",arr);
        if(flag==-1){
            break;
        }
        i++;
        example a;
        examplify(arr,&a);
        insertIntoSet(se,a);
    }
}

/* gets the Threshold */
void getTS(int *ts, setOfExamples* se)
{
	for(int i=1; i<=NUMATTRS; i++){
		if(i != 2 && i != 4 && i != 6 && i != 7 && i != 8 && i != 9 && i != 10 && i != 14 && i != 15)
			ts[i-1] = findTS(se, i-1);
	}
}

/* modifies the continuous attributes accordingly for further processing */
void modifyContAttrs(setOfExamples* se, int* ts)
{
	for(int i=0; i<(*se).size; i++){
		for(int j=1; j<=NUMATTRS; j++){
			if(j != 2 && j != 4 && j != 6 && j != 7 && j != 8 && j != 9 && j != 10 && j != 14 && j != 15){
                if((*se).eset[i].attrValues[j-1] > ts[j-1]){
                    (*se).eset[i].attrValues[j-1] = 2;
                }
                else{
                    (*se).eset[i].attrValues[j-1] = 1;
                }
            }
		}
	}
}

int getRandomNumber(int low,int high)
{
	int a = rand()*741;
    return (a%high+(2*high))%high;
}

/* function to build random forest recursively */
void buildRF(DTNode * randomForest,int RFcount,setOfExamples *training,int size,int zz){
      int i,j;
      for(i=0;i<RFcount;i++){
        setOfExamples temp;
        init(&temp);
        int tsize=size;
        while(tsize>0){
            int randomNumber=getRandomNumber(0,size);
            insertIntoSet(&temp,(*training).eset[randomNumber]);
            tsize--;
        }
        randomForest[i].eSet = temp;
        randomForest[i].nodeType = 0;
        for(j=0;j<NUMATTRS-1;j++){
            randomForest[i].visited[j]=1;
        }
        int count = 0;
        while(count < zz){
        	int randomNumber = getRandomNumber(0,15);
        	if(randomForest[i].visited[randomNumber] == 1){
        		randomForest[i].visited[randomNumber] = 0;
        		count++;
        	}
        }
        buildDT(&randomForest[i]);
    }
}

/* calculates the accuracy after using random forests */
double calculateRFAccuracy(DTNode * randomForest,int RFcount,setOfExamples *test,double *pr,double *re){
    double a,b;
    int p,q,i,j;
    double aa=0,bb=0,cc=0;
    p=(*test).size;
    q=0;
    for(i=0;i<(*test).size;i++){
        int g,h=0;
        g=findValue((*test).eset[i]);
        for(j=0;j<RFcount;j++){
            h=h+getDTNodeType((*test).eset[i],&randomForest[j]);
        }
        if(h>0){
            h=1;
        }
        else if(h<0){
            h=-1;
        }
        else h=g;

        if(g==h){
            q++;
        }
        if(g==1 && h==1){
            aa=aa+1;
            bb=bb+1;
            cc=cc+1;
        }
        if(g==1 && h==-1){
            bb=bb+1;
        }
        if(g==-1 && h==1){
            cc=cc+1;
        }
    }
    a=p;
    b=q;
    *pr = aa/cc;
    *re = aa/bb;
    return b/a;
}

/* gets the precision */
double getPrecision(DTNode *DTR,setOfExamples *v){
    double correct,total;
    int i;
    total = 0;
    correct =0;
    for(i=0;i<(*v).size;i++){
        int a=0;
        a=getDTNodeType((*v).eset[i],DTR);
        if(a==1){
        	if(findValue((*v).eset[i])==1)
        		correct = correct + 1;
		total++;
	}

    }
    return correct/total;
}

/* gets the recall */
double getRecall(DTNode *DTR,setOfExamples *v){
    double correct,total;
    int i;
    total = 0;
    correct =0;
    for(i=0;i<(*v).size;i++){
        int a=0;
        a=getDTNodeType((*v).eset[i],DTR);
        if(findValue((*v).eset[i])==1){
        	if( a==1)
        		correct = correct + 1;
		total++;
	}

    }
    return correct/total;
}

int main()
{
	int n,flag = 1;
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            map[i][j]=-1;
        }
    }

    setOfExamples training, validation, testing;

    init(&training); init(&validation); init(&testing);

    input(&training);
    //input(&test);

    printf("Data size = %d\n",training.size);

    int Threshold[30]={0};
    getTS(Threshold, &training);
    modifyContAttrs(&training,Threshold);

    for(int i=0;i<(training.size/3);i++)
        insertIntoSet(&validation,deleteExample(&training));

    DTNode DTR;
    DTR.eSet = training;
    DTR.nodeType = 0;

    for(int i=0;i<NUMATTRS-1;i++) DTR.visited[i]=0;
    buildDT(&DTR);
    totalNodes = numNodes(&DTR);

    int totalNodesAfterPruning;
    double accuracy,newAccuracyAfterPruning;
    double precision,recall,ffactor;

    printf("total nodes = %d\n",totalNodes);
    accuracy = calculateAccuracy(&DTR,&validation);
    precision = getPrecision(&DTR,&validation);
    recall = getRecall(&DTR,&validation);
    ffactor = (2*precision*recall)/(precision+recall);
    printf("before pruning :  precision=%lf recall=%lf fvalue=%lf\n",precision,recall,ffactor);


    printf("accuracy over validation set = %lf\n",accuracy);
    newAccuracyAfterPruning = prune(&DTR,&DTR,&validation,accuracy);
    printf("newAccuracy over validation set AfterPruning = %lf\n",newAccuracyAfterPruning);
    totalNodesAfterPruning = numNodes(&DTR);
    printf("totalNodesAfterPruning = %d\n",totalNodesAfterPruning);


    precision = getPrecision(&DTR,&validation);
    recall = getRecall(&DTR,&validation);
    ffactor = (2*precision*recall)/(precision+recall);
    printf("after pruning :  precision=%lf recall=%lf fvalue=%lf\n",precision,recall,ffactor);


    int RFcount=100;
    int attributeCount=7;
    int RFsize=training.size;
    DTNode randomForest[RFcount];
    buildRF(randomForest,RFcount,&training,RFsize,attributeCount);
    accuracy = calculateRFAccuracy(randomForest,RFcount,&validation,&precision,&recall);
    printf("Random Forest Accuracy = %lf\n",accuracy);
    ffactor = (2*precision*recall)/(precision+recall);
    printf("Random Forest :  precision=%lf recall=%lf fvalue=%lf\n",precision,recall,ffactor);

}
