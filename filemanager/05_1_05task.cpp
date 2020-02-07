#include <stdio.h>
#include <vector>
#include <string.h>
#include <iostream>
#include<cstring>
#include <string>
#include "os_file.h"
using namespace std;
bool is_created = false;
int Disk_Size = 0;

struct node {
    string name;
    bool is_dir = true;
    int size = 0;
    node *par=0;
    string path;
    vector<node*>children;
};
node* ROOT_DIR;
vector<node>tree;
string cur_dir_path;

int my_create(int disk_size) {
    if (is_created == false) {
        node root_dir;
        root_dir.name = "/";
        root_dir.path = "/";
        cur_dir_path = "/";
        Disk_Size = disk_size;
        tree.push_back(root_dir);
        ROOT_DIR = &tree[0];
        is_created = true;
        return 1;
    }
    return 0;
}

int Count_Slash(string path) {
    int count_slash = 0;
    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '/') count_slash++;
    }
    return count_slash;
}
string get_dir_name_from_path(string path, int n, int j, int *kol) { //j-nomer nujnoy papki

    string dir_name;
    int  k = 0, i = 0, g=0;
    while (i < n) {
        if (path[i] == '/') {
            k++;

        }
        if (k == j && path[i] != '/') {
            dir_name.push_back(path[i]); g++;
        }
        i++;
    }
    *kol = g;

    return dir_name;
}
string create_abs_path(string path) {

    int j = 1, count_slash = 0, kol = 0, i = 0, count_slash1 = 0;

    string name = "";
    string abs_path = "";
    vector<string>a_path;
    string path1 = cur_dir_path;

    count_slash = Count_Slash(path);
    count_slash1 = Count_Slash(path1);
    if (path1 != "/" && path[0] != '/') {//&&path[0]!='/'
        for (j = 1; j <= count_slash1; j++) {
            name = get_dir_name_from_path(path1, path1.length(), j, &kol);
            a_path.push_back(name);
        }
    }
    if (path[0] == '/') { i = 1; }
    for (j = i; j <= count_slash; j++) {
        name = get_dir_name_from_path(path, path.length(), j, &kol);
        if (name == ".") continue;
        if (name == "..") {
            if (!a_path.empty()) {
                a_path.pop_back();
                continue;
            }
            return "";
        }
        a_path.push_back(name);

    }
    for (i = 0; i < a_path.size(); i++) {
        abs_path += "/" + a_path[i];
    }
    return abs_path;

}
int path_to_dir_exists_rec(string path, node*cur_dir, int count_slash, int deep) {//deep=1
    int i = 0, kol = 0, rtn = 0;
    string name = "";

    name = get_dir_name_from_path(path, path.length(), deep, &kol);
    if (kol == 0 || kol > 32) return 0;

    if (deep == count_slash) {
        if (name == "." || name == "..") return 0;

        for (i = 0; i < cur_dir->children.size(); i++) {
            if (cur_dir->children[i]->name == name && cur_dir->children[i]->is_dir == true)
                return 1;
        }
        return 0;
    }


    for (i = 0; i < cur_dir->children.size(); i++) {
        if (cur_dir->children[i]->name == name && cur_dir->children[i]->is_dir == true) {
            cur_dir = cur_dir->children[i];
            deep++;
            rtn = path_to_dir_exists_rec(path, cur_dir, count_slash, deep);
            if (rtn == 1)return 1;

        }

    }

    return 0;


}

int add_dir_abs(string path, int j, node* cur_dir, int count_slash, int deep) {

    if (j > count_slash) return 0;

    int i = 0, kol, rtn = 0;

    if (deep == 1) {
        string dir_name = ""; dir_name = get_dir_name_from_path(path, path.length(), j, &kol);
        for (i = 0; i < cur_dir->children.size(); i++) {
            if (cur_dir->children[i]->name == dir_name && cur_dir->is_dir == true ) {
                return 0;
            }

        }
        if (dir_name == "." || dir_name == ".." || kol>32 || kol == 0) return 0;

        node *new_dir=new node();
        new_dir->name =dir_name;
        new_dir->par = cur_dir;
        new_dir->path = path;
        cur_dir->children.push_back(new_dir);
        return 1;
    }

    for (i = 0; i <cur_dir->children.size(); i++) {

        if (cur_dir->children[i]->name == get_dir_name_from_path(path, path.length(), j,&kol) && cur_dir->children[i]->is_dir==true) {
            j++;
            cur_dir=cur_dir->children[i];
            deep--;
            rtn=add_dir_abs(path, j, cur_dir, count_slash,deep);
            if (rtn == 1) return 1;
        }
    }
    return 0;

}
int my_create_dir(const char* path) {
    string path1="";
    path1 = path;
    string abs_path = "";
    int count_slash=0;
    int i = 0;
    int rtn = 0;

    abs_path = create_abs_path(path1);
    if (abs_path == "") return 0;
    count_slash = Count_Slash(abs_path);

    rtn = add_dir_abs(abs_path, 1, ROOT_DIR, count_slash, count_slash);


    return rtn;
}

int add_file_abs(string path, int j, node* cur_dir, int count_slash, int deep, int file_size) {

    if (j > count_slash) return 0;

    int i = 0, kol, rtn=0;

    if (deep == 1) {
        string dir_name = ""; dir_name = get_dir_name_from_path(path, path.length(), j, &kol);
        for (i = 0; i < cur_dir->children.size(); i++) {
            if (cur_dir->children[i]->name == dir_name && cur_dir->children[i]->is_dir == false) {
                return 0;
            }

        }
        if (dir_name == "." || dir_name == ".." || kol>32 || kol == 0) return 0;

        node *new_dir = new node();
        new_dir->name = dir_name;
        new_dir->par = cur_dir;
        new_dir->is_dir = false;
        new_dir->path = path;
        new_dir->size = file_size;
        cur_dir->children.push_back(new_dir);
        return 1;
    }

    for (i = 0; i <cur_dir->children.size(); i++) {

        if (cur_dir->children[i]->name == get_dir_name_from_path(path, path.length(), j, &kol) && cur_dir->children[i]->is_dir== true) {
            j++;
            cur_dir = cur_dir->children[i];
            deep--;
            rtn=add_file_abs(path, j, cur_dir, count_slash, deep,file_size);
            if (rtn == 1)return 1;
        }
    }
    return 0;


}
int my_create_file(const char* path, int file_size) {
    if (is_created == true) {
        if (Disk_Size - file_size >= 0) {

            int count_slash = 0, i = 0, rtn=0;
            string path1 = "";
            path1 = path;
            string abs_path = "";

            abs_path = create_abs_path(path1);
            if (abs_path == "") return 0;
            count_slash = Count_Slash(abs_path);
            rtn = add_file_abs(abs_path, 1, ROOT_DIR, count_slash, count_slash, file_size);

            if (rtn == 1) { Disk_Size -= file_size; }
            return rtn;

        }
        return 0;

    }

    return 0;
}

int my_change_dir(const char* path) {
    string path1 = path;
    string abs_path = "";
    int count_slash=0;

    abs_path = create_abs_path(path1);

    count_slash = Count_Slash(abs_path);

    if (path_to_dir_exists_rec(abs_path, ROOT_DIR, count_slash, 1) == 1) {
        cur_dir_path = abs_path;
        return 1;
    }
    return 0;
}

void my_get_cur_dir(char *dst) {

    char *mas;
    mas = (char*)malloc(cur_dir_path.length()*sizeof(char));
    for (int i = 0; i < cur_dir_path.length(); i++) {
        mas[i] = cur_dir_path[i];
    }
    memcpy(dst, mas, cur_dir_path.length());
    dst[cur_dir_path.length()]='\0';

}

void  get_all_paths(node *cur_dir, vector <string> *paths) {

    int i = 0;
    for (i = 0; i < cur_dir->children.size(); i++) {
        cur_dir = cur_dir->children[i];
        get_all_paths(cur_dir, paths);
        cur_dir = cur_dir->par;
    }
    (*paths).push_back(cur_dir->path);

}
vector <string> sort_all_paths(vector <string> paths) {
    string temp;
    vector<string> ans;
    for (int j = 0; j < (paths).size(); j++) {
        for (int i = 0; i < (paths).size()-1; i++) {
            if ((paths)[i].length()<(paths)[i + 1].length()) {
                temp = (paths)[i];
                (paths)[i] = (paths)[i + 1];
                (paths)[i + 1] = temp;
            }
        }
    }
    ans=paths;
    return ans;
}
int delete_dir_file(string path, node* cur_dir, int count_slash, int deep, int recursive) {
    if (deep > count_slash+1) return 0;
    int i = 0,j=0,kol=0, rtn=-1;
    string name;

    name = get_dir_name_from_path(path, path.length(), deep, &kol);
    if (deep == count_slash+1) {

        name = get_dir_name_from_path(path, path.length(), deep-1, &kol);

        if (cur_dir->children.size() == 0 && (recursive==1 || recursive==0)) {
            cur_dir = cur_dir->par;
            for (i = 0; i < cur_dir->children.size(); i++) {

                if (cur_dir->children[i]->name == name) {
                    if (cur_dir->children[i]->is_dir == false) { Disk_Size += cur_dir->children[i]->size; }
                    cur_dir->children.erase(cur_dir->children.begin() + i);
                    return 1;
                }

            }
        }

        if (cur_dir->children.size()>0 && recursive == 0) {
            return 0;
        }

        if (cur_dir->children.size() > 0 && recursive == 1) {

            vector<string > paths;
            get_all_paths(cur_dir,&paths);
            //отсортировать по длине путей
            paths=sort_all_paths(paths);

            for (i = 0; i < paths.size(); i++) {
                count_slash = Count_Slash(paths[i]);
                delete_dir_file(paths[i], ROOT_DIR, count_slash, 1, recursive);

            }
            return 1;
        }
    }


    for (i = 0; i < cur_dir->children.size(); i++) {

        if (cur_dir->children[i]->name == name) {

            deep++;
            cur_dir = cur_dir->children[i];
            rtn = delete_dir_file(path, cur_dir, count_slash, deep, recursive);
            if (rtn != -1) {
                return rtn;
            }
        }
    }

    return 0;


}
int my_remove(const char* path, int recursive) {

    string abs_path = "";
    string path1 = path;
    int rtn = 0, count_slash=0;
    if (path1 != cur_dir_path) {
        abs_path = create_abs_path(path1);
    }
    else abs_path = cur_dir_path;

    if (abs_path == "/") return 0;

    count_slash = Count_Slash(abs_path);

    if (abs_path == cur_dir_path) {
        rtn = delete_dir_file(abs_path, ROOT_DIR, count_slash, 1, recursive);
        if (rtn == 1) {
            cur_dir_path = "/";
        }
    }
    else rtn = delete_dir_file(abs_path, ROOT_DIR, count_slash, 1, recursive);


    return rtn;
}

int delete_file_manager() {
    vector<string> paths;
    int count_slash = 0;
    int rtn = 0;
    get_all_paths(ROOT_DIR, &paths);
    paths=sort_all_paths(paths);
    for (int i = 0; i < paths.size(); i++) {
        if (paths[i] != "/") {
            count_slash = Count_Slash(paths[i]);
            rtn = delete_dir_file(paths[i], ROOT_DIR, count_slash, 1, 0);
            if (rtn == 0) return 0;
        }
    }
    tree.erase(tree.begin() + 0);
    return 1;
}
int my_destroy() {
    if (!is_created) return 0;
    int rtn = 0;
    rtn=delete_file_manager();
    cur_dir_path = "";
    is_created = false;
    Disk_Size = 0;
    return rtn;
}

vector<string> sort_children(vector <string> children) {
    string temp;
    vector <string> ans;
    for (int i = 0; i < (children).size(); i++) {
        for (int j = 0; j < (children).size() - 1; j++) {
            if ((children)[j] > (children)[j + 1]) {
                temp = (children)[j];
                children[j] = children[j + 1];
                (children)[j + 1] = temp;

            }

        }
    }
    ans=children;
    return ans;
}
string create_not_abs_path(string n_abs, string abs_path){
    int count_slash=0, count_slash1=0, i,j, kol, f=0, d,e;
    count_slash=Count_Slash(n_abs);
    count_slash1=Count_Slash(abs_path);
    string name, name1;
    string ans;
    for(i=0;i<=count_slash;i++){
        name=get_dir_name_from_path(n_abs,n_abs.length(),i,&kol);
        for(j=1;j<=count_slash1;j++){
            name1=get_dir_name_from_path(abs_path,abs_path.length(),j,&kol);
            if(name==name1){
                d=i;e=j;
                f=1;break;
            }

        }
        if(f==1) break;
    }
    for(i=0;i<d;i++){
        ans+=get_dir_name_from_path(n_abs,n_abs.length(),i,&kol);
        ans+="/";
    }
    for(j=e;j<=count_slash1;j++){
        ans+=get_dir_name_from_path(abs_path,abs_path.length(),j,&kol);
        if(j!= count_slash1)ans+="/";
    }
    return  ans;
}
int print_dir_null(node * cur_dir,bool not_abs, string n_abs) {


    vector<string> children;
    int i = 0, j=0;
    string bad_path;
    for (i = 0; i < cur_dir->children.size(); i++) {
        children.push_back(cur_dir->children[i]->name);
    }
    if (children.size() != 0) { children=sort_children(children); }
    if(not_abs){bad_path =create_not_abs_path(n_abs,cur_dir->path); cout<<bad_path<<":"<<"\n";}
    else cout << cur_dir->path<<":"<<"\n";


    if (children.size() != 0) {
        for (j = 0; j < children.size(); j++) {
            cout << children[j] << "\n";
        }
    }


    for (j = 0; j < children.size(); j++) {
        for (i = 0; i < cur_dir->children.size(); i++) {
            if (cur_dir->children[i]->name == children[j] && cur_dir->children[i]->is_dir) {
                cur_dir = cur_dir->children[i];
                print_dir_null(cur_dir,not_abs,n_abs);
                cur_dir = cur_dir->par;
            }
        }
    }

    return 1;

}
int print_dir_one(node* cur_dir,bool not_abs, string n_abs){
    vector<string> child_dir;
    vector <string>child_file;
    string bad_path;
    int i=0,j=0;

    for(i=0;i<cur_dir->children.size();i++){
        if(cur_dir->children[i]->is_dir){
            child_dir.push_back(cur_dir->children[i]->name);
        }
        if(cur_dir->children[i]->is_dir==false){
            child_file.push_back(cur_dir->children[i]->name);
        }
    }
    if(child_dir.size()!=0){ child_dir=sort_children(child_dir);}
    if(child_file.size()!=0){ child_file=sort_children(child_file);}

    for(i=0;i<child_file.size();i++){
        child_dir.push_back(child_file[i]);
    }
    if(not_abs){bad_path =create_not_abs_path(n_abs,cur_dir->path); cout<<bad_path<<":"<<"\n";}
    else cout << cur_dir->path<<":"<<"\n";
    for(i=0;i<child_dir.size();i++){
        cout<<child_dir[i]<<"\n";
    }


    for (j = 0; j < child_dir.size(); j++) {
        for (i = 0; i < cur_dir->children.size(); i++) {
            if (cur_dir->children[i]->name == child_dir[j] && cur_dir->children[i]->is_dir) {
                cur_dir = cur_dir->children[i];
                print_dir_one(cur_dir,not_abs,n_abs);
                cur_dir = cur_dir->par;
            }
        }
    }

    return 1;



}
int find_dir_list_rec(node *cur_dir,int count_slash, int deep, string path, int dir_first,bool not_abs,string n_abs) {//deep=1

    if (deep > count_slash+1) return 0;
    int i = 0, kol = 0, rtn=0;
    string name;
    if(path=="/") {
        if(dir_first==0) {
            rtn = print_dir_null(cur_dir,not_abs,n_abs); //dir_first=0
        }
        if(dir_first==1){
            rtn=print_dir_one(cur_dir,not_abs,n_abs);
        }
        return rtn;
    }

    name = get_dir_name_from_path(path, path.length(), deep, &kol);

    if (deep == count_slash +1) {
        if(dir_first==0){
            rtn=print_dir_null(cur_dir,not_abs,n_abs);
        }
        if(dir_first==1){
            rtn=print_dir_one(cur_dir,not_abs,n_abs);
        }
        return rtn;
    }

    for (i = 0; i < cur_dir->children.size(); i++) {
        if (cur_dir->children[i]->name == name) {
            deep++;
            cur_dir = cur_dir->children[i];
            rtn=find_dir_list_rec(cur_dir, count_slash, deep, path, dir_first, not_abs,n_abs);
        }
    }
    return rtn;
}
int my_list_recursive(const char* path, int dir_first) {
    string path1;
    path1 = path;
    bool not_abs=false;
    if(path1[0]!='/') not_abs=true; //относительный путь
    int count_slash = 0, rtn=0;
    string abs_path = create_abs_path(path1);
    count_slash = Count_Slash(abs_path);
    rtn=find_dir_list_rec(ROOT_DIR,count_slash,1, abs_path, dir_first,not_abs,path1);
            cout<<"\n";
    return rtn;
}

void setup_file_manager(file_manager_t *fm){
    fm->create=my_create;
    fm->destroy=my_destroy;
    fm->create_dir=my_create_dir;
    fm->create_file=my_create_file;
    fm->remove=my_remove;
    fm->change_dir=my_change_dir;
    fm->get_cur_dir=my_get_cur_dir;
    fm->list_recursive=my_list_recursive;

}

