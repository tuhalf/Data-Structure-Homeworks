#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
using namespace std;
#define UNICODE 1
LRESULT CALLBACK hwproc(HWND,UINT,WPARAM,LPARAM);
void Controls(HWND);
HWND hArray1; //1. metin kutusu için handler
HWND hArray2; //2. metin kutusu için handler
//Array için class
class array{
    public:
    int *start;
    int size;
    //Arrayin değerlerini atama
    void setArray(int aSize, int *arr){
        size = aSize;
        start = new int[aSize];
        for(int i = 0;i<size;i++){
            if(arr[i])
                start[i]=arr[i];
            else 
                start[i]=0;
        }
    }
    //Array'i yeniden boyutlandırma
    void resize(int aSize){
        array temp;
        temp.setArray(aSize,start);
        delete start;
        size = aSize;
        start = temp.start;
    }
    //wchar kullanarak array'e değer atama. Değerler virgülle ayrılır
    void setByWChar(wchar_t text[]){
        char textC[100];
        sprintf(textC, "%ws", text );
        char temp[20];
        int tArray[20];
        int k = 0;int l = 0;

        for(int i = 0;i<=strlen(textC)&& textC[i-1]!='\0';i++){
            if(textC[i]==',' || textC[i]=='\0'){
                sscanf(temp, "%d", &tArray[l]);
                for(int m = 0;m<20;m++){
                    temp[m] = NULL;
                }
                k = 0;
                l++;
            }else{
            temp[k] = textC[i];
            k++;
            }

        } 
        delete start;
        size = l;
        start = new int[size];
        for(int i = 0;i<size;i++){
        if(tArray[i])
            start[i]=tArray[i];
        else 
            start[i]=0;
        }
    }
    //Sıralama
    void sort(){
        int i,k;
        for(i=0;i<size;i++){
            for(k=0;k<size-1;k++){
                if(start[k]>start[k+1]){
                    int temp = start[k];
                    start[k]=start[k+1];
                    start[k+1]=temp;
                }
            }
        }
        printArray();
    }
    //Yazdırma
    void printArray(){
        for(int i=0;i<size;i++)
            cout<<start[i]<<",";
        cout<<endl;
    }
    //Kontrol
    int checkSub(array *temp){
        int f=0;
        for (int i = 0;i<size;i++){
            if(start[i]==temp->start[f])    f++;
            else f=0;
            if (f==temp->size) return 1;
        }
        return 0;
    }
};
//Arrayler
array myArr1;
array myArr2;
//Pencere handler fonksiyonu
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR args, int ncmdshow){

    WNDCLASSW wnd = {0};

    wnd.hbrBackground=(HBRUSH)COLOR_WINDOW;
    wnd.hCursor= LoadCursor(NULL,IDC_ARROW);
    wnd.hInstance=hInst;
    wnd.lpszClassName=L"hw1class";
    wnd.lpfnWndProc=hwproc;

    if(!RegisterClassW(&wnd))
        return -1;
    
    CreateWindowW(L"hw1class",L"Array Kontrol",WS_OVERLAPPEDWINDOW | WS_VISIBLE,700,400,425,250,NULL,NULL,NULL,NULL);

    MSG msg ={0};
    while(GetMessage(&msg,NULL,NULL,NULL)){

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }
}
//Pencereden dönen mesajları yorumlamak için fonksiyon
LRESULT CALLBACK hwproc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){

    switch(msg){
        case WM_CREATE:
            Controls(hWnd);
            break;
        case WM_COMMAND:
            switch (wp)
            {
            case 1:
                    wchar_t text1[100];         
                    GetWindowTextW(hArray1,text1,100); 
                    myArr1.setByWChar(text1);
                    wchar_t text2[100];         
                    GetWindowTextW(hArray2,text2,100);
                    myArr2.setByWChar(text2);
                    myArr1.sort();
                    myArr2.sort();

                    wchar_t arr[myArr1.size*7];
                    snwprintf(arr,myArr1.size*7,L"%d",myArr1.start[0]);
                    for(int i=1;i<myArr1.size;i++)
                        snwprintf(arr,myArr1.size*7,L"%ws,%d",arr,myArr1.start[i]);
                    SetWindowTextW(hArray1,arr);

                    snwprintf(arr,myArr2.size*7,L"%d",myArr2.start[0]);
                    for(int i=1;i<myArr2.size;i++)
                        snwprintf(arr,myArr2.size*7,L"%ws,%d",arr,myArr2.start[i]);
                    SetWindowTextW(hArray2,arr);

                    if(myArr1.checkSub(&myArr2)) MessageBoxW(hWnd,L"Array 2 Array 1'in alt-setidir.",L"Bilgi",MB_ICONINFORMATION);
                    else if(!myArr1.checkSub(&myArr2)) MessageBoxW(hWnd,L"Array 2 Array 1'in alt-seti değildir.",L"Bilgi",MB_ICONINFORMATION);
                    break;
            }
            break;             
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd,msg,wp,lp);

    }

}
//Penceredeki her şeyin yerinin atanması
void Controls(HWND hWnd){
    CreateWindowW(L"static",L"1. Array", WS_VISIBLE | WS_CHILD, 50,50,100,50,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"static",L"2. Array", WS_VISIBLE | WS_CHILD, 50,100,100,50,hWnd,NULL,NULL,NULL);
    hArray1 = CreateWindowW(L"edit",L"1", WS_VISIBLE | WS_CHILD, 150,50,200,20,hWnd,NULL,NULL,NULL);
    hArray2 = CreateWindowW(L"edit",L"1", WS_VISIBLE | WS_CHILD, 150,100,200,20,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"button",L"Sırala ve Kontrol Et", WS_VISIBLE | WS_CHILD, 112,150,200,50,hWnd,(HMENU)1,NULL,NULL);
}
