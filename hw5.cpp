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
//Ağaç yapım için node tanımı
typedef struct node{
    node *left;
    node *right;
    int data;
} node;
//Ağacım için sınıf tanımım
class tree{
    public:
    node *start;
    //Ağaca int array'den değer ataması
    void setTree(int *arr,int size){
        deleteTree(start);
        start = NULL;
        for(int i = 0;i<size;i++){
            putInt(arr[i],start);
        }
    }
    //Ağacın temizlenmesi
    void deleteTree(node *node){
         if (node == NULL) return;  
  
        /* first delete both subtrees */
        deleteTree(node->left);  
        deleteTree(node->right);  
        
        /* then delete the node */
        delete(node);
    }
    //Ağaca node ekleme
    node *putInt(int v,node *curr){
        if(curr){
            node *temp;
            if(v<curr->data){
               temp= putInt(v,curr->left);
               curr->left = temp;
            }else{
                temp = putInt(v,curr->right);
                curr->right=temp;
            }
        }else{
            curr = new node;
            curr->data= v;
            curr->left=NULL;
            curr->right=NULL;

        }
        start = curr;
        return curr;
    }
    //Ağaca wchar değişkeniyle değer atama. DEğerler virgülle ayrılır.
    void setByWChar(wchar_t text[]){
        char textC[100];
        sprintf(textC, "%ws", text );
        char temp[20];
        int tArray[20];
        int k = 0;
        int l = 0;

        for(int i = 0;i<=strlen(textC);i++){
            if(textC[i]==',' || textC[i]=='\0'){
                tArray[l]=atoi(temp);
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
        setTree(tArray,l);
    }
    //Ağacı post order olarak wchar'a yazdırma
    node *postOrderWChar(wchar_t *output,node *curr){
        if(curr->left)
            postOrderWChar(output,curr->left);
        if(curr->right)
            postOrderWChar(output,curr->right);
        snwprintf(output,100,L"%ws%d,",output,curr->data);
        cout<<curr->data<<endl;
    }
};
tree myTree1;
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
                    myTree1.setByWChar(text1);
                    wchar_t arr[100]=L""; 
                    myTree1.postOrderWChar(arr,myTree1.start);
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
void Controls(HWND hWnd){
    CreateWindowW(L"static",L"1. Array", WS_VISIBLE | WS_CHILD, 50,50,100,50,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"static",L"2. Array", WS_VISIBLE | WS_CHILD, 50,100,100,50,hWnd,NULL,NULL,NULL);
    hArray1 = CreateWindowW(L"edit",L"1", WS_VISIBLE | WS_CHILD, 150,50,200,20,hWnd,NULL,NULL,NULL);
    hArray2 = CreateWindowW(L"edit",L"1", WS_VISIBLE | WS_CHILD, 150,100,200,20,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"button",L"Sırala ve Kontrol Et", WS_VISIBLE | WS_CHILD, 112,150,200,50,hWnd,(HMENU)1,NULL,NULL);
}