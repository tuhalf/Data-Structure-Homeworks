#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <fstream>
using namespace std;
#define UNICODE 1
LRESULT CALLBACK hwproc(HWND,UINT,WPARAM,LPARAM);
void Controls(HWND);
HWND hArray1;
HWND hArray2;
//liste için nodun tanımı
typedef struct node{
    node *next;
    node *prev;
    char data;
} node;
//Liste sınıfı
class list{
    public:
    node *start;
    node *end;
    //Listeyi boşaltma fonksiyonu
    void emptyList(){
        while(start){
            deleteFromEnd();
        }
    }
    //Verilen dosya yolundan dosyayı okuyarak liste olusturma
    void readFilee(char *path){
        emptyList();
        ifstream myReadFile;
        myReadFile.open(path);
        char output[100]="";
        if (myReadFile.is_open()) {
            char c;
            node *prev = NULL;
        while (myReadFile.get(c)) {
            node *temp;
            temp = new node;
            temp->data=c;
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
        myReadFile.close();
    }
    //dosya seçim diyaloğu oluşturucu
    void fileDialog(HWND hWnd){
        OPENFILENAME ofn;
        char file[100];
        ZeroMemory(&ofn,sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner= hWnd;
        ofn.lpstrFile=file;
        ofn.lpstrFile[0]='\0';
        ofn.nMaxFile=100;
        ofn.lpstrFilter="Text Files\0*.TXT\0";
        ofn.nFilterIndex=1;

        GetOpenFileName(&ofn);

        readFilee(ofn.lpstrFile);
    }
    //listeyi wchar olarak yazdırma işlemi
    void printWchar(wchar_t *print){
        node *temp = start;
        while(temp){
            snwprintf(print,10000,L"%ws%c",print,temp->data);
            temp = temp->next;
        }
    }
    //Palindrom kontrolü
    void control(wchar_t *print){
        int i=1;
        printWchar(print);
        while(start!=end){      
            snwprintf(print,10000,L"%ws\r\nAdım %d: ",print,i);
            if(start->data==end->data){
                deleteFromEnd();
                deleteFromBeg();            
                printWchar(print);
                cout<<"i"<<endl;
            }else{
                snwprintf(print,10000,L"%wsBirden fazla eleman var, dolayısıyla palindrom değildir",print);
                cout<<"sda"<<endl;
                return;
            }
            i++;
            cout<<i<<endl;
        }
        if(start){
            snwprintf(print,10000,L"%wsTek elaman var, dolayısıyla palindromdur. ",print);
            cout<<"aa"<<endl;
        }
        else{
            snwprintf(print,10000,L"%wsEleman yok, dolayısıyla palindromdur. ",print);
            cout<<"ss"<<endl;
        }

    }
    //Polindroma dönüştürme fonksiyonu
    void edit(wchar_t *print){
        int i=1;
        printWchar(print);
        while(start!=end){      
            snwprintf(print,10000,L"%ws\r\nAdım %d: ",print,i);
            if(start->data==end->data){
            deleteFromEnd();
            deleteFromBeg();
            printWchar(print);
            }else{
            addToBeg();
            printWchar(print);
            }
            i++;
            cout<<i<<endl;
        }
        if(start){
            snwprintf(print,10000,L"%wsTek elaman var, dolayısıyla palindromdur. ",print);
            cout<<"aa"<<endl;
        }
        else{
            snwprintf(print,10000,L"%wsEleman yok, dolayısıyla palindromdur. ",print);
            cout<<"ss"<<endl;
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
    void deleteFromEnd(){
        node *temp;
        temp = end;
        if(start!=end){
        end = end->prev;
        end->next = NULL;
        temp->prev= NULL;
        delete temp;
        }else{
            delete temp;
            start=NULL;
            end=NULL;
        }
    }
    //Başa ekleme
    void addToBeg(){
        node *newN;
        newN = new node;
        newN->data=end->data;
        newN->next = start;
        start->prev=newN;
        start = newN;
    }
};
list listM;
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR args, int ncmdshow){

    WNDCLASSW wnd = {0};

    wnd.hbrBackground=(HBRUSH)COLOR_WINDOW;
    wnd.hCursor= LoadCursor(NULL,IDC_ARROW);
    wnd.hInstance=hInst;
    wnd.lpszClassName=L"hw1class";
    wnd.lpfnWndProc=hwproc;

    if(!RegisterClassW(&wnd))
        return -1;
    
    CreateWindowW(L"hw1class",L"Palindrom Kontrol",WS_OVERLAPPEDWINDOW | WS_VISIBLE,700,400,500,500,NULL,NULL,NULL,NULL);

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
                {   wchar_t text1[100]=L"";
                    listM.fileDialog(hWnd);
                    listM.printWchar(text1);
                    SetWindowTextW(hArray1,text1);
                    break;
                }
                case 2:
                {   wchar_t text1[10000]=L"";
                    listM.control(text1);
                    SetWindowTextW(hArray2,text1);
                    break;
                }
                case 3:
                {
                    wchar_t text1[10000]=L"";
                    listM.edit(text1);
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
    CreateWindowW(L"static",L"İşlemler", WS_VISIBLE | WS_CHILD, 50,100,100,50,hWnd,NULL,NULL,NULL);
    hArray1 = CreateWindowW(L"static",L"Dosya Seçin", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 112,50,200,20,hWnd,NULL,NULL,NULL);
    hArray2 = CreateWindowW(L"static",L"---", WS_VISIBLE | WS_CHILD | SS_SUNKEN, 50,150,250,300,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"button",L"Dosya Seç", WS_VISIBLE | WS_CHILD, 350,50,100,50,hWnd,(HMENU)1,NULL,NULL);
    CreateWindowW(L"button",L"Kontrol", WS_VISIBLE | WS_CHILD, 350,125,100,50,hWnd,(HMENU)2,NULL,NULL);
    CreateWindowW(L"button",L"Düzenle", WS_VISIBLE | WS_CHILD, 350,200,100,50,hWnd,(HMENU)3,NULL,NULL);
}