#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <wchar.h>
using namespace std;
#define UNICODE 1
LRESULT CALLBACK hwproc(HWND,UINT,WPARAM,LPARAM);
void Controls(HWND);
HWND hArray1;
HWND hArray2;
HWND outA;
HWND outB;
HWND coA;
HWND coB;
//listem için node tanımı
typedef struct node{
    node *next;
    node *prev;
    char data;
} node;
//Liste sınıf tanımı
class list{
    public:
    node *start;
    node *end;
    int coef;
    //listeyi boşaltma fonksiyonu
    void emptyList(){
        while(start){
            deleteFromBeg();
        }
    }
    //Karakter olarak okuyarak listeye yazma
    void readChar(char textC[]){
        emptyList();
        int size = strlen(textC);
        node *prev;
        for (int i = 0; i < size; i++){
            node *temp;
            temp = new node;
            temp->data=textC[i];
            temp->prev = prev;
            temp->next = NULL;
            if(!start)
                start = temp;
            else
                prev->next = temp;
            end = temp;
            prev = temp;
        }
    }
    //Listeyi Wchar'a yazma
    void printWchar(wchar_t *print){
        node *temp = start;
        while(temp){
            snwprintf(print,10000,L"%ws%c",print,temp->data);
            temp = temp->next;
        }
    }
    //Baştan silme
    void deleteFromBeg(){
        node *temp;
        temp = start;
        if(start!=end){
        start = start->next;
        start->prev = NULL;
        temp->next= NULL;
        delete temp;
        }else{
            delete temp;
            start=NULL;
            end=NULL;
        }
    }
    //sondan silme
    void addToBeg(){
        node *newN;
        newN = new node;
        newN->data=end->data;
        newN->next = start;
        start->prev=newN;
        start = newN;
    }
};
list listA;
list listB;
//iki ayrı stack'i aynı anda doldurmak için fonksiyon
void FillStacks(wchar_t arr[]){
    char a[100]="";
    char b[100]="";
    char aC='a';
    char bC='b';
    for (int i = 0; arr[i] ; i++)
    {
        if(arr[i]==L'a'){
            sprintf(a,"%s%c",a,aC);
        }else{
            sprintf(b,"%s%c",b,bC);
        }
    }
    listA.readChar(a);
    listB.readChar(b);
}
//Stacklerin, adım adım düzenli olup olmadığını kontrol için fonksiyon
void checkStacks(wchar_t *print){
    int i=1;
        listA.printWchar(print);
        listB.printWchar(print);
        int aCount = 0;
        int bCount = 0;
        while(listB.start){      
            snwprintf(print,10000,L"%ws\r\nAdım %d: ",print,i);
            if(bCount<listB.coef){
                listB.deleteFromBeg();          
                bCount++;
            }
            if(listB.coef==bCount){
                for (aCount = 0; aCount < listA.coef; aCount++){
                    if(listA.start)
                    listA.deleteFromBeg();
                    else{
                    snwprintf(print,10000,L"%ws Düzenli İfade Değildir ",print);
                    return;
                    }
                }   
                bCount=0;
            }
            cout<<"sa"<<endl;
            listA.printWchar(print);
            listB.printWchar(print);
            i++;
        }
        if(listA.start){
            snwprintf(print,10000,L"%wsDÜZENLİ İFADE DEĞİLDİR",print);
        }
        else{
            snwprintf(print,10000,L"%wsDÜZENLİ İFADEDİR",print);
        }
}
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR args, int ncmdshow){

    WNDCLASSW wnd = {0};

    wnd.hbrBackground=(HBRUSH)COLOR_WINDOW;
    wnd.hCursor= LoadCursor(NULL,IDC_ARROW);
    wnd.hInstance=hInst;
    wnd.lpszClassName=L"hw1class";
    wnd.lpfnWndProc=hwproc;

    if(!RegisterClassW(&wnd))
        return -1;
    
    CreateWindowW(L"hw1class",L"Düzenli İfade Bul",WS_OVERLAPPEDWINDOW | WS_VISIBLE,700,400,500,500,NULL,NULL,NULL,NULL);

    MSG msg ={0};
    while(GetMessage(&msg,NULL,NULL,NULL)){

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }
}

LRESULT CALLBACK hwproc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){

    switch(msg){
        case WM_CREATE:
            Controls(hWnd);
            break;
        case WM_COMMAND:
            switch (wp)
            {
                case 1:
                {   wchar_t textA[100]=L"";
                    wchar_t textB[100]=L"";
                    wchar_t input[100]=L"";
                    wchar_t coaW[10]=L"";
                    wchar_t cobW[10]=L"";
                    GetWindowTextW(hArray1,input,100);
                    FillStacks(input);
                    listA.printWchar(textA);
                    listB.printWchar(textB);
                    GetWindowTextW(coA,coaW,10);
                    GetWindowTextW(coB,cobW,10);
                    listA.coef=_wtoi(coaW);
                    listB.coef=_wtoi(cobW);
                    SetWindowTextW(outA,textA);
                    SetWindowTextW(outB,textB);
                    break;
                }
                case 2:
                {   wchar_t text1[10000]=L"";
                    wchar_t coaW[10]=L"";
                    wchar_t cobW[10]=L"";
                    GetWindowTextW(coA,coaW,10);
                    GetWindowTextW(coB,cobW,10);
                    listA.coef=_wtoi(coaW);
                    listB.coef=_wtoi(cobW);
                    checkStacks(text1);
                    SetWindowTextW(hArray2,text1);
                    break;
                }
            }
            break;             
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd,msg,wp,lp);

    }

}
void Controls(HWND hWnd){
    CreateWindowW(L"static",L"Liste", WS_VISIBLE | WS_CHILD, 50,50,100,50,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"static",L"Çıktılar", WS_VISIBLE | WS_CHILD, 50,100,100,50,hWnd,NULL,NULL,NULL);
    hArray1 = CreateWindowW(L"edit",L"Sadece A ve B ile Doldurun", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 112,50,200,20,hWnd,NULL,NULL,NULL);
    outA = CreateWindowW(L"static",L"", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 112,100,100,20,hWnd,NULL,NULL,NULL);
    outB = CreateWindowW(L"static",L"", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 212,100,100,20,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"static",L"Katsayılar", WS_VISIBLE | WS_CHILD, 50,150,100,50,hWnd,NULL,NULL,NULL);
    coA = CreateWindowW(L"edit",L"1", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 112,150,100,20,hWnd,NULL,NULL,NULL);
    coB = CreateWindowW(L"edit",L"1", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 212,150,100,20,hWnd,NULL,NULL,NULL);
    hArray2 = CreateWindowW(L"static",L"---", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 50,200,250,300,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"button",L"Giriş", WS_VISIBLE | WS_CHILD, 350,50,100,50,hWnd,(HMENU)1,NULL,NULL);
    CreateWindowW(L"button",L"Kontrol", WS_VISIBLE | WS_CHILD, 350,125,100,50,hWnd,(HMENU)2,NULL,NULL);
    //CreateWindowW(L"button",L"Düzenle", WS_VISIBLE | WS_CHILD, 350,200,100,50,hWnd,(HMENU)3,NULL,NULL);
}

/*
int main()
{
    printf("Hello World");

    return 0;
}*/
