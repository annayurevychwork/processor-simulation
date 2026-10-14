#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>

using namespace std;

const int BITS_AMOUNT = 20;

//переводимо число із десяткової форми у двійкову
bitset<BITS_AMOUNT> toBinary(int num)
{
    bitset<BITS_AMOUNT> binary;
    binary = bitset<BITS_AMOUNT>(bitset<BITS_AMOUNT-1>(abs(num)).to_string());
    //встановлюємо старший біт
    if (num < 0) binary[BITS_AMOUNT-1] = 1;
    else binary[BITS_AMOUNT-1] = 0;
    return binary;
}

//вивід результату в консоль
void output(const string & cmd, const bitset<BITS_AMOUNT> & r1, const bitset<BITS_AMOUNT> & r2, const bitset<BITS_AMOUNT> & r3, const bitset<BITS_AMOUNT> & stack, const int & sign, const int & num_cmd, const int & tact)
{
    cout << "IR: " << cmd << endl; //регістр команди, містить текстовий запис команди, яка є поточною
    //перший регістр
    cout << "R1: ";
    for (int i = BITS_AMOUNT - 1; i >= 0; i--) cout << r1[i] << " ";
    cout << endl;
    //другий регістр
    cout << "R2: "; 
    for (int i = BITS_AMOUNT - 1; i >= 0; i--) cout << r2[i] << " ";
    cout << endl;
    //третій регістр
    cout << "R3: ";
    for (int i = BITS_AMOUNT - 1; i >= 0; i--) cout << r3[i] << " ";
    cout << endl;
    //стек
    cout << "ST: ";
    for (int i = BITS_AMOUNT - 1; i >= 0; i--) cout << stack[i] << " ";
    cout << endl; 
    cout << "PS: " << sign << endl; //регістр статусу, може містити лише знак останнього результату 
    cout << "PC: " << num_cmd << endl; //регістр лічильника команд 
    cout << "TC: " << tact << endl << endl; //регістр лічильника тактів 
}

void output_execute()
{
    cout << "Press Enter";
    cin.get();
    cout << endl << "Execute:" << endl;
}

int main()
{
    ifstream file;
    file.open("commands.txt");
    if (file.is_open())
    {
        bitset<BITS_AMOUNT> r1, r2, r3, stack;
        string cmd;
        int num_cmd = 0, tact = 0, sign = 0;
        
        while (getline(file, cmd))
        {
            num_cmd++;
            tact = 1;
            string command, operand1, operand2, operand3;
            stringstream ss(cmd);
            ss >> command >> operand1 >> operand2 >> operand3;
            //команда mov (надає операнду певного значення)
            if (command == "MOV")
            {
                cout << "Prepare:" << endl;
                //перший операнд це R1
                if (operand1 == "R1")
                {
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                    if (operand2 == "R2") r1 = r2;
                    else if (operand2 == "R3") r1 = r3;
                    else r1 = toBinary(stoi(operand2));
                    tact++;
                    output_execute();
                    sign = r1[BITS_AMOUNT-1];
                    stack = r1;
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                }
                //перший операнд це R2
                else if (operand1 == "R2")
                {
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                    if (operand2 == "R1") r2 = r1;
                    else if (operand2 == "R3") r2 = r3;
                    else r2 = toBinary(stoi(operand2));
                    tact++;
                    output_execute();
                    sign = r2[BITS_AMOUNT-1];
                    stack = r2;
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                }
                //перший операнд це R3
                else if (operand1 == "R3")
                {
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                    if (operand2 == "R1") r3 = r1;
                    else if (operand2 == "R2") r3 = r2;
                    else r3 = toBinary(stoi(operand2));
                    tact++;
                    output_execute();
                    sign = r3[BITS_AMOUNT-1];
                    stack = r3;
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                }
                //помилка
                else 
                {
                    cout << "Error" << endl;
                    return 0;
                }
            }
            //команда inv (інвертує лише парні / непарні біти в залежності від значення в іншому операнді)
            else if (command == "INV")
            {
                cout << "Prepare:" << endl;
                //перший операнд це R1
                if (operand1 == "R1")
                {
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                    if (operand2 == "R1")
                    {
                        if (operand3 == "0") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r1[i] = !r1[i];
                                else r1[i] = r1[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r1[i] = !r1[i];
                                else r1[i] = r1[i]; 
                            }
                        }
                    }
                    if (operand2 == "R2")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r1[i] = !r2[i];
                                else r1[i] = r2[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r1[i] = !r2[i];
                                else r1[i] = r2[i]; 
                            }
                        }
                    }
                    if (operand2 == "R3")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r1[i] = !r3[i];
                                else r1[i] = r3[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r1[i] = !r3[i];
                                else r1[i] = r3[i]; 
                            }
                        }
                    }
                    if (operand2 == "ST")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r1[i] = !stack[i];
                                else r1[i] = stack[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r1[i] = !stack[i];
                                else r1[i] = stack[i]; 
                            }
                        }
                    }
                    tact++;
                    output_execute();
                    sign = r1[BITS_AMOUNT-1];
                    stack = r1;
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                }
                //перший операнд це R2
                else if (operand1 == "R2")
                {
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                    if (operand2 == "R1")
                    {
                        if (operand3 == "0") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r2[i] = !r1[i];
                                else r2[i] = r1[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r2[i] = !r1[i];
                                else r2[i] = r1[i]; 
                            }
                        }
                    }
                    if (operand2 == "R2")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r2[i] = !r2[i];
                                else r2[i] = r2[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r2[i] = !r2[i];
                                else r2[i] = r2[i]; 
                            }
                        }
                    }
                    if (operand2 == "R3")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r2[i] = !r3[i];
                                else r2[i] = r3[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r2[i] = !r3[i];
                                else r2[i] = r3[i]; 
                            }
                        }
                    }
                    if (operand2 == "ST")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r2[i] = !stack[i];
                                else r2[i] = stack[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r2[i] = !stack[i];
                                else r2[i] = stack[i]; 
                            }
                        }
                    }
                    tact++;
                    output_execute();
                    sign = r2[BITS_AMOUNT-1];
                    stack = r2;
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                }
                //перший операнд це R3
                else if (operand1 == "R3")
                {
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                    if (operand2 == "R1")
                    {
                        if (operand3 == "0") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r3[i] = !r1[i];
                                else r3[i] = r1[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r3[i] = !r1[i];
                                else r3[i] = r1[i]; 
                            }
                        }
                    }
                    if (operand2 == "R2")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r3[i] = !r2[i];
                                else r3[i] = r2[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r3[i] = !r2[i];
                                else r3[i] = r2[i]; 
                            }
                        }
                    }
                    if (operand2 == "R3")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r3[i] = !r3[i];
                                else r3[i] = r3[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r3[i] = !r3[i];
                                else r3[i] = r3[i]; 
                            }
                        }
                    }
                    if (operand2 == "ST")
                    {
                        if (operand3 == "0")
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 0) r3[i] = !stack[i];
                                else r3[i] = stack[i]; 
                            }
                        }
                        else if (operand3 == "1") 
                        {
                            for (int i = 0; i < BITS_AMOUNT; i++) 
                            {
                                if (i % 2 == 1) r3[i] = !stack[i];
                                else r3[i] = stack[i]; 
                            }
                        }
                    }
                    tact++;
                    output_execute();
                    sign = r3[BITS_AMOUNT-1];
                    stack = r3;
                    output(cmd, r1, r2, r3, stack, sign, num_cmd, tact);
                }
                //помилка
                else 
                {
                    cout << "Error" << endl;
                    return 0;
                }
            }
            cout << "Press Enter" << endl;
            cin.get();
        }
        file.close();
    }
    else
    {
        cout << "Failed to open file" << endl;
    }
    return 0;
}