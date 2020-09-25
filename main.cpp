#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

void reverseArray(vector<bool> &X, int tam)
{
    for(int i=0;i<(tam/2);i++){
        X.swap(X[i],X[tam-i-1]);
    }
}

bool overflowError(int number, int tam)
{
    bool Y;
    number = abs(number);

    int maxLimit{0};

    for(int i=0;i<tam;i++){
        maxLimit += pow(2,i);
    }
    Y = maxLimit>=number?false:true;
    return Y;
}

vector<bool> toComplementTwo(int number, int tam)
{

    int numb = abs(number);
    vector<bool> numbCompTwo;

    while(numb>0){
        numbCompTwo.push_back(numb%2);
        numb /= 2;
    }

    int completBits = tam-numbCompTwo.size();
    while(completBits--){
        numbCompTwo.push_back(0);
    }

    reverseArray(numbCompTwo,tam);

    if(number>=0){
        return numbCompTwo;
    }else{
        int counter{1};
        for(int i = numbCompTwo.size()-1;i>0;i--){
            if(numbCompTwo[i]==1){
                int j = (numbCompTwo.size()-counter);
                while(j--){
                    numbCompTwo[i-1]=numbCompTwo[i-1]?0:1;
                    i--;
                }
            }else{
                counter++;
            }
        }
        return numbCompTwo;
    }

}

int toDecimal(vector<bool> X, vector<bool> Y, int tam){
    int answ{0};
    int MSB = -1*(X[0]*pow(2,tam*2-1));
    int j=0;
    for(int i=tam-1;i>0;i--){
        answ += (X[i]*pow(2,(tam+j)));
        j++;
    }
    j=0;
    for(int i=tam-1;i>=0;i--,j++){
        answ += (Y[i]*pow(2,j));
    }
    answ += MSB;
    return answ;
}

vector<bool> sumBinary(vector<bool> X, vector<bool> Y, int tam)
{
    vector<bool> sum;
    int carry = 0;

    for(int i=tam-1; 0<=i; i--){

        int XOR = (((X[i])&!(Y[i])) || (!(X[i])&(Y[i])));
        int suma = carry + XOR;

        if(suma<=1){
            carry = ((X[i])&(Y[i]));
        }else{
            suma = 0;
            carry = 1;
        }
        sum.push_back(suma);
    }

    reverseArray(sum,tam);
    return sum;
}

vector<bool> signChange(vector<bool> &X, int tam)
{

    vector<bool> plusOne(tam);

    for(int i=0;i<tam;i++){
        if(i==tam-1){
            plusOne[i]=1;
        }else{
            plusOne[i]=0;
        }
    }

    for(int i = 0; i<tam;i++){
        X[i] = !(X[i]);
    }

    vector<bool> change = sumBinary(X,plusOne,tam);

    return change;
}

vector<bool> substBinary(vector<bool> X, vector<bool> Y, int tam)
{
    return sumBinary(X,signChange(Y,tam),tam);
}

vector<bool> SRA(vector<bool> &X, vector<bool> &Y, int &q, int tam)
{
    int first = X[0];
    int numberOfSwaps = tam - 1;

    while(numberOfSwaps!=0){
        X.swap(X[numberOfSwaps-1],X[numberOfSwaps]);
        Y.swap(Y[numberOfSwaps-1],Y[numberOfSwaps]);
        numberOfSwaps--;
    }

    q=Y[0];
    Y[0] = X[0];
    X[0] = first;
}

void printVectors(vector<bool> X, int tam)
{
    for(int i=0;i<tam;i++){
        cout << X[i] << " ";
    }
}

void printProcess(vector<bool>X, vector<bool>Y, vector<bool>Z,int q_1,int k,int counter)
{

    cout << setw(3)<< counter <<setw(k);
    printVectors(X,k);
    cout << setw(3);
    printVectors(Y,k);
    cout << setw(6) << q_1 << setw(k+1);
    printVectors(Z,k);
    cout << endl;

}

void printAuxProcess(vector<bool>X, vector<bool>Y, vector<bool>Z,int q_1,int k,int counter)
{
    cout << setw(k+3);
    printVectors(X,k);
    cout << setw(3);
    printVectors(Y,k);
    cout << setw(6) << q_1 << setw(k+1);
    printVectors(Z,k);
    cout << endl;
}

int main(){

    int Q{0}, M{0}, n;
    int Q_1 = false;

    cout << "<----------   Algoritmo de Booth   ---------->" << endl;

    cout << "Ingrese el multiplicando Q(En decimal): ";
    cin >> Q;

    cout << "Ingrese el multiplicador M(En decimal): ";
    cin >> M;

    cout << "Ingrese la cantidad de bits de los factores n: ";
    cin >> n;


    if(overflowError(Q,n)||overflowError(M,n)){
        cout << "Unable to represent Q or M in " << n << " bits" <<endl;
    }else{

    vector<bool> A(n);
    for(int i=0;i<n;i++){
        A[i]=0;
    }

    vector<bool> toComplement_Q = toComplementTwo(Q,n);
    vector<bool> toComplement_M = toComplementTwo(M,n);

    cout <<"  n  "<<setw(n+2)<<"A"<<setw(n*2+2)<<"Q"<<setw(n*2+3)<<" Q_1 "<<setw(n+3)<<"M"<<endl;


    printProcess(A,toComplement_Q,toComplement_M,Q_1,n,n);

    int contador = n;

    while(contador--){
        if(toComplement_Q[toComplement_Q.size()-1]==1&&Q_1==0){
            A = substBinary(A,toComplement_M,n);
            printAuxProcess(A,toComplement_Q,toComplement_M,Q_1,n,contador);

        }
        if(toComplement_Q[toComplement_Q.size()-1]==0&&Q_1==1){
            A = sumBinary(A,toComplement_M,n);
             printAuxProcess(A,toComplement_Q,toComplement_M,Q_1,n,contador);
        }



        SRA(A,toComplement_Q,Q_1,n);

        printProcess(A,toComplement_Q,toComplement_M,Q_1,n,contador);

    }
        cout << "\nBinary result: ";
        printVectors(A,n);
        printVectors(toComplement_Q,n);
        cout << "\nDecimal result: " << toDecimal(A,toComplement_Q,n) << endl;
    }
}
