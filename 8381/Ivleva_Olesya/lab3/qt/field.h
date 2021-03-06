#include <iostream>
#include <string>
using namespace std;
//#pragma once
#include <mediator.h>

class Field{
protected:
    Item*** map;
    Land*** land;
    IMediator *mediator;
    int max_item = 5;
    int now_item = 0;
    unsigned int width = 0;
    unsigned int height = 0;

public:
    Field(unsigned int n,unsigned int m): width(m), height(n){
        map = new Item**[n];
        for(unsigned int i = 0; i < n; i++)
            map[i] = new Item*[m];
        for(unsigned int i = 0; i < n; i++)
            for(unsigned int j = 0; j < m; j++)
                map[i][j] = nullptr;

        land = new Land**[n];
        for(unsigned int i = 0; i < n; i++)
            land[i] = new Land*[m];
        for(unsigned int i = 0; i < n; i++)
            for(unsigned int j = 0; j < m; j++)
                land[i][j] = nullptr;

        mediator = new Mediator;
    }

    ~Field(){
        for(unsigned int i = 0; i < height; i++){
            for(unsigned int j = 0; j < width; j++){
                delete map[i][j];
                map[i][j] = nullptr;
            }
        }
    }

    Field(Field &&field): max_item(field.max_item), now_item(field.now_item), width(field.width), height(field.height){
        /*map = new Item**[static_cast<unsigned int>(height)];
        for(int i = 0; i < height; i++)
            map[i] = new Item*[static_cast<unsigned int>(width)];
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
                map[i][j] = nullptr;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                swap(map[i][j], field.map[i][j]);
                //map[i][j] = field.map[i][j];
                //field.map[i][j] = nullptr;
            }
        }*/
        map = nullptr;
        swap(map, field.map);
    }

    Field(const Field &field): max_item(field.max_item), now_item(field.now_item), width(field.width), height(field.height){
        map = new Item**[height];
        for(unsigned int i = 0; i < height; i++)
            map[i] = new Item*[width];
        for(unsigned int i = 0; i < height; i++)
            for(unsigned int j = 0; j < width; j++)
                map[i][j] = nullptr;
        for(unsigned int i = 0; i < height; i++){
            for(unsigned int j = 0; j < width; j++){
                if (field.map[i][j] != nullptr)
                    map[i][j] = field.map[i][j]->copy();
            }
        }
    }

    IMediator *get_mediator(){
        return mediator;
    }

    unsigned int get_width(){
        return width;
    }

    unsigned int get_height(){
        return height;
    }

    void add_land(unsigned int x,unsigned int y, Land* landscape){
        land[x][y] = landscape;
    }

    int add_item(unsigned int x,unsigned int y, Item* obj, string &logs){
        if ((x < height) && (y < width)){
            if (map[x][y] == nullptr){
                map[x][y] = obj;
                return 1;
            }
            else {
                logs += "Already have an item!!!\n";
                return 0;
            }
        }
        else {
            logs += "Coordinate error!\n";
            return 0;
        }
    }

    void get_unit(unsigned int x, unsigned int y, string &logs){
        if (map[x][y] != nullptr){
            logs += "  x:";
            logs += to_string(x);
            logs += "   y: ";
            logs += to_string(y);
            logs += "\tHealth: \n";
            logs += to_string(map[x][y]->getHealth());
            logs += "\n";
            logs += "Near attack: \n";
            logs += to_string(map[x][y]->get_nearAttack());
            logs += "\n";
            logs += "Further attack: \n";
            logs += to_string(map[x][y]->get_furtherAttack());
            logs += "\n";
        }
    }

    Item* who_am_I(unsigned int x, unsigned int y){
        if (map[x][y] != nullptr)
            return map[x][y];
        else {
            return nullptr;
        }
    }

    int delete_unit(unsigned int x, unsigned int y, string &logs){

        if (map[x][y] != nullptr){
            if (map[x][y]->print_who() == "BAS"){
                logs += "This is base!!!";
                return 0;
            }
            logs += "Delete item: x:";
            logs += to_string(x);
            logs += "   y: ";
            logs += to_string(y);
            logs += "\n";
            delete map[x][y];
            map[x][y] = nullptr;
            return 1;
        }
        else {
            logs += "No unit in this place!\n";
            return 0;
        }
    }

    void attack_unit(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, string &logs){
        int result = mediator->attack_unit(map[x1][y1], map[x2][y2]);
        if (result == 0){
            delete_unit(x2, y2, logs);
        }
        else if (result == 2) {
            logs += "Attack error!!!\n";
        }
    }

    void move_unit(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, string &logs){
        if (map[x2][y2] == nullptr && map[x1][y1] != nullptr && (x1 != x2 || y1 != y2) && (x2 < height) && (y2 < width)){
            if (land[x2][y2]->CheckAccess(map[x1][y1]->print_who())){
                map[x2][y2] = map[x1][y1];
                map[x1][y1] = nullptr;
                logs += "Unit moved...\n";
            }
            else {
                logs += "The landscape is not suitable for a unit!!!!!!\n";
            }
        }
        else if (map[x2][y2]->print_who() == "B" && map[x1][y1] != nullptr && (x1 != x2 || y1 != y2) && (x2 < height) && (y2 < width)) {
            if (land[x2][y2]->CheckAccess(map[x1][y1]->print_who())){
                *(map[x1][y1]) += map[x2][y2];
                map[x2][y2] = map[x1][y1];
                map[x1][y1] = nullptr;
                logs += "Unit add bonus...\n";
                get_unit(x2, y2, logs);
            }
            else {
                logs += "The landscape is not suitable for a unit!!!!!!\n";
            }
        }
        else {
            logs += "Move error!!!\n";
        }
    }

    void print_unit(string &print){
        print = "";
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                if (map[i][j] != nullptr){
                    print += map[i][j]->print_who();
                    print += "\t";
                }
                else {
                    //print += 35;
                    print += land[i][j]->get_type();
                    print += "\t";
                }
            }
            print += "\n";
        }
    }
};

class Iterator_field{
    int in_field = 1;
    unsigned int x = 0;
    unsigned int y = 0;
    Field *field;

public:
    Iterator_field(Field *field_pr): field(field_pr){}

    int xy_in_field(){
        return in_field;
    }

    Item* operator->(){
        if (in_field)
            return field->who_am_I(x, y);
        return nullptr;
    }

    Item* operator++(){
        if (x < field->get_height()){
            if (y < field->get_width()){
                y++;

                return field->who_am_I(x, y-1);
            }
            else {
                y = 0;
                x++;
                return field->who_am_I(x-1, y);
            }
        }
        else {
            in_field = 0;
            return nullptr;
        }
    }
};
