#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
using namespace std;
#define UNICODE 1
LRESULT CALLBACK hwproc(HWND,UINT,WPARAM,LPARAM);
void Controls(HWND);
HWND hArray1;
HWND hArray2;
//String Sınıfım
class myString{
    public:
    char *start;
    int size;
    //stringi wchar kullanarak atanıyor
    void setString(wchar_t arr[]){
        char textC[100];
        sprintf(textC, "%ws", arr );
        size = strlen(textC);
        start = new char[size*2+1];
        for (int i = 0; i < size; i++){
            start[i]=textC[i];
        }
    }
    //Ters çevirme
    void reverse(){
      //  if(!reversed){
        for (int i = size; i >0; i--){
            start[i+size]=start[size-i];
            start[size-i]=NULL;
        }
        start = start + size+1;
        cout<<start[0]<<start[1]<<start[size-1];
      /*  reversed = 1;
        }else{
            start= start-size;
            reversed = 0;
        }*/
        
    }

};
//stringim
myString mystr;
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
    
    CreateWindowW(L"hw1class",L"String Ters Çevir",WS_OVERLAPPEDWINDOW | WS_VISIBLE,700,400,425,250,NULL,NULL,NULL,NULL);

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
                mystr.setString(text1);
                mystr.reverse();
                wchar_t arr[mystr.size];
                snwprintf(arr,mystr.size+1,L"%c",mystr.start[0]);
                for (int i = 1; i < mystr.size; i++)
                snwprintf(arr,mystr.size+1,L"%ws%c",arr,mystr.start[i]);
                SetWindowTextW(hArray2,arr);
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
    CreateWindowW(L"static",L"Yazı", WS_VISIBLE | WS_CHILD, 50,50,100,50,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"static",L"Tersi", WS_VISIBLE | WS_CHILD, 50,100,100,50,hWnd,NULL,NULL,NULL);
    hArray1 = CreateWindowW(L"edit",L"1", WS_VISIBLE | WS_CHILD, 112,50,200,20,hWnd,NULL,NULL,NULL);
    hArray2 = CreateWindowW(L"static",L"1", WS_VISIBLE | WS_CHILD, 112,100,200,20,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"button",L"Ters Çevir", WS_VISIBLE | WS_CHILD, 112,150,200,50,hWnd,(HMENU)1,NULL,NULL);
}
