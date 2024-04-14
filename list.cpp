#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
using namespace std;
const double MAX_VAL = pow(2,63)+1;
const double MIN_VAL = -pow(2,63)-1;

class list_exception;
template<typename T>
struct node
{
public:
    T value;
    node<T>*next = NULL;
    node<T>*prev = NULL;
    node(T val):value(val){}
    node(){}
};
template <typename T>
class List{
protected:
    node<T>*head;
    node<T>*tail;
    int length;
    void insert(node<T>*cur,node<T>*value){
        value->prev = cur;
        value->next = cur->next;
        cur->next->prev = value;
        cur->next = value;
        ++length;
    }
public: 
    List():length(0){
        head = new node<T>;
        tail = new node<T>;
        head->next = tail;
        tail->prev = head;
        this->length = length;
    }   
    virtual void add(T val){
        node<T>*cur = new node<T>(val);
        cur->prev = tail->prev;
        tail->prev->next = cur;
        tail->prev = cur;
        cur->next = tail;
        ++length;
    }   
    //вставляет на позицию после pos элемента 
    void insert(int pos,T val){
        if (length<pos || pos<0)throw list_exception("Вы пытаетесь вставить за несуществующий элемент");
        node<T>*value = new node<T>(val);
        node<T>*cur = head;
        for(int i = 0; i <= pos;i++){
            if (i == pos)
            {
                insert(cur,value);
            }
            cur = cur->next;
        }
    }
    void show() const noexcept{
        node<T> *cur = head->next;
        while (cur != tail)
        {
            cout << cur->value <<endl;
            cur = cur->next;
        }
    }
    int Length() const noexcept{return this->length;}
    List& operator+(List&arr){
        this->tail->prev->next = (&arr)->head->next;
        (&arr)->head->next->prev = this->tail->prev;
        this->tail = (&arr)->tail;
        return *this;
    }
        T operator[](int s){
        if (abs(s)>this->length )throw list_exception("Такого индекса нет в массиве");
        if (s >= 0)
        {
            node<T> *cur = this->head->next;
            for(int i = 0; i < s;i++)cur = cur->next;
            return cur->value;
        }
        if (s >= 0)
        {
            node<T> *cur = this->tail->prev;
            for(int i = 0; i < abs(s)-1;i++)cur = cur->prev;
            return cur->value;
        }
        
    }
};
class list_exception:public exception{
    private:char* message;
    public: list_exception(const char* mess){
        message = new char[strlen(mess)+1];
        strcpy(message,mess);
    }
    ~list_exception(){delete[] message;}
    const char* what() const noexcept override{
        return message;
    }
};
//переопределенный класс распологающий элементы по возрастанию/убыванию
//Sorted_List(-1) - сортировка по убыванию
template <typename T> 
class Sorted_List: public List<T>
{
protected:
    T max_val,min_val;
    int increase_decrease;
    //insert служит так же для переопределения максимального и минимального элемента
    //private так как нельзя его вызвать снаружи так как insert не определен для сортированного списка
    void insert(node<T>*cur,node<T>*value, T val){
        value->prev = cur;
        value->next = cur->next;
        cur->next->prev = value;
        cur->next = value;
        max_val = (max_val<val)?val:max_val;
        min_val = (min_val > val)?val:min_val;
        ++this->length;
    }
public:
    Sorted_List(int increase_decrease = 1):increase_decrease(increase_decrease){
        max_val = MIN_VAL;
        min_val = MAX_VAL;
        int multiple = (this->increase_decrease==-1)?-1:1;
        this->head->value = multiple*MIN_VAL;
        this->tail->value = multiple*MAX_VAL;
    }
    //вывод минимального элемента
    T min()const noexcept{
        if (min_val == MIN_VAL)throw list_exception("Минимального значения еще нет в списке");
        return min_val;
    }
    //вывод максимального элемента
    T max()const noexcept{
        if (max_val == MAX_VAL)throw list_exception("Максимального значения еще нет в списке");
        return max_val;
    }

    //для численных типов данных
    void add(T val) override{
        node<T>* value = new node<T>(val);
        node<T>*cur = this->head;
        while(cur!=this->tail){
            //если элемент находится между двух значений - вставляем его туда
            if(increase_decrease==1){//если по возрастанию
                if (val >= cur->value && val <= cur->next->value)
                {
                    insert(cur,value, val);
                    return;
                }
            }
            else{//По убыванию
                if (val <= cur->value && val >= cur->next->value)
                {
                    insert(cur,value, val);
                    
                    return;
                }
            }
            cur = cur->next;
        }
        throw list_exception("Вы ввели некоректные значения");
    }

    Sorted_List& operator+(Sorted_List&arr){
        for(int i = 0; i < arr.Length();i++){
            this->add(arr[i]);
        }
        return *this;
    }

};
int main(){
    Sorted_List<double> list;
    list.add(10.12);list.add(912);
    Sorted_List<double> arr;
    arr.add(91);arr.add(9.124);
    Sorted_List<double> sm = arr + list;
    sm.show();
}
