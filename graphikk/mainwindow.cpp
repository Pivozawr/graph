#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include "QPainter"
#include "QPainterPath"
#include "math.h"
#include "QPen"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    resize(800, 600);
    button = new QPushButton(this);
    button->setGeometry(width()-5-100, 5, 100, 20);
    button->setText("Push");
    connect(button, SIGNAL(clicked()), this, SLOT(Clicked()));
    //connect(button, SIGNAL(clicked()), this, SLOT(myCLicked()));

}



using namespace std;


struct Stack {
    double num;
    char sign;
    Stack *next;
};

// 63/93-sin(lg(tg(53^11)/()9)-5)-57*6
void push(Stack *&top, double num, char sign) {
    Stack *p = (Stack *) malloc(sizeof(Stack));
    p->num = num;
    p->sign = sign;
    p->next = top;
    top = p;
}

Stack pop(Stack *&top) {
    Stack temp = *top;
    top = top->next;
    return temp;
}

int prior(char a) {
    if (a == '(') return 0;
    if (a == '+' || a == '-') return 1;
    if (a == '*' || a == '/') return 2;
    if (a == '^') return 3;
    if (a == 'S' || a == 'C' || a == 'T' || a == 'Q' || a == 'L' || a == 'A') return 4;
    return -1;
}

bool apply(Stack *&top1, char sign) {
    double b;
    double a;
    if (top1 != nullptr) b = pop(top1).num;
    else
        return true;

    if (prior(sign) != 4) {
        if (top1 != nullptr)
            a = pop(top1).num;
        else
            return true;
    }

    if (sign == '+') push(top1, a + b, '@');
    else if (sign == '-') push(top1, a - b, '@');
    else if (sign == '*') push(top1, a * b, '@');
    else if (sign == '/' && b != 0) push(top1, a / b, '@');
    else if (sign == '^') push(top1, pow(a, b), '@');
    else if (sign == 'S') push(top1, sin(b), '@');
    else if (sign == 'C') push(top1, cos(b), '@');
    else if (sign == 'T') push(top1, tan(b), '@');
    else if (sign == 'A') push(top1, abs(b), '@');
    else if (sign == 'L' && b > 0) push(top1, log(b), '@');
    else if (sign == 'Q' && b >= 0) push(top1, sqrt(b), '@');
    else
        return true;
    return false;
}

Stack *top1 = nullptr;
Stack *top2 = nullptr;

bool calculate(std::string s, double& rez) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '-' && !('0' <= s[i - 1] && s[i - 1] <= '9' || s[i - 1] == ')'))
            s.insert(i, "0");
    }

    Stack *top3 = nullptr;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            push(top3, 0.0, s[i]);
        } else if (s[i] == ')' && s[i - 1] != '(' && top3 != nullptr) {
            pop(top3);
        } else if (s[i] == ')') {
            return false;
        }
    }
    if (top3 != nullptr) {
        return false;
    }

    std::string f = "";
    for (int i = 0; i < s.size(); i++) {
        while ('a' <= s[i] && s[i] <= 'z') {
            f += s[i];
            s[i] = ' ';
            i++;
        }
        if (f == "sin") {
            s[i - 1] = 'S';
        } else if (f == "cos") {
            s[i - 1] = 'C';
        } else if (f == "tg") {
            s[i - 1] = 'T';
        } else if (f == "sqrt") {
            s[i - 1] = 'Q';
        } else if (f == "ln") {
            s[i - 1] = 'L';
        } else if (f == "abs") {
            s[i - 1] = 'A';
        } else if (f != "") {
            return false;
        }
        f = "";
    }

    for (int i = 0; i < s.size() - 1; i++) {
        if (prior(s[i]) > 0 && prior(s[i + 1]) > 0) {
            return false;
        }
    }
    if ((prior(s[0]) > 0) && prior(s[0]) != 4) {
        return false;
    }


    for (int i = 0; i < s.size(); i++) {
        if ('0' <= s[i] && s[i] <= '9' || s[i] == '.') {
            std::string tmp = "";
            while ('0' <= s[i] && s[i] <= '9' || s[i] == '.') {
                tmp += s[i];
                i++;
            }
            double a = stod(tmp);
            push(top1, a, '@');
        }
        if (s[i] == '(') {
            push(top2, 0.0, s[i]);
        } else if (s[i] == ')') {
            while (top2->sign != '(') {
                if (apply(top1, pop(top2).sign)) {
                    return false;
                }
            }
            pop(top2);
        } else if (0 <= prior(s[i])) {
            while (top2 != nullptr && prior(top2->sign) >= prior(s[i])) {
                if (apply(top1, pop(top2).sign)) {
                    return false;

                }
            }
            push(top2, 0.0, s[i]);
        }
    }

    while (top2 != nullptr) {
        if (apply(top1, pop(top2).sign)) {
            return false;

        }
    }
    rez = pop(top1).num;

    return true;
}




void MainWindow::Clicked(){

    ui->label->setGeometry(5,5,this->width()-5-100-10,this->height()-5-5);
    QPixmap pixmap(ui->label->width(), ui->label->height());
    pixmap.fill(Qt::white);
    QPainter painter;
    QPen pen;
    QPainterPath path;
    painter.setPen(Qt::black);
    double xmin = ui->xminSpinBox->value();
    double xmax = ui->xmaxSpinBox->value();
    double ymin = ui->yminSpinBox->value();
    double ymax = ui->ymaxSpinBox->value();
    double xgmin = 0;
    double xgmax = pixmap.width();
    double ygmin = 0;
    double ygmax = pixmap.height();
    double kx = (xgmax - xgmin)/(xmax - xmin);
    double ky = (ygmin - ygmax)/(ymax - ymin);
    double step = (xmax - xmin)/(xgmax - xgmin)/100;
    double y = ymin;
    double x0 = xgmin - kx * xmin;
    double y0 = ygmin - ky * ymax;
    painter.begin(&pixmap);
    painter.setPen(pen);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawLine(x0, 0, x0, ui->label->height());
    painter.drawLine(0, y0, ui->label->width(), y0);
    painter.setPen(Qt::red);
    bool check = true;
    for (double x = xmin; x < xgmax; x+=step){
        double rez;
        string s = ui->functionLineEdit->text().toStdString();
        for (int j = 0; j < s.size(); j++){
            if (s[j] == 'x'){
                s.replace(j, 1, to_string(x));
            }
        }
        double ty = y;
        bool check2 = calculate(s, rez);
        if (check2){
            y = rez;
            double xg = xgmin + kx*(x-xmin);
            double yg = ygmin + ky*(y-ymax);
            if (x == 0 || check == true){
                check = false;
                path.moveTo(xg, yg);
            }
            if (ty - rez <= ymax && ty - rez >= ymin){
                path.lineTo(xg,yg);
            } else {
                x += step;
                y += step;
                double xg = xgmin + kx*(x-xmin);
                double yg = ygmin + ky*(y-ymax);
                path.moveTo(xg, yg);
            }
        }
    }
    painter.drawPath(path);
    ui->label->setPixmap(pixmap);

}

MainWindow::~MainWindow()
{
    delete ui;
}
