#include "os_mem.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstring>

using namespace std;
bool is_created=false;
int MAX_OBJECTS;

struct MM_Object{
    string name;
    vector<MM_Object*> refs_child;
    vector<MM_Object*> refs_par;

};

vector<MM_Object*> Objects;

int CreateMM(int size, int num_pages){
    if(!is_created){
        MAX_OBJECTS=size;
        is_created=true;
        return 1;
    }
    return 0;

}

int Destroy(){
    if(!is_created) return 0;
  for(auto it=Objects.begin(); it!=Objects.end();){
      it=(Objects).erase(it);
  }
    is_created=false;
    return 1;
}

int CreateMM_Object(const char* name){
    if(is_created) {
        string name1;
        name1 = name;
        if (name1.length() > 32) return 0;
        if (Objects.size() >= MAX_OBJECTS) return 0;
        for (int i = 0; i < Objects.size(); i++) {
            if (Objects[i]->name == name1) return 0;
        }
        MM_Object *obj = new MM_Object();
        obj->name = name1;
        Objects.push_back(obj);
        return 1;
    }
    return 0;
}

void EraseChildRefs(MM_Object *obj){
    int i,j;
    for(i=0;i<obj->refs_child.size();i++){
        for(j=0;j<obj->refs_child[i]->refs_par.size();j++){
            if(obj->refs_child[i]->refs_par[j]->name==obj->name){
                obj->refs_child[i]->refs_par.erase(obj->refs_child[i]->refs_par.begin()+j);
            }
        }
    }
}

void EraseParRefs(MM_Object *obj){
    int i,j;
    for(i=0;i<obj->refs_par.size();i++){
        for(j=0;j<obj->refs_par[i]->refs_child.size();j++){
            if(obj->refs_par[i]->refs_child[j]->name==obj->name){
                 obj->refs_par[i]->refs_child.erase(obj->refs_par[i]->refs_child.begin()+j);
            }
        }
    }

}

int DestroyMM_Object(const char* name){
    if(!is_created)return 0;
    string name1;
    name1=name;
    for(int i=0;i<Objects.size();i++){
        if(Objects[i]->name==name1){
            {
                 if(Objects[i]->refs_child.size()>0){
                     EraseChildRefs(Objects[i]);
                 }
                 if(Objects[i]->refs_par.size()>0){
                     EraseParRefs(Objects[i]);
                 }
                Objects.erase(Objects.begin()+i);

            return 1;
            }
        }
    }
    return 0;
}

void sort_obj(){
    MM_Object *temp;
    int i,j;
    for(i=0;i<Objects.size();i++){
        for(j=0;j<Objects.size()-1;j++){
            if(Objects[j]->name>Objects[j+1]->name){
                temp=Objects[j];
                Objects[j]=Objects[j+1];
                Objects[j+1]=temp;
            }
        }
    }
}

void PrintMM_Objects(){
    sort_obj();
    for(int i=0;i<Objects.size();i++){
        cout<<Objects[i]->name<<endl;
    }
   cout<<"\n";
}

int LinkMM_Objects(const char* objectname1, const char*objectname2){
    if(!is_created){ return 0;}
    string name1;
    string name2;
    name1=objectname1;
    name2=objectname2;
    int f1=0,f2=0,g=0;

    int i,j;
    for(i=0;i<Objects.size();i++){
        if(Objects[i]->name==name1){
           f1=1; break;
        }
    }
    for(j=0;j<Objects.size();j++){
        if(Objects[j]->name==name2){
           f2=1; break;
        }
    }
    if(f1==1 && f2==1 && Objects[i]->refs_child.size()<=16) {

       for(g=0;g<Objects[i]->refs_child.size();g++){ //если ссылка уже сушествует
           if(Objects[i]->refs_child[g]->name==Objects[j]->name){
               return 0;
           }
       }
       for(g=0;g<Objects[j]->refs_par.size();g++){  //если ссылка уже сушествует
           if(Objects[j]->refs_child[g]->name==Objects[i]->name){
               return 0;
           }
       }
        Objects[i]->refs_child.push_back(Objects[j]);
        Objects[j]->refs_par.push_back(Objects[i]);

        return 1;
    }
    return 0;

}

void Print_Link_Counts(){
    sort_obj();
    for(int i=0;i<Objects.size();i++){
        cout<<Objects[i]->name<<" "<<Objects[i]->refs_par.size()<<endl;
    }
    cout<<"\n";
}

void setup_memory_manager(memory_manager_t *mm){
  mm->create=CreateMM;
  mm->destroy=Destroy;
  mm->create_object=CreateMM_Object;
  mm->destroy_object=DestroyMM_Object;
  mm->print_objects=PrintMM_Objects;
  mm->link=LinkMM_Objects;
  mm->print_link_counts=Print_Link_Counts;

}

