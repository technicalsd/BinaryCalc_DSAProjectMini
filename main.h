#include<stdio.h> 
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>


typedef struct node {
    int d;
    struct node *next, *prev;
}node;


typedef struct list {
    int length;
    char sign;
    node *tail, *n;
}list;

typedef struct stack_operands{
    list *a;
    int size;
    int top;
}stack_operands;

typedef struct stack_operators{
    char *a;
    int size;
    int top;
}stack_operators;


void init(list *l);
void append(list *l, int data);
void add_ahead(list* l, int data);
list extract(char* s);
list add(list l1, list l2);
list sub(list l1, list l2);
list mul(list l1, list l2);
list division(list l1, list l2);
int compare(list l1, list l2);
void display(list l);
void change_sign(list *l3, list *l1, list *l2);
list calc(list l1, list l2, char op);
int precedence(char op);
void infix_eval(char *s);
void rm_zero(list *l);

void init1(stack_operands *s, int len);
int isFull1(stack_operands s) ;
int isEmpty1(stack_operands s);
void push1(stack_operands *s, list d);
list pop1(stack_operands *s);


void init2(stack_operators *s, int len);
int isFull2(stack_operators s) ;
int isEmpty2(stack_operators s);
void push2(stack_operators *s, char d);
char pop2(stack_operators *s);



void init(list* l) {

    l->n = NULL;
    l->tail = NULL;
    l->length = 0;
    l->sign = '+';      //default sign set to + if none is entered
    return;
}


list extract(char* s) {

    list l;
    init(&l);

    int x = 0, i = 0;
    char ch = s[0];

    if(ch == '-') {
        l.sign = '-';
    }
    else {
        l.sign = '+';
    }
    while(s[i] != '\0') {
        if(isdigit(s[i])) {
            x = s[i] - '0';     //extracting a character and converting to 'int'
            append(&l, x);
        }
        i++;
    }

    return l;
}

void append(list *l, int data) {

    node *newnode;
    newnode = (node*)malloc(sizeof(node));      //creating a new node

    if(newnode) {
        newnode->next = NULL;
        newnode->d = data;
        newnode->prev = NULL;
    }
    else {
        return;
    }

    if(l->n == NULL) {                  //if the node to be added is the first node
        l->n = newnode;
        l->tail = newnode;
        l->length = 1;
        return;
    }

    newnode->prev = l->tail;
    l->tail->next = newnode;
    l->tail = newnode;
    l->length = l->length + 1;

    return;
}


void add_ahead(list* l, int data) {

    node *p, *newnode;
    p = l->n;
    newnode = (node*)malloc(sizeof(node));          //creating a new node

    if(newnode) {
        newnode->d = data;
        newnode->next = NULL;
        newnode->prev = NULL;
    }
    else {
        return;
    }

    if(l->n == NULL) {                         //if the node to be added is the first node
        l->n = newnode;
        l->tail = newnode;                     //treated as a separate case since tail is modified
        l->length = 1;
        return;
    }

    newnode->next = p;
    p->prev = newnode;
    l->n = newnode;
    l->length = l->length + 1;

    return;

}

list add(list l1, list l2) {

    list l3;
    init(&l3);

    change_sign(&l3, &l1, &l2);

    if(l3.sign == '-') {
        if(l1.sign == '-') {   //calling sub in num2 - num1 format
            l1.sign = '+';
            l3 = sub(l2, l1);
            return l3;
        }
        else {
            l2.sign = '+';
            l3 = sub(l1, l2);   //calling sub in num1 - num2 format
            return l3;
        }
    }

    int s = 0, c = 0;           //sum and carry initialized to zero

    node *end1, *end2;          //pointers to the last digit of l1 and l2 respectively
    end1 = l1.tail;
    end2 = l2.tail;

    while (end1 != NULL || end2 != NULL) {
        if (end1 != NULL && end2 != NULL) {
            s = ((end1->d) + (end2->d) + c) % 10;
            c = ((end1->d) + (end2->d) + c) / 10;
            end1 = end1->prev;
            end2 = end2->prev;
        }
        else if (end1 == NULL && end2 != NULL) {
            s = ((end2->d) + c) % 10;
            c = ((end2->d) + c) / 10;
            end2 = end2->prev;
        }
        else if (end1 != NULL && end2 == NULL) {
            s = ((end1->d) + c) % 10;
            c = ((end1->d) + c) / 10;
            end1 = end1->prev;
        }

        // inserting the sum digit
        add_ahead(&l3, s);
    }

    // inserting last carry
    if (c != 0) {
        add_ahead(&l3, c);
    }

    if(l1.sign == '-' && l2.sign == '-') {      //if both the numbers are negative
        l3.sign = '-';
    }

    return l3;

}

//subtracting two lists
list sub(list l1, list l2) {

    list l3;
    init(&l3);

    node *end1, *end2;                          //pointers to the last digit of l1 and l2 respectively

    rm_zero(&l1);                               //removing preceding zeros if any
    rm_zero(&l2);                               //returns NULL if the number is zero

    if(l1.n == NULL && l2.n == NULL) {          //case:  0 - 0
        append(&l3, 0);
        return l3;
    }
    else if(l1.n != NULL && l2.n == NULL) {     //case: num1 - 0
        return l1;
    }
    else if(l1.n == NULL && l2.n != NULL) {     //case: 0 - num2
        if(l2.sign == '-'){
            l2.sign = '+';
        }
        else {
            l2.sign = '-';
        }
        return l2;
    }

    if(l1.sign == '-' && l2.sign == '+') {   //-num1 - (+num2) implies addition of two negative numbers
        l2.sign = '-';
        l3 = add(l1, l2);
        return l3;
    }
    else if(l1.sign == '+' && l2.sign == '-') {  //+num1 - (-num2) implies addition of two positive numbers
        l2.sign = '+';
        l3 = add(l1, l2);
        return l3;
    }
    else if(l1.sign == '+' && l2.sign == '+') {  //+num1 - (num2) standard case
        int check = compare(l1, l2);
        if(check == INT_MIN) {                   //if l2 is zero
            return l1;
        }
        if(check < 0) {                         //if the negative number is greater in magnitude
            l3.sign = '-';
            end2 = l1.tail;
            end1 = l2.tail;

        }
        if(check >= 0) {
            end1 = l1.tail;
            end2 = l2.tail;
        }
    }
    else {                                      //-num1 - (-num2) reverse case
        int check = compare(l1, l2);
        if(check > 0) {                         //if the negative number is greater in magnitude
            l3.sign = '-';
            end1 = l1.tail;
            end2 = l2.tail;
        }
        if(check <= 0) {
            end1 = l2.tail;
            end2 = l1.tail;
        }
    }


    int s = 0, c = 0;                       //sum and carry(borrow) initialized to zero


    while (end1 != NULL || end2 != NULL) {
        if (end1 != NULL && end2 != NULL) {
            if ((end1->d) + c >= (end2->d)) {
                s = ((end1->d) + c - (end2->d));
                c = 0;
            }
            else {
                s = ((end1->d) + c + 10 - (end2->d));
                c = -1;
            }
            end1 = end1->prev;
            end2 = end2->prev;
        }
        else if (end1 != NULL && end2 == NULL) {
            if (end1->d >= 1) {
                s = ((end1->d) + c);
                c = 0;
            }
            else {                                  //special case when the l1 has zeros
                if (c != 0) {
                    s = ((end1->d) + 10 + c);       //case when borrow has taken place
                    c = -1;
                }
                else
                    s = end1->d;                    //case when borrow has not taken place
            }
            end1 = end1->prev;
        }
        add_ahead(&l3, s);
    }

    return l3;
}

//multiplying two lists
list mul(list l1, list l2) {

    list ans, f_answer, temp;

    init(&f_answer);
    init(&ans);
    init(&temp);

    int k = 0, i = 0;
    int s = 0, c = 0;
    node *end2, *endp;                
    end2 = l2.tail;

    while(end2 != NULL) {

        init(&ans);
        init(&temp);

        endp = l1.tail;
        c = 0;
        s = 0;

        while(endp != NULL) {           
            s = ((endp->d) * (end2->d) + c) % 10;
            c = ((endp->d) * (end2->d) + c) / 10;
            add_ahead(&ans, s);
            endp = endp->prev;
        }

        if (c != 0) {
            add_ahead(&ans, c);
        }

        for (i = 0; i < k; i++) {  
            append(&ans, 0);
        }

        if(k == 0) {
            f_answer = ans;
        }
        else {
            temp = add(f_answer, ans);
            f_answer = temp;
        }
        k++;
        end2 = end2->prev;
    }

    change_sign(&f_answer, &l1, &l2);
    return f_answer;
}

list division(list l1, list l2) {

    int x = compare(l1, l2);
    int i = 0;

    list l3;
    init(&l3);

    change_sign(&l3, &l1, &l2);

    if(x == INT_MIN) {          
        printf("Divide by zero error\n");
        exit(1);
    }

    if(x < 0) {                    
        append(&l3, 0);
        return l3;
    }
    else if(x == 0) {               
        append(&l3, 1);
        return l3;
    }
    else {

        int length1 = l1.length;
        int length2 = l2.length;

        node *p = l1.n;         
        node *t = l2.n;         
        while(p->d == 0) {     
            p = p->next;
        }

        while(t->d == 0) {
            t = t->next;
        }
        int digits = 0;         
        while(t != NULL) {
            digits++;
            t = t->next;
        }

        list temp, num, l22, q, j, quo;

        init(&q);
        init(&j);
        init(&num);
        init(&temp);
        init(&l22);
        init(&quo);

        append(&q, 0);
        append(&j, 1);
        l22 = l2;

        int pass = 0;


        while(p != NULL) {
            if(pass == 0) {
                for(i = 0; i < digits; i++) {
                    if(p != NULL) {
                        append(&num, p->d);
                        p = p->next;
                    }
                    else {
                        break;
                    }
                }
                pass++;
            }
            else {
                append(&num, p->d);
                p = p->next;
            }

            int r = compare(l2, num);

            if(r == INT_MIN) {
                append(&quo, 0);
                continue;
            }

            int check = 0;
            l22 = l2;                   

            while(r < 0) {                 
                q = add(q, j);
                l22 = mul(l2, q);
                r = compare(l22, num);
                check++;
            }

            if(check == 0 && r != 0) {      
                append(&quo, 0);
                temp = num;
            }
            else if(check == 0 && r == 0){
                append(&quo, 1);            
                temp = sub(num, l22);
            }

            else if(r == 0) {               
                append(&quo, q.tail->d);
                temp = sub(num, l22);
            }
            else {                         
                q = sub(q, j);              
                append(&quo, (q.tail->d));
                l22 = sub(l22, l2);
                temp = sub(num, l22);
            }

            init(&q);
            append(&q, 0);
            num = temp;
            init(&l22);
        }

        change_sign(&quo, &l1, &l2);
        return (quo);
    }

}

int compare(list l1, list l2) {

    int c1 = 0, c2 = 0, count_zero = 0, c11 = 0, c22 = 0;
    node *p = l1.n;
    node *q = l2.n;

    c1 = l1.length;
    c2 = l2.length;

    while(q != NULL) {
        if(q->d == 0) {
            count_zero++;
            c22++;      
        }
        else {
            break;
        }
        q = q->next;
    }
    if(count_zero == c2) {  
        return INT_MIN;
    }
    while(p != NULL) {
        if(p->d == 0) {
            p = p->next;
            c11++;
        }
        else {
            break;
        }
    }

    int comp = (c1 - c11) - (c2 - c22); 

    if(comp < 0) {
        return -1;
    }
    else if(comp > 0) {
        return 1;
    }
    else if(comp == 0) {            
        while((p->d - q->d == 0) && (p->next != NULL && q ->next != NULL)) {
            p = p->next;
            q = q -> next;
        }
        if(p->d - q->d > 0) {
            return 1;
        }
        else if(p->d - q->d < 0) {
            return -1;
        }
        else {
            return 0;
        }
    }
    return 0;
}


void display(list l) {

    if(l.sign == '-') {
        printf("-");
    }

    node *p = l.n;

    while(p != NULL) {
        printf("%d", p->d);
        p = p->next;
    }

    printf("\n");
    return;
}

void change_sign(list *l3, list *l1, list *l2) {

    if((l1->sign == '-' && l2->sign == '-') || (l1->sign == '+' && l2->sign == '+')) {
            l3->sign = '+';
    }
    else {
            l3->sign = '-';
    }
}



void init1(stack_operands *s, int len){
    s->a = (list*)malloc(sizeof(list)*len);
    s->size = len;
    s->top = -1;
}
int isFull1(stack_operands s) {
    if(s.top == s.size-1)
        return 1;
    else
        return 0;
}

int isEmpty1(stack_operands s) {
    if(s.top == -1)
        return 1;
    else
        return 0;
}
void push1(stack_operands *s, list d){
    if(isFull1(*s))
        return ;
    else {
        s->top++;
        s->a[s->top] = d;
        return;
    }
}

list pop1(stack_operands *s) {
    list l;
    init(&l);
    if(isEmpty1(*s)) {
        return l;
    }
    else {

        l = s->a[s->top];
        s->top--;
        return l;
    }
}


void init2(stack_operators *s, int len){
    s->a = (char*)malloc(sizeof(char)*len);
    s->size = len;
    s->top = -1;
}
int isFull2(stack_operators s) {
    if(s.top == s.size-1)
        return 1;
    else
        return 0;
}

int isEmpty2(stack_operators s) {
    if(s.top == -1)
        return 1;
    else
        return 0;
}
void push2(stack_operators *s, char d){
    if(isFull2(*s))
        return ;
    else {
        s->top++;
        s->a[s->top] = d;
        return;
    }
}

char pop2(stack_operators *s) {
    char ch = ' ';
    if(isEmpty2(*s)) {
        return ch;
    }
    else {

        ch = s->a[s->top];
        s->top--;
        return ch;
    }
}

list calc(list l1, list l2, char op) {

    switch(op){
        case '+': return add(l1, l2);
        case '-': return sub(l1, l2);
        case '*': return mul(l1, l2);
        case '/': return division(l1, l2);
    }
}

int precedence(char op) {
    if(op == '+'||op == '-')
        return 1;
    if(op == '*'||op == '/')
        return 2;
    return 0;
}

void infix_eval(char *s) {

    stack_operands s1;
    init1(&s1, 50);


    stack_operators s2;
    init2(&s2, 50);


    list l;
    init(&l);

    list op1, op2, ans;
    init(&op1);
    init(&op2);
    init(&ans);

    char number[10000];
    char ch, op;
    int i = 0;
    int cb_op = 0, cb_cl = 0;
    while(s[i] != '\0') {

        ch = s[i];
        if(isdigit(ch)) {    
            strncat(number, &ch, 1);
            i++;
            ch = s[i];
            while(isdigit(ch) && s[i] != '\0') {  
                strncat(number, &ch, 1);
                i++;
                ch = s[i];

            }
            if(ch == '\0') {
                l = extract(number);
                push1(&s1, l);
                strcpy(number, "");
                break;
            }
            else if(ch == ' ') {
                l = extract(number);
                push1(&s1, l);
                strcpy(number, "");
                i++;
                ch = s[i];
                if(isdigit(ch) || ch =='(' || ((ch == '+' || ch == '-') && isdigit(s[i+1]))) {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else if(ch == ')'){
                l = extract(number);
                push1(&s1, l);
                strcpy(number, "");
            }

            else {
                printf("Error in evaluation\n");
                exit(1);
            }
        }
        else if(ch == '+' || ch == '-'){
            strncat(number, &ch, 1);
            i++;
            ch = s[i];
            if(isdigit(ch)) {            
                strncat(number, &ch, 1);
                i++;
                ch = s[i];
                while(isdigit(ch)) {
                    strncat(number, &ch, 1);
                    i++;
                    ch = s[i];
                }
                if(ch == ' ') {
                    l = extract(number);
                    push1(&s1, l);
                    strcpy(number, "");
                    i++;
                    if(isdigit(ch) || ch =='(' || ((ch == '+' || ch == '-') && isdigit(s[i+1]))) {
                        printf("Error in evaluation\n");
                        exit(1);
                    }

                    continue;
                }
                else if(ch == '\0') {
                    l = extract(number);
                    push1(&s1, l);
                    strcpy(number, "");
                    break;
                }
                else if(ch == ')'){
                    l = extract(number);
                    push1(&s1, l);
                    strcpy(number, "");
                }
                else {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else if(ch == ' '){         
                if(i == 1) {
                    printf("Error in evaluation\n"); 
                    exit(1);
                }
                strcpy(number, "");
                i--;
                ch = s[i];
                i++;
                if(isEmpty2(s2)) {            
                    push2(&s2, ch);
                }
                else if(!isEmpty2(s2)) {

                    if(precedence(ch) > precedence(s2.a[s2.top])) {
                        push2(&s2, ch);
                    }
                    else {
                        while(!isEmpty2(s2) && (precedence(ch) <= precedence(s2.a[s2.top]))) {
                            init(&op1);
                            init(&op2);
                            init(&ans);
                            op = pop2(&s2);
                            if(op == '(') {
                               push2(&s2, op);
                               continue;
                            }
                            op2 = pop1(&s1);
                            op1 = pop1(&s1);

                            ans = calc(op1, op2, op);
                            push1(&s1, ans);
                        }
                        push2(&s2, ch);
                    }
                }
                i++;
                ch = s[i];

                if(ch == '\0' || ch == '*' || ch == '/' || ch == ')') {
                    printf("Error in evaluation\n");
                    exit(1);
                }
                else if(ch == '+' || ch == '-' )  {
                    if(!isdigit(s[i+1])) {
                        printf("Error in evaluation\n");
                        exit(1);
                    }
                }
            }
            else {
                printf("Error in evaluation\n");
                exit(1);
            }

        }
        else if(ch == '*' || ch == '/') {                 
            if(i == 0) {
                printf("Error in evaluation\n");            
                exit(1);
            }
            i++;
            ch = s[i];
            if(ch == ' '){                                   
                ch = s[i-1];

                if(isEmpty2(s2)) {
                    push2(&s2, ch);
                }
                else if(!isEmpty2(s2)) {
                    if(precedence(ch) > precedence(s2.a[s2.top])) {
                        push2(&s2, ch);
                    }
                    else {
                        while(!isEmpty2(s2) && (precedence(ch) <= precedence(s2.a[s2.top]))) {
                            init(&op1);
                            init(&op2);
                            init(&ans);
                            op = pop2(&s2);
                            if(op == '(') {
                               push2(&s2, op);
                               continue;
                            }

                            op2 = pop1(&s1);
                            op1 = pop1(&s1);

                            ans = calc(op1, op2, op);
                            push1(&s1, ans);
                        }
                        push2(&s2, ch);
                    }
                }
                i++;
                ch = s[i];
                if(ch == '\0' || ch == '*' || ch == '/' || ch == ')') {
                    printf("Error in evaluation\n");
                    exit(1);
                }
                else if(ch == '+' || ch == '-' )  {
                    if(!isdigit(s[i+1])) {
                        printf("Error in evaluation\n");
                        exit(1);
                    }
                }
            }
            else {
                printf("Error in evaluation\n");
                exit(1);
            }
        }
        else if(ch == '(') {
            cb_op++;
            push2(&s2, ch);
            i++;
            ch = s[i];
            if(isdigit(ch) || ch == '(') {
                continue;
            }
            else if(ch == '+' || ch == '-') {
                if(!isdigit(s[i+1])) {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
            else {
                printf("Error in evaluation\n");
                exit(1);
            }
        }
        else if(ch == ')') {
            cb_cl++;
            if(i == 0) {
                printf("Error in evaluation\n");
                exit(1);
            }
            while(!isEmpty2(s2) && (precedence(ch) <= precedence(s2.a[s2.top]))) {  
                init(&op1);
                init(&op2);
                init(&ans);
                op = pop2(&s2);
                if(op == '(') {
                    break;
                }
                op2 = pop1(&s1);
                op1 = pop1(&s1);
                ans = calc(op1, op2, op);
                push1(&s1, ans);
            }
            i++;
            ch = s[i];
            if(ch == '\0'){
                break;
            }
            if(ch == ' ') {
                i++;
                ch = s[i];
                if(ch == '\0' || isdigit(ch)) {
                    printf("Error in evaluation\n");
                    exit(1);
                }
            }
        }
        else {
            printf("Error in evaluation\n");
            exit(1);
        }

    }

    while(!isEmpty2(s2)) {
        init(&op1);
        init(&op2);
        init(&ans);
        op2 = pop1(&s1);
        op1 = pop1(&s1);
        op = pop2(&s2);
        ans = calc(op1, op2, op);
        push1(&s1, ans);
    }
    if(cb_cl != cb_op) {          
        printf("Error in evaluation\n");
        exit(1);
    }
    printf("Final answer: ");
    display(s1.a[s1.top]);
    return;
}


void rm_zero(list *l) {

    node *p = l->n;
    while(p != NULL && p->d == 0) {     //checking nullity condition before zero condition

        l->length = l->length - 1;
        l->n = p->next;
        p = p->next;
        if(p != NULL) {
            free(p->prev);
            p->prev = NULL;
        }
    }
    return;
}